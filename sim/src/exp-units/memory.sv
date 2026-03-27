module Memory(

    ExpansionConnect_t expConnect,

    ExpansionSignals_if.unit memUnitSignals

    );

    always @(posedge clk) begin

        if (r) begin
            integer i;
            for (i = 0; i < 2**24; i = i + 1) begin
                memory[i] = 8'b0;
            end
        end else if (we) begin
            memory[addr + 1] <= data[7:0];
            memory[addr] <= data[15:8];
        end

    end

endmodule
