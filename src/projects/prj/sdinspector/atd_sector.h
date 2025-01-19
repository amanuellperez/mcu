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

#pragma once

#ifndef __ATD_SECTOR_H__
#define __ATD_SECTOR_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Sector de un disco duro, SD card, EEPROM, ...	
 *  En las EEPROM se llaman páginas. Es lo mismo.
 *  Un sector no es más que un array de bytes. En principio el único dato que
 *  necesita es el número de sector.
 *
 * HISTORIA
 *    Manuel Perez
 *    20/02/2023 Versión mínima
 *    18/01/2025 ...
 *
 ****************************************************************************/

#include <array>
#include <ostream>
#include <atd_ostream.h>
#include <atd_math.h>
#include <limits>

#include <atd_print.h>

namespace atd{

// Ejemplo de Cfg:
//
//  struct Sector_cfg{
//      static constexpr size_t sector_size = SDCard::block_size;
//      static constexpr uint8_t nbytes_per_line = 16;
//      using Address = SDCard::Address;
//  };
//
template <typename Cfg>
struct Sector: std::array<uint8_t, Cfg::sector_size>{
// Cfg del sector
    using Address = Cfg::Address;
    static constexpr Address size() {return Cfg::sector_size;}

// Data
// uint8_t data[sector_size]; <-- implícito al heredar de std::array
    // me suena mejor 'number' que 'address' (???)
    // Si address == max ==> el sector consideramos que no está cargado en
    // memoria.
    Address address = std::numeric_limits<Address>::max(); 

    bool is_valid() const { return !is_invalid(); }
    bool is_invalid() const 
		{return address == std::numeric_limits<Address>::max();}


private:


};


template <typename Cfg>
void print(std::ostream& out, const Sector<Cfg>& sector, size_t i0, size_t sz)
{
    out << "Sector: " << sector.address << '\n';

    atd::xxd_print(out, sector, i0, sz);
}

template <typename Cfg>
void print(std::ostream& out, const Sector<Cfg>& sector)
{
    atd::xxd_print(out, sector, 0u, sector.size());
}


}// namespace
 
#endif


