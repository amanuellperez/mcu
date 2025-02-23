// Copyright (C) 2023-2025 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
//
// This file is part of the MCU++ Library.
//
// MCU++ Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "prj_main.h"
#include "dev_print.h"	// print_line
			

void Main::run()
{
    while(1){
	uart << '\n';
	print_line(uart);
	
	atd::print(uart, msg_main_menu);

	uint16_t cmd{};
	uart >> cmd;

	switch(cmd){
	    break; case 1: read_status();
	    break; case 2: print_sector();
	    break; case 3: print_MBR_boot_sector();
	    break; case 4: print_FAT32_boot_sector();
	    break; case 5: print_FS_info();
	    break; case 6: print_FAT32_entry();
	    break; case 7: print_root_directory_short_entries();
	    break; case 8: print_root_directory_long_entries();
	    break; case 9: print_directory_ls();
	    break; case 10: print_file_sectors();

	}
    }
}


