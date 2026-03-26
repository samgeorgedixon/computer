`include "src/core.svh"

module ControlUnit(

    input logic clk, r,
    input logic [15:0] instr,
    input logic [7:0] flags,

    output logic [4:0] bus_src,
    output logic [4:0] bus_dest,
    output logic [3:0] bus_dest_special,

    output logic [3:0] alu_op_sel__seg_sel,
    output logic       alu__seg_pc,

    output logic [3:0] alu_a_sel,
    output logic [3:0] alu_b_sel,

    output logic pc_e,
    output logic flags_e

);

    logic [3:0] microCodeIndex;
    logic       instrEnd;

    always_comb begin

        // Defaults
        bus_src              = 5'd0;
        bus_dest             = 5'd0;
        bus_dest_special     = 4'd0;

        alu_op_sel__seg_sel  = 4'd0;
        alu__seg_pc          = 1'b0;

        alu_a_sel            = 4'd0;
        alu_b_sel            = 4'd0;

        pc_e                 = 1'b0;
        flags_e              = 1'b0;

        instrEnd             = 1'b0;

        if (microCodeIndex == 4'd0) begin
            bus_src  = 5'd`CS_PC;
            bus_dest = 5'd`CS_MEM;
        end
        else if (microCodeIndex == 4'd1) begin
            bus_src  = 5'd16; // 16? = ram
            bus_dest = 5'd`CS_INSTR;
            pc_e     = 1'b1;
        end

        case (instr[5:0]) // Opcode
            6'd0: begin // NOP
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