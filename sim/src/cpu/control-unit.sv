`include "src/core.svh"

`define SET_CS_RAW(signal, value) controlSignalsRaw.signal = value; // Set Raw Control Signal

`define OPCODE instr[15:10]

// Instruction Groups
`define INSTR_MEMORY        `INSTR_LDW, `INSTR_STW, `INSTR_LDE, `INSTR_STE      // [src-addr, offset]
`define INSTR_MEMORYR       `INSTR_LDWR, `INSTR_STWR, `INSTR_LDER, `INSTR_STER  // [src-addr]
`define INSTR_MEMORYP       `INSTR_LDWP, `INSTR_STWP, `INSTR_LDEP, `INSTR_STEP  // [src-addr] + Post Increment

`define INSTR_JMPS          `INSTR_JMP, `INSTR_JMPF, `INSTR_BZ, `INSTR_BNZ, `INSTR_BC, `INSTR_BNC, `INSTR_BS, `INSTR_BNS, `INSTR_BO, `INSTR_BNO

`define INSTR_ALU           `INSTR_ADD, `INSTR_SUB, `INSTR_INC, `INSTR_INC2, `INSTR_DEC, `INSTR_DEC2, \
                            `INSTR_NOT, `INSTR_AND, `INSTR_OR, `INSTR_XOR, `INSTR_SLL, `INSTR_SRL, `INSTR_SRA, `INSTR_NEG, `INSTR_CMP

`define INSTR_POP_B_RET_F   `INSTR_POP, `INSTR_POPB, `INSTR_RET, `INSTR_RETF
`define INSTR_CALL_F        `INSTR_CALL, `INSTR_CALLF

// Operand Roles (Use Inside SET_CS_RAW)

// TODO: Impliment Decodes for New Operands
// (00: Off, 01: seg_sel, 10: bus_dest_raw, 11: bus_src_raw)
// (00: Off, 01: bus_dest_raw, 10: bus_src_raw, 01&alu_e_raw: alu-a-sel, 11: bus_dest_raw&alu-a-sel)
// (00: Off, 01: bus_dest_raw, 10: bus_src_raw, 01&alu_e_raw: alu-b-sel, 11&alu_e_raw: alu-a-sel)

`define OP0_SEG_SEL     2'b01
`define OP0_BUS_DEST    2'b10
`define OP0_BUS_SRC     2'b11

`define OP1_BUS_DEST    2'b01
`define OP1_ALU_A_SEL   2'b01 // Use with alu_e_raw Enabled
`define OP1_BUS_SRC     2'b10
`define OP1_BUS_DEST_ALU_A_SEL 2'b11

`define OP2_BUS_DEST    2'b01
`define OP2_ALU_B_SEL   2'b01 // Use with alu_e_raw Enabled
`define OP2_BUS_SRC     2'b10
`define OP2_ALU_A_SEL   2'b11 // Use with alu_e_raw Enabled

// Segments
`define SEL_CS          4'b0001
`define SEL_DS          4'b0010
`define SEL_SS          4'b0100
`define SEL_ES          4'b1000

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
        `SET_CS_RAW(bus_dest_mem_sp_raw      , 1'd0);

        `SET_CS_RAW(seg_sel__alu_op_sel_raw  , 4'd0);
        `SET_CS_RAW(alu_e_raw                , 1'd0);

        `SET_CS_RAW(alu_a_sel_raw            , 4'd0);
        `SET_CS_RAW(alu_b_sel_raw            , 4'd0);

        `SET_CS_RAW(pc_e                     , 1'd0);
        `SET_CS_RAW(flags_e                  , 1'd0);
        `SET_CS_RAW(byte_low                 , 1'd0);

        `SET_CS_RAW(operand_0_raw            , 2'd0);
        `SET_CS_RAW(operand_1_raw            , 2'd0);
        `SET_CS_RAW(operand_2_raw            , 1'd0);

        instrEnd = 1'd0;

        // Instruction Logic

        // Fetch
            // 0: mov pc -> mem
            // 1: ld cs (exp1) -> instr / pc_e
        if          (microCodeIndex == 4'd0) begin
            `SET_CS_RAW(bus_src_raw  , 5'd`R_PC);
            `SET_CS_RAW(bus_dest_raw , 5'd`R_MEM);
        end else if (microCodeIndex == 4'd1) begin
            `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);
            `SET_CS_RAW(bus_src_raw  , 5'd`R_EXP1); // Memory
            `SET_CS_RAW(bus_dest_raw , 5'd`R_INSTR);
            `SET_CS_RAW(pc_e         , 1'd1);
        end

        /* verilator lint_off CASEINCOMPLETE */
        unique case (`OPCODE)
            `INSTR_NOP: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        instrEnd = 1'd1;
                    end
                endcase
            end

            // Moving

            // mov / movi / lea
                // 2: mov op2 -> op1
            // movi / lea
                // 2: mov pc -> mem
                // 3: ld cs (exp1) -> op1 / pc_e
            // lea
                // 4: add op1, op2 -> op1, mem / alu_e
            `INSTR_MOV: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                        `SET_CS_RAW(operand_2_raw, `OP2_BUS_SRC);
                        instrEnd = 1'd1;
                    end
                endcase
            end
            `INSTR_MOVI, `INSTR_LEA: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(bus_src_raw,  5'd`R_PC);
                        `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);
                        `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                        `SET_CS_RAW(bus_src_raw, 5'd`R_EXP1); // Memory
                        `SET_CS_RAW(pc_e, 1'd1);

                        if (`OPCODE != `INSTR_LEA) begin
                            instrEnd = 1'd1;
                        end
                    end
                    4'd4: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`R_MEM);
                        `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST_ALU_A_SEL);
                        `SET_CS_RAW(operand_2_raw, `OP2_ALU_B_SEL);

                        `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);

                        instrEnd = 1'd1;
                    end
                endcase
            end

            // Memory

            // memory (ld(w/b/e) / st(w/b/e))
                // 2: mov pc -> mem
                // 3: ld cs (exp1) -> mem / pc_e
                // 4: add mem, op2 -> mem / alu_e
                // 5: ld/st (op0 / es) (exp1 / op1) <-> (op1 / op0) / (b -> byte_low)
            // memoryr (register only)
                // 2: mov op2 -> mem
                // 3: ld/st (op0 / es) (exp1 / op1) <-> (op1 / op0) / (b -> byte_low)
            // memoryp (register / post increment)
                // 4: inc(2) mem -> op2
            `INSTR_MEMORY, `INSTR_MEMORYR, `INSTR_MEMORYP: begin
                unique case (`OPCODE)
                    `INSTR_MEMORY: begin
                        if (microCodeIndex != 4'd5) begin
                            unique case (microCodeIndex)
                                4'd2: begin
                                    `SET_CS_RAW(bus_src_raw,  5'd`R_PC);
                                    `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                                end
                                4'd3: begin
                                    `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);
                                    `SET_CS_RAW(bus_src_raw, 5'd`R_EXP1); // Memory
                                    `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                                    
                                    `SET_CS_RAW(pc_e, 1'd1);
                                end
                                4'd4: begin
                                    `SET_CS_RAW(alu_e_raw, 1'd1);
                                    `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_ADD);

                                    `SET_CS_RAW(alu_a_sel_raw, 4'd`R_MEM);
                                    `SET_CS_RAW(operand_2_raw, `OP2_ALU_B_SEL);

                                    `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                                end
                            endcase
                        end
                    end
                    `INSTR_MEMORYR, `INSTR_MEMORYP: begin
                        if (microCodeIndex == 4'd2) begin
                            `SET_CS_RAW(operand_2_raw, `OP2_BUS_SRC);
                            `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                        end
                        else if (microCodeIndex == 4'd3 || microCodeIndex == 4'd5) begin
                            unique case (`OPCODE)
                                `INSTR_LDW, `INSTR_STW, `INSTR_LDB, `INSTR_STB:   begin `SET_CS_RAW(operand_0_raw, `OP0_SEG_SEL); end
                                `INSTR_LDE, `INSTR_STE, `INSTR_LDEB, `INSTR_STEB: begin `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_ES); end
                            endcase

                            if (`OPCODE == `INSTR_LDB || `OPCODE == `INSTR_STB || `OPCODE == `INSTR_LDEB || `OPCODE == `INSTR_STEB) begin
                                `SET_CS_RAW(byte_low, 1'd1);
                            end

                            unique case (`OPCODE)
                                `INSTR_LDW, `INSTR_LDB: begin
                                    `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                                    `SET_CS_RAW(bus_src_raw, 5'd`R_EXP1); // Memory
                                end
                                `INSTR_LDE, `INSTR_LDEB: begin
                                    `SET_CS_RAW(operand_0_raw, `OP0_BUS_DEST);
                                    `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC);
                                end
                                `INSTR_STW, `INSTR_STB: begin
                                    `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC);
                                    `SET_CS_RAW(bus_dest_raw, 5'd`R_EXP1); // Memory
                                end
                                `INSTR_STE, `INSTR_STEB: begin
                                    `SET_CS_RAW(operand_0_raw, `OP0_BUS_SRC);
                                    `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                                end
                            endcase

                            unique case (`OPCODE) 
                                `INSTR_MEMORY, `INSTR_MEMORYR: begin instrEnd = 1'd1; end
                                `INSTR_MEMORYP: begin end
                            endcase
                        end
                        else if (microCodeIndex == 4'd4) begin
                            `SET_CS_RAW(alu_e_raw, 1'd1);

                            unique case (`OPCODE) 
                                `INSTR_LDBP, `INSTR_STBP, `INSTR_LDEBP, `INSTR_STEBP: begin `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_INC); end
                                `INSTR_LDWP, `INSTR_STWP, `INSTR_LDEP, `INSTR_STEP: begin `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_INC2); end
                            endcase

                            `SET_CS_RAW(operand_2_raw, `OP2_ALU_A_SEL);
                            `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                        end
                    end
                endcase
            end

            // Jump / Branch Conditionss
            
            // b...
                // 2: if not flag -> instrEnd
            // jmp / jmpf / b...
                // 2: mov pc -> mem
                // 3: ld cs (exp1) -> pc
                // 4: add pc, op2 -> pc / alu_e
            // jmpf
                // 5: 
            `INSTR_JMPS: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        case (`OPCODE)
                            `INSTR_BZ: begin if (!flags[`F_ZERO])       begin `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'd1; end end
                            `INSTR_BNZ: begin if (flags[`F_ZERO])       begin `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'd1; end end
                            `INSTR_BC: begin if (!flags[`F_CARRY])      begin `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'd1; end end
                            `INSTR_BNC: begin if (flags[`F_CARRY])      begin `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'd1; end end
                            `INSTR_BS: begin if (!flags[`F_SIGN])       begin `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'd1; end end
                            `INSTR_BNS: begin if (flags[`F_SIGN])       begin `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'd1; end end
                            `INSTR_BO: begin if (!flags[`F_OVERFLOW])   begin `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'd1; end end
                            `INSTR_BNO: begin if (flags[`F_OVERFLOW])   begin `SET_CS_RAW(pc_e,  1'd1); instrEnd = 1'd1; end end
                        endcase
                        
                        `SET_CS_RAW(bus_src_raw,  5'd`R_PC);
                        `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);

                        `SET_CS_RAW(bus_src_raw, 5'd`R_EXP1); // Memory
                        `SET_CS_RAW(bus_dest_raw, 5'd`R_PC);
                    end
                    4'd4: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`R_PC);
                        `SET_CS_RAW(operand_2_raw, `OP2_ALU_B_SEL);

                        `SET_CS_RAW(bus_dest_raw, 5'd`R_PC);

                        if (`OPCODE != `INSTR_JMPF) begin
                            instrEnd = 1'd1;
                        end
                    end
                    4'd5: begin
                        `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC);
                        `SET_CS_RAW(bus_dest_raw, 5'd`R_CS);
                        
                        instrEnd = 1'd1; 
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

                        if (`OPCODE != `INSTR_CMP) begin
                            `SET_CS_RAW(operand_0_raw, `OP0_BUS_DEST);

                            /* verilator lint_off WIDTHEXPAND */ /* verilator lint_off WIDTHTRUNC */
                            `SET_CS_RAW(seg_sel__alu_op_sel_raw, (`OPCODE - `INSTR_ADD) + 4'd1);
                            /* verilator lint_on WIDTHEXPAND */ /* verilator lint_on WIDTHTRUNC */
                        end else begin
                            `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_SUB);
                        end

                        instrEnd = 1'd1;
                    end
                endcase
            end

            // Stack (sp points to current) / Functions
            
            // push / pushb
                // 2: dec(2) sp -> mem, sp / alu_e
                // 3: st ss (exp1) <- op1

            // pop/popb / ret/retf
                // 2: mov sp -> mem
                // 3: ld ss (exp1) -> (op1 / (ret -> pc) / (retf -> cs)) / b -> byte_low
                // 4: inc(2) sp -> sp, mem
            // retf
                // 5: ld ss (exp1) -> pc
                // 6: inc(2) sp -> sp, mem
            
            // call / callf
                // 2: dec2 sp -> sp, mem / pc_e
                // 3: st ss (exp1) <- pc
                // 4: dec2 pc -> mem
                // 5: ld cs (exp1) -> pc
                // 6: add pc, op2 -> pc
            // callf
                // 7: dec2 sp -> sp, mem
                // 8: st ss (exp1) <- cs
                // 9: mov op1 -> cs

            `INSTR_PUSH, `INSTR_PUSHB: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);

                        unique case (`OPCODE)
                            `INSTR_PUSH:  begin `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC2); end
                            `INSTR_PUSHB: begin `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC); end
                        endcase

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`R_SP);
                        `SET_CS_RAW(bus_dest_mem_sp_raw, 1'd1);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_SS);

                        if (`OPCODE == `INSTR_PUSHB) `SET_CS_RAW(byte_low, 1'd1);

                        `SET_CS_RAW(bus_dest_raw, 5'd`R_EXP1); // Memory
                        `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC);

                        instrEnd = 1'd1;
                    end
                endcase
            end
            `INSTR_POP_B_RET_F: begin // retf: pop cs then pc
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(bus_src_raw, 5'd`R_SP);
                        `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                    end
                    4'd3, 4'd5: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_SS);

                        `SET_CS_RAW(bus_src_raw, 5'd`R_EXP1); // Memory

                        if          (`OPCODE == `INSTR_POP) begin
                            `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                        end else if          (`OPCODE == `INSTR_POPB) begin
                            `SET_CS_RAW(operand_1_raw, `OP1_BUS_DEST);
                            `SET_CS_RAW(byte_low, 1'd1);
                        end else if (`OPCODE == `INSTR_RET || microCodeIndex == 4'd5) begin
                            `SET_CS_RAW(bus_dest_raw, 5'd`R_PC);
                        end else if (`OPCODE == `INSTR_RETF) begin
                            `SET_CS_RAW(bus_dest_raw, 5'd`R_CS);
                        end
                    end
                    4'd4, 4'd6: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);

                        unique case (`OPCODE)
                            `INSTR_POP, `INSTR_RET, `INSTR_RETF:    begin `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_INC2); end
                            `INSTR_POPB:                            begin `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_INC); end
                        endcase

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`R_SP);
                        `SET_CS_RAW(bus_dest_mem_sp_raw, 5'd`R_SP);

                        if (`OPCODE != `INSTR_RETF && microCodeIndex != 4'd7) instrEnd = 1'd1;
                    end
                endcase
            end
            `INSTR_CALL_F: begin // callf: push pc then cs
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC2);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`R_SP);
                        `SET_CS_RAW(bus_dest_mem_sp_raw, 1'd1);
                        `SET_CS_RAW(pc_e, 1'd1);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_SS);

                        `SET_CS_RAW(bus_dest_raw, 5'd`R_EXP1); // Memory
                        `SET_CS_RAW(bus_src_raw, 5'd`R_PC);
                    end
                    4'd4: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC2);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`R_PC);
                        `SET_CS_RAW(bus_dest_raw, 5'd`R_MEM);
                    end
                    4'd5: begin
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_CS);

                        `SET_CS_RAW(bus_src_raw, 5'd`R_EXP1); // Memory
                        `SET_CS_RAW(bus_dest_raw, 5'd`R_PC);
                    end
                    4'd6: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, 4'd`R_PC);
                        `SET_CS_RAW(operand_2_raw, `OP2_ALU_B_SEL);
                        `SET_CS_RAW(bus_dest_raw, 5'd`R_PC);

                        if (`OPCODE == `INSTR_CALL) instrEnd = 1'd1;
                    end
                endcase

                if          (`OPCODE == `INSTR_CALLF && microCodeIndex == 4'd7) begin
                    `SET_CS_RAW(alu_e_raw, 1'd1);
                    `SET_CS_RAW(seg_sel__alu_op_sel_raw, `ALU_DEC2);

                    `SET_CS_RAW(alu_a_sel_raw, 4'd`R_SP);
                    `SET_CS_RAW(bus_dest_mem_sp_raw, 1'd1);
                end else if (`OPCODE == `INSTR_CALLF && microCodeIndex == 4'd8) begin
                    `SET_CS_RAW(seg_sel__alu_op_sel_raw, `SEL_SS);

                    `SET_CS_RAW(bus_dest_raw, 5'd`R_EXP1); // Memory
                    `SET_CS_RAW(bus_src_raw, 5'd`R_CS);
                end else if (`OPCODE == `INSTR_CALLF && microCodeIndex == 4'd9) begin
                    `SET_CS_RAW(bus_dest_raw, 5'd`R_CS);
                    `SET_CS_RAW(operand_1_raw, `OP1_BUS_SRC);

                    instrEnd = 1'd1;
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
