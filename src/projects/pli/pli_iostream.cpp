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

#include "pli_iostream.h"

namespace pli{
void press_key(std::iostream& os)
{
    char c{};
    os >> c;
}

void press_key_to(const char* msg, std::iostream& os)
{
    os << "Press a key to " << msg << '\n';
    press_key(os);
}

uint8_t ask_uint8_t(std::iostream& os, const char* msg)
{
    os << msg;
    uint16_t x{};
    os >> x;
    return static_cast<uint8_t>(x);
}

bool ask_yesno(std::iostream& os, const char* msg)
{
    os << msg << " (y/n)?";
    char ans{};
    os >> ans;
    if (ans == 'n' or ans == 'N')
	return false;

    return true;    // default
}


}// namespace
 
