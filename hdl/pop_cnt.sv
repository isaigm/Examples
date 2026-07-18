module pop_cnt #(
    parameter int N = 17
) (
    input logic [N - 1: 0] data_in,
    output logic [$clog2(N): 0] cnt
);
    localparam int MID = N / 2;
    localparam HI_SIZE = N - MID;
    localparam LO_SIZE = MID;
    generate
      
        if (N == 1) begin
            assign cnt = data_in[0];
        end else if (N == 2) begin
            assign cnt = data_in[0] + data_in[1];
            
        end else begin
            logic [$clog2(HI_SIZE): 0] hi_cnt;
            logic [$clog2(LO_SIZE): 0] lo_cnt;
            pop_cnt #(
            .N(HI_SIZE)
            ) pop_cnt_hi (
               .data_in(data_in[N - 1: MID]),
               .cnt(hi_cnt)
            );
            
            pop_cnt #(
            .N(LO_SIZE)
            ) pop_cnt_low (
               .data_in(data_in[MID - 1: 0]),
               .cnt(lo_cnt)
            );
            assign cnt = hi_cnt + lo_cnt;
        end
    endgenerate
endmodule
