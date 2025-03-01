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

#include <ctype.h> // isprint

#include "prj_dev.h"
#include "prj_strings.h"
#include "dev_print.h"
#include "sdc_print.h"
#include "prj_main.h"
#include "atd_fat.h"

#include <atd_ostream.h>

void print_as_char(std::ostream& out, std::span<uint8_t> str)
{
    for (auto x: str){
	if (x == '\0')
	    return;

	out << x;
    }
}

void print(std::ostream& out, std::span<uint8_t> str)
{
    for (auto x: str)
	out << x;
}


void Main::print_sector()
{
    uart << "Sector number to print: ";
    uint32_t nsector{};
    uart >> nsector;

    Sector_driver::Sector sector{};
    auto r = SDCard::read(nsector, sector);
    if (!r.ok()){
	uart << "ERROR reading sector " << nsector << '\n';
	return;
    }

    print_line(uart);

    uart << "Sector: " << nsector << '\n';
    atd::xxd_print(uart, sector);
    
}






void Main::print_MBR_boot_sector()
{
    uart << '\n';
    atd::print(uart, msg_print_sector_as_MBR);
    print_line(uart);

    using MBR = atd::MBR;
    using MBR_type = atd::MBR::Partition_type;
    
    Sector_driver driver;
    auto mbr = driver.template lock_sector_and_view_as<MBR>(0);


    print_question(uart, msg_is_MBR_valid_sig, false);
    print_bool_as_yes_no(uart, mbr->is_valid());

    uart << '\n';
    atd::print(uart, msg_partition1);

    print_question(uart, msg_is_fat32);
    print_bool_as_yes_no(uart, mbr->partition1.type == MBR_type::fat32_lba);

    uart << '\t';
    atd::print(uart, msg_lba_offset);
    uart << " = " << mbr->partition1.lba_first_sector << '\n';

    uart << "\tlba size = " << mbr->partition1.lba_size << '\n';

    print_question(uart, msg_is_bootable);
    print_bool_as_yes_no(uart, mbr->partition1.is_bootable());


}


void print_long_name_entry(std::ostream& out, 
				const atd::FAT32::Long_file_name_entry& fn)
{
    for (uint8_t i = 0; i < 10 / 2; ++i){
	if (fn.name1[i] == 0x0000) return;

	out << (uint8_t) fn.name1[i];
    }

    for (uint8_t i = 0; i < 12 / 2; ++i){
	if (fn.name2[i] == 0x0000) return;

	out << (uint8_t) fn.name2[i];
    }

    for (uint8_t i = 0; i < 4 / 2; ++i){
	if (fn.name3[i] == 0x0000) return;

	out << (uint8_t) fn.name3[i];
    }

}


void print(std::ostream& out, Attribute att, bool with_tab = true)
{
    using Att = Attribute;

    switch(att){
	break; case Att::read_only  : out << "read_only";
	break; case Att::hidden	    : out << "hidden";
	break; case Att::system	    : out << "system";
	break; case Att::volume_id  : out << "volume_id";
	break; case Att::directory  : out << "directory";
	break; case Att::archive    : 
				      out << "archive";
				      if (with_tab) out << '\t';
	break; case Att::long_name  : out << "long_name";
    }

}



void Main::root_directory_print_short_entries()
{
    using Volume = atd::FAT32::Volume<Sector_driver>;

    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }


    Volume vol{nsector};

    if (vol.error()){
	uart << "ERROR\n";
	return;
    }


//    using File = atd::FAT32::File<Sector_driver, uint8_t>;
    
// ----------------
//    File f{vol, root_dir}; 
//    
//    uint32_t counter =  0; // limitamos el número de entradas
//    for (auto p = f.begin(); 
//	 p != f.end() and counter < 32 * sizeof(uint32_t); // 30 entradas máximo
//	 ++p, ++counter){
//	
//	if ((counter % 32) == 0 and counter != 0)
//	    uart << '\n';
//
//	atd::print_int_as_hex(uart, *p, false);
//	uart << ' ';
//    }
//    uart << '\n';
//    print_line(uart);
//
//    counter = 0;
//    for (auto p = f.begin(); 
//	 p != f.end() and counter < 32 * sizeof(uint32_t); // 30 entradas máximo
//	 ++p, ++counter){
//	
//	if ((counter % 32) == 0 and counter != 0)
//	    uart << '\n';
//
//	if (isprint(*p))
//	    uart << *p;
//	else
//	    uart << '.';
//    }
//    uart << '\n';
//    print_line(uart);
// ----------------
    {
    using Entry = Directory::Entry;

    uart << "n \t|type\t|name\t\t|attr\t|cluster\t|size\t|"
	"created\t|access\t|modified\t|\n";

    auto root_dir = vol.root_directory_first_cluster();
    if (root_dir == 0){
	uart << "Error reading root_directory_first_cluster\n";
	return;
    }

    Directory dir{vol, root_dir};
    dir.first_entry();
    for (uint8_t i = 0; i < 30; ++i){
	Entry entry;
	dir.read(entry);
	
	uart << (uint16_t) i << '\t';

	std::array<uint8_t, 30> name;
	if (entry.type() == Entry::Type::short_entry){
	    auto len = entry.copy_short_name(name);

	    uart << "short\t";
	    print(uart, std::span<uint8_t>{name.data(), len});

	    uart << '\t';
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

	    uart << (uint16_t) day << '/'
		 << (uint16_t) month << '/'
		 << year << ' '
		 << (uint16_t) hours 
		 << ':' << (uint16_t) minutes 
		 << ':' << (uint16_t) seconds
		 << '.' << (uint16_t) entry.creation_time_tenth_of_seconds() 
		<< '\t';

	    Entry::uint16_t2date(entry.last_access_date(), day, month, year);
	    uart << (uint16_t) day << '/'
		 << (uint16_t) month << '/'
		 << year << ' '
		<< '\t';

	    Entry::uint16_t2date(entry.last_modification_date(), 
							    day, month, year);
	    Entry::uint16_t2time(entry.last_modification_time(),
						    seconds, minutes, hours);
	    uart << (uint16_t) day << '/'
		 << (uint16_t) month << '/'
		 << year << ' '
		 << (uint16_t) hours 
		 << ':' << (uint16_t) minutes 
		 << ':' << (uint16_t) seconds
		<< '\n';

	} else if (entry.type() == Entry::Type::long_entry){
	    auto len = entry.copy_long_name(name);

	    uart << "long\t";
	    print(uart, std::span<uint8_t>{name.data(), len});
	    uart << "\t";
	    print(uart, entry.attribute());
	    uart << "\torder: ";
	    atd::print_int_as_hex(uart, entry.extended_order_with_mask());
	    if (entry.is_last_member_of_long_name()){
		uart << "(last: " << (uint16_t) entry.extended_order() << ")";
	    }
	    uart << "\tcheck sum: ";
	    atd::print_int_as_hex(uart,  entry.check_sum());
	    uart << '\n';
	} else if (entry.type() == Entry::Type::free){
	    uart << "FREE ENTRY\n";
	} else if (entry.type() == Entry::Type::last_entry){
	    uart << "LAST ENTRY\n";
	}

//	atd::xxd_print(uart, entry.data);
//	uart << '\n';

	
    }
    uart << '\n';
    print_line(uart);
    }

}



void Main::root_directory_print_long_entries()
{
    using Volume = atd::FAT32::Volume<Sector_driver>;

    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }


    Volume vol{nsector};

    if (vol.error()){
	uart << "ERROR\n";
	return;
    }


    auto root_dir = vol.root_directory_first_cluster();
    if (root_dir == 0){
	uart << "Error reading root_directory_first_cluster\n";
	return;
    }
    using Entry = Directory::Entry;

    Directory dir{vol, root_dir};
    dir.first_entry();
    
    uart << "n|name\t|attr\t\t|cluster\t|size\t|"
	"created\t|access\t|modified\t|\n";
    for (uint8_t i = 0; i < 25; ++i){
	Entry_info info;
	std::array<uint8_t, 32> name;

	if (!dir.read_long_entry(info, name)){
	    if (dir.last_entry())
		uart << "LAST ENTRY found\n";
	    else
		uart << "Error\n";
	    return;
	}

    
	uart << (uint16_t) i << ": [";
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

	    uart << (uint16_t) day << '/'
		 << (uint16_t) month << '/'
		 << year << ' '
		 << (uint16_t) hours 
		 << ':' << (uint16_t) minutes 
		 << ':' << (uint16_t) seconds
		 << '.' << (uint16_t) info.creation_time_tenth_of_seconds
		<< '\t';

	    Entry::uint16_t2date(info.last_access_date, day, month, year);
	    uart << (uint16_t) day << '/'
		 << (uint16_t) month << '/'
		 << year << ' '
		<< '\t';

	    Entry::uint16_t2date(info.last_modification_date, 
							    day, month, year);
	    Entry::uint16_t2time(info.last_modification_time,
						    seconds, minutes, hours);
	    uart << (uint16_t) day << '/'
		 << (uint16_t) month << '/'
		 << year << ' '
		 << (uint16_t) hours 
		 << ':' << (uint16_t) minutes 
		 << ':' << (uint16_t) seconds
		<< '\n';
    }
    uart << '\n';
    print_line(uart);

}


void Main::print_ls(Volume& vol, Directory& dir, Attribute att)
{
    dir.first_entry();
    
    for (uint8_t i = 0; i < 25; ++i){
	Entry_info info;
	std::array<uint8_t, 32> name;

	if (!dir.read_long_entry(info, name, att)){
	    if (dir.last_entry())
		uart << "LAST ENTRY found\n";
	    else
		uart << "Error\n";
	    return;
	}

    
	uart << (uint16_t) i << ": [";
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

void Main::print_directory_ls()
{
    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }


    Volume vol{nsector};

    if (vol.error()){
	uart << "ERROR\n";
	return;
    }

    auto cluster = vol.root_directory_first_cluster();
    if (cluster == 0){
	uart << "Error reading root_directory_first_cluster\n";
	return;
    }

    // Voy a definir aqui Directory porque quiero probar las funciones de esta
    // clase.
    Directory dir{vol, cluster};

    while(1){

	uart << "\nMenu\n"
		"----\n"
		"\t0. Back main menu\n"
		"\t1. ls archives\n"
		"\t2. ls directories\n"
		"\t3. cd\n"
		"\t4. Print file\n";

	uint16_t opt{};
	uart >> opt;
	switch(opt){
	    break; case 0: return;
	    break; case 1: print_ls(vol, dir, Attribute::archive);
	    break; case 2: print_ls(vol, dir, Attribute::directory);
    //	break; case '3': att = Attribute::archive | Attribute::read_only;
	    break; case 3: 
			uart << "Cluster new directory: ";
			uart >> cluster;
			dir.cd(cluster);

	    break; case 4: print_file();
	}

    }
}




// Esta función prueba File_sectors
// Aunque es parecida a print_FAT32_entry, esa otra prueba FAT_area
void Main::print_file_sectors()
{
    using File_sectors   = atd::FAT32::File_sectors<Sector_driver>;

    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }

    Volume vol{nsector};

    if (vol.error()){
	uart << "ERROR reading FAT volume\n";
	return;
    }

    uart << "First cluster of the file (>= 2): ";
    uint32_t cluster0{};
    uart >> cluster0;
    
    if (cluster0 < 0){
	uart << "Error, cluster has to be >= 2\n";
	return;
    }

    File_sectors file(vol, cluster0);    // file.begin(cluster0);
   
    if(file.error()){
	uart << "Error creating file\n";
	return;
    }

    uint32_t i = 0;

    while(1){
	uart << "Cluster " << (uint16_t) i << ": " 
			   << file.global_sector_number() 
			   << "; first sector (global to disk): " 
			   << file.global_sector_number() 
			   << '\n';
	if (file.end_of_sectors()){
	    uart << "EOC\n";
	    return;
	} else if(file.error()){
	    uart << "Error\n";
	    return;
	}

	//file.next_sector();
	if(!file.next_cluster()){
	    if (file.end_of_sectors())
		uart << "EOC\n";
	    else
		uart << "ERROR reading cluster\n";

	    return;
	}
    
	++i;
    }

}

void Main::print_file()
{
    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }

    Volume vol{nsector};

    if (vol.error()){
	uart << "ERROR reading FAT volume\n";
	return;
    }

    uart << "File cluster to print: ";
    uint32_t fcluster{};
    uart >> fcluster;

    uart << "File size: ";
    uint32_t fsize;
    uart >> fsize;

    File file{vol, fcluster, fsize};
    
    std::array<uint8_t, 16> data;

    uint32_t addr = 0;

    while(1){
	auto n = file.read(data);

	if (n == 0){
	    if (file.end_of_file())
		uart << "EOF\n";
	    else
		uart << "Error: n == 0 but is not EOF\n";

	    return;
	}

	atd::xxd_print(uart, {data.data(), n}, addr);
	addr += atd::Xxd::nbytes_per_line;

    }

}


