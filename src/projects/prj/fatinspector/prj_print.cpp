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

#include "prj_print.h"


void print_as_char(std::ostream& out, std::span<uint8_t> str)
{
    for (auto x: str){
	if (x == '\0')
	    return;

	out << x;
    }
}

void print(std::ostream& out, std::span<uint8_t> str)
{
    for (auto x: str)
	out << x;
}


void print(std::ostream& out, Attribute att, bool with_tab)
{
    using Att = Attribute;

    switch(att){
	break; case Att::read_only  : out << "read_only";
	break; case Att::hidden	    : out << "hidden";
	break; case Att::system	    : out << "system";
	break; case Att::volume_id  : out << "volume_id";
	break; case Att::directory  : out << "directory";
	break; case Att::archive    : 
				      out << "archive";
				      if (with_tab) out << '\t';
	break; case Att::long_name  : out << "long_name";
    }

}


