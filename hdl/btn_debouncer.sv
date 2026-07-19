module btn_debouncer 
#(parameter int WAIT_CYCLES = 50000 )
(
    input logic clk,
    input logic rst_n,
    input logic btn,
    output logic pressed
);
    logic ff1;
    logic ff2;
    logic last_pulse;
    logic counter_started;
    logic [$clog2(WAIT_CYCLES) - 1: 0] cnt;

    always_ff @( posedge clk or negedge rst_n ) begin 
        if (!rst_n) begin
            ff1             <= '0;
            ff2             <= '0;
            last_pulse      <= '0;
            pressed         <= '0;
            counter_started <= '0;
            cnt             <= '0;
        end else begin
            ff1        <= btn;
            ff2        <= ff1;
            last_pulse <= ff2;
            pressed    <= 1'b0; 

           
            if (!last_pulse && ff2 && !counter_started) begin
                counter_started <= 1'b1;
            end

            if (counter_started) begin
                if (cnt == WAIT_CYCLES - 1) begin
                    counter_started <= 1'b0;
                    cnt             <= '0;
                    if (ff2) begin
                        pressed <= 1'b1;
                    end
                end else begin
                    cnt <= cnt + 1;
                end
            end
        end
    end
endmodule
