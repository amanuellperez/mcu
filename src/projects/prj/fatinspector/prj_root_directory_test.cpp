// Copyright (C) 2025 Manuel Perez 
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

#include "prj_dev.h"
#include "prj_strings.h"
#include "dev_print.h"
#include "sdc_print.h"
#include "prj_main.h"
#include "atd_fat.h"
#include "prj_print.h"

#include <atd_ostream.h>

void print_date(std::ostream& out, uint8_t day, uint8_t month, uint16_t year)
{
    out << (uint16_t) day << '/'
	 << (uint16_t) month << '/'
	 << year;
}

void print_time(std::ostream& out, 
		uint8_t hours, uint8_t minutes, uint8_t seconds)
{
     out << (uint16_t) hours 
	 << ':' << (uint16_t) minutes 
	 << ':' << (uint16_t) seconds;
}

void Main::root_directory_menu()
{
    using Volume = atd::FAT32::Volume<Sector_driver>;

    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }


    Volume vol{nsector};

    if (vol.last_operation_fail()){
	uart << "ERROR\n";
	return;
    }

    auto root_dir = vol.root_directory_first_cluster();
    if (root_dir == 0){
	uart << "Error reading root_directory_first_cluster\n";
	return;
    }

    Directory dir{vol, root_dir};

    while(1){
	uart << '\n';
	print_line(uart);
	
	//atd::print(uart, msg_root_directory_menu);
    
	uart << "Root directory menu\n"
		"\t0. Back main menu\n"
	        "\t1. Print short entries\n"
		"\t2. Print long entries\n"
		"\t3. ls archives\n"
		"\t4. ls directories\n"
		"\t5. New entry\n"
		"\t6. Remove short entry\n"
		"\t7. Remove long entry\n"
		"\t8. cd\n"
		"\t9. Print file\n";

	uint16_t cmd{};
	uart >> cmd;

	uint32_t cluster{};

	switch(cmd){
	    break; case 0: return;
	    break; case 1: root_directory_print_short_entries(vol, dir);
	    break; case 2: root_directory_print_long_entries(vol, dir);
	    break; case 3: print_ls(vol, dir, Attribute::archive);
	    break; case 4: print_ls(vol, dir, Attribute::directory);
	    break; case 5: new_entry(vol, dir);
	    break; case 6: remove_short_entry(vol, dir);
	    break; case 7: remove_long_entry(vol, dir);
	    break; case 8: 
			uart << "Cluster new directory: ";
			uart >> cluster;
			dir.cd(cluster);

	    break; case 9: print_file();

	}
    }

}


void Main::root_directory_print_short_entries(Volume& vol, Directory& dir)
{
    using Entry = Directory::Entry;

    static constexpr uint16_t max_entries = 60;
    uart << "Printing " << max_entries << " entries\n";

    uart << "n \t|type\t|name\t\t|attr\t|cluster\t|size\t|"
	"created\t|access\t|modified\t|\n";

    uint8_t k = 0;
    for (auto i = dir.index_begin(); 
	 i != dir.index_end() and k < max_entries; ++i, ++k){
	Entry entry;
	dir.read_short_entry(i, entry);

	uart << (uint16_t) k << '\t';

	std::array<uint8_t, 30> name;
	if (entry.type() == Entry::Type::info_entry){
	    auto len = entry.read_short_name(name);

	    uart << "info\t[";
	    print(uart, std::span<uint8_t>{name.data(), len});

	    uart << "]\t";
	    print(uart, entry.attribute());
	    uart << "\t" << entry.file_cluster()
		<< '\t' << entry.file_size()
		<< '\t';

	    uint8_t seconds, minutes, hours;
	    uint8_t day, month;
	    uint16_t year;

	    Entry::uint16_t2time(entry.creation_time(),
						    seconds, minutes, hours);
	    Entry::uint16_t2date(entry.creation_date(), day, month, year);

	    print_date(uart, day, month, year);
	    uart << ' ';
	    print_time(uart, hours, minutes, seconds);
	    uart << '.' << (uint16_t) entry.creation_time_tenth_of_seconds()
		<< '\t';


	    Entry::uint16_t2date(entry.last_access_date(), day, month, year);
	    print_date(uart, day, month, year);
	    uart << '\t';

	    Entry::uint16_t2date(entry.last_modification_date(), 
							    day, month, year);
	    Entry::uint16_t2time(entry.last_modification_time(),
						    seconds, minutes, hours);
	    print_date(uart, day, month, year);
	    uart << ' ';
	    print_time(uart, hours, minutes, seconds);

	    uart << '\n';

	} else if (entry.type() == Entry::Type::name_entry){
	    auto len = entry.read_long_name(name);

	    uart << "name("
		 << (uint16_t) entry.extended_order()
		 << ")";

	    uart << "\t[";
	    print(uart, std::span<uint8_t>{name.data(), len});
	    uart << "]\t";


	    print(uart, entry.attribute());

	    uart << "\tcheck sum: ";
	    atd::print_int_as_hex(uart,  entry.name_entry_check_sum());
	    uart << '\n';
	} else if (entry.type() == Entry::Type::free_available){
	    uart << "AVAILABLE ENTRY\n";
	} else if (entry.type() == Entry::Type::free_nomore){
	    uart << "LAST ENTRY\n";
	    return;
	}

//	atd::xxd_print(uart, entry.data);
//	uart << '\n';

	
    }
    uart << '\n';
    print_line(uart);

}



void Main::root_directory_print_long_entries(Volume& vol, Directory& dir)
{
    using Entry = Directory::Entry;
    
    uart << "n|name\t|attr\t\t|cluster\t|size\t|"
	"created\t|access\t|modified\t|\n";
    
    uint8_t k = 0;
    for (auto i = dir.index_begin(); 
	 i != dir.index_end() and k < 25; ++k){
	Entry_info info;
	std::array<uint8_t, 32> name;

	i = dir.read_long_entry(i, info, name);
	if (i == dir.index_end()){
	    if (dir.last_entry_error()){
		uart << "LAST ENTRY found\n";
		return;
	    }
	    else{
		uart << "ERROR: ";
		if(dir.read_error())
		    uart << "Read error\n";
		else if (dir.long_entry_corrupted_error())
		    uart << "long entry corrupted\n";
		else
		    uart << "Unknown\n";

	    }
	    return;
	}

    
	uart << (uint16_t) k << ": [";
	print_as_char(uart, name);
	uart << "]\n\t";

	print(uart, info.attribute);

	uart << '\t'
	     << info.file_cluster 
	     << '\t'
	     << info.file_size
	     << '\t';

	    uint8_t seconds, minutes, hours;
	    uint8_t day, month;
	    uint16_t year;

	    Entry::uint16_t2time(info.creation_time,
						    seconds, minutes, hours);
	    Entry::uint16_t2date(info.creation_date, day, month, year);
	
	    print_date(uart, day, month, year);
	    uart << ' ';
	    print_time(uart, hours, minutes, seconds);
	    uart << '.' << (uint16_t) info.creation_time_tenth_of_seconds
		<< '\t';


	    Entry::uint16_t2date(info.last_access_date, day, month, year);
	    print_date(uart, day, month, year);
	    uart << '\t';

	    Entry::uint16_t2date(info.last_modification_date, 
							    day, month, year);
	    Entry::uint16_t2time(info.last_modification_time,
						    seconds, minutes, hours);
	    print_date(uart, day, month, year);
	    uart << ' ';
	    print_time(uart, hours, minutes, seconds);
	    uart << '.' << (uint16_t) info.creation_time_tenth_of_seconds
		<< '\n';
    }
    uart << '\n';
    print_line(uart);

}


void Main::print_ls(Volume& vol, Directory& dir, Attribute att)
{
    uint8_t k = 0;
    for (auto i = dir.index_begin(); 
	 i != dir.index_end() and k < 25; ++k){

	Entry_info info;
	std::array<uint8_t, 32> name;

	i = dir.read_long_entry(i, info, name, att);
	if (i == dir.index_end()){
	    if (dir.last_entry_error())
		uart << "LAST ENTRY found\n";
	    else
		uart << "Error\n";
	    return;
	}

    
	uart << (uint16_t) k << ": [";
	print_as_char(uart, name);
	uart << "] (";

	print(uart, info.attribute, false);

	uart << "); cluster = "
	     << info.file_cluster 
	     << "; size = "
	     << info.file_size
	     << '\n';

    }
    uart << '\n';
    print_line(uart);
}



void Main::new_entry(Volume& vol, Directory& dir)
{
    uart << "Filename of new entry: ";
    std::array<uint8_t, 30> name;
    name.fill(0);

    read_cstring(uart, name);

    auto info = Directory::Entry_info::make_empty_entry();

    info.attribute = Directory::Attribute::archive;
    info.file_cluster = 25;
    info.file_size = 123;

    // (TODO) Esto es peligroso!!! Simplificar
    // ¿usar string_view en vez de span para new_long_entry?
    auto sz = strlen((const char*)(name.data()));
uart << "size = " << sz << '\n';
std::span<const uint8_t> sp{name.data(), sz};
uart << "sp.size = " << sp.size() << '\n';
    //uint8_t n = dir.new_long_entry(info, {name.data(), sz});
    uint8_t n = dir.new_long_entry(info, sp);
    uart << (int) n << " new short entries!!!\n";

    vol.flush();

}

void Main::remove_short_entry(Volume& vol, Directory& dir)
{
    uart << "Short entry to remove: ";
    uint16_t nentry{};
    uart >> nentry;

    if (dir.remove_short_entry(nentry)){
	uart << "OK\n";
	vol.flush();
    }
    else
	uart << "FAIL\n";
}



void Main::remove_long_entry(Volume& vol, Directory& dir)
{
    uart << "Long entry to remove: ";
    uint16_t nentry{};
    uart >> nentry;

    if (dir.remove_long_entry(nentry)){
	uart << "OK\n";
	vol.flush();
    }
    else
	uart << "FAIL\n";

}

