module barrel_shifter #(
    parameter int N = 32
) (
    input logic [N - 1: 0] in_data,
    input logic [$clog2(N) - 1: 0] amount,
    output logic [N - 1: 0] shifted_data
);
    always_comb begin 
        logic [N - 1: 0] last_stage;
        last_stage = in_data;
        for (int i = 0; i < $clog2(N); i++) begin
            logic [N - 1: 0] temp;
            if (amount[i]) begin
                temp = (last_stage >> (1 << i)) | (last_stage << (N - (1 << i))); 
            end else begin
                temp = last_stage;
            end
            last_stage = temp;
        end
        shifted_data = last_stage;
    end
    
endmodule
