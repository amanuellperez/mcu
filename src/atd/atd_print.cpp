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

#include "atd_print.h"

#include "atd_math.h" // is_even
#include "atd_ostream.h"    // print_int_as_hex

namespace atd{

void Xxd::print(std::ostream& out, std::span<const uint8_t> bf, 
		size_t i0, size_t sz, size_t addr0) 
{
    size_t ie = i0 + sz; // [i0, ie) intervalo a imprimir
    for (; ie - i0 > nbytes_per_line; i0 += nbytes_per_line){
	print_line(out, bf, i0, nbytes_per_line, addr0);
	addr0 += nbytes_per_line;
    }

    print_line(out, bf, i0, ie - i0, addr0);

}

void Xxd::print_line(std::ostream& out, std::span<const uint8_t> bf
				      , size_t i, size_t N
				      , size_t addr0) 
{
// header:
    out.fill('0');
    out.width(3);
    out << addr0 << ':';

// in hexadecimal:
    for (uint8_t k = 0; k < N; ++k){
	if (is_even(k)) out << ' ';
	print_int_as_hex(out, bf[i + k], false);
    }

    // ajustamos al final de línea en caso de que N < nbytes_per_line
    for (uint8_t k = 0; k < nbytes_per_line - N; ++k){
	if (is_even(k)) out << ' ';
	out << "  "; 
    }


// as chararacters:
    out << ' ';

    for (uint8_t k = 0; k < N; ++k){
//	if (is_even(k)) out << ' ';

	if (isprint(bf[i + k]))
	    out << bf[i + k];
	else 
	    out << '.';
    }

    out << '\n';
}

}// namespace atd



