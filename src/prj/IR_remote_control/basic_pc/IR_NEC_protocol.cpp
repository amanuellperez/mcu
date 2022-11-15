// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include "IR_NEC_protocol.h"


std::ostream& operator<<(std::ostream& out, const NEC_message& msg)
//std::ostream& operator<<(avr::UART_iostream& out, const NEC_message& msg)
{
    atd::print_int_as_hex(out, msg.address);
    out << ' ';
    atd::print_int_as_hex(out, msg.inv_address);
    out << ' ';
    atd::print_int_as_hex(out, msg.command);
    out << ' ';
    atd::print_int_as_hex(out, msg.inv_command);

    return out;
}
