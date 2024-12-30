// Copyright (C) 2024 Manuel Perez 
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

#ifndef __MEGA0_DEBUG_H__
#define __MEGA0_DEBUG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones internas para depurar.	
 *
 * HISTORIA
 *    Manuel Perez
 *    08/11/2024 print_spi_registers
 *
 ****************************************************************************/
#include <atd_ostream.h>    // print_int_as_hex

namespace mega0_{

template <typename Int>
static 
void print_register(std::ostream& out, const char* name, Int value)
{
    out << name << " = ";
    atd::print_int_as_hex(out, value);
    out << '\n';
}


template <typename SPI>
void print_spi_registers(std::ostream& out)
{
    print_register(out, "CTRLA", SPI::reg()->CTRLA);
    print_register(out, "CTRLB", SPI::reg()->CTRLB);
    print_register(out, "INTCTRL", SPI::reg()->INTCTRL);
    print_register(out, "INTFLAGS", SPI::reg()->INTFLAGS);
    print_register(out, "DATA", SPI::reg()->DATA);
}

}// mega0_

#endif


