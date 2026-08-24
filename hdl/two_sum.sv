module two_sum #(
    parameter int N = 4,
    parameter int W = 16
) (
    input  logic clk,
    input  logic rst,
    input  logic start,
    input  logic [W - 1: 0] target,
    input  logic [N - 1: 0][W - 1: 0] nums,
    output logic done,
    output logic valid,
    output logic [W - 1: 0] first,
    output logic [W - 1: 0] second
);
    typedef enum int {IDLE, ITER} status_t;
    logic [N - 1: 0]           valid_keys = '0;
    logic [N - 1: 0][W - 1: 0] keys  = '0;
    logic [W - 1: 0]           key;
    logic [$clog2(N): 0] curr_idx    = '0;
    logic [$clog2(N): 0] keys_idx    = '0;
    logic [$clog2(N): 0] idx;
    logic found;
    status_t curr_status = IDLE;
    assign key = (curr_idx < N) ? (target - nums[curr_idx]) : '0;
    always @(posedge clk) begin
        if (rst) begin
            done        <= '0;
            valid_keys  <= '0;
            curr_idx    <= '0;
            curr_status <= IDLE;
            keys        <= '0;
            first       <= '0;
            second      <= '0;
            keys_idx    <= '0;
            valid       <= '0;
        end else begin
            case (curr_status)
                IDLE: begin
                    if (start) begin
                        done        <= '0;
                        valid_keys  <= '0;
                        curr_idx    <= '0;
                        curr_status <= ITER;
                        keys        <= '0;
                        first       <= '0;
                        second      <= '0;
                        keys_idx    <= '0;
                        valid       <= '0;
                    end
                end
                ITER: begin
                    if (found) begin
                        curr_status <= IDLE;
                        first  <= key;
                        second <= target - key;
                        valid  <= '1;
                        done   <= '1;
                    end else if (curr_idx == (N - 1)) begin
                        curr_status <= IDLE;
                        done        <= '1;
                    end else begin
                        keys[keys_idx]       <= nums[curr_idx];
                        valid_keys[keys_idx] <= '1;
                        keys_idx             <= keys_idx + 1;
                        curr_idx             <= curr_idx + 1;
                        curr_status          <= ITER;
                    end
                end 
                
                default: curr_status <= IDLE;
            endcase
        end 
    end
    cam #(.N(N), 
        .W(W) 
    ) cam_inst (
        .valid_keys(valid_keys),
        .keys(keys),
        .key(key),
        .found(found),
        .idx(idx)
    );
endmodule
