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
	    break; case 6: FAT32_area_menu();
	    break; case 7: root_directory_menu();
	    break; case 8: print_directory_ls();
	    break; case 9: print_file_sectors();

	}
    }
}


void Main::FAT32_area_menu()
{
    while(1){
	uart << '\n';
	print_line(uart);
	
	//atd::print(uart, msg_fat_area_menu);
    
	uart << "FAT area menu\n"
		"\t0. Back main menu\n"
	        "\t1. Print list of clusters of a file\n"
		"\t2. New list\n"
		"\t3. Remove list\n"
		"\t4. Add cluster\n"
		"\t5. Remove next cluster\n";

	uint16_t cmd{};
	uart >> cmd;

	switch(cmd){
	    break; case 0: return;
	    break; case 1: FAT32_area_print_clusters();
	    break; case 2: FAT32_area_new_list();
	    break; case 3: FAT32_area_remove_list();
	    break; case 4: FAT32_area_add_cluster();
	    break; case 5: FAT32_area_remove_next_cluster();

	}
    }

}

void Main::root_directory_menu()
{
    while(1){
	uart << '\n';
	print_line(uart);
	
	//atd::print(uart, msg_root_directory_menu);
    
	uart << "Root directory menu\n"
		"\t0. Back main menu\n"
	        "\t1. Print short entries\n"
		"\t2. Print long entries\n";

	uint16_t cmd{};
	uart >> cmd;

	switch(cmd){
	    break; case 0: return;
	    break; case 1: root_directory_print_short_entries();
	    break; case 2: root_directory_print_long_entries();

	}
    }

}
