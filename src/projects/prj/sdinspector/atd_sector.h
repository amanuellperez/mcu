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
#include <limits>

#include <atd_print.h>

namespace atd{

// La única diferencia entre un sector y un array es que el sector tiene un
// número de sector.
template <size_t sector_size>
struct Sector: std::array<uint8_t, sector_size>{
// Types
    using Base = std::array<uint8_t, sector_size>;
    using size_type = Base::size_type;

// Data
    // Si number == max ==> el sector consideramos que no está cargado en
    // memoria.
    size_type number = std::numeric_limits<size_type>::max(); 

// Functions
    // (???) ¿Por qué no es static size() en std::array?
    static constexpr size_type size() {return sector_size; }

    bool is_valid() const { return !is_invalid(); }
    bool is_invalid() const 
		{return number == std::numeric_limits<size_type>::max();}

};


template <size_t N>
void print(std::ostream& out, const Sector<N>& sector, size_t i0, size_t sz)
{
    out << "Sector: " << sector.number << '\n';

    atd::xxd_print(out, sector, i0, sz);
}

template <size_t N>
void print(std::ostream& out, const Sector<N>& sector)
{
    atd::xxd_print(out, sector, 0u, sector.size());
}


}// namespace
 
#endif


