
library IEEE;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use ieee.math_real.all;

entity rosenbrock is
  port (
    clk: in std_logic;
    rst: in std_logic
  );
end rosenbrock;

architecture Behavioral of rosenbrock is
    constant A:   integer := 1;
    constant B:   integer := 100;
    constant ONE: integer := 2**14;  
    constant LR:  integer := 1;

    signal curr_x, curr_y : signed(15 downto 0) := (others => '0');   
    signal x_square       : signed(16 downto 0);  
    signal xsub1          : signed(16 downto 0);   
    signal xsub2          : signed(17 downto 0);   
    signal xsub3          : signed(27 downto 0);  
    signal xsub4          : signed(27 downto 0);   
    signal ysub1          : signed(24 downto 0);
               

begin
    x_square <= resize(shift_right(curr_x * curr_x, 14), 17);
    xsub1    <= resize(curr_y, 17) - x_square;
    xsub2    <= shift_left(resize(curr_x, 18), 1) - to_signed(2 * A * ONE, 18);
    xsub3    <= resize(shift_right(4 * B * curr_x * xsub1, 14), 28);
    xsub4    <= xsub2 - xsub3;
    ysub1    <= resize(2 * B * xsub1, 25);
    
    process(clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                curr_x <= (others => '0');
                curr_y <= (others => '0');
            else
                curr_x <= resize(curr_x - shift_right(xsub4, 10), 16);
                curr_y <= resize(curr_y - shift_right(ysub1, 10), 16);
            end if;
        end if;
    end process;

end architecture Behavioral; 
