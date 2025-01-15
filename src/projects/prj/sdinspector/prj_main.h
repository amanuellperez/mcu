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


class Main{
public:
    Main();
    void run();
    
private:
// Hardware

// Data
    // Último sector leído. En caso de no haberse leido nada contendrá basura.
    Sector sector; 


// Functions
    void init_uart();
    void init_spi();

    // Cargamos en memoria `sector`
    bool load_sector();

// Main menu
    uint16_t main_menu();
    void run_command_invalid_sector(uint16_t cmd);
    void run_command_valid_sector(uint16_t cmd);

// User commands:
    void read_status();

    void flush_sector();

    void edit_sector(bool in_hexadecimal);
    void edit_sector_from_in_hexadecimal(Sector::Address);
    void edit_sector_from_with_chars(Sector::Address);

    void print_sector();

    void print_sector_fromto();
	bool print_sector_fromto_ask(Sector::Address& from, size_t& sz);

    void print_sector_as_MBR();
    void print_sector_as_FS_info();
    void print_sector_as_FAT32_boot_sector();
};


#endif


