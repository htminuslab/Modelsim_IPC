---------------------------------------------------------------------------------------------------
-- Simple FLI demo using shared memory
--
--
-- https://github.com/htminuslab            
--                                       
---------------------------------------------------------------------------------------------------
--                                                    
---------------------------------------------------------------------------------------------------
-- 
--  Revision History:                                                        
--                                                                           
--  Date:        Revision    Author         
--  19 Aug 2019  0.1         Hans Tiggeler 
--  18 May 2022  0.2         Hans Tiggeler  Added input string for MapName and checked with 2022.2
---------------------------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package pkg is
	function CreateMemoryMap(mapname:in string) return boolean;
	attribute FOREIGN of CreateMemoryMap : function is "CreateMemoryMap ./fli_mmap.dll";	

	function FreeMemoryMap return boolean;
	attribute FOREIGN of FreeMemoryMap : function is "FreeMemoryMap ./fli_mmap.dll";			
	
	procedure ReadMemoryMap(valid:out boolean; vector:out string);
	attribute FOREIGN of ReadMemoryMap : procedure is "ReadMemoryMap ./fli_mmap.dll";	
end;

package body pkg is	
	function CreateMemoryMap(mapname:in string) return boolean is begin return false; end;
	function FreeMemoryMap return boolean is begin return false; end;
	procedure ReadMemoryMap(valid:out boolean; vector:out string) is begin end;
end;


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb is
end tb;

use work.pkg.all;

architecture example of tb is
begin

	process
		variable retval_v : boolean;
		variable vector_v : string(1 to 128):=(others => '0');	
	begin
		retval_v:=CreateMemoryMap("Local\Test");				-- Open Shared Memory	
		assert (retval_v) report("Failed to create memory map") severity failure;
		wait for 1 ns;
	
		loop	
			ReadMemoryMap(retval_v, vector_v);					-- Read vector
			if retval_v then
				report("Receiving vector "& vector_v);
				exit when vector_v(29)='1';						-- Exit on last vector
			end if;
			wait for 1 ns;
		end loop;
	
		retval_v:=FreeMemoryMap;								-- Clean up
		assert (retval_v) report("Failed to release memory map") severity error;
		wait;
	end process;

end;

