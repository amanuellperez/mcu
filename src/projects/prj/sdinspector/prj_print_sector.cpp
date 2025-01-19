// Copyright (C) 2023 Manuel Perez 
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

#include <atd_ostream.h>

void Main::print_sector()
{
    UART_iostream uart;
    print_line(uart);
    print(uart, sector);

}

bool Main::print_sector_fromto_ask(Sector::Address& from, size_t& sz)
{
    UART_iostream uart;

    uart << '\n';
    print_line(uart);

    print_question(uart, msg_print_sector_from, false);
    uart >> from;
    if (from > Sector::size()){
	atd::print(uart, msg_print_sector_from_to_big);
	uart << Sector::size()<< '\n';
	return false;
    }

    uart << '\n';
    print_question(uart, msg_print_sector_size, false);
    sz = 0;
    uart >> sz; // TODO: si se da retorno de carro modifica 'sz' ????
		// De hecho hace cosas raras. Para no depurarlo le indico al
		// usuario que escriba explícitamente '0'. TODO: depurarlo!!!
		// Al rato ha empezado a funcionar bien (???)

    if (sz == 0)    // imprimimos hasta el final
	sz = Sector::size() - from;

    else if (from + sz > Sector::size()){
	atd::print(uart, msg_print_sector_size_to_big);
	uart << '\n';
	return false;
    }

    uart << '\n';

    return true;
}


void Main::print_sector_fromto()
{
    UART_iostream uart;

    Sector::Address from{};
    size_t sz{};
    if (!print_sector_fromto_ask(from, sz))
	return;

    uart << '\n';
    print_line(uart);
    print(uart, sector, from, sz);

}


void Main::print_sector_as_MBR()
{
    UART_iostream uart;
    uart << '\n';
    atd::print(uart, msg_print_sector_as_MBR);
    print_line(uart);

    using MBR = atd::MBR;
    using MBR_type = atd::MBR::Partition_type;

    MBR* mbr = reinterpret_cast<MBR*>(sector.data());

    print_question(uart, msg_is_MBR_valid_sig, false);
    print_bool_as_yes_no(uart, mbr->is_valid());

    uart << '\n';
    atd::print(uart, msg_partition1);

    print_question(uart, msg_is_fat32);
    print_bool_as_yes_no(uart, mbr->partition1.type == MBR_type::fat32_lba);

    uart << '\t';
    atd::print(uart, msg_lba_offset);
    uart << " = " << mbr->partition1.lba_offset << '\n';

    uart << "\tlba size = " << mbr->partition1.lba_size << '\n';

    print_question(uart, msg_is_bootable);
    print_bool_as_yes_no(uart, mbr->partition1.is_bootable());


}

void print_as_str(std::ostream& out, std::span<uint8_t> str)
{
    out << "'";
    for (auto c: str)
	out << c;
    out << "'";
}

void Main::print_sector_as_FAT32_boot_sector()
{
    UART_iostream uart;
    uart << '\n';
    atd::print(uart, msg_print_sector_as_FAT_boot_sector);
    print_line(uart);

    using Boot_sector = atd::FAT32::Boot_sector;

    Boot_sector* bt = reinterpret_cast<Boot_sector*>(sector.data());

    uart << "jmp_boot[3]\t";
    atd::print_int_as_hex(uart, bt->jmp_boot[0]);
    uart << ' ';
    atd::print_int_as_hex(uart, bt->jmp_boot[1]);
    uart << ' ';
    atd::print_int_as_hex(uart, bt->jmp_boot[2]);
    uart << "\nOEM_name[8]\t";
    print_as_str(uart, bt->OEM_name);
    uart << "\nbyte_per_sec\t" << bt->byte_per_sec << '\n'
    << "sec_per_clus\t" << (int) bt->sec_per_clus << '\n'
    << "rsvd_sec_cnt\t" << bt->rsvd_sec_cnt << '\n'
    << "num_fats \t" << (int) bt->num_fats << '\n'
    << "root_ent_cnt\t" << bt->root_ent_cnt << '\n'
    << "tot_sec16\t" << bt->tot_sec16 << '\n'
    << "media    \t";
    atd::print_int_as_hex(uart, bt->media);
    uart << "\nfat_sz16 \t" << bt->fat_sz16 << '\n'
    << "sec_per_trk\t" << bt->sec_per_trk << '\n'
    << "num_heads\t" << bt->num_heads << '\n'
    << "hidd_sec \t" << bt->hidd_sec << '\n'
    << "tot_sec32\t" << bt->tot_sec32 << '\n'
    << "fat_sz32 \t" << bt->fat_sz32 << '\n'
    << "ext_flags\t" << bt->ext_flags << '\n'
    << "fs_ver   \t";
    atd::print_int_as_hex(uart, bt->fs_ver);
    uart << "\nfs_info  \t" << bt->fs_info << '\n'
    << "bk_boot_sec\t" << bt->bk_boot_sec << '\n'
    << "reserved[12]\t";
    print_as_str(uart, bt->reserved);
    uart << "\ndrv_num \t";
    atd::print_int_as_hex(uart, bt->drv_num);
    uart << "\nreserved1\t" << (int) bt->reserved1 << '\n'
    << "boot_sig\t";
    atd::print_int_as_hex(uart, bt->boot_sig);
    uart << "\nvol_id  \t" << bt->vol_id << '\n'
    << "vol_lab[11]\t";
    print_as_str(uart, bt->vol_lab);
    uart << "\nfil_sys_type[8]\t";
    print_as_str(uart, bt->fil_sys_type);
//    uart << "boot_code[420]\t" << bt->boot_code << '\n'
   uart << "\nboot_code[420]\t...\n"
	<< "sign     \t";
   atd::print_int_as_hex(uart, bt->sign);
   uart << '\n';
    
   uart << "FAT area\n";
   uart << "\tFAT size (number of sectors) = " << bt->FAT_area_size() << '\n';

   for (uint8_t i = 0; i < bt->number_of_FATs(); ++i){
       uart << "\tfirst sector FAT" << (int) i 
	<< ": " << bt->FAT_area_offset(i) << '\n';
   }


   uart << "DATA area\n"
        << "\tfirst sector:  "
	<< bt->data_area_offset() << '\n';

   uart << "\tsize (number of sectors) = " 
        << bt->data_area_size () << '\n';

    uart << "\nRoot directory\n"
	 << "\tfirst sector: " 
	 << bt->first_sector_of_root_directory() 
	 << '\n';
}

void Main::print_sector_as_FS_info()
{
    UART_iostream uart;
    uart << '\n';
    atd::print(uart, msg_print_sector_as_FS_info);
    print_line(uart);

    using FS_info = atd::FAT32::FS_info;

    FS_info* info = reinterpret_cast<FS_info*>(sector.data());

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

void print_directory_entry(std::ostream& out, const atd::FAT32::Directory& dir)
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

void print_directory(std::ostream& out, const atd::FAT32::Directory& dir)
{
    using Dir = atd::FAT32::Directory;

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
    UART_iostream uart;
    uart << "FAT32: directory\n";

    print_line(uart);

    using Dir = atd::FAT32::Directory;

    Dir* dir = reinterpret_cast<Dir*>(sector.data());

    uart << "n \t|attr\t\t|name\t|cluster\t|size\t|tenth\t|time\t|date\t|acc_date\t|wrt\t|wrt\t|\n";
    for (uint8_t i = 0; i < sector.size() / sizeof(Dir); ++i){
	uart << (int) i << '\t';
	print_directory(uart, dir[i]);
    }

}
