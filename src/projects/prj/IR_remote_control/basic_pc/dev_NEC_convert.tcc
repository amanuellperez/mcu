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

#pragma once

#ifndef __NEC_CONVERT_TCC__
#define __NEC_CONVERT_TCC__

#include "dev_NEC_protocol.h"

#include <atd_cast.h>	// convert(x).into(y)
#include <atd_bit.h>	// write_one/zero

namespace dev{

// Se envia el least significant bit first, por eso hay que cambiar el orden.
template <typename Message_it>
Message_it NEC_protocol::convert_byte(uint8_t& y,
					Message_it p, Message_it pe)
{
    if (pe - p < 8) 
	return pe;

    for (int8_t i = 0; i < 8; ++i){
	if (is_equal(p[i].time_low, t_burst_0)){
	    uint16_t T = p[i].period();

	    if (is_equal(T, t_period_0))
		atd::write_zero(y, i);

	    else if (is_equal(T, t_period_1))
		atd::write_one(y, i);

	    else
		return pe;
	}

	else 
	    return pe;
    }

    return (p + 8);
}


template <size_t N>
bool NEC_protocol::convert_x_into_y(const dev::Train_of_pulses<N>& pulse, 
						    NEC_message& msg)
{
    auto pe = pulse.end();
    auto p = look_for_start(pulse.begin(), pe);

    if (p == pe)
	return false;

    msg.time_first_pulse = p->time_low;
    ++p;   

    p = convert_byte(msg.address, p, pe);

    if (p == pe)
	return false;

    p = convert_byte(msg.inv_address, p, pe);

    if (p == pe)
	return false;

    p = convert_byte(msg.command, p, pe);

    if (p == pe)
	return false;


    p = convert_byte(msg.inv_command, p, pe);

    if (p == pe)	// queda el pulso final  ¿comprobarlo?
	return false;	// observar que por no comprobarlo estoy relajando
			// el protocolo. 

    return true;
}





// Usage: atd::convert(ptrain).into(NEC_msg);
template <size_t N>
inline 
bool convert_x_into_y(const dev::Train_of_pulses<N>& ptrain, 
		     NEC_message& msg)
{
    return NEC_protocol::convert_x_into_y(ptrain, msg);
}



}// namespace
 
#endif
