library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use work.rc4_pkg.all;

entity rc4 is
  generic (
    MSG_LEN : integer := 11;
    KEY_LEN : integer := 11
  );
  port (
    clk            : in  std_logic;
    rst            : in  std_logic;
    start          : in  std_logic;
    msg            : in  array_t(0 to MSG_LEN - 1)(7 downto 0);
    key            : in  array_t(0 to KEY_LEN - 1)(7 downto 0);
    done           : out std_logic;
    encrypted_data : out array_t(0 to MSG_LEN - 1)(7 downto 0)
  );
end entity;

architecture Behavioral of rc4 is
  type status_t is (IDLE, INIT, KSA_S1, KSA_S2, KSA_S3, PRGA_S1, PRGA_S2, PRGA_S3, WAIT_SWAP);
  signal curr_status : status_t               := IDLE;
  signal S           : array_t(0 to 255)(7 downto 0);
  signal s_idx       : unsigned(7 downto 0) := (others => '0');
  signal key_idx     : integer range 0 to KEY_LEN := 0; 
  signal msg_idx     : integer range 0 to MSG_LEN := 0;
  signal j_idx       : unsigned(7 downto 0);
  signal temp_j_idx  : unsigned(7 downto 0) := (others => '0');
  signal encr_data   : array_t(0 to MSG_LEN - 1)(7 downto 0) := (others => (others => '0'));
  signal temp_val    : std_logic_vector(7 downto 0) := (others => '0');
begin
    encrypted_data <= encr_data;
    j_idx          <= temp_j_idx + unsigned(S(to_integer(s_idx))) + unsigned(key(key_idx));
    
    process (clk)
        variable t: unsigned(7 downto 0);
    begin
        if rising_edge(clk) then
            if rst = '1' then
                msg_idx     <= 0;
                key_idx     <= 0;
                s_idx       <= (others => '0');
                curr_status <= IDLE;
                temp_j_idx  <= (others => '0');
                encr_data   <= (others => (others => '0'));
                done        <= '0';
            else
                done <= '0';
                case (curr_status) is
                    when IDLE =>
                        if start = '1' then
                            curr_status <= INIT;
                            msg_idx     <= 0;
                            key_idx     <= 0;
                            s_idx       <= (others => '0');
                            temp_j_idx  <= (others => '0');
                            encr_data   <= (others => (others => '0'));
                        end if;

                    when INIT =>
                        S(to_integer(s_idx)) <= std_logic_vector(s_idx);
                        if s_idx = 255 then
                            s_idx <= (others => '0');
                            curr_status <= KSA_S1;
                        else
                            s_idx <= s_idx + 1;
                        end if;

                    when KSA_S1 =>
                        temp_val <= S(to_integer(s_idx));
                        temp_j_idx  <= j_idx;
                        curr_status <= KSA_S2;
                    
                    when KSA_S2 =>
                        S(to_integer(s_idx))   <= S(to_integer(temp_j_idx));
                        curr_status <= KSA_S3;
                    
                    when KSA_S3 =>
                        S(to_integer(temp_j_idx))   <= temp_val;
                        if key_idx = KEY_LEN - 1 then
                            key_idx <= 0;
                        else
                            key_idx <= key_idx + 1;
                        end if;
                        if s_idx = 255 then
                            s_idx       <= (others => '0');
                            temp_j_idx  <= (others => '0');
                            curr_status <= PRGA_S1;
                        else
                            curr_status <= KSA_S1;
                            s_idx <= s_idx + 1;
                        end if;
                    
                    when PRGA_S1 =>
                        temp_val    <= S(to_integer(s_idx + 1));
                        temp_j_idx  <= temp_j_idx + unsigned(S(to_integer(s_idx + 1)));
                        curr_status <= PRGA_S2;
                    
                    when PRGA_S2 =>
                        S(to_integer(s_idx + 1)) <= S(to_integer(temp_j_idx));
                        curr_status <= PRGA_S3;
                    
                    when PRGA_S3 =>
                        S(to_integer(temp_j_idx)) <= temp_val;
                        s_idx       <= s_idx + 1;
                        curr_status <= WAIT_SWAP;
                    
                    when WAIT_SWAP =>
                        t := unsigned(S(to_integer(s_idx))) + unsigned(S(to_integer(temp_j_idx)));
                        encr_data(msg_idx) <= msg(msg_idx) xor s(to_integer(t));
                        if msg_idx = MSG_LEN - 1 then
                            curr_status <= IDLE;
                            done <= '1';
                        else
                            curr_status <= PRGA_S1;
                            msg_idx <= msg_idx + 1;
                        end if;
            
                    when others =>

                end case;

            end if;
        end if;
    end process;
end architecture;
