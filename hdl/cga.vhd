library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.cga_pkg.all;

entity cga is
    generic (
        ROWS : positive := 4;
        COLS : positive := 4  
    );
    port (
        clk  : in  std_logic;
        rst  : in  std_logic;
        done: out std_logic  
    );
end entity cga;

architecture rtl of cga is
    signal chroms : chrom_array(0 to ROWS*COLS - 1);
    signal fits   : fit_array(0 to ROWS*COLS - 1);
begin

    gen_row : for i in 0 to ROWS - 1 generate
        gen_col : for j in 0 to COLS - 1 generate
          
            constant IDX_ME    : natural := i*COLS + j;
            constant IDX_UP    : natural := ((i + ROWS - 1) mod ROWS)*COLS + j;
            constant IDX_DOWN  : natural := ((i + 1) mod ROWS)*COLS + j;
            constant IDX_LEFT  : natural := i*COLS + (j + COLS - 1) mod COLS;
            constant IDX_RIGHT : natural := i*COLS + (j + 1) mod COLS;
        begin
            u_cell : entity work.cell
                generic map (
                    SEED => SEEDS(IDX_ME)
                )
                port map (
                    clk         => clk,
                    rst         => rst,
                    left_fit    => fits(IDX_LEFT),
                    right_fit   => fits(IDX_RIGHT),
                    up_fit      => fits(IDX_UP),
                    down_fit    => fits(IDX_DOWN),
                    left_chrom  => chroms(IDX_LEFT),
                    right_chrom => chroms(IDX_RIGHT),
                    up_chrom    => chroms(IDX_UP),
                    down_chrom  => chroms(IDX_DOWN),
                    fit_out     => fits(IDX_ME),
                    chrom_out   => chroms(IDX_ME)
                );
        end generate gen_col;
    end generate gen_row;

    process (fits)
        variable any : std_logic;
    begin
        any := '0';
        for k in fits'range loop
            if fits(k) = to_unsigned(8, 4) then
                any := '1';
            end if;
        end loop;
        done <= any;
    end process;

end architecture rtl;
