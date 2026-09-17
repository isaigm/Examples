library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

entity cordic_vec is
  port (
        clk:        in  std_logic;
        rst:        in  std_logic;
        s_valid:    in  std_logic;
        in_x:       in  signed(19 downto 0);
        in_y:       in  signed(19 downto 0);
        m_valid:    out std_logic;
        out_mag:    out signed(19 downto 0);
        out_angle:  out signed(19 downto 0)
    );
end entity;

architecture Behavioral of cordic_vec is
    type angles_t is array (0 to 15) of signed(19 downto 0);
    type arr_t is array(0 to 16) of signed(19 downto 0);
    constant PI_POS: signed(19 downto 0) := to_signed(205887, 20);
    constant PI_NEG: signed(19 downto 0) := to_signed(-205887, 20);
    constant angles: angles_t := ( to_signed(51472, 20), to_signed(30386, 20), to_signed(16055, 20), to_signed(8150, 20), 
    to_signed(4091, 20), to_signed(2048, 20), to_signed(1024, 20),
    to_signed(512, 20), to_signed(256, 20), to_signed(128, 20), 
    to_signed(64, 20), to_signed(32, 20), to_signed(16, 20), 
    to_signed(8, 20), to_signed(4, 20), to_signed(2, 20) );
    signal x_st: arr_t  := (others => (others => '0'));
    signal y_st: arr_t  := (others => (others => '0'));
    signal z_st: arr_t  := (others => (others => '0'));
    signal valid: std_logic_vector(16 downto 0) := (others => '0');
begin
    valid(0)  <= s_valid;
    m_valid   <= valid(16);
    out_mag   <= x_st(16);
    out_angle <= z_st(16);
    process(all)
    begin
        x_st(0) <= in_x;
        y_st(0) <= in_y;
        z_st(0) <= (others => '0');
        if in_x < 0 then
            x_st(0) <= -in_x;
            y_st(0) <= -in_y;
            if in_y > 0 then
                z_st(0) <= PI_POS;
            else
                z_st(0) <= PI_NEG;
            end if;
        end if;
    end process;
    
    stages : for i in 0 to 15 generate 
    begin
        process(clk)                         
        variable alpha: signed(19 downto 0);
        begin
            
            if rising_edge(clk) then
                if rst = '1' then
                    x_st(i + 1)  <= (others => '0');
                    y_st(i + 1)  <= (others => '0');
                    z_st(i + 1)  <= (others => '0');
                    valid(i + 1) <= '0';
                else
                    valid(i + 1) <= valid(i);
                    alpha := angles(i);
                    if y_st(i)(19) = '1' then
                        z_st(i + 1) <= z_st(i) - alpha;
                        x_st(i + 1) <= x_st(i) - shift_right(y_st(i), i);
                        y_st(i + 1) <= y_st(i) + shift_right(x_st(i), i);  
                    else 
                        z_st(i + 1) <= z_st(i) + alpha;
                        x_st(i + 1) <= x_st(i) + shift_right(y_st(i), i);
                        y_st(i + 1) <= y_st(i) - shift_right(x_st(i), i);  
                    end if; 
                end if;

            end if;
        end process;

    end generate;
   

end architecture Behavioral;
