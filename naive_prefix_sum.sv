module naive_prefix_sum #(
    parameter int N = 8,
    parameter int W = 8
) (
    input logic  [N - 1: 0][W - 1: 0] arr,
    output logic [N - 1: 0][W - 1: 0] prefix_sum
);
    generate
        if (N == 2) begin

            assign prefix_sum[0] = arr[0];
            assign prefix_sum[1] = arr[0] + arr[1];
        
        end else begin
            logic [N / 2 - 1: 0][W - 1: 0] lo_prefix_sum;
            logic [N / 2 - 1: 0][W - 1: 0] hi_prefix_sum;
            naive_prefix_sum #(.N(N / 2), .W(W)) 
            naive_prefix_sum_inst_low (
                .arr(arr[N / 2 - 1: 0]), 
                .prefix_sum(lo_prefix_sum)
            );
            naive_prefix_sum #(.N(N / 2), .W(W)) 
            naive_prefix_sum_inst_hi (
                .arr(arr[N - 1: N / 2]), 
                .prefix_sum(hi_prefix_sum)
            );
            for (genvar i = 0; i < N / 2; i++) begin
                assign prefix_sum[i] = lo_prefix_sum[i];
            end
            for (genvar i = 0; i < N / 2; i++) begin
                assign prefix_sum[N / 2 + i] = lo_prefix_sum[N / 2 - 1] + hi_prefix_sum[i];
            end
        end
    endgenerate
endmodule
