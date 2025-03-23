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

#pragma once

#ifndef __PRJ_MAIN_H__
#define __PRJ_MAIN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Inspector de FAT32 filesystems
 *
 * HISTORIA
 *    Manuel Perez
 *    24/04/2025 Escrito
 *
 ****************************************************************************/
#include "prj_dev.h"
//#include "prj_cfg.h"
#include "prj_strings.h"

#include <atd_print.h> // xxd_print

#include "atd_fat.h"
#include "atd_mbr.h"



class Main{
public:
    Main();
    void run();
    
private:
// Hardware

// Data
    using Sector = Sector_driver::Sector;

// Init
    void init_uart();
    void init_spi();

// SD card commands
    void read_status();
    void print_sector();

// Sector driver test
    void sector_driver_menu();
	void sector_driver_fill_n_test();
    
// MBR area test
    void print_MBR_boot_sector();

// Reserved area test
    void reserved_area_menu();
	void reserved_area_print_FAT32_boot_sector();
	void reserved_area_print_FS_info();

// FAT area test
    void FAT32_area_menu();
	void FAT32_area_print_clusters(Volume& vol);
	void FAT32_area_new_list(Volume& vol);
	void FAT32_area_remove_list(Volume& vol);
	void FAT32_area_add_cluster(Volume& vol);
	void FAT32_area_remove_next_cluster(Volume& vol);

// FAT area list test
    void FAT32_area_list_menu();
	void FAT32_area_list_print_ls(Volume& vol);
	void FAT32_area_push_back_cluster(Volume& vol);

// Root directory test
    void root_directory_menu();
	void root_directory_print_short_entries(Volume& vol, Directory& dir);
	void root_directory_print_long_entries(Volume& vol, Directory& dir);
	void print_ls(Volume& vol, Directory& dir, Attribute att);
	void print_file();
	void new_entry(Volume& vol, Directory& dir);
	void remove_short_entry(Volume& vol, Directory& dir);
	void remove_long_entry(Volume& vol, Directory& dir);
	void mkfile(Volume& vol, Directory& dir);

    void print_sector(std::ostream& out, const Sector& sector, size_t i0, size_t sz)
    { atd::xxd_print(out, sector, i0, sz); }

    void print_sector(std::ostream& out, const Sector& sector)
    { atd::xxd_print(out, sector, 0u, sector.size()); }

// FAT
    uint32_t fat_volume_first_sector(uint8_t npartition);
	uint32_t fat_volume_first_sector(const atd::MBR_partition& p, uint8_t n);

// Automatic
    void automatic_test();
};

#endif


