module arg_max #(
    parameter int N = 16,
    parameter int W = 8
) (
    input  logic [N - 1: 0][W - 1: 0] arr,
    output logic [$clog2(N + 1) - 1: 0] idx,
    output logic [W - 1: 0] max
);
    generate
        logic [W - 1: 0] max_low;
        logic [W - 1: 0] max_high;
        logic [$clog2(N / 2 + 1) - 1: 0] idx_low;
        logic [$clog2(N / 2 + 1) - 1: 0] idx_hi;
        if (N == 2)
        begin : base
            assign max = arr[0] > arr[1] ? arr[0] : arr[1];
            assign idx = arr[0] > arr[1] ? 1'b0: 1'b1;
        end else begin : rec
            arg_max #(.N(N / 2), 
            .W(W)) arg_max_high(
                .arr(arr[N - 1: N / 2]),
                .idx(idx_hi), 
                .max(max_high));
            arg_max #(.N(N / 2), 
            .W(W)) arg_max_low(
                .arr(arr[N / 2 - 1: 0]),
                .idx(idx_low), 
                .max(max_low));
            assign max = max_low > max_high ? max_low : max_high;
            assign idx = max_low > max_high ? idx_low : N / 2 + idx_hi;
        end
    endgenerate
endmodule
