module Reg8(

    input clk, r,
    input we, oe,
    input [7:0] a,
    output [7:0] oc, reg [7:0] o = 0

    );

    assign oc = oe ? o : 8'bz;

    always @(posedge clk) begin
        
        if (r) begin
            o <= 0;
        end else if (we) begin
            o <= a;
        end

    end

endmodule
