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
    Sector_driver::read(nsector, sector);

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
    
    MBR mbr{};
    atd::read<Sector_driver>(mbr);

    print_question(uart, msg_is_MBR_valid_sig, false);
    print_bool_as_yes_no(uart, mbr.is_valid());

    uart << '\n';
    atd::print(uart, msg_partition1);

    print_question(uart, msg_is_fat32);
    print_bool_as_yes_no(uart, mbr.partition1.type == MBR_type::fat32_lba);

    uart << '\t';
    atd::print(uart, msg_lba_offset);
    uart << " = " << mbr.partition1.lba_first_sector << '\n';

    uart << "\tlba size = " << mbr.partition1.lba_size << '\n';

    print_question(uart, msg_is_bootable);
    print_bool_as_yes_no(uart, mbr.partition1.is_bootable());


}

template <size_t n>
void print_as_str(std::ostream& out, std::span<const uint8_t, n> str)
{
    out << "'";
    for (auto c: str)
	out << c;
    out << "'";
}

void print_as_str(std::ostream& out, std::span<uint8_t> str)
{
    out << "'";
    for (auto c: str)
	out << c;
    out << "'";
}


void Main::print_FAT32_boot_sector()
{
    uart << '\n';
    atd::print(uart, msg_print_sector_as_FAT_boot_sector);
    print_line(uart);

    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }

    using Boot_sector = atd::FAT32::Boot_sector;

    Sector_driver driver;

    atd::Lock lock(driver);
    Boot_sector* bs = driver.sector_pointer_as<Boot_sector>(nsector);
    if (bs == nullptr){
	uart << "ERROR: can't read boot sector\n";
	return;
    }


// Volcado de datos para depurar y entender cómo funciona FAT32
// ------------------------------------------------------------
    uart << "jmp_boot[3]\t";
    atd::print_int_as_hex(uart, bs->jmp_boot[0]);
    uart << ' ';
    atd::print_int_as_hex(uart, bs->jmp_boot[1]);
    uart << ' ';
    atd::print_int_as_hex(uart, bs->jmp_boot[2]);
    uart << "\nOEM_name[8]\t";
    print_as_str(uart, bs->OEM_name);
    uart << "\nnum_fats \t" << (int) bs->num_fats << '\n'
    << "root_ent_cnt\t" << bs->root_ent_cnt << '\n'
    << "tot_sec16\t" << bs->tot_sec16 << '\n'
    << "media    \t";
    atd::print_int_as_hex(uart, bs->media);
    uart << "\nfat_sz16 \t" << bs->fat_sz16 << '\n'
    << "sec_per_trk\t" << bs->sec_per_trk << '\n'
    << "num_heads\t" << bs->num_heads << '\n'
    << "fat_sz32 \t" << bs->fat_sz32 << '\n'
    << "ext_flags\t" << bs->ext_flags << '\n'
    << "fs_ver   \t";
    atd::print_int_as_hex(uart, bs->fs_ver);
    uart << "\nreserved[12]\t";
    print_as_str(uart, bs->reserved);
    uart << "\ndrv_num \t";
    atd::print_int_as_hex(uart, bs->drv_num);
    uart << "\nreserved1\t" << (int) bs->reserved1 << '\n'
    << "boot_sig\t";
    atd::print_int_as_hex(uart, bs->boot_sig);
    uart << "\nfil_sys_type[8]\t";
    print_as_str(uart, bs->fil_sys_type);
//    uart << "boot_code[420]\t" << bs->boot_code << '\n'
    uart << "\nboot_code[420]\t...\n"
	<< "sign     \t";
    atd::print_int_as_hex(uart, bs->sign);
    uart << '\n';
 
// Estructuramos la salida
// -----------------------
    uart << "\nVOLUME\n";

    if (bs->is_volume_label_and_id_set()){
	uart << "\tLabel: ";
	print_as_str(uart, bs->volumen_label());
	uart << "\n\tID   : " << bs->volume_id() << '\n';
    }
    uart <<     "\tNumber of sectors: " << bs->volume_number_of_sectors() 
	   << "\n\tBytes per sector : " << bs->bytes_per_sector() 
	   << '\n';

    uart << "HIDDEN AREA\n"
	   "\tNumber of sectors: " << bs->hidden_area_number_of_sectors() << '\n';

    uart << "RESERVED AREA\n"
	     "\tNumber of sectors : " << bs->reserved_area_number_of_sectors() <<
	   "\n\tFS Info sector    : " << bs->FS_info_sector()<<
	   "\n\tBackup boot sector: " << bs->backup_boot_sector()<<
	   "\n";

   uart << "FAT AREA\n";
   uart << "\tNumber of sectors: " << bs->FAT_area_number_of_sectors() 
        << "\n\tis FAT mirrored at runtime into all FATs? ";

   if (bs->FAT_is_mirror_enabled())
       uart << "yes\n";

   else {
       uart << "no\n"
	       "\tActive FAT: " << (int) bs->number_of_active_FATs() 
	       << '\n';
    }

    uart << "\tNumber of FATs: " << (int) bs->number_of_FATs() << '\n';
   
    for (uint8_t i = 0; i < bs->number_of_FATs(); ++i){
	uart << "\t\tFirst sector FAT" << (int) i 
	<< ": " << bs->FAT_first_sector(i) << '\n';
    }

   uart << "FAT[i]"
	<< "\n\tNumber of entries : " << bs->FAT_number_of_entries()
        << "\n\tNumber of sectors : " << (int) bs->FAT_number_of_sectors() 
        << "\n\tNumber of entries per sector: " << (int) bs->FAT_number_of_entries_per_sector()
	<< "\n\tUnused entries    : " << bs->FAT_unused_entries()
	<< "\n\tUnused sectors    : " << bs->FAT_unused_sectors() 
	<< '\n';


   uart << "DATA AREA\n"
	     "\tNumber of sectors    : " << bs->data_area_number_of_sectors() 
	<< "\n\tSectors per cluster  : " << (int) bs->data_area_sectors_per_cluster()
	<< "\n\tNumber of clusters   : " << bs->data_area_number_of_clusters()
	<< "\n\tFirst sector         : " << bs->data_area_first_sector()
	<< "\n\tRoot directory cluster: " << bs->root_directory_first_cluster()
	<< '\n';


}

void Main::print_FS_info()
{
    using Volume = atd::FAT32::Volume<Sector_driver>;
//    using Boot_sector = atd::FAT32::Boot_sector;
    using Boot_sector_min = atd::FAT32::Boot_sector_min;
    using FS_info = atd::FAT32::FS_info;

    uart << '\n';
    atd::print(uart, msg_print_sector_as_FS_info);
    print_line(uart);

    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }

// TODO: esto es un poco ineficiente. Por una parte Volume tiene un
// Sector_driver dentro, que son 512 bytes y luego el Sector_driver que creo
// localmente aquí tiene otros 512 bytes, reservando 1KB de memoria en esta
// función!!! (Pero como estoy probando tampoco preocupa)
    Boot_sector_min bs_min{};
    Volume vol{nsector, bs_min};

    if (vol.error()){
	uart << "ERROR\n";
	return;
    }

    Sector_driver driver;

    atd::Lock lock(driver);
    FS_info* info = driver.sector_pointer_as<FS_info>(
	    vol.first_sector() + bs_min.FS_info_sector());

    if (info == nullptr){
	uart << "ERROR: can't FS info sector\n";
	return;
    }

    uart << "Check integrity? ";
    print_bool_as_yes_no(uart, info->check_integrity());

    uart << "\nLast known free cluster count: "
	 << info->last_known_free_cluster_count() 
	 << "\nFirst available free cluster: "
	 << info->first_available_free_cluster()
	 << '\n';

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



void Main::print_root_directory_short_entries()
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



void Main::print_root_directory_long_entries()
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
		"\t0. return\n"
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



void Main::print_FAT32_entry()
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

    atd::ctrace<9>() << "FAT:"
	        "\n\tfirst_sector         : " << vol.fat_area.first_sector(0) 
	     << "\n\tnumber of sectors    : " << vol.fat_area.number_of_sectors()
	     << "\n\tnumber of entries    : " << vol.fat_area.number_of_entries()
	     << "\n\tbytes per sector     : " << vol.fat_area.bytes_per_sector()
	     << "\n\tnumber of active FATs: " << (uint16_t) vol.fat_area.number_of_active_FATs()
	     << '\n';

    uart << "Cluster to print: ";
    uint32_t cluster{};
    uart >> cluster;
    
    using State = Volume::Cluster_state;

    uart << "Max. number of cluster to read (0 to read all): ";
    uint16_t nclusters = 0;
    uart >> nclusters;
    if (nclusters == 0)
	nclusters = std::numeric_limits<uint16_t>::max();

    for (uint16_t i = 0; i < nclusters; ++i){
	uint32_t next_cluster;
	auto state = vol.fat_area.read_next(cluster, next_cluster);

	if (state != State::allocated){
	    switch (state){
		break; case State::free: uart << "free\n";
		break; case State::allocated: uart << "allocate\n";
		break; case State::bad     : uart << "bad\n";
		break; case State::end_of_file: uart << "EOC\n";
		break; case State::reserved: uart << "reserved\n";
		break; case State::read_error: uart << "read error\n";
	    }

	    break;
	}
	uart << next_cluster << ' ';
	cluster = next_cluster;
    }

    uart << '\n';

    
    

}

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

    uart << "Number of cluster to read (0 to read all): ";
    uint16_t nsectors = 0;
    uart >> nsectors;

    if (nsectors == 0)
	nsectors = std::numeric_limits<uint16_t>::max();

    for (uint16_t i = 0; i < nsectors; ++i){
	uart << "Cluster " << (uint16_t) i << ": " 
			   << file.global_sector_number() 
			   << "; first sector (global to disk): " 
			   << file.global_sector_number() 
			   << '\n';
	if (file.end_of_file()){
	    uart << "EOC\n";
	    return;
	} else if(file.error()){
	    uart << "Error\n";
	    return;
	}

	//file.next_sector();
	if(!file.next_cluster()){
	    if (file.end_of_file())
		uart << "EOC\n";
	    else
		uart << "ERROR reading cluster\n";

	    return;
	}

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
    size_t fsize;
    uart >> fsize;

    // TODO: size
    File file{vol, fcluster};
    
    uint32_t counter = 20;
    std::array<uint8_t, 16> data;

    for (; file.read(data) != 0 and counter > 0; --counter){
	atd::xxd_print(uart, data);
    }

}


