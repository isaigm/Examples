library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use ieee.math_real.all;
entity ca_cfar is
  generic (
    SAMPLE_W      : integer := 16;    
    N_REF         : integer := 16;   
    N_GUARD       : integer := 1;    
    ALPHA_W       : integer := 16;    
    ALPHA_FP      : integer := 3188   

  );
  port (
    clk, rst : in  std_logic;
    s_data   : in  std_logic_vector(SAMPLE_W-1 downto 0); 
    s_valid  : in  std_logic;
    m_cut    : out std_logic_vector(SAMPLE_W-1 downto 0);  
    m_detect : out std_logic;                              
    m_valid  : out std_logic                               
  );
end entity;


architecture Behavioral of ca_cfar is
  constant TOTAL_SAMPLES : integer := N_REF + 2 * N_GUARD + 1;
  constant SUM_W : integer := SAMPLE_W + integer(ceil(log2(real(N_REF))));
  constant T_W: integer := SUM_W + ALPHA_W;
  
  type window_t is array (0 to TOTAL_SAMPLES - 1) of std_logic_vector(SAMPLE_W - 1 downto 0);
  signal window: window_t := (others => (others => '0'));
  signal curr_idx: integer range 0 to TOTAL_SAMPLES := 0;
  signal full: std_logic;
  signal sum_all: unsigned(SUM_W - 1 downto 0) := (others => '0');
  signal ref_sum: unsigned(SUM_W - 1 downto 0);
  signal threshold: unsigned(T_W - 1 downto 0);
begin
  full <= '1' when curr_idx >= TOTAL_SAMPLES else '0';

  ref_sum <= sum_all - resize(unsigned(window(8)), SUM_W) - 
             resize(unsigned(window(9)), SUM_W) - 
             resize(unsigned(window(10)), SUM_W);
  
  m_cut    <= window(9);
  
  m_valid <= '1' when (full = '1' and s_valid = '1') else '0';
  
  threshold <= resize(ref_sum * to_unsigned(ALPHA_FP, T_W), T_W);
  
  m_detect <= '1' when (shift_left(resize(unsigned(window(9)), T_W), 12) > threshold) else '0';

  process(clk)
  begin
    if rising_edge(clk) then
      if rst = '1' then
        curr_idx <= 0;
        sum_all <= (others => '0');
        
      elsif s_valid = '1' then 
        window <= window(1 to TOTAL_SAMPLES - 1) & s_data;
        sum_all <= sum_all
          + resize(unsigned(s_data),      SUM_W)
          - resize(unsigned(window(0)), SUM_W);
          
        if full = '0' then
          curr_idx <= curr_idx + 1;
        end if;     
      end if;
    end if;
  end process;
end architecture;
