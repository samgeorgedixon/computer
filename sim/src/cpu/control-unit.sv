`include "src/core.svh"

module ControlUnit(

    input logic clk, r,
    
    input logic [15:0] instr,
    input logic [7:0] flags,

    ControlSignals_if.control controlSignalsRaw // TODO: Change Names

);

    logic [3:0] microCodeIndex; // Max 16
    logic       instrEnd;

    always_comb begin

        // Defaults
        controlSignalsRaw.bus_src_raw               = 5'd0;
        controlSignalsRaw.bus_dest_raw              = 5'd0;
        controlSignalsRaw.bus_dest_special_raw      = 4'd0;

        controlSignalsRaw.seg_sel__alu_op_sel_raw   = 4'd0;
        controlSignalsRaw.alu_e_raw                 = 1'd0;

        controlSignalsRaw.alu_a_sel_raw             = 4'd0;
        controlSignalsRaw.alu_b_sel_raw             = 4'd0;

        controlSignalsRaw.pc_e                      = 1'd0;
        controlSignalsRaw.flags_e                   = 1'd0;

        controlSignalsRaw.operand_0_raw             = 2'd0;
        controlSignalsRaw.operand_1_raw             = 4'd0;
        controlSignalsRaw.operand_2_raw             = 4'd0;

        instrEnd = 1'b0;

        // Instruction Logic

        if (microCodeIndex == 4'd0) begin
            bus_src  = 5'd`CS_PC;
            bus_dest = 5'd`CS_MEM;
        end
        else if (microCodeIndex == 4'd1) begin
            bus_src  = 5'd`CS_EXP2; // ram
            bus_dest = 5'd`CS_INSTR;
            pc_e     = 1'b1;
        end

        case (instr[5:0]) // Opcode
            6'd0: begin // nop
                case (microCodeIndex)
                    4'd2: begin
                        instrEnd = 1'b1;
                    end
                endcase
            end

            6'd1: begin // mov
                case (microCodeIndex)
                    4'd2: begin
                        instrEnd = 1'b1;
                    end
                endcase
            end

            6'd2: begin // li
                case (microCodeIndex)
                    4'd2: begin
                        instrEnd = 1'b1;
                    end
                endcase
            end
        endcase

    end

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