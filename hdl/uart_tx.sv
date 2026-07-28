module uart_tx 
#(  
    parameter int BAUD_RATE = 9600,
    parameter int CLK_FREQ  = 50_000_000,
    parameter int TOTAL_CYCLES = CLK_FREQ / BAUD_RATE // Fixed integer math
)
(
    input  logic clk,
    input  logic rst_n,       
    input  logic start,
    input  logic [7:0] byte_in, 
    output logic bit_out, 
    output logic busy
);
    typedef enum logic [1:0] { IDLE, TX, STOP } status_t;
    status_t status;
    
    logic [$clog2(TOTAL_CYCLES):0] cnt;
   
    logic [3:0] bit_idx;  
        assign busy = (status != IDLE);

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            status  <= IDLE;
            bit_out <= 1'b1;
            cnt     <= '0;
            bit_idx <= '0;
        end else begin
            case (status)
                IDLE: begin
                    bit_out <= 1'b1; 
                    if (start) begin
                        status  <= TX;
                        bit_out <= 1'b0; 
                        cnt     <= '0;
                        bit_idx <= '0;
                    end
                end
                
                TX: begin
                    if (cnt == TOTAL_CYCLES - 1) begin
                        cnt <= '0; 
                        
                        if (bit_idx == 4'd8) begin
                    
                            status  <= STOP;
                            bit_out <= 1'b1; 
                        end else begin
                        
                            bit_out <= byte_in[bit_idx];
                            bit_idx <= bit_idx + 1'b1;
                        end
                    end else begin
                        cnt <= cnt + 1'b1; 
                    end
                end
                
                STOP: begin
                
                    if (cnt == TOTAL_CYCLES - 1) begin
                        status <= IDLE;
                        cnt    <= '0;
                    end else begin
                        cnt <= cnt + 1'b1;
                    end
                end
                
                default: status <= IDLE; 
            endcase
        end
    end
endmodule
