module uart_rx 
#(  
    parameter int BAUD_RATE = 9600,
    parameter int CLK_FREQ  = 50_000_000,
    parameter int TOTAL_CYCLES = CLK_FREQ / BAUD_RATE 
)
(
    input logic clk,
    input logic rst_n,
    input logic bit_in,
    output logic data_valid,
    output logic[7:0] byte_out
);
    typedef enum logic[1:0] { IDLE, WAIT_HALF_BIT, DATA, STOP } status_t;
    logic  ff1;
    logic  ff2;
    logic  last_in;
    status_t curr_state;
    logic [$clog2(TOTAL_CYCLES) - 1: 0] cnt;
    logic [7: 0] shift_reg;
    logic [2: 0] bit_idx;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            curr_state  <= IDLE;
            data_valid <= 1'b0;
            cnt     <= '0;
            ff1 <= 1'b1;
            ff2 <= 1'b1;
            last_in <= 1'b1;
            shift_reg <= '0;
            bit_idx <= '0;
            byte_out <= '0;
        end else begin
            ff1 <= bit_in;
            ff2 <= ff1;
            last_in <= ff2;
            data_valid <= 1'b0;
            case (curr_state)
                IDLE: begin
                    if (!ff2 && last_in) begin
                        cnt <= '0;
                        curr_state <= WAIT_HALF_BIT;
                    end
                end
                WAIT_HALF_BIT: begin
                    if (cnt == (TOTAL_CYCLES >> 1) - 1) begin
                        if (!ff2) begin
                            curr_state <= DATA;
                        end else begin 
                            curr_state <= IDLE;
                        end
                        cnt <= '0;
                    end else begin
                        cnt <= cnt + 1;
                    end
                end
                DATA: begin
                    if (cnt == TOTAL_CYCLES - 1) begin
                        shift_reg <= {ff2, shift_reg[7:1]};
                        cnt <= '0;
                         
                        if (bit_idx == 3'd7) begin                    
                            curr_state  <= STOP;
                            bit_idx <= '0;
                        end else begin
                
                            bit_idx <= bit_idx + 1'b1;
                        end
                    end else begin
                        cnt <= cnt + 1;
                    end
                end
                STOP: begin
                 if (cnt == TOTAL_CYCLES - 1) begin
                        if (ff2) begin
                            byte_out <= shift_reg;
                            data_valid <= '1;
                            curr_state <= IDLE;
                        end else begin
                            curr_state <= IDLE;
                        end
                    end else begin
                        cnt <= cnt + 1;
                    end
                end
                
                default:  curr_state <= IDLE;
            endcase
        end
    end
endmodule
