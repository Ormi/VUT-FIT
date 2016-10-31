-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2015 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Michal Ormos, xormos00@stud.fit.vutbr.cz
-- This is my first larger project in VHDL, sorry for many useless comments

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	signal PcReg: std_logic_vector(12 downto 0);
	signal PcInc: std_logic;
	signal PcDec: std_logic;

	signal PtrReg: std_logic_vector(12 downto 0);
	signal PtrInc: std_logic;
	signal PtrDec: std_logic;

	signal CntReg: std_logic_vector(7 downto 0);
	signal CntInc: std_logic;
	signal CntDec: std_logic;
	
	signal TmpReg: std_logic_vector(7 downto 0) := "00000000";

	type a_state is (s_idle, s_load, s_load_reg, s_decode, 
	s_inc_ptr, s_dec_ptr, 
	s_inc_ptr_val, s_dec_ptr_val, s_dec_ptr_val_temp, s_inc_ptr_val_temp,
	s_while_false, s_while_false_temp, s_while_false_load, s_while_false_load_2, s_while_false_load_3,
	s_while_true, s_while_true_temp, s_while_true_load, s_while_true_load_2, s_while_true_load_3,	
	s_print_ptr_val, s_read_val_ptr, s_print_ptr_val_temp, s_read_val_ptr_temp,
	s_store_val_ptr, s_store_val_ptr_temp, s_store_ptr_val, 
	s_halt, 
	s_skip);
	signal State: a_state;
	signal NextState: a_state;

	type t_instruction is (
	inc_ptr, 
	dec_ptr, 
	inc_ptr_val, 
	dec_ptr_val, 
	while_false, 
	while_true, 
	print_ptr_val, 
	read_val_ptr, 
	store_val_ptr, 
	store_ptr_val, 
	halt, 
	skip);

	signal instruction: t_instruction;

	signal sel_mx2: std_logic_vector(1 downto 0);
	signal sel_mx1: std_logic;	

--------------------------------------------------------------------------------
--||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
--------------------------------------------------------------------------------
begin

	-- Programovy Citac / Registr PC
	PcRegProc: process(CLK, RESET)
	begin
		if (RESET = '1') then -- If needed, RESET
			PcReg <= (others=>'0');		
		elsif (CLK'event) and (CLK = '1') then -- else CLK run
			if (PcInc = '1') then
				PcReg <= PcReg + 1;
			elsif (PcDec = '1') then
				PcReg <= PcReg - 1;
			end if;
		end if;	
	end process; 

	PtrRegProc: process(CLK, RESET)
	begin
		if (RESET = '1') then -- If needed, RESET
			PtrReg <= "1000000000000";	
		elsif (CLK'event) and (CLK = '1') then -- else CLK run
			if (PtrInc = '1') then
				PtrReg <= PtrReg + 1;
			elsif (PtrDec = '1') then
				PtrReg <= PtrReg - 1;
			end if;
		end if;	
	end process; 

	CntRegProc: process(CLK, RESET)
	begin
		if (RESET = '1') then -- If needed, RESET
			CntReg <= (others=>'0');	
		elsif (CLK'event) and (CLK = '1') then -- else CLK run
			if (CntInc = '1') then
				CntReg <= CntReg + 1;
			elsif (CntDec = '1') then
				CntReg <= CntReg - 1;
			end if;
		end if;	
	end process;

	-- Instruction decoder
	IDekoder: process(DATA_RDATA) -- decode instruction from ROM
	begin
		case (DATA_RDATA) is
			when X"3E" => instruction <= inc_ptr;  		--">"
			when X"3C" => instruction <= dec_ptr;  		--"<"
			when X"2B" => instruction <= inc_ptr_val;  	--"+"
			when X"2D" => instruction <= dec_ptr_val;  	--"-"
			when X"5B" => instruction <= while_false;  	--"["
			when X"5D" => instruction <= while_true;  	--"]"
			when X"2E" => instruction <= print_ptr_val; --"."
			when X"2C" => instruction <= read_val_ptr;  --","
			when X"24" => instruction <= store_val_ptr; --"$"
			when X"21" => instruction <= store_ptr_val; --"!"
			when X"00" => instruction <= halt;  		--"null"
			when others => instruction <= skip; 		--others -> skip
		end case;
	end process;

	MX1: process(CLK, sel_mx1, PcReg, PtrReg)
	begin
		case sel_mx1 is
			when '0' => DATA_ADDR <= PtrReg;
			when '1' => DATA_ADDR <= PcReg;
			when others =>
		end case;
	end process;

	MX2: process(CLK, sel_mx2, DATA_RDATA, IN_DATA)
	begin
		case sel_mx2 is
			when "00" => DATA_WDATA <= IN_DATA;
			when "01" => DATA_WDATA <= DATA_RDATA + 1;
			when "10" => DATA_WDATA <= DATA_RDATA - 1;
			when "11" => DATA_WDATA <= TmpReg;
			when others =>
		end case;
	end process;

	-- FSM present state
	a_pstate: process(RESET, CLK)
	begin
	  if (RESET='1') then
		 State <= s_idle;
		elsif (CLK'event) and (CLK='1') then
		if(EN = '1') then
			State <= NextState; --change for begin state
		end if;
	  end if;
	end process;

	Automata: process(IN_VLD, IN_DATA, DATA_RDATA, OUT_BUSY, State, instruction, CntReg, sel_mx1, sel_mx2)
	begin
		DATA_EN <= '0';
		DATA_RDWR <= '1';
		sel_mx2 <= "11";
		PtrDec <= '0';
		PtrInc <= '0';
		PcDec <= '0';
		PcInc <= '0';
		CntInc <= '0';
		CntDec <= '0';
		IN_REQ <= '0';
		OUT_WE <= '0';		

		case State is
			when s_idle =>
				NextState <= s_load;

			-- load instruction, ROM request
			when s_load =>
				NextState <= s_decode;
				DATA_EN <= '1';
				sel_mx1 <= '1';

			when s_decode =>
				case instruction is
					when inc_ptr =>					-- >
						NextState <= s_inc_ptr;

					when dec_ptr =>					-- <
						NextState <= s_dec_ptr;

					when inc_ptr_val => 			-- +
						NextState <= s_inc_ptr_val;

					when dec_ptr_val => 			-- -
						NextState <= s_dec_ptr_val;

					when while_false => 			-- [
						NextState <= s_while_false;

					when while_true => 				-- ]
						NextState <= s_while_true;

					when print_ptr_val => 			-- .
						NextState <= s_print_ptr_val;

					when read_val_ptr => 			-- ,
						NextState <= s_read_val_ptr;

					when store_val_ptr => 			-- $
						NextState <= s_store_val_ptr;

					when store_ptr_val => 			-- ,
						NextState <= s_store_ptr_val;

					when halt =>					-- NULL
						NextState <= s_halt;

					when others =>
						NextState <= s_skip;
				end case;

			-- > 
			-- ptr += 1;
			--------------
			-- PTR <- PTR + 1, PC <- PC + 1
			when s_inc_ptr =>
				PtrInc <= '1';
				PtrDec <= '0';

				PcDec <= '0';
				PcInc <= '1';

				NextState <= s_load;

			-- < 
			-- ptr += 1;
			--------------
			-- PTR <- PTR - 1, PC <- PC + 1
			when s_dec_ptr =>
				PtrInc <= '0';
				PtrDec <= '1';

				PcDec <= '0';
				PcInc <= '1';

				NextState <= s_load;	

			-- + 
			-- *ptr += 1;
			-------------
			-- DATA_RDATA <- RAM[PTR]
			-- ram[PTR] <- DATA_RDATA + 1 (sel_mx2)
			-- PC <- PC + 1
			when s_inc_ptr_val =>
				DATA_EN <= '1';
				sel_mx1 <= '0';
				DATA_RDWR <= '1';		

				NextState <= s_inc_ptr_val_temp;

				when  s_inc_ptr_val_temp =>
					DATA_EN <= '1'; 	--memory usage allowed
					DATA_RDWR <= '0'; 	-- write on

					sel_mx1 <= '0';
					sel_mx2 <= "01";

					PcDec <= '0';
					PcInc <= '1';

					NextState <= s_load;

			-- -
			-- *ptr -= 1;
			-------------
			-- DATA_RDATA <- RAM[PTR]
			-- ram[PTR] <- DATA_RDATA - 1 (sel_mx2)
			-- PC <- PC + 1
			when s_dec_ptr_val =>
				DATA_EN <= '1';
				sel_mx1 <= '0';
				DATA_RDWR <= '1';		

				NextState <= s_dec_ptr_val_temp;

				when  s_dec_ptr_val_temp =>
					DATA_EN <= '1';
					DATA_RDWR <= '0';

					sel_mx1 <= '0';
					sel_mx2 <= "10";

					PcDec <= '0';
					PcInc <= '1';

					NextState <= s_load;	

			-- . 
			-- putchar(*ptr);
			-----------------
			-- while (OUT_BUSY) {}
			-- OUT_DATA <- ram[PTR], PC <- PC + 1
			when s_print_ptr_val =>
				DATA_EN <= '1'; -- memory usage allowed
				sel_mx1 <= '0'; -- memory bus allowed	
				DATA_RDWR <= '1'; -- read mode on			
				
				NextState <= s_print_ptr_val_temp;

				when s_print_ptr_val_temp =>	
					NextState <= s_print_ptr_val_temp;	
					if (OUT_BUSY = '0') then -- LCD write allowed
						OUT_WE <= '1';
						OUT_DATA <= DATA_RDATA;
						NextState <= s_load;
						PcDec <= '0';
						PcInc <= '1';
					end if;

			-- , 
			-- *ptr = getchar();
			-----------------
			-- IN_REQ <- 1
			-- while (!IN_VLD) {}
			-- ram[PTE] <- IN_DATA, PC <- PC + 1;
			when s_read_val_ptr =>
				IN_REQ <= '1';
				NextState <= s_read_val_ptr_temp;

			when s_read_val_ptr_temp =>
				NextState <= s_read_val_ptr_temp;
				IN_REQ <= '1';				
				if (IN_VLD = '1') then
					DATA_EN <= '1'; -- memory usage allowed	
					DATA_RDWR <= '0'; -- write mode on
					PcDec <= '0';
					PcInc <= '1';
					sel_mx1 <= '0';
					sel_mx2 <= "00";
					NextState <= s_load;
				end if;

			-- [
			-- while (*ptr) {
			-----------------			
			-- if (ram[PTR] == 0)
			--		CNT ← 1
			--		while (CNT != 0)
			--			c ← rom[PC]
			--			if (c == ’[’) CNT ← CNT + 1 elsif (c == ’]’) CNT ← CNT - 1
			--			PC ← PC + 1			
			when s_while_false =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				PcDec <= '0';
				PcInc <= '1';
				sel_mx1 <= '0';

				NextState <= s_while_false_temp;				
			
				when s_while_false_temp =>
					NextState <= s_load;
					if (DATA_RDATA = "00000000") then
						NextState <= s_while_false_load;
						CntInc <= '1';
					end if;

				when s_while_false_load	=>
					NextState <= s_while_false_load_2; -- loop begin
					DATA_EN <= '1';
					sel_mx1 <= '1';
				when s_while_false_load_2 =>
					NextState <= s_while_false_load_3;
					PcInc <= '1';	
					if (instruction = while_false) then
						CntInc <= '1';
					elsif (instruction = while_true) then
						CntDec <= '1';
					end if;

				when s_while_false_load_3 =>
					NextState <= s_while_false_load;
				if (CntReg = "00000000") then --h jump to loop begin
					NextState <= s_load;
				end if;


			-- ]
			-- ]
			-----------------
			-- 	if (ram[PTR] == 0)
			--		PC ← PC + 1
			--	else	
			--		CNT ← 1
			--		PC ← PC - 1
			--		while (CNT != 0)
			--			c ← rom[PC]
			--			if (c == ’]’) CNT ← CNT + 1 elsif (c == ’[’) CNT ← CNT - 1
			--			if (CNT == 0) PC ← PC + 1 else PC ← PC - 1				
			when s_while_true =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';

				sel_mx1 <= '0';
				NextState <= s_while_true_temp;
		
				when s_while_true_temp =>
					NextState <= s_load;
					if (DATA_RDATA = "00000000") then
						PcInc <= '1';
						PcDec <= '0';
					else
						PcDec <= '1';
						PcInc <= '0';

						CntInc <= '1';
						CntDec <= '0';

						NextState <= s_while_true_load;
					end if;

				when s_while_true_load =>
					DATA_EN <= '1';
					sel_mx1 <= '1';
					NextState <= s_while_true_load_2;

				when s_while_true_load_2 =>
					NextState <= s_while_true_load_3;
					if (instruction = while_false) then
						CntDec <= '1';
					elsif (instruction = while_true) then
						CntInc <= '1';
					end if;
	
				when s_while_true_load_3 =>
					Nextstate <= s_load;
					if (CntReg = "00000000") then
						PcInc <= '1';
						PcDec <= '0';
					else
						PcDec <= '1';
						PcInc <= '0';

						NextState <= s_while_true_load;						
					end if; 

			-- $
			-- tmp = *ptr;
			--------------
			--	DATA RDATA ← ram[PTR]
			--	TMP ← DATA RDATA, PC ← PC + 1
			when s_store_val_ptr =>
				DATA_EN <= '1';
				sel_mx1 <= '0';
				DATA_RDWR <= '1';		

				NextState <= s_store_val_ptr_temp;

				when  s_store_val_ptr_temp =>
					DATA_EN <= '1'; -- memory usage allowed
					DATA_RDWR <= '0'; -- we will write

					sel_mx1 <= '0';
					TmpReg <= DATA_RDATA;

					PcDec <= '0';
					PcInc <= '1';

					NextState <= s_load;	
					
			-- !
			-- *ptr = tmp;
			--------------
			--	ram[PTR] ← TMP, PC ← PC + 1
			when s_store_ptr_val =>
				DATA_EN <= '1'; -- memory usage allowed
				DATA_RDWR <= '0'; -- we will write
				PcDec <= '0';
				PcInc <= '1';
					-- important
					sel_mx1 <= '0';
					-- make all work
					sel_mx2 <= "11";
				
				NextState <= s_load;

			-- NULL
			-- return
			---------
			--	PC ← PC
			when s_halt =>
				NextState <= s_halt;					

			-- else
			--	PC ← PC + 1
			when s_skip =>
				PcDec <= '0';
				PcInc <= '1';
				NextState <= s_load;

			when others =>

		end case;
	end process;


end behavioral;
 
