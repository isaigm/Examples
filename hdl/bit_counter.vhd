----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 24.05.2026 15:12:26
-- Design Name: 
-- Module Name: bit_counter - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity bit_counter is
    generic (N: integer := 16);
    Port (input_data: in std_logic_vector(N - 1 downto 0);
         result: out std_logic_vector(N - 1 downto 0));
end bit_counter;

architecture Behavioral of bit_counter is
function pop_cnt(nibble: in std_logic_vector(3 downto 0)) 
    return unsigned is
    variable sum: unsigned(3 downto 0) := (others => '0');
    begin
        for i in 0 to 3 loop
            if nibble(i) = '1' then
                sum := sum + 1;
            end if;
        end loop;
        return sum;
    end pop_cnt;
begin
    
    process(input_data)
    variable cnt: unsigned(N - 1 downto 0) := (others => '0');
    variable sum1: unsigned(3 downto 0) := (others => '0');
    variable sum2: unsigned(3 downto 0) := (others => '0');
    variable sum3: unsigned(3 downto 0) := (others => '0');
    variable sum4: unsigned(3 downto 0) := (others => '0');

    begin
        sum1 := pop_cnt(input_data(3 downto 0));
        sum2 := pop_cnt(input_data(7 downto 4));
        sum3 := pop_cnt(input_data(11 downto 8));
        sum4 := pop_cnt(input_data(15 downto 12));
        
        cnt := resize(sum1, N) + resize(sum2, N) + resize(sum3, N) + resize(sum4, N);

        result <= std_logic_vector(cnt);

    end process;
end Behavioral;
