-- 			INP Projekt N.1 		   --
-- Name: Zobrazovani na maticovom displeji riziene z FPGA  --
-- 		       Autor: Michal Ormos 		   --
-- 		         Login: xormos00  		   --
-- 		         Date: 8.11.2015		   --


-- including necessarily libraries
library IEEE;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

-- definition of main entity
entity ledc8x8 is
	port(
		SMCLK: in std_logic;
		ROW: out std_logic_vector (0 to 7);
		LED: out std_logic_vector (0 to 7);
		RESET: in std_logic		
	);
end entity ledc8x8;

architecture behaviour of ledc8x8 is
	signal control_count: STD_LOGIC_VECTOR (21 downto 0);
	signal ce_chg: STD_LOGIC;
	signal row_count: STD_LOGIC_VECTOR (0 to 7) := "01111111";
	signal letters_change: STD_LOGIC; -- switch name variable
	signal input_letters: STD_LOGIC_VECTOR (0 to 7); -- 8 bit display letter
	signal phase_change: STD_LOGIC := '0'; -- help variable to not blicking second letter
begin

-- Process of signal divide
deleniSig: process (RESET, SMCLK)
begin
	if RESET = '1' then
		control_count <= "0000000000000000000000";
	elsif SMCLK'event and SMCLK = '1' then
		control_count <= control_count + 1;
		if control_count(7 downto 0) = "11111111" then 
			ce_chg <= '1';
		else ce_chg <= '0'; 
		end if;

	end if;
	letters_change <= control_count(21); -- right phase for 0.5 blick
end process;

row_activator: process (RESET, SMCLK) --Aktivace radku
begin
	if RESET = '1'  then
		row_count <= "10000000";	
	-- Shifting row after row with help of letters process
	-- 0 is led on
	-- 1 is led off
	elsif SMCLK'event and SMCLK = '1' then
		if ce_chg = '1' then
			case row_count is         
				when "00000001" => row_count <= "10000000";
				when "00000010" => row_count <= "00000001";
				when "00000100" => row_count <= "00000010";
				when "00001000" => row_count <= "00000100";
				when "00010000" => row_count <= "00001000";
				when "00100000" => row_count <= "00010000";
				when "01000000" => row_count <= "00100000";
				when "10000000" => row_count <= "01000000";	
				when others => null;
			end case;
		end if;
	end if;		
end process;

-- Process of assignment right LEDs position to matrix
letters: process (row_count)
begin
	if letters_change = '1' then
		-- display full MO
		case row_count is         
			when "10000000" => input_letters <= "01110111";
			when "01000000" => input_letters <= "00100111";
			when "00100000" => input_letters <= "01010111";
			when "00010000" => input_letters <= "01110111";
			when "00001000" => input_letters <= "11111001";
			when "00000100" => input_letters <= "11110110";
			when "00000010" => input_letters <= "11110110";
			when "00000001" => input_letters <= "11111001";	
			when others => null;
			phase_change <= '1';
		end case;
	else
		-- If phase_change = 0, so we display the first letter
		-- If phase_change = else, so we display full two letters
		-- and we blink between the letters_change 1 and letters_change else
		if phase_change = '0' then
			-- display full M
			case row_count is
				when "10000000" => input_letters <= "01110111";
				when "01000000" => input_letters <= "00100111";
				when "00100000" => input_letters <= "01010111";
				when "00010000" => input_letters <= "01110111";
				when "00001000" => input_letters <= "11111111";
				when "00000100" => input_letters <= "11111111";
				when "00000010" => input_letters <= "11111111";
				when "00000001" => input_letters <= "11111111";		
				when others => null;
			end case;
		else
			-- display full MO
			case row_count is
				when "10000000" => input_letters <= "01110111";
				when "01000000" => input_letters <= "00100111";
				when "00100000" => input_letters <= "01010111";
				when "00010000" => input_letters <= "01110111";
				when "00001000" => input_letters <= "11111001";
				when "00000100" => input_letters <= "11110110";
				when "00000010" => input_letters <= "11110110";
				when "00000001" => input_letters <= "11111001";	
				when others => null;
			end case;
		end if;
	end if;
end process;

-- Process of turnning on the LEDs
display: process(row_count)
begin
	ROW <= row_count;
	LED <= input_letters;
end process;

end architecture behaviour;

-- END of ledc8x8.vhd




















