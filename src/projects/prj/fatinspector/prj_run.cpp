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
	    break; case 3: sector_driver_menu();
	    break; case 4: print_MBR_boot_sector();
	    break; case 5: reserved_area_menu();
	    break; case 6: FAT32_area_menu();
	    break; case 7: FAT32_area_list_menu();
	    break; case 8: root_directory_menu();
	    break; case 9: print_directory_ls();

	}
    }
}





void Main::reserved_area_menu()
{
    while(1){
	uart << '\n';
	print_line(uart);
	
	//atd::print(uart, msg_root_directory_menu);
    
	uart << "Reserved area menu\n"
		"\t0. Back main menu\n"
	        "\t1. Print FAT32 boot sector\n"
		"\t2. Print FS info sector\n";

	uint16_t cmd{};
	uart >> cmd;

	switch(cmd){
	    break; case 0: return;
	    break; case 1: reserved_area_print_FAT32_boot_sector();
	    break; case 2: reserved_area_print_FS_info();

	}
    }

}

void Main::sector_driver_menu()
{
    while(1){
	uart << '\n';
	print_line(uart);
	
	//atd::print(uart, msg_root_directory_menu);
    
	uart << "Sector driver test\n"
		"\t0. Back main menu\n"
	        "\t1. fill_n test\n";

	uint16_t cmd{};
	uart >> cmd;

	switch(cmd){
	    break; case 0: return;
	    break; case 1: sector_driver_fill_n_test();

	}
    }

}

