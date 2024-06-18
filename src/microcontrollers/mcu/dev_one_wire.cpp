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

/****************************************************************************
 *
 * DESCRIPCION
 *	One wire protocol
 *
 * HISTORIA
 *    Manuel Perez
 *    28/08/2023 Escrito
 *
 ****************************************************************************/
#include "dev_one_wire.h"
#include <atd_ostream.h>

namespace dev{

bool One_wire_device::is_CRC_ok() const
{
    if (ROM[0] == 0x00)
	return false;

    return (CRC() == atd::CRC8_Maxim(ROM, 7));
}


void print_family_name(std::ostream& out, const One_wire_device& dev)
{
    switch (dev.ROM[0]){
	break; case 0x28: out << "DS18B20";
	break; default  : out << "Unknown or wrong";
    }
}

void print_rom_as_hex(std::ostream& out, const One_wire_device& dev)
{
    for (uint8_t i = 0; i < One_wire_device::ROM_size; ++i){
	atd::print_int_as_hex(out, dev.ROM[i]);
	out << ' ';
    }
}

}// dev


