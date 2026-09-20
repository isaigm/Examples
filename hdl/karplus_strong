library IEEE;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use work.utils.all;
  use ieee.math_real.all;
entity karplus_strong is
    
    port (
        clk:        in std_logic;
        rst:        in std_logic;
        LED:        out std_logic_vector(15 downto 0) 
    ); 
end entity karplus_strong;

architecture Behavioral of karplus_strong is
    constant FS: real := 100.0e6 / 2268.0;     
    constant F: real := 440.0;
    constant SIZE: natural := integer(round(FS/F - 0.5));
    constant T60: real := 2.0; 
    constant COEF : signed(15 downto 0) := to_signed(integer(round(0.5 * exp(-6.908/(F*T60)) * 32768.0)), 16);    
    type status_t is (FILL_MEM, READ, WAIT_READ, WRITE_BACK);
    signal we: std_logic;
    signal d_prev: signed(15 downto 0) := (others => '0');
    signal write_addr: unsigned(sizeof(SIZE) - 1 downto 0) := (others => '0');
    signal read_addr:  unsigned(sizeof(SIZE) - 1 downto 0) := (others => '0');
    signal write_data: signed(15 downto 0);
    signal read_data:  signed(15 downto 0);
    signal lsfr_en: std_logic;
    signal rand: std_logic_vector(15 downto 0);
    signal curr_status: status_t := FILL_MEM;
    signal y: signed(15 downto 0);
    signal sum : signed(16 downto 0);
    signal prod : signed(32 downto 0);
    signal y_reg : signed(15 downto 0);
    signal out_sample: signed(15 downto  0);
begin
    lsfr_en    <= '1' when curr_status = FILL_MEM else '0';
    we         <= '1' when curr_status = FILL_MEM or curr_status = WRITE_BACK else '0';
    write_data <= signed(rand) when curr_status = FILL_MEM else
              y_reg        when curr_status = WRITE_BACK else (others => '0');

    sum <= resize(read_data, 17) + resize(d_prev, 17);
    prod <= COEF * sum;                                  
    y    <= resize(shift_right(prod + 16384, 15), 16);
    mem_inst: entity work.mem
    generic map (
        SIZE => SIZE
    )
    port map(
        clk         => clk,
        we          => we,
        write_addr  => write_addr,
        read_addr   => read_addr,
        write_data  => write_data,
        read_data   => read_data
    );
    LED <= std_logic_vector(out_sample);
    lsfr16_inst: entity work.lfsr16
    port map(
        clk => clk,
        rst => rst,
        en  => lsfr_en,
        rand => rand
    );

    process(clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                curr_status <= FILL_MEM;
                write_addr  <= (others => '0');
                read_addr   <= (others => '0');
                out_sample  <= (others => '0');
                d_prev      <= (others => '0');
            else 
                case curr_status is
                    
                    when FILL_MEM => 
                        if write_addr = SIZE - 1 then
                            curr_status <= READ;
                        else 
                            write_addr <= write_addr + 1;
                        end if;
                    when READ =>
                        curr_status <= WAIT_READ;
                    when WAIT_READ =>
                        if read_addr = SIZE - 1 then
                            read_addr <= (others => '0');
                        else 
                            read_addr <= read_addr + 1;
                        end if;
                        write_addr  <= read_addr;
                        d_prev      <= read_data;
                        curr_status <= WRITE_BACK;
                        y_reg       <= y;
                        out_sample  <= y;
                    when WRITE_BACK =>
                        curr_status <= WAIT_READ;
                    when others => curr_status <= FILL_MEM; 
                end case;
            end if;
        end if;
    end process;
end Behavioral;
