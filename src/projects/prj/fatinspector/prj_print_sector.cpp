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

    using Volume = atd::FAT32::Volume<Sector_driver>;
    using Boot_sector = atd::FAT32::Boot_sector;

    Volume vol{nsector};

    if (vol.error()){
	uart << "ERROR\n";
	return;
    }

    Boot_sector* bs = vol.read_global_as<Boot_sector>(nsector);

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
	<< "\n\tRoot directory sector: " << bs->root_directory_first_sector()
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

    Boot_sector_min bs_min;
    Volume vol{nsector, bs_min};

    if (vol.error()){
	uart << "ERROR\n";
	return;
    }

    FS_info* info = vol.read_as<FS_info>(bs_min.FS_info_sector());

    uart << "Check integrity? ";
    print_bool_as_yes_no(uart, info->check_integrity());

    uart << "\nLast known free cluster count: "
	 << info->last_known_free_cluster_count() 
	 << "\nFirst available free cluster: "
	 << info->first_available_free_cluster()
	 << '\n';

}

void print_long_name_entry(std::ostream& out, const atd::FAT32::Long_file_name_entry& fn)
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

void print_directory_entry(std::ostream& out, 
				const atd::FAT32::Directory_entry& dir)
{
    // name
    for (uint8_t i = 0; i < 11; ++i)
	out << dir.name[i];


    out << '\t' << dir.first_data_cluster_number()
	  << '\t' <<  dir.file_size;

    out << '\t';

    out << '\t';
    atd::print_int_as_hex(out, dir.nt_res);
    out << '\t';
    atd::print_int_as_hex(out, dir.crt_time_tenth);
    out << '\t';
    atd::print_int_as_hex(out, dir.crt_time);
    out << '\t';
    atd::print_int_as_hex(out, dir.crt_date);
    out << '\t';
    atd::print_int_as_hex(out, dir.lst_acc_date);
    out << '\t';
    atd::print_int_as_hex(out, dir.wrt_time);
    out << '\t';
    atd::print_int_as_hex(out, dir.wrt_date);
 
}

void print_directory(std::ostream& out, const atd::FAT32::Directory_entry& dir)
{
    using Dir = atd::FAT32::Directory_entry;

    if (dir.is_empty()){
	out << "EMPTY\n";
	return;
    }

    switch (dir.attribute()){
	break; case Dir::Attribute::read_only: out << "read only";
	break; case Dir::Attribute::hidden: out << "hidden";
	break; case Dir::Attribute::system: out << "system";
	break; case Dir::Attribute::volume_id: out << "volume_id";
	break; case Dir::Attribute::directory: out << "directory";
	break; case Dir::Attribute::archive: out << "archive\t";
	break; case Dir::Attribute::long_name: out << "long_name";
    }

    out << '\t';

    if (dir.attribute() == Dir::Attribute::long_name)
	print_long_name_entry(out, 
		    reinterpret_cast<const atd::FAT32::Long_file_name_entry&>(dir));
    else
	print_directory_entry(out, dir);

    out << '\n';
}

void Main::print_sector_as_directory_array()
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


    using File = atd::FAT32::File<Sector_driver, uint8_t>;
    
    File f{vol, 2}; // El root directory es el cluster 2
		    // (TODO) ¿dónde lo pone???
    
    uint32_t counter =  0; // limitamos el número de entradas
    for (auto p = f.begin(); 
	 p != f.end() and counter < 32 * sizeof(uint32_t); // 30 entradas máximo
	 ++p, ++counter){
	
	if ((counter % 32) == 0 and counter != 0)
	    uart << '\n';

	atd::print_int_as_hex(uart, *p, false);
	uart << ' ';
    }
    uart << '\n';
    print_line(uart);

    counter = 0;
    for (auto p = f.begin(); 
	 p != f.end() and counter < 32 * sizeof(uint32_t); // 30 entradas máximo
	 ++p, ++counter){
	
	if ((counter % 32) == 0 and counter != 0)
	    uart << '\n';

	if (isprint(*p))
	    uart << *p;
	else
	    uart << '.';
    }
    uart << '\n';
    print_line(uart);


    Sector sector{};
    Sector_driver::read(vol.data_area.first_sector(), sector);

    using Dir = atd::FAT32::Directory_entry;

    Dir* dir = reinterpret_cast<Dir*>(sector.data());

    uart << "n \t|attr\t\t|name\t|cluster\t|size\t|tenth\t|time\t|date\t|acc_date\t|wrt\t|wrt\t|\n";
    for (uint8_t i = 0; i < sector.size() / sizeof(Dir); ++i){
	uart << (int) i << '\t';
	print_directory(uart, dir[i]);
    }

}

void Main::print_FAT32_entry()
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
    
    static constexpr uint8_t file_size = 4;
    std::array<uint32_t, file_size> file{};

    auto res = vol.fat_area.read_next(cluster, file);

    uart << (uint16_t) res.nread << " clusters readed: ";
    for (uint8_t i = 0; i < res.nread; ++i)
	uart << file[i] << "; ";
    uart << "; state = ";
    using State = Volume::Cluster_state;

    switch (res.state){
	break; case State::free: uart << "free\n";
	break; case State::allocated: uart << "allocate\n";
	break; case State::bad     : uart << "bad\n";
	break; case State::end_of_file: uart << "EOC\n";
	break; case State::reserved: uart << "reserved\n";
	break; case State::read_error: uart << "read error\n";
    }
    
    

}

void Main::print_file_sectors()
{
    using Volume = atd::FAT32::Volume<Sector_driver>;
    using File_sectors   = atd::FAT32::File_sectors<Sector_driver>;
//    using Sector = File_sectors::Sector;


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

    for (uint8_t i = 0; i < 10; ++i){
	uart << "Cluster " << (uint16_t) i << ": " 
			   << file.global_sector_number() 
			   << "; first sector (global to disk): " 
			   << file.global_sector_number() 
			   << '\n';
	//file.next_sector();
	file.next_cluster();
	if (file.end_of_file()){
	    uart << "EOC\n";
	    return;
	} else if(file.error()){
	    uart << "Error\n";
	    return;
	}
    }


}

