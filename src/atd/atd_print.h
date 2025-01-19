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

#ifndef __ATD_PRINT_H__
#define __ATD_PRINT_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para imprimir con formato diferentes cosas.	
 *
 * HISTORIA
 *    Manuel Perez
 *    18/01/2025 xxd_print
 *
 ****************************************************************************/
#include <span>
#include <cstdint>
#include <ostream>


namespace atd{

/***************************************************************************
 *				xxd_print
 ***************************************************************************/
// Imprimamos en hexadecimal un array, usando un formato similar al que usa el
// programa `xxd`
class Xxd{
public:
    // Muestra en el flujo indicado el sector cargado en memoria.
    // De momento es responsabilidad del usuario garantizar que se haya
    // cargado el sector correspondiente.
    static void print(std::ostream& out, std::span<const uint8_t> bf) 
    { print(out, bf, 0, bf.size()); }

    // Imprimimos bf[i0 .. i0 + N)
    // De momento no se valida que i0, i0+N estén dentro de bf. Es
    // responsabilidad del cliente.
    static void print(std::ostream& out, std::span<const uint8_t> bf, 
					size_t i0, size_t N);

private:

// Cfg
    static constexpr uint8_t nbytes_per_line = 16;

// Helpers
    static void print_line(std::ostream& out, std::span<const uint8_t> bf, 
							size_t i, size_t N);
    
};


inline void xxd_print(std::ostream& out, std::span<const uint8_t> bf) 
{ Xxd::print(out, bf); }

inline void xxd_print(std::ostream& out, std::span<const uint8_t> bf
			    , size_t i0, size_t N) 
{ Xxd::print(out, bf, i0, N); }


}// namespace td

#endif


