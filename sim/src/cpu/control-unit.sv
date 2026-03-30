`include "src/core.svh"

`define SET_CS_RAW(signal, value) controlSignalsRaw.signal = value; // Set Raw Control Signal

`define OPCODE instr[15:10]

// Instructions for Cases
`define INSTR_NOP       6'd0
`define INSTR_MOV       6'd1
`define INSTR_LI        6'd2

`define INSTR_MEMORY    `INSTR_LDW, `INSTR_STW, `INSTR_LDE, `INSTR_STE
`define INSTR_LDW       6'd3
`define INSTR_STW       6'd4
`define INSTR_LDE       6'd5
`define INSTR_STE       6'd6

`define INSTR_JMPS      `INSTR_JMP, `INSTR_JMPF, `INSTR_JZ, `INSTR_JC
`define INSTR_JMP       6'd7
`define INSTR_JMPF      6'd8
`define INSTR_JZ        6'd9
`define INSTR_JC        6'd10

`define INSTR_ALU       `INSTR_ADD, `INSTR_SUB, `INSTR_CMP, `INSTR_INC, `INSTR_DEC, `INSTR_MUL, `INSTR_DIV, `INSTR_AND, `INSTR_OR   // ? 16
`define INSTR_ADD       6'd11
`define INSTR_SUB       6'd12
`define INSTR_CMP       6'd13
`define INSTR_INC       6'd14
`define INSTR_INC2      6'd15
`define INSTR_DEC       6'd16
`define INSTR_DEC2      6'd17
`define INSTR_MUL       6'd18
`define INSTR_DIV       6'd19
`define INSTR_AND       6'd20
`define INSTR_OR        6'd21

`define INSTR_POP_RET_F `INSTR_POP, `INSTR_RET, `INSTR_RETF
`define INSTR_CALL_F    `INSTR_CALL, `INSTR_CALLF
`define INSTR_PUSH      6'd22
`define INSTR_POP       6'd23
`define INSTR_CALL      6'd24
`define INSTR_CALLF     6'd25
`define INSTR_RET       6'd26
`define INSTR_RETF      6'd27

// ALU Operations
`define ALU_ADD       4'd1
`define ALU_SUB       4'd2
`define ALU_CMP       4'd3
`define ALU_INC       4'd4
`define ALU_INC2      4'd5
`define ALU_DEC       4'd6
`define ALU_DEC2      4'd7
`define ALU_MUL       4'd8
`define ALU_DIV       4'd9
`define ALU_AND       4'd10
`define ALU_OR        4'd11

// Operand Roles (Use Inside SET_CS_RAW)

// (00: Off, 01: seg_sel, 10: bus_dest_raw, 11: bus_src_raw)
// (00: Off, 01: bus_dest_raw, 10: bus_src_raw, 01&alu_e_raw: alu-a-sel)
// (0: Off, 1&alu_e_raw: alu-b-sel, 1: bus_src_raw)

`define OP0_SEG_SEL     2'b01
`define OP0_BUS_DEST    2'b10
`define OP0_BUS_SRC     2'b11

`define OP1_BUS_DEST    2'b01
`define OP1_ALU_A_SEL   2'b01 // Use with alu_e_raw Enabled
`define OP1_BUS_SRC     2'b10

`define OP2_BUS_SRC     1'b1
`define OP2_ALU_B_SEL   1'b1 // Use with alu_e_raw Enabled

// Segments
`define SEL_CS          4'b0001
`define SEL_DS          4'b0010
`define SEL_SS          4'b0100
`define SEL_ES          4'b1000

// Flags (flags[index])
`define F_ZERO          0
`define F_CARRY         1

module ControlUnit(

    input logic clk, r,
    
    input logic [15:0] instr,
    input logic [7:0] flags,

    ControlSignals_if.control controlSignalsRaw

);

    logic [3:0] microCodeIndex; // Max 16
    logic       instrEnd;

    always_comb begin

        // Defaults
        `SET_CS_RAW(bus_src_raw              , 5'd0);
        `SET_CS_RAW(bus_dest_raw             , 5'd0);
        `SET_CS_RAW(bus_dest_special_raw     , 4'd0);

        `SET_CS_RAW(seg_sel__alu_op_sel_raw  , 4'd0);
        `SET_CS_RAW(alu_e_raw                , 1'd0);

        `SET_CS_RAW(alu_a_sel_raw            , 4'd0);
        `SET_CS_RAW(alu_b_sel_raw            , 4'd0);

        `SET_CS_RAW(pc_e                     , 1'd0);
        `SET_CS_RAW(flags_e                  , 1'd0);

        `SET_CS_RAW(operand_0_raw            , 2'd0);
        `SET_CS_RAW(operand_1_raw            , 2'd0);
        `SET_CS_RAW(operand_2_raw            , 1'd0);

        instrEnd = 1'd0;

        // Instruction Logic

        // Fetch
        if          (microCodeIndex == 4'd0) begin
            `SET_CS_RAW(bus_src_raw  , 5'd`CS_PC);
            `SET_CS_RAW(bus_dest_raw , 5'd`CS_MEM);
        end else if (microCodeIndex == 4'd1) begin
            `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);
            `SET_CS_RAW(bus_src_raw  , 5'd`CS_EXP1); // Memory
            `SET_CS_RAW(bus_dest_raw , 5'd`CS_INSTR);
            `SET_CS_RAW(pc_e         , 1'd1);
        end

        /* verilator lint_off CASEINCOMPLETE */
        unique case (`OPCODE)
            `INSTR_NOP: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        instrEnd = 1'b1;
                    end
                endcase
            end
            // Moving
            `INSTR_MOV: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                        `SET_CS_RAW(operand_2_raw, `OP2_BUS_SRC);
                        instrEnd = 1'b1;
                    end
                endcase
            end
            `INSTR_LI: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(bus_src_raw,  5'd`CS_PC);
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_MEM);
                    end
                    4'd3: begin
                        `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);
                        `SET_CS_RAW(bus_src_raw, 5'd`CS_EXP1); // Memory
                        `SET_CS_RAW(pc_e, 1'd1);
                        instrEnd = 1'b1;
                    end
                endcase
            end
            `INSTR_MEMORY: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(bus_src_raw,  5'd`CS_PC);
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_MEM);
                    end
                    4'd3: begin
                        `SET_CS_RAW(bus_src_raw, 5'd`CS_EXP1); // Memory
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_MEM);
                        `SET_CS_RAW(pc_e, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);
                    end
                    4'd4: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`CS_MEM);
                        `SET_CS_RAW(operand_2_raw, `OP2_ALU_B_SEL);

                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_MEM);
                    end
                    4'd5: begin
                        unique case (`OPCODE)
                            `INSTR_LDW, `INSTR_STW: begin `SET_CS_RAW(operand_0_raw, `OP0_SEG_SEL); end
                            `INSTR_LDE, `INSTR_STE: begin `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_ES); end
                        endcase

                        unique case (`OPCODE)
                            `INSTR_LDW, `INSTR_LDE: begin `SET_CS_RAW(bus_src_raw, 5'd`CS_EXP1); end // Memory
                            `INSTR_STW, `INSTR_STE: begin `SET_CS_RAW(bus_dest_raw, 5'd`CS_EXP1); end // Memory
                        endcase
                        unique case (`OPCODE)
                            `INSTR_LDW: begin `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST); end
                            `INSTR_LDE: begin `SET_CS_RAW(operand_0_raw, `OP0_BUS_DEST); end
                            `INSTR_STW: begin `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC); end
                            `INSTR_STE: begin `SET_CS_RAW(operand_0_raw, `OP0_BUS_SRC); end
                        endcase

                        instrEnd = 1'b1;
                    end
                endcase
            end
            // Jump / Conditions
            `INSTR_JMPS: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        if      (`OPCODE == `INSTR_JZ && !flags[`F_ZERO]) begin
                            `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'b1; end
                        else if (`OPCODE == `INSTR_JC && !flags[`F_CARRY]) begin
                            `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'b1; end

                        `SET_CS_RAW(bus_src_raw,  5'd`CS_PC);
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_MEM);
                    end
                    4'd3: begin
                        `SET_CS_RAW(bus_src_raw, 5'd`CS_EXP1); // Memory
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_PC);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);
                    end
                    4'd4, 4'd5: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`CS_PC);
                        `SET_CS_RAW(operand_2_raw, `OP2_ALU_B_SEL);

                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_PC);

                        if (`OPCODE == `INSTR_JMP) begin
                            instrEnd = 1'b1;
                        end else if (microCodeIndex == 4'd5) begin
                            `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC);
                            `SET_CS_RAW(bus_dest_raw, 5'd`CS_CS);
                            
                            instrEnd = 1'b1;
                        end
                    end
                endcase
            end
            // ALU
            `INSTR_ALU: begin // microCodeIndex = 4'd2
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(operand_1_raw, `OP1_ALU_A_SEL);
                        `SET_CS_RAW(operand_2_raw, `OP2_ALU_B_SEL);
                        `SET_CS_RAW(flags_e, 1'd1);

                        if (`OPCODE != `INSTR_CMP) `SET_CS_RAW(operand_0_raw, `OP0_BUS_DEST);

                        /* verilator lint_off WIDTHEXPAND */ /* verilator lint_off WIDTHTRUNC */
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, (`OPCODE - `INSTR_ADD) + 4'd1);
                        /* verilator lint_on WIDTHEXPAND */ /* verilator lint_on WIDTHTRUNC */

                        instrEnd = 1'b1;
                    end
                endcase
                
            end
            // Stack (sp points to current) / Functions
            `INSTR_PUSH: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC2);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`CS_SP);
                        `SET_CS_RAW(bus_dest_special_raw, 4'd`SPECIAL_MEM_SP);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_SS);

                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_EXP1); // Memory
                        `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC);

                        instrEnd = 1'b1;
                    end
                endcase
            end
            `INSTR_POP_RET_F: begin // retf: pop cs then pc
                unique case (microCodeIndex)
                    4'd2, 4'd5: begin
                        `SET_CS_RAW(bus_src_raw, 5'd`CS_SP);
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_MEM);
                    end
                    4'd3, 4'd6: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_SS);

                        `SET_CS_RAW(bus_src_raw, 5'd`CS_EXP1); // Memory

                        if          (`OPCODE == `INSTR_POP) begin
                            `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                        end else if (`OPCODE == `INSTR_RET || microCodeIndex == 4'd6) begin
                            `SET_CS_RAW(bus_dest_raw, 5'd`CS_PC);
                        end else if (`OPCODE == `INSTR_RETF) begin
                            `SET_CS_RAW(bus_dest_raw, 5'd`CS_CS);
                        end
                    end
                    4'd4, 4'd7: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_INC2);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`CS_SP);
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_SP);

                        if (`OPCODE != `INSTR_RETF && microCodeIndex != 4'd7) instrEnd = 1'b1;
                    end
                endcase
            end
            `INSTR_CALL_F: begin // callf: push pc then cs
                unique case (microCodeIndex)
                    4'd2: begin
                        // dec2 sp -> sp, mem / pc_e
                        // ss ex1 = pc
                        // dec2 pc -> mem
                        // cs pc = ex1
                        // add pc, op2 -> pc
                            // dec2 sp -> sp, mem
                            // ss ex1 = cs
                            // cs = op1

                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC2);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`CS_SP);
                        `SET_CS_RAW(bus_dest_special_raw, 4'd`SPECIAL_MEM_SP);
                        `SET_CS_RAW(pc_e, 1'd1);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_SS);

                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_EXP1); // Memory
                        `SET_CS_RAW(bus_src_raw, 5'd`CS_PC);
                    end
                    4'd4: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC2);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`CS_PC);
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_MEM);
                    end
                    4'd5: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);

                        `SET_CS_RAW(bus_src_raw, 5'd`CS_EXP1); // Memory
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_PC);
                    end
                    4'd6: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`CS_PC);
                        `SET_CS_RAW(operand_2_raw, `OP2_ALU_B_SEL);
                        `SET_CS_RAW(bus_dest_raw, 5'd`CS_PC);

                        if (`OPCODE == `INSTR_CALL) instrEnd = 1'b1;
                    end
                endcase

                if          (`OPCODE == `INSTR_CALLF && microCodeIndex == 4'd7) begin
                    `SET_CS_RAW(alu_e_raw, 1'd1);
                    `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC2);

                    `SET_CS_RAW(alu_a_sel_raw, 4'd`CS_SP);
                    `SET_CS_RAW(bus_dest_special_raw, 4'd`SPECIAL_MEM_SP);
                end else if (`OPCODE == `INSTR_CALLF && microCodeIndex == 4'd8) begin
                    `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_SS);

                    `SET_CS_RAW(bus_dest_raw, 5'd`CS_EXP1); // Memory
                    `SET_CS_RAW(bus_src_raw, 5'd`CS_CS);
                end else if (`OPCODE == `INSTR_CALLF && microCodeIndex == 4'd9) begin
                    `SET_CS_RAW(bus_dest_raw, 5'd`CS_CS);
                    `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC);

                    instrEnd = 1'b1;
                end
            end
        endcase

    end
    /* verilator lint_on CASEINCOMPLETE */

    always_ff @(posedge clk or posedge r) begin
        if (r) begin
            microCodeIndex <= 4'd0;
        end
        else if (instrEnd) begin
            microCodeIndex <= 4'd0;
        end
        else begin
            microCodeIndex <= microCodeIndex + 4'd1;
        end
    end

endmodule
