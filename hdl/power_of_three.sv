module power_of_three #(
    parameter int N = 16
) (
    input  logic clk,
    input  logic rst,
    input  logic en,
    input  logic [N-1:0] number,
    output logic valid,
    output logic is_power_of_three
);
    logic [N:0] curr_pw;
    always_ff @(posedge clk) begin
        if (rst) begin
            curr_pw           <= (N+1)'(1);
            valid             <= '0;
            is_power_of_three <= '0;
        end else if (en && !valid) begin
            if (curr_pw == number) begin
                valid             <= '1;
                is_power_of_three <= '1;
            end else if (curr_pw > number) begin
                valid             <= '1;   
            end
            curr_pw <= 3 * curr_pw;
        end
    end
endmodule
