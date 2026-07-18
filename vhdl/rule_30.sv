module rule_30 #(
    parameter  int unsigned N = 16,
    parameter logic [7:0] RULE = 8'd90 
) (
    input logic clk,
    input logic rst_n,
    input logic en,
    output logic [N - 1: 0]  row
);
    logic [N - 1: 0] curr_row;
    logic [N - 1: 0] next_row;
  
    assign row = curr_row;
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            curr_row <= '0;
            curr_row[N / 2] <= 1'b1;
        end else if (en) begin
            curr_row <= next_row;
        end
    end
    always_comb begin
        
        for (int unsigned i = 0; i < N; i++) begin
            logic p, q, r;
            logic [2:0] idx;
            p = curr_row[(i + N - 1) % N];
            q = curr_row[i];
            r = curr_row[(i + 1) % N];
            idx = {p, q, r};
            next_row[i] = RULE[idx];
        end
        
    end
endmodule
