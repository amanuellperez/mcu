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

#include <ctype.h> // isprint
#include "prj_dev.h"
#include "prj_strings.h"
#include "dev_print.h"
#include "sdc_print.h"
#include "prj_main.h"
#include "atd_fat.h"

#include <atd_ostream.h>

template <size_t n>
void print_as_str(std::ostream& out, std::span<const uint8_t, n> str)
{
    out << "'";
    for (auto c: str)
	out << c;
    out << "'";
}

void print_as_str(std::ostream& out, std::span<const uint8_t> str)
{
    out << "'";
    for (auto c: str)
	out << c;
    out << "'";
}


void Main::reserved_area_print_FS_info()
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

    if (vol.last_operation_fail()){
	uart << "ERROR\n";
	return;
    }

    Sector_driver driver;
    auto info = driver.template lock_sector_and_view_as<FS_info>(
	    vol.first_sector() + bs_min.FS_info_sector());

    if (info.is_null()){
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

void Main::reserved_area_print_FAT32_boot_sector()
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

    auto bs = driver.template lock_sector_and_view_as<Boot_sector>(nsector);
    if (bs.is_null()){
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
	<< "\n\tNumber of entries : " << bs->FAT_number_of_clusters()
        << "\n\tNumber of sectors : " << (int) bs->FAT_number_of_sectors() 
        << "\n\tNumber of entries per sector: " << (int) bs->FAT_number_of_clusters_per_sector()
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
