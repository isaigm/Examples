module remove_duplicates #(
    parameter int N = 6,
    parameter int W = 8
) (
    input  logic  clk,
    input  logic  rst,
    input  logic  [N - 1: 0][W - 1: 0] arr,
    output logic  [N - 1: 0][W - 1: 0] out_arr,
    output logic  [$clog2(N + 1) - 1: 0] out_len, 
    output logic  done
);
    logic [$clog2(N + 1) - 1: 0] idx;
    logic [$clog2(N + 1) - 1: 0] out_idx; 

    logic [W - 1: 0] last_val;
    logic flag;

    always_ff @(posedge clk) begin
        if (rst) begin
            idx      <= '0;
            done     <= '0;
            last_val <= '0;
            out_idx  <= '0;
            out_len  <= '0;
            flag     <= '0;
            out_arr  <= '0; 
        end else begin
            done <= '0;
            if (idx == N) begin
                done    <= '1;
                out_len <= out_idx; 
            end else begin
                if (!flag || arr[idx] != last_val) begin
                    out_arr[out_idx] <= arr[idx];
                    out_idx          <= out_idx + 1;
                end
                flag     <= '1;
                last_val <= arr[idx];
                idx      <= idx + 1;
            end
        end        
    end
endmodule
