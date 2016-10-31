-- fsm.vhd: Finite State Machine
-- Author(s): Michal Ormos , xormos00 , VUT FIT , v 1.0

--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST_1, TEST_2A, TEST_2B, TEST_3A, TEST_3B, TEST_4A, TEST_4B, TEST_5A, TEST_5B, TEST_6A, TEST_6B, TEST_7A, TEST_7B, TEST_8A, TEST_8B, TEST_9A, TEST_9B, TEST_10, TEST_END, WRONG, FINISH, PRINT_MESSAGE_OK, PRINT_MESSAGE_WRONG);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST_1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - - TEST FIRST DIGIT
   when TEST_1 =>
      next_state <= TEST_1;
      if (KEY(1) = '1') then
      	 next_state <= TEST_2A;
      elsif (KEY(3) = '1') then
	 next_state <= TEST_2B;	 
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG;	 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - TEST SECOND DIGIT
   when TEST_2A =>
      next_state <= TEST_2A;
      if (KEY(1) = '1') then
      	 next_state <= TEST_3A;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_2B =>
      next_state <= TEST_2B;
      if (KEY(4) = '1') then
      	 next_state <= TEST_3B;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -  TEST THIRD DIGIT
   when TEST_3A =>
      next_state <= TEST_3A;
      if (KEY(5) = '1') then
      	 next_state <= TEST_4A;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - 
   when TEST_3B =>
      next_state <= TEST_3B;
      if (KEY(5) = '1') then
      	 next_state <= TEST_4B;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - TEST FOURTH DIGIT      
   when TEST_4A =>
      next_state <= TEST_4A;
      if (KEY(1) = '1') then
      	 next_state <= TEST_5A;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_4B =>
      next_state <= TEST_4B;
      if (KEY(4) = '1') then
	 next_state <= TEST_5B;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - TEST FIFTH DIGIT     
   when TEST_5A =>
      next_state <= TEST_5A;
      if (KEY(5) = '1') then
      	 next_state <= TEST_6A;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_5B =>
      next_state <= TEST_5B;
      if (KEY(6) = '1') then
      	 next_state <= TEST_6B;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - TEST SIXT DIGIT
   when TEST_6A =>
      next_state <= TEST_6A;
      if (KEY(4) = '1') then
      	 next_state <= TEST_7A;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_6B =>
      next_state <= TEST_6B;
      if (KEY(3) = '1') then
      	 next_state <= TEST_7B;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - TEST SEVENTH DIGIT
   when TEST_7A =>
      next_state <= TEST_7A;
      if (KEY(5) = '1') then
      	 next_state <= TEST_8A;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_7B =>
      next_state <= TEST_7B;
      if (KEY(6) = '1') then
      	 next_state <= TEST_8B;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - TEST EIGHT DIGIT
   when TEST_8A =>
      next_state <= TEST_8A;
      if (KEY(5) = '1') then
      	 next_state <= TEST_9A;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_8B =>
      next_state <= TEST_8B;
      if (KEY(6) = '1') then
      	 next_state <= TEST_9B;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;     
   -- - - - - - - - - - - - - - - - - - - - - - - TEST NINTH DIGIT
   when TEST_9A =>
      next_state <= TEST_9A;
      if (KEY(3) = '1') then
      	 next_state <= TEST_10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;     
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_9B =>
      next_state <= TEST_9B;
      if (KEY(0) = '1') then
      	 next_state <= TEST_10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;    
   -- - - - - - - - - - - - - - - - - - - - - - - TEST TENTH DIGIT
   when TEST_10 =>
      next_state <= TEST_10;
      if (KEY(4) = '1') then
      	 next_state <= TEST_END;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if;     
   -- - - - - - - - - - - - - - - - - - - - - - - TEST # (ENTER)
   when TEST_END =>
      next_state <= TEST_END;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_OK;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	 next_state <= WRONG; 
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - - TO MUCH/LESS/BAD NUMBERS
   when WRONG =>
      next_state <= WRONG;
      if (KEY(15) = '1') then
	 next_state <= PRINT_MESSAGE_WRONG;
   end if;
   -- - - - - - - - - - - - - - - - - - - - - - - PRINT "PRISTUP POVOLEN"
   when PRINT_MESSAGE_OK =>
     next_state <= PRINT_MESSAGE_OK;
     if (CNT_OF = '1') then
       next_state <= FINISH;
   end if;
   -- - - - - - - - - - - - - - - - - - - - - - - PRINT "PRISTUP ODEPREN"
   when PRINT_MESSAGE_WRONG =>
     next_state <= PRINT_MESSAGE_WRONG;
     if (CNT_OF = '1') then
       next_state <= FINISH;
   end if;
   -- - - - - - - - - - - - - - - - - - - - - - - START ANOTHER TRY
   when FINISH =>
     next_state <= FINISH;
     if (KEY(15) = '1') then
       next_state <= TEST_1;
   end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_WRONG =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
      FSM_MX_MEM     <= '0';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
      FSM_MX_MEM     <= '1';      
   -- - - - - - - - - - - - - - - - - - - - - - -     
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
     if (KEY(14 downto 0) /= "000000000000000") then
       FSM_LCD_WR	<= '1';
     end if;
     if (KEY(15) = '1') then
       FSM_LCD_CLR	<= '1';
     end if;
   -- - - - - - - - - - - - - - - - - - - - - - -     
   end case;
end process output_logic;

end architecture behavioral;

