module johnson_counter #(
    parameter int N = 4
) (
    input logic clk,
    input logic rst,
    output logic[N - 1: 0] counter
);
    logic [N - 1: 0] cnt;
    always_ff @( posedge clk ) begin
        if (rst) begin
            cnt <= '0;
        end else begin
            cnt[0] <= !cnt[N - 1];
            for (int i = 1; i < N; i++) begin
                cnt[i] <= cnt[i - 1];
            end
        end
    end
    assign counter = cnt;
endmodule
