`include "src/core.svh"

`define SET_CS_RAW(signal, value) controlSignalsRaw.signal = value; // Set Raw Control Signal

// Instruction Groups
`define INSTR_MEMORY        `INSTR_LDW, `INSTR_STW, `INSTR_LDXW, `INSTR_STXW, \
                            `INSTR_LDB, `INSTR_STB, `INSTR_LDXB, `INSTR_STXB        // [src-addr, offset]
`define INSTR_MEMORYR       `INSTR_LDWR, `INSTR_STWR, `INSTR_LDXWR, `INSTR_STXWR, \
                            `INSTR_LDBR, `INSTR_STBR, `INSTR_LDXBR, `INSTR_STXBR    // [src-addr]
`define INSTR_MEMORYP       `INSTR_LDWP, `INSTR_STWP, `INSTR_LDXWP, `INSTR_STXWP, \
                            `INSTR_LDBP, `INSTR_STBP, `INSTR_LDXBP, `INSTR_STXBP    // [src-addr] + Post Increment   

`define INSTR_JMPS          `INSTR_JMP, `INSTR_JMPF, `INSTR_B, `INSTR_BN

`define INSTR_ALU           `INSTR_ADD, `INSTR_SUB, `INSTR_INC, `INSTR_INC2, `INSTR_DEC, `INSTR_DEC2, \
                            `INSTR_NOT, `INSTR_AND, `INSTR_OR, `INSTR_XOR, `INSTR_SLL, `INSTR_SRL, `INSTR_SRA, `INSTR_NEG, `INSTR_CMP

`define INSTR_POP_B_RET_F   `INSTR_POP, `INSTR_POPB, `INSTR_RET, `INSTR_RETF
`define INSTR_CALL_F        `INSTR_CALL, `INSTR_CALLF

module ControlUnit(

    input logic clk, r,
    
    input logic [15:0] instr,
    input logic [7:0] flags,

    input logic irq_state,

    ControlSignals_if.control controlSignalsRaw

);

    logic [3:0] microCodeIndex; // Max 16
    logic       instrEnd;

    logic       irq_state_delayed;
    logic       inInterrupt;

    always_comb begin

        // Defaults
        `SET_CS_RAW(bus_src_raw             , 5'd0);
        `SET_CS_RAW(bus_dest_raw            , 5'd0);
        `SET_CS_RAW(bus_dest_ar_and_sp      , 1'd0);

        `SET_CS_RAW(seg_sel_or_alu_op_sel   , 4'd0);
        `SET_CS_RAW(alu_e_raw               , 1'd0);

        `SET_CS_RAW(alu_a_sel_raw           , 4'd0);
        `SET_CS_RAW(alu_b_sel_raw           , 4'd0);

        `SET_CS_RAW(ip_e                    , 1'd0);
        `SET_CS_RAW(flags_e                 , 1'd0);
        `SET_CS_RAW(byte_low                , 1'd0);

        `SET_CS_RAW(operand_0               , 2'd0);
        `SET_CS_RAW(operand_1               , 2'd0);
        `SET_CS_RAW(operand_2               , 2'd0);

        instrEnd = 1'd0;

        // Instruction Logic

        // Interupts

        // push r1
        // push r2
        // push r3
        
        //   push r4
        //   push ds
        //   push es

        // callf (push cs, ip)

        // mov r1 sp
        // mov r2 bp
        // mov r3 ss

        // Interrupt Stack
        // li ss 10
        // li bp 0xff
        // mov sp bp

        // push r1
        // push r2
        // push r3

        // Interrupt Program

        // pop ss
        // pop bp
        // pop sp



        // callf int_handler

        // 0: dec2 sp -> sp, ar
        // 1: st ss (exp1) <- cs
        // 2: dec2 sp -> sp, ar
        // 3: st ss (exp1) <- ip
        // 4: mov cs <- z
        // 5: mov ip <- 0x80 ?? constant?


        // 6: dec2 sp -> sp, ar
        // 7: st ss (exp1) <- r1
        // 8: dec2 sp -> sp, ar
        // 9: st ss (exp1) <- r2
        // 10: dec2 sp -> sp, ar
        // 11: st ss (exp1) <- r3
        // 12: mov r1 <- sp
        // 13: mov r2 <- bp
        // 14: mov r3 <- ss

        // Interrupt Handler
        // push r4

        /* verilator lint_off CASEINCOMPLETE */
        if (irq_state_delayed == 1'd1 && !inInterrupt) begin
            unique case (microCodeIndex)
                4'd0: begin
                    //instrEnd = 1'd1;
                end
            endcase
        end

        // Fetch
            // 0: mov ip -> ar
            // 1: ld cs (exp1) -> instr / ip_e
        if     (microCodeIndex == 4'd0) begin
            `SET_CS_RAW(bus_src_raw  , `R_IP);
            `SET_CS_RAW(bus_dest_raw , `R_AR);
        end else if (microCodeIndex == 4'd1) begin
            `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_CS);
            `SET_CS_RAW(bus_src_raw  , `R_XU1); // Memory
            `SET_CS_RAW(bus_dest_raw , `R_IR);
            `SET_CS_RAW(ip_e         , 1'd1);
        end

        else begin
        unique case (`opcode)
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
                // 2: mov ip -> ar
                // 3: ld cs (exp1) -> op1 / ip_e
            // lea
                // 4: add op1, op2 -> op1, ar / alu_e
            `INSTR_MOV: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(operand_1, `OP1_BUS_DEST);
                        `SET_CS_RAW(operand_2, `OP2_BUS_SRC_OR_ALU_B_SEL);
                        instrEnd = 1'd1;
                    end
                endcase
            end
            `INSTR_MOVI, `INSTR_LEA: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(bus_src_raw,  `R_IP);
                        `SET_CS_RAW(bus_dest_raw, `R_AR);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_CS);
                        `SET_CS_RAW(operand_1, `OP1_BUS_DEST);
                        `SET_CS_RAW(bus_src_raw, `R_XU1); // Memory
                        `SET_CS_RAW(ip_e, 1'd1);

                        if (`opcode != `INSTR_LEA) begin
                            instrEnd = 1'd1;
                        end
                    end
                    4'd4: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, `R_AR);
                        `SET_CS_RAW(operand_1, `OP1_BUS_DEST_AND_ALU_A_SEL);
                        `SET_CS_RAW(operand_2, `OP2_BUS_SRC_OR_ALU_B_SEL);

                        `SET_CS_RAW(bus_dest_raw, `R_AR);

                        instrEnd = 1'd1;
                    end
                endcase
            end

            // Memory

            // arory (ld(w/b/e) / st(w/b/e))
                // 2: mov ip -> ar
                // 3: ld cs (exp1) -> ar / ip_e
                // 4: add ar, op2 -> ar / alu_e
                // 5: ld/st (op0 / es) (exp1 / op1) <-> (op1 / op0) / (b -> byte_low)
            // aroryr (register only)
                // 2: mov op2 -> ar
                // 3: ld/st (op0 / es) (exp1 / op1) <-> (op1 / op0) / (b -> byte_low)
            // aroryp (register / post increment)
                // 4: inc(2) ar -> op2
            `INSTR_MEMORY, `INSTR_MEMORYR, `INSTR_MEMORYP: begin
                unique case (`opcode)
                    `INSTR_MEMORY: begin
                        unique case (microCodeIndex)
                            4'd2: begin
                                `SET_CS_RAW(bus_src_raw,  `R_IP);
                                `SET_CS_RAW(bus_dest_raw, `R_AR);
                            end
                            4'd3: begin
                                `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_CS);
                                `SET_CS_RAW(bus_src_raw, `R_XU1); // Memory
                                `SET_CS_RAW(bus_dest_raw, `R_AR);
                                
                                `SET_CS_RAW(ip_e, 1'd1);
                            end
                            4'd4: begin
                                `SET_CS_RAW(alu_e_raw, 1'd1);
                                `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_ADD);

                                `SET_CS_RAW(alu_a_sel_raw, `R_AR);
                                `SET_CS_RAW(operand_2, `OP2_BUS_SRC_OR_ALU_B_SEL);

                                `SET_CS_RAW(bus_dest_raw, `R_AR);
                            end
                            4'd5: begin
                                unique case (`opcode)
                                    `INSTR_LDW, `INSTR_STW, `INSTR_LDB, `INSTR_STB: begin
                                        `SET_CS_RAW(operand_0, `OP0_SEG_SEL);
                                    end
                                    `INSTR_LDXW, `INSTR_STXW, `INSTR_LDXB, `INSTR_STXB: begin
                                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_ES);
                                    end
                                endcase

                                if (`opcode == `INSTR_LDB || `opcode == `INSTR_STB || `opcode == `INSTR_LDXB || `opcode == `INSTR_STXB) begin
                                    `SET_CS_RAW(byte_low, 1'd1);
                                end

                                unique case (`opcode)
                                    `INSTR_LDW, `INSTR_LDB: begin
                                        `SET_CS_RAW(operand_1, `OP1_BUS_DEST);
                                        `SET_CS_RAW(bus_src_raw, `R_XU1); // Memory
                                    end
                                    `INSTR_LDXW, `INSTR_LDXB: begin
                                        `SET_CS_RAW(operand_0, `OP0_BUS_DEST);
                                        `SET_CS_RAW(operand_1, `OP1_BUS_SRC_OR_ALU_A_SEL);
                                    end
                                    `INSTR_STW, `INSTR_STB: begin
                                        `SET_CS_RAW(operand_1, `OP1_BUS_SRC_OR_ALU_A_SEL);
                                        `SET_CS_RAW(bus_dest_raw, `R_XU1); // Memory
                                    end
                                    `INSTR_STXW, `INSTR_STXB: begin
                                        `SET_CS_RAW(operand_0, `OP0_BUS_SRC);
                                        `SET_CS_RAW(operand_1, `OP1_BUS_DEST);
                                    end
                                endcase

                                instrEnd = 1'd1;
                            end
                        endcase
                    end
                    `INSTR_MEMORYR, `INSTR_MEMORYP: begin
                        if (microCodeIndex == 4'd2) begin
                            `SET_CS_RAW(operand_2, `OP2_BUS_SRC_OR_ALU_B_SEL);
                            `SET_CS_RAW(bus_dest_raw, `R_AR);
                        end
                        else if (microCodeIndex == 4'd3) begin
                            unique case (`opcode)
                                `INSTR_LDWR, `INSTR_STWR, `INSTR_LDBR, `INSTR_STBR,
                                `INSTR_LDWP, `INSTR_STWP, `INSTR_LDBP, `INSTR_STBP: begin
                                    `SET_CS_RAW(operand_0, `OP0_SEG_SEL);
                                end
                                `INSTR_LDXWR, `INSTR_STXWR, `INSTR_LDXBR, `INSTR_STXBR,
                                `INSTR_LDXWP, `INSTR_STXWP, `INSTR_LDXBP, `INSTR_STXBP: begin
                                    `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_ES);
                                end
                            endcase

                            if (`opcode == `INSTR_LDBR || `opcode == `INSTR_STBR || `opcode == `INSTR_LDXBR || `opcode == `INSTR_STXBR ||
                                `opcode == `INSTR_LDBP || `opcode == `INSTR_STBP || `opcode == `INSTR_LDXBP || `opcode == `INSTR_STXBP) begin
                                `SET_CS_RAW(byte_low, 1'd1);
                            end

                            unique case (`opcode)
                                `INSTR_LDWR, `INSTR_LDBR, `INSTR_LDWP, `INSTR_LDBP: begin
                                    `SET_CS_RAW(operand_1, `OP1_BUS_DEST);
                                    `SET_CS_RAW(bus_src_raw, `R_XU1); // Memory
                                end
                                `INSTR_LDXWR, `INSTR_LDXBR, `INSTR_LDXWP, `INSTR_LDXBP: begin
                                    `SET_CS_RAW(operand_0, `OP0_BUS_DEST);
                                    `SET_CS_RAW(operand_1, `OP1_BUS_SRC_OR_ALU_A_SEL);
                                end
                                `INSTR_STWR, `INSTR_STBR, `INSTR_STWP, `INSTR_STBP: begin
                                    `SET_CS_RAW(operand_1, `OP1_BUS_SRC_OR_ALU_A_SEL);
                                    `SET_CS_RAW(bus_dest_raw, `R_XU1); // Memory
                                end
                                `INSTR_STXWR, `INSTR_STXBR, `INSTR_STXWP, `INSTR_STXBP: begin
                                    `SET_CS_RAW(operand_0, `OP0_BUS_SRC);
                                    `SET_CS_RAW(operand_1, `OP1_BUS_DEST);
                                end
                            endcase

                            unique case (`opcode) 
                                `INSTR_MEMORYR: begin instrEnd = 1'd1; end
                                `INSTR_MEMORYP: begin end
                            endcase
                        end
                        else if (microCodeIndex == 4'd4) begin
                            `SET_CS_RAW(alu_e_raw, 1'd1);

                            unique case (`opcode)
                                `INSTR_LDBP, `INSTR_STBP, `INSTR_LDXBP, `INSTR_STXBP: begin `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_INC); end
                                `INSTR_LDWP, `INSTR_STWP, `INSTR_LDXWP, `INSTR_STXWP: begin `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_INC2); end
                            endcase

                            `SET_CS_RAW(alu_a_sel_raw, `R_AR);
                            `SET_CS_RAW(operand_2, `OP2_BUS_DEST);

                            instrEnd = 1'd1;
                        end
                    end
                endcase
            end

            // Jump / Branch Conditionss
            
            // b...
                // 2: if not flag -> instrEnd
            // jmp / jmpf / b...
                // 2: mov ip -> ar
                // 3: ld cs (exp1) -> ip
                // 4: add ip, op2 -> ip / alu_e
            // jmpf
                // 5: 
            `INSTR_JMPS: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        /* verilator lint_off WIDTHTRUNC */
                        case (`opcode)
                            `INSTR_B: begin if (!flags[`operand_1])       begin `SET_CS_RAW(ip_e,  1'd1); instrEnd = 1'd1; end end
                            `INSTR_BN: begin if (flags[`operand_1])       begin `SET_CS_RAW(ip_e,  1'd1); instrEnd = 1'd1; end end
                        endcase
                        /* verilator lint_on WIDTHTRUNC */

                        `SET_CS_RAW(bus_src_raw,  `R_IP);
                        `SET_CS_RAW(bus_dest_raw, `R_AR);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_CS);

                        `SET_CS_RAW(bus_src_raw, `R_XU1); // Memory
                        `SET_CS_RAW(bus_dest_raw, `R_IP);
                    end
                    4'd4: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, `R_IP);
                        `SET_CS_RAW(operand_2, `OP2_BUS_SRC_OR_ALU_B_SEL);

                        `SET_CS_RAW(bus_dest_raw, `R_IP);

                        if (`opcode != `INSTR_JMPF) begin
                            instrEnd = 1'd1;
                        end
                    end
                    4'd5: begin
                        `SET_CS_RAW(operand_1, `OP1_BUS_SRC_OR_ALU_A_SEL);
                        `SET_CS_RAW(bus_dest_raw, `R_CS);
                        
                        instrEnd = 1'd1; 
                    end
                endcase
            end

            // ALU
            `INSTR_ALU: begin // microCodeIndex = 4'd2
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(operand_1, `OP1_BUS_SRC_OR_ALU_A_SEL);
                        `SET_CS_RAW(operand_2, `OP2_BUS_SRC_OR_ALU_B_SEL);
                        `SET_CS_RAW(flags_e, 1'd1);

                        if (`opcode != `INSTR_CMP) begin
                            `SET_CS_RAW(operand_0, `OP0_BUS_DEST);

                            /* verilator lint_off WIDTHEXPAND */ /* verilator lint_off WIDTHTRUNC */
                            `SET_CS_RAW(seg_sel_or_alu_op_sel, (`opcode - `INSTR_ADD) + 4'd1);
                            /* verilator lint_on WIDTHEXPAND */ /* verilator lint_on WIDTHTRUNC */
                        end else begin
                            `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_SUB);
                        end

                        instrEnd = 1'd1;
                    end
                endcase
            end

            // Stack (sp points to current) / Functions
            
            // push / pushb
                // 2: dec(2) sp -> ar, sp / alu_e
                // 3: st ss (exp1) <- op1

            // pop/popb / ret/retf
                // 2: mov sp -> ar
                // 3: ld ss (exp1) -> (op1 / (ret -> ip) / (retf -> cs)) / b -> byte_low
                // 4: inc(2) sp -> sp, ar
            // retf
                // 5: ld ss (exp1) -> ip
                // 6: inc(2) sp -> sp, ar
            
            // call / callf
                // 2: dec2 sp -> sp, ar / ip_e
                // 3: st ss (exp1) <- ip
                // 4: dec2 ip -> ar
                // 5: ld cs (exp1) -> ip
                // 6: add ip, op2 -> ip
            // callf
                // 7: dec2 sp -> sp, ar
                // 8: st ss (exp1) <- cs
                // 9: mov op1 -> cs

            `INSTR_PUSH, `INSTR_PUSHB: begin
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);

                        unique case (`opcode)
                            `INSTR_PUSH:  begin `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_DEC2); end
                            `INSTR_PUSHB: begin `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_DEC); end
                        endcase

                        `SET_CS_RAW(alu_a_sel_raw, `R_SP);
                        `SET_CS_RAW(bus_dest_ar_and_sp, 1'd1);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_SS);

                        if (`opcode == `INSTR_PUSHB) `SET_CS_RAW(byte_low, 1'd1);

                        `SET_CS_RAW(bus_dest_raw, `R_XU1); // Memory
                        `SET_CS_RAW(operand_1, `OP1_BUS_SRC_OR_ALU_A_SEL);

                        instrEnd = 1'd1;
                    end
                endcase
            end
            `INSTR_POP_B_RET_F: begin // retf: pop cs then ip
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(bus_src_raw, `R_SP);
                        `SET_CS_RAW(bus_dest_raw, `R_AR);
                    end
                    4'd3, 4'd5: begin
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_SS);

                        `SET_CS_RAW(bus_src_raw, `R_XU1); // Memory

                        if          (`opcode == `INSTR_POP) begin
                            `SET_CS_RAW(operand_1, `OP1_BUS_DEST);
                        end else if          (`opcode == `INSTR_POPB) begin
                            `SET_CS_RAW(operand_1, `OP1_BUS_DEST);
                            `SET_CS_RAW(byte_low, 1'd1);
                        end else if (`opcode == `INSTR_RET || microCodeIndex == 4'd5) begin
                            `SET_CS_RAW(bus_dest_raw, `R_IP);
                        end else if (`opcode == `INSTR_RETF) begin
                            `SET_CS_RAW(bus_dest_raw, `R_CS);
                        end
                    end
                    4'd4, 4'd6: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);

                        unique case (`opcode)
                            `INSTR_POP, `INSTR_RET, `INSTR_RETF:    begin `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_INC2); end
                            `INSTR_POPB:                            begin `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_INC); end
                        endcase

                        `SET_CS_RAW(alu_a_sel_raw, `R_SP);
                        `SET_CS_RAW(bus_dest_ar_and_sp, 1'd1);

                        if (`opcode != `INSTR_RETF && microCodeIndex != 4'd7) instrEnd = 1'd1;
                    end
                endcase
            end
            `INSTR_CALL_F: begin // callf: push ip then cs
                unique case (microCodeIndex)
                    4'd2: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_DEC2);

                        `SET_CS_RAW(alu_a_sel_raw, `R_SP);
                        `SET_CS_RAW(bus_dest_ar_and_sp, 1'd1);
                        `SET_CS_RAW(ip_e, 1'd1);
                    end
                    4'd3: begin
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_SS);

                        `SET_CS_RAW(bus_dest_raw, `R_XU1); // Memory
                        `SET_CS_RAW(bus_src_raw, `R_IP);
                    end
                    4'd4: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_DEC2);

                        `SET_CS_RAW(alu_a_sel_raw, `R_IP);
                        `SET_CS_RAW(bus_dest_raw, `R_AR);
                    end
                    4'd5: begin
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_CS);

                        `SET_CS_RAW(bus_src_raw, `R_XU1); // Memory
                        `SET_CS_RAW(bus_dest_raw, `R_IP);
                    end
                    4'd6: begin
                        `SET_CS_RAW(alu_e_raw, 1'd1);
                        `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_ADD);

                        `SET_CS_RAW(alu_a_sel_raw, `R_IP);
                        `SET_CS_RAW(operand_2, `OP2_BUS_SRC_OR_ALU_B_SEL);
                        `SET_CS_RAW(bus_dest_raw, `R_IP);

                        if (`opcode == `INSTR_CALL) instrEnd = 1'd1;
                    end
                endcase

                if          (`opcode == `INSTR_CALLF && microCodeIndex == 4'd7) begin
                    `SET_CS_RAW(alu_e_raw, 1'd1);
                    `SET_CS_RAW(seg_sel_or_alu_op_sel, `ALU_DEC2);

                    `SET_CS_RAW(alu_a_sel_raw, `R_SP);
                    `SET_CS_RAW(bus_dest_ar_and_sp, 1'd1);
                end else if (`opcode == `INSTR_CALLF && microCodeIndex == 4'd8) begin
                    `SET_CS_RAW(seg_sel_or_alu_op_sel, `SEL_SS);

                    `SET_CS_RAW(bus_dest_raw, `R_XU1); // Memory
                    `SET_CS_RAW(bus_src_raw, `R_CS);
                end else if (`opcode == `INSTR_CALLF && microCodeIndex == 4'd9) begin
                    `SET_CS_RAW(bus_dest_raw, `R_CS);
                    `SET_CS_RAW(operand_1, `OP1_BUS_SRC_OR_ALU_A_SEL);

                    instrEnd = 1'd1;
                end
            end

            // Interrupts

            // int [index]
                // push r1
                // push r2
                // push r3
                // push r4

                // push bp

                // push ds
                // push es

                // push cs
                // push ip

                    // if nestedInterrupt
                    // mov r1 sp
                    // mov r2 ss
                // swap sp, ss registers
                    // if nestedInterrupt
                    // push sp from r1, r2 stackAddress
                    // push ss from r1, r2 stackAddress
                // ld cs (exp1) -> ar
                // jmpf zs ar

            // rint
                // swap sp, ss registers
                    // if nestedInterrupt
                    // pop r2
                    // pop r1
                // swap sp, ss registers
                    // if nestedInterrupt
                    // mov sp r1
                    // pop ss r2
                // swap sp, ss registers
                
                // pop ip
                // pop cs
                
                // pop es
                // pop ds

                // pop bp
                
                // pop r4
                // pop r3
                // pop r2
                // pop r1
            
        endcase
        /* verilator lint_on CASEINCOMPLETE */

        end

    end

    always_ff @(posedge clk or posedge r) begin
        if (r) begin
            microCodeIndex <= 4'd0;
        end
        else if (instrEnd) begin
            microCodeIndex <= 4'd0;

            irq_state_delayed <= irq_state;
        end
        else begin
            microCodeIndex <= microCodeIndex + 4'd1;
        end
    end

endmodule
