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

// Si TRACE_LEVEL está definido trazamos todas las trazas menores que ese
// level.
// Considero:
//	1 - trazas críticas
//	2 - menos críticas
//	9 - trazando para depurar programa
static constexpr bool trace(uint8_t trace_level = 9)
{
#ifdef TRACE_LEVEL
    return trace_level <= TRACE_LEVEL;
#else
    return false;
#endif
}



class Main{
public:
    Main();
    void run();
    
private:
// Hardware

// Data
    using Sector = Sector_driver::Sector;

    // Último sector leído. En caso de no haberse leido nada contendrá basura.
    Sector sector; 
    uint32_t sector_number = std::numeric_limits<uint32_t>::max();

    bool sector_is_valid() const { return !sector_is_invalid(); }
    bool sector_is_invalid() const 
    {return sector_number == std::numeric_limits<uint32_t>::max();}

// Functions
    void init_uart();
    void init_spi();

    // Cargamos en memoria `sector`
    bool load_sector();
    bool load_sector(const SDCard::Address&);

// Main menu
    uint16_t main_menu();
    void run_command_invalid_sector(uint16_t cmd);
    void run_command_valid_sector(uint16_t cmd);

// User commands:
    void read_status();

    void flush_sector();

    void edit_sector(bool in_hexadecimal);
    void edit_sector_from_in_hexadecimal(Sector::size_type);
    void edit_sector_from_with_chars(Sector::size_type);

    void print_sector();

    void print_sector_fromto();
	bool print_sector_fromto_ask(Sector::size_type& from, size_t& sz);

    void print_sector(std::ostream& out, const Sector& sector, size_t i0, size_t sz)
    {
	out << "Sector: " << sector_number << '\n';

	atd::xxd_print(out, sector, i0, sz);
    }

    void print_sector(std::ostream& out, const Sector& sector)
    { atd::xxd_print(out, sector, 0u, sector.size()); }

};

#endif


