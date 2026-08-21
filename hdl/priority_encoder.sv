module priority_encoder #(
    parameter int N = 8
) (
    input logic [N - 1: 0] data_in,
    output logic found,
    output logic [$clog2(N): 0] idx
);
    localparam int MID = N / 2;
    localparam HI_SIZE = N - MID;
    localparam LO_SIZE = MID;
    generate
       
        
        if (N == 1) begin
            assign found = data_in[0] == 1'b1;
            assign idx = '0;
        end else begin
            logic left_found;
            logic right_found;
            logic [$clog2(HI_SIZE): 0] hi_idx;
            logic [$clog2(LO_SIZE): 0] lo_idx;

           priority_encoder #(
            .N(HI_SIZE)
            ) priority_encoder_hi (
               .data_in(data_in[N - 1: MID]),
               .found(left_found),
               .idx(hi_idx)
            );
            
            priority_encoder #(
            .N(LO_SIZE)
            ) priority_encoder_low(
               .data_in(data_in[MID - 1: 0]),
               .found(right_found),
               .idx(lo_idx)
            );
            assign idx = right_found ? lo_idx : (left_found ? N / 2 + hi_idx: '0);
            assign found = right_found | left_found;
        end 
    endgenerate
endmodule
