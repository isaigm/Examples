library ieee; 
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;


entity cell is
    generic (
        SEED : std_logic_vector(31 downto 0) := x"00000001"
    );
    port (
        clk:         in std_logic;
        rst:         in std_logic;
        left_fit:    in unsigned(3 downto 0);
        right_fit:   in unsigned(3 downto 0);
        up_fit:      in unsigned(3 downto 0);
        down_fit:    in unsigned(3 downto 0);
        left_chrom:  in std_logic_vector(7 downto 0);
        right_chrom: in std_logic_vector(7 downto 0);
        up_chrom:    in std_logic_vector(7 downto 0);
        down_chrom:  in std_logic_vector(7 downto 0);
        fit_out:     out unsigned(3 downto 0);
        chrom_out:   out std_logic_vector(7 downto 0)
    );
end cell;

architecture Behavioral of cell is
    type status_t is (INIT, GET_FIT, GET_BEST, CROSSOVER, MUTATE, EVAL_CHILD);
    signal curr_status: status_t := INIT;
    signal chrom: std_logic_vector(7 downto 0) := (others => '0');
    signal lsfr_q: std_logic_vector(7 downto 0);
    signal best_fit: unsigned(3 downto 0);
    signal parent_fit: unsigned(3 downto 0);
    signal child_fit: unsigned(3 downto 0);
    signal best_chrom: std_logic_vector(7 downto 0);
    signal cross_chrom: std_logic_vector(7 downto 0);

    signal child_chrom: std_logic_vector(7 downto 0);
begin
    chrom_out <= chrom;

    lfsr32_inst: entity work.lfsr32
    generic map (
      SEED => SEED
    )
    port map (
      clk => clk,
      rst => rst,
      q   => lsfr_q
    );
    process(clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                curr_status <= INIT;
            else 
                case (curr_status) is
                
                    when INIT =>
                        chrom       <= lsfr_q;
                        curr_status <= GET_FIT;
                    when GET_FIT =>
                        fit_out     <= parent_fit;
                        curr_status <= CROSSOVER;
                    when GET_BEST =>
                        curr_status <= CROSSOVER;
                    when CROSSOVER =>
                        curr_status <= MUTATE;
                        child_chrom <= cross_chrom;
                    when MUTATE =>
                        if unsigned(lsfr_q) < 127 then
                            child_chrom(to_integer(unsigned(lsfr_q(2 downto 0)))) <= not child_chrom(to_integer(unsigned(lsfr_q(2 downto 0))));                    
                        end if;
                        curr_status <= EVAL_CHILD;
                    when EVAL_CHILD =>
                        if child_fit >= parent_fit then
                            fit_out <= child_fit;
                            chrom   <= child_chrom;
                        end if;
                        curr_status <= GET_BEST;
                    when others =>
    
                end case;
            end if;
        end if;
    end process;
    process(chrom, child_chrom)
        variable count_parent:  unsigned(3 downto 0);
        variable count_child:   unsigned(3 downto 0);
    begin
        count_parent := (others => '0');
        count_child  := (others => '0');
        for i in 0 to 7 loop
            if chrom(i) = '1' then
                count_parent := count_parent + 1;
            end if;
            if child_chrom(i) = '1' then
                count_child := count_child + 1;
            end if;
        end loop;
        parent_fit <= count_parent;
        child_fit  <= count_child;
    end process;
    cross: process(all)
        variable idx: unsigned(2 downto 0);
        variable child_chr: std_logic_vector(7 downto 0);
    begin
        child_chr := (others => '0');
        idx       := unsigned(lsfr_q(2 downto 0));
        for i in 0 to 7 loop
            if i < idx then
                child_chr(i) := best_chrom(i);
            else 
                child_chr(i) := chrom(i);
            end if;
        end loop;
        cross_chrom <= child_chr;
    end process;

    get_best_neighbour: process(all) 
        variable best_f: unsigned(3 downto 0);
        variable best_chr: std_logic_vector(7 downto 0);
    begin
        best_f   := (others => '0');
        best_chr := (others => '0');
        
        if left_fit > right_fit then
            best_f   := left_fit;
            best_chr := left_chrom;
        else
            best_f   := right_fit;
            best_chr := right_chrom;
        end if;

        if up_fit > best_f then
            best_f   := up_fit;
            best_chr := up_chrom;
        end if;

        if down_fit > best_f then
            best_f   := down_fit;
            best_chr := down_chrom;
        end if;
        best_fit <= best_f;
        best_chrom <= best_chr;
    end process;

end architecture; 
