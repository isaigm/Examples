library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

entity cordic is
  port (
        clk:            in  std_logic;
        rst:            in  std_logic;
        target_angle:   in  signed(19 downto 0);
        out_x:          out signed(19 downto 0);
        out_y:          out signed(19 downto 0)
    );
end entity;

architecture Behavioral of cordic is
    type angles_t is array (0 to 15) of signed(19 downto 0);
    type arr_t is array(0 to 16) of signed(19 downto 0);
    constant angles: angles_t := ( to_signed(51472, 20), to_signed(30386, 20), to_signed(16055, 20), to_signed(8150, 20), 
    to_signed(4091, 20), to_signed(2048, 20), to_signed(1024, 20),
    to_signed(512, 20), to_signed(256, 20), to_signed(128, 20), 
    to_signed(64, 20), to_signed(32, 20), to_signed(16, 20), 
    to_signed(8, 20), to_signed(4, 20), to_signed(2, 20) );
    signal x_st: arr_t  := (to_signed(39797,20), others => (others => '0'));
    signal y_st: arr_t  := (others => (others => '0'));
    signal z_st: arr_t  := (others => (others => '0'));
begin
    x_st(0) <= to_signed(39797, 20);
    y_st(0) <= (others => '0');
    z_st(0) <= target_angle;   
    out_x <= x_st(16);
    out_y <= y_st(16);
    stages : for i in 0 to 15 generate 
    begin
        process(clk)                         
        variable alpha: signed(19 downto 0);
        begin
            
            if rising_edge(clk) then
                if rst = '1' then
                    x_st(i + 1) <= (others => '0');
                    y_st(i + 1) <= (others => '0');
                    z_st(i + 1) <= (others => '0');
                else
                    alpha := angles(i);
                    if z_st(i)(19) = '1' then
                        z_st(i + 1) <= z_st(i) + alpha;
                        x_st(i + 1) <= x_st(i) + shift_right(y_st(i), i);
                        y_st(i + 1) <= y_st(i) - shift_right(x_st(i), i);  
                    else 
                        z_st(i + 1) <= z_st(i) - alpha;
                        x_st(i + 1) <= x_st(i) - shift_right(y_st(i), i);
                        y_st(i + 1) <= y_st(i) + shift_right(x_st(i), i);  
                    end if; 
                end if;

            end if;
        end process;

    end generate;
   

end architecture Behavioral;
