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
#include <atd_string.h>	    // binary_char_to
#include <atd_ostream.h>    // print_int_as_hex

// Aunque la especificación del NEC indica que el pulso de start tiene que ser
// 9ms low seguido de 4'5 ms high, tengo un mando que envía 4'5 ms low y luego
// 4'5 ms high. De momento lo implemento así.
bool NEC_protocol::is_start_pulse(const Cycle& pulse)
{
    if (!(is_equal(pulse.time_low, 9000) or is_equal(pulse.time_low, 4500)))
	    return false;

    return is_equal(pulse.time_high, 4500);
}



// NEC bit format (pulse distance encoding):
//	* logical '0': a 562.5us pulse burst followed by 562.us space, with
//	               a total transmit time of 1.125 ms.
//	* logical '1': a 562.5us pulse burst followed by a 1.678 ms space,
//	               with a total transmit time of 2.25 ms.
char NEC_protocol::pulse_to_bit(const uint16_t T)
{
    if (is_equal(T, 1125))
	return '0';

    else if (is_equal(T, 2250))
	return '1';

    else 
	return '?';
}

