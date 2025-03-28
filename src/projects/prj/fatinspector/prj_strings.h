// Copyright (C) 2025 Manuel Perez 
// mail: <manuel2perez@proton.me> 
// https://github.com/amanuellperez/mcu 
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU General Public License for more details. 
// 
// You should have received a copy of the GNU General Public License 
// along with this program. If not, see <https://www.gnu.org/licenses/>. 

// This file was automatically generated by str2rom 
 
#pragma once 
 
#ifndef __PRJ_STRINGS_H__ 
#define __PRJ_STRINGS_H__ 


constexpr myu::ROM_string<102> msg_hello   
	PROGMEM{"\n\n----------------------------------------\nSDCard inspector\n----------------------------------------\n\n"};

constexpr myu::ROM_string<19> msg_main_no_sector_load 
	PROGMEM{"No sector in memory"};

constexpr myu::ROM_string<27> msg_main_sector_load 
	PROGMEM{"Sector in memory. Address: "};

constexpr myu::ROM_string<156> msg_main_menu	
	PROGMEM{"Read:\n\t1. Status register\n\t2. Sector\n\t3. Sector driver test\n\t4. MBR boot sector\n\t5. Reserved area\n\t6. FAT area\n\t7. FAT area list\n\t8. Root directory\n"};

constexpr myu::ROM_string<16> msg_sector_address	
	PROGMEM{"Sector address: "};

constexpr myu::ROM_string<29> msg_print_sector_from 
	PROGMEM{"Print sector from byte number"};

constexpr myu::ROM_string<39> msg_print_sector_size 
	PROGMEM{"Number of bytes to print (0 to the end)"};

constexpr myu::ROM_string<39> msg_print_sector_from_to_big 
	PROGMEM{"\nError: first byte has to be less than "};

constexpr myu::ROM_string<19> msg_print_sector_size_to_big 
	PROGMEM{"\nError: size to big"};

constexpr myu::ROM_string<72> msg_edit_sector_instructions_in_hex 
	PROGMEM{"Write all modifications in hexadecimal without 0x.\nPress return to end.\n"};

constexpr myu::ROM_string<55> msg_edit_sector_instructions_in_char 
	PROGMEM{"Write all modifications in chars.\nPress return to end.\n"};

constexpr myu::ROM_string<18> msg_edit_sector_from 
	PROGMEM{"First byte to edit"};

constexpr myu::ROM_string<13> msg_edit_sector_from_error 
	PROGMEM{"Error reading"};

constexpr myu::ROM_string<19> msg_print_sector_as_MBR 
	PROGMEM{"Master Boot Record\n"};

constexpr myu::ROM_string<19> msg_is_MBR_valid_sig 
	PROGMEM{"MBR has a valid sig"};

constexpr myu::ROM_string<12> msg_partition1 
	PROGMEM{"Partition 1\n"};

constexpr myu::ROM_string<8> msg_is_fat32 
	PROGMEM{"is FAT32"};

constexpr myu::ROM_string<10> msg_lba_offset 
	PROGMEM{"LBA offset"};

constexpr myu::ROM_string<11> msg_is_bootable 
	PROGMEM{"is bootable"};

constexpr myu::ROM_string<19> msg_print_sector_as_FAT_boot_sector 
	PROGMEM{"FAT32: boot sector\n"};

constexpr myu::ROM_string<15> msg_print_sector_as_FS_info 
	PROGMEM{"FAT32: FS info\n"};

constexpr myu::ROM_string<292> msg_root_directory_menu 
	PROGMEM{"Root directory menu\n\t0. Back main menu\n\t------------------------\n\t1. Print short entries\n\t2. Print long entries\n\t3. New entry\n\t4. Remove short entry\n\t5. Remove long entry\n\t------------------------\n\t6. ls archives\n\t7. ls directories\n\t8. cd\n\t9. mkfile\n\t10. rmfile\n\t11. Print file\n"};


 
#endif

