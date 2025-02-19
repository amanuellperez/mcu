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

#pragma once

#ifndef __PRJ_MAIN_H__
#define __PRJ_MAIN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Inspector de SD cards
 *
 * HISTORIA
 *    Manuel Perez
 *    24/04/2023 Escrito
 *
 ****************************************************************************/
#include "prj_dev.h"
//#include "prj_cfg.h"
#include "prj_strings.h"

#include <atd_print.h> // xxd_print

#include "atd_fat.h"

// Si TRACE_LEVEL está definido trazamos todas las trazas menores que ese
// level.
// Considero:
//	1 - trazas críticas
//	2 - menos críticas
//	5 - errores de funciones de biblioteca.
//	9 - trazando para depurar programa
//static constexpr bool trace(uint8_t trace_level = 9)
//{
//#ifdef TRACE_LEVEL
//    return trace_level <= TRACE_LEVEL;
//#else
//    return false;
//#endif
//}



class Main{
public:
    Main();
    void run();
    
private:
// Hardware

// Data
    using Sector = Sector_driver::Sector;

// Functions
    void init_uart();
    void init_spi();

    // Cargamos en memoria `sector`
    bool load_sector();
    bool load_sector(const SDCard::Address&);


// User commands:
    void read_status();

    void print_sector();
    void print_MBR_boot_sector();
    void print_FS_info();
    void print_FAT32_boot_sector();
    void print_root_directory_short_entries();
    void print_root_directory_long_entries();
    void print_FAT32_entry();
    void print_file_sectors();

    void print_sector(std::ostream& out, const Sector& sector, size_t i0, size_t sz)
    { atd::xxd_print(out, sector, i0, sz); }

    void print_sector(std::ostream& out, const Sector& sector)
    { atd::xxd_print(out, sector, 0u, sector.size()); }

// FAT
    uint32_t fat_volume_first_sector(uint8_t npartition);
	uint32_t fat_volume_first_sector(const atd::MBR_partition& p, uint8_t n);
};

#endif


