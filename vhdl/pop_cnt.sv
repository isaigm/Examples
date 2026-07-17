module pop_cnt #(
    parameter int N = 16
) (
    input logic [N - 1: 0] data_in,
    output logic [$clog2(N): 0] cnt
);
    generate
        logic [$clog2(N / 2): 0] hi_cnt;
        logic [$clog2(N / 2): 0] lo_cnt;
        if (N == 2) begin
            assign cnt = data_in[0] + data_in[1];
            
        end else begin
            pop_cnt #(
            .N(N / 2)
            ) pop_cnt_hi (
               .data_in(data_in[N - 1: N / 2]),
               .cnt(hi_cnt)
            );
            
            pop_cnt #(
            .N(N / 2)
            ) pop_cnt_low (
               .data_in(data_in[N / 2 - 1: 0]),
               .cnt(lo_cnt)
            );
            assign cnt = hi_cnt + lo_cnt;
        end
    endgenerate
endmodule