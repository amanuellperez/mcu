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

#pragma once

#ifndef __IR_NEC_TRANSMIT_TCC__
#define __IR_NEC_TRANSMIT_TCC__


#include "IR_NEC_protocol.h"


template <typename Clock_us0, typename SWG0>
class NEC_transmitter{
public:
    using Clock_us = Clock_us0;
    using SWG      = SWG0;

    void transmit(Clock_us::counter_type time_first_burst_in_us, 
		 const NEC_message& msg);

private:
    void transmit_byte(uint8_t b);
    void transmit_one(Clock_us::counter_type time_first_burst_in_us, 
	      const NEC_message& msg);
};


// inline porque no se si es suficientemente eficiente
template <typename C, typename S>
inline void NEC_transmitter<C, S>::transmit_byte(uint8_t b)
{
    for (uint8_t i = 0; i < 8; ++i){
	if (b & 0x01){
	    SWG::burst_38kHz_of(562);
	    SWG::wait_us(562 * 3);
	}
	else{
	    SWG::burst_38kHz_of(562);
	    Clock_us::wait_us(562);
	}

	b >>= 1;
    }
}


template <typename C, typename S>
void NEC_transmitter<C, S>::
	transmit_one(Clock_us::counter_type time_first_burst_in_us, 
		     const NEC_message& msg)
{
    SWG::burst_38kHz_of(time_first_burst_in_us);
    Clock_us::wait_us(4500);

    transmit_byte(msg.address);
    transmit_byte(msg.inv_address);
    transmit_byte(msg.command);
    transmit_byte(msg.inv_command);

    SWG::burst_38kHz_of(562);

}


template <typename C, typename S>
void NEC_transmitter<C,S>::transmit
		(Clock_us::counter_type time_first_burst_in_us, 
		 const NEC_message& msg)
{
    Clock_us::on();
//    for (uint8_t i = 0; i < 3; ++i){
	transmit_one(time_first_burst_in_us,  msg);
//	timer_wait_us(30000);
//    }
    Clock_us::off();
}






#endif


