module Reg16(

    input clk, r,
    input we, oe,
    input [15:0] a,
    output [15:0] oc, reg [15:0] o

    );

    assign oc = oe ? o : 16'bz;

    always @(posedge clk) begin
        
        if (r) begin
            o <= 0;
        end else if (we) begin
            o <= a;
        end

    end

endmodule
