module cam #(
    parameter int N = 12,
    parameter int W = 8
) (
    input  logic [N - 1: 0] valid_keys,
    input  logic [N - 1: 0][W - 1: 0] keys,
    input  logic [W - 1: 0] key,
    output logic found,
    output logic [$clog2(N): 0] idx
);
    
    logic [N - 1: 0] match;
    always_comb begin
        match = '0;
        for (int i = 0; i < N; i++) begin
            match[i] = keys[i] == key & valid_keys[i];
        end
    end
    priority_encoder #(
        .N(N)
    ) priority_encoder_inst (
        .data_in(match),
        .found(found),
        .idx(idx)
    ); 
endmodule
