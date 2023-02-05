// Copyright (C) 2022 Manuel Perez 
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

#include "dev_NEC_protocol.h"

namespace dev{

std::ostream& operator<<(std::ostream& out, const NEC_message& msg)
//std::ostream& operator<<(atmega::UART_iostream& out, const NEC_message& msg)
{
    out << "NEC(" << msg.time_first_pulse << "): ";
    atd::print_int_as_hex(out, msg.address);
    out << ' ';
    atd::print_int_as_hex(out, msg.inv_address);
    out << ' ';
    atd::print_int_as_hex(out, msg.command);
    out << ' ';
    atd::print_int_as_hex(out, msg.inv_command);

    return out;
}




// Aunque la especificación del NEC indica que el pulso de start tiene que ser
// 9ms low seguido de 4'5 ms high, tengo un mando que envía 4'5 ms low y luego
// 4'5 ms high. De momento lo implemento así.
bool NEC_protocol::is_start_pulse(const dev::Cycle& pulse)
{
// TODO: cambiar números mágicos!!!
    if (!(is_equal(pulse.time_low, 9000) or is_equal(pulse.time_low, 4500)))
	    return false;

    return is_equal(pulse.time_high, 4500);
}



// NEC bit format (pulse distance encoding):
//	* logical '0': a 562.5us pulse burst followed by 562.us space, with
//	               a total transmit time of 1.125 ms.
//	* logical '1': a 562.5us pulse burst followed by a 1.678 ms space,
//	               with a total transmit time of 2.25 ms.
char NEC_protocol::decode_period_as_char(const uint16_t T)
{
    if (is_equal(T, t_period_0))
	return '0';

    else if (is_equal(T, t_period_1))
	return '1';

    else 
	return '?';
}

}// namespace 
