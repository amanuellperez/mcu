// Copyright (C) 2019-2020 Manuel Perez 
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

#include "atd_cstddef.h"

namespace atd{

void print_in_binary(std::ostream& out, std::byte b)
{
    out << "0b";
    print_bit<7>(out, b);
    print_bit<6>(out, b);
    print_bit<5>(out, b);
    print_bit<4>(out, b);
    print_bit<3>(out, b);
    print_bit<2>(out, b);
    print_bit<1>(out, b);
    print_bit<0>(out, b);
}

void print_in_hex(std::ostream& out, std::byte b)
{
    out << "0x";
    print_nibble(out, b >> 4);
    print_nibble(out, b & std::byte{0x0F});
}


}// namespace


