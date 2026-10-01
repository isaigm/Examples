library IEEE;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use ieee.math_real.all;

entity fir is
    generic (
        FRAC_BITS: integer := 15;
        SAMPLE_W:  integer := 12;
        COEF_W:    integer := 16;
        COEFS:     integer_vector := (3277, 6554, 9830, 13107)
    );
    port (
        clk:        in  std_logic;
        rst:        in  std_logic;
        sample_in:  in  signed(SAMPLE_W - 1 downto 0);
        sample_out: out signed(SAMPLE_W - 1 downto 0)
    );
end fir;

architecture Behavioral of fir is
    constant N_TAPS:  integer := COEFS'length;
    constant STAGE_W: integer := SAMPLE_W + COEF_W + integer(ceil(log2(real(N_TAPS))));
    type coef_array_t is array (0 to N_TAPS - 1) of signed(COEF_W - 1 downto 0);

    function to_coef_array(v: integer_vector) return coef_array_t is
        variable r: coef_array_t;
    begin
        for i in 0 to N_TAPS - 1 loop
            r(i) := to_signed(v(v'low + i), COEF_W);
        end loop;
        return r;
    end function;

    constant coef: coef_array_t := to_coef_array(COEFS);

    type array_t is array (0 to N_TAPS - 1) of signed(STAGE_W - 1 downto 0);
    signal stages: array_t := (others => (others => '0'));
begin

    sample_out <= resize(shift_right(stages(N_TAPS - 1), FRAC_BITS), SAMPLE_W);
    process(clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                stages <= (others => (others => '0'));
            else
                stages(0) <= resize(sample_in * coef(N_TAPS - 1), STAGE_W);
                for i in 1 to N_TAPS - 1 loop
                    stages(i) <= sample_in * coef(N_TAPS - 1 - i) + stages(i - 1);
                end loop;
            end if;
        end if;
    end process;
end architecture;
