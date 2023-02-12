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

#include "dev_debug_eeprom.h"

#include <atd_cstddef.h>    // print_in_hex
#include <iomanip>

namespace dev{

void EEPROM_debug::dump_buffer_fmt(std::ostream& out, 
		uint16_t addr0, const uint8_t* buf, uint8_t n)
{
    const uint8_t* pe = buf + n;

    for (uint8_t i = 0; i < n; ){
	out << std::setw(3) << std::right << addr0 + i << " | ";
	out << std::left;
	const uint8_t* p = &buf[i];
	for (uint8_t j = 0; p != pe and j < 10; ++p, ++j){
	    atd::print_in_hex(out, std::byte{*p});    // TODO: out << std::hex << *p;
	    //out << std::to_integer<char>(*p) << "  ";
	    out << ' ';
	    // out << std::to_integer<uint16_t>(buf[i]) << " ";
	}
	out << "  |";
	for (uint8_t j = 0; j < 10 and i < n; ++j, ++i){
	    char c = static_cast<char>(buf[i]);
	    if (c != '\n'){
		if (isprint(c))
		    out << c;
		else 
		    out << '*';
	    }
	    else
		out << "\\n";
	}
	
	out << "\n";
    }

}


}// namespace


