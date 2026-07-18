module bin_sort #(
    parameter int N = 16
) (
    input  logic [N-1:0] data_in,
    output logic [N-1:0] sorted_data
);
    logic [$clog2(N):0] cnt;

    pop_cnt #(.N(N)) pop_cnt_inst (
        .data_in(data_in),
        .cnt    (cnt)
    );

    always_comb begin
        for (int i = 0; i < N; i++)
            sorted_data[i] = (i < cnt);
    end
endmodule
