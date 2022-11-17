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

#include "dev.h"
#include "cfg.h"
#include <avr_interrupt.h>

#include "pulse.h"
#include "IR_NEC_protocol.h"
#include "square_wave.h"
#include "transmit.h"


// inline porque no se si es suficientemente eficiente
inline static void NEC_transmit_byte(uint8_t b)
{
    for (uint8_t i = 0; i < 8; ++i){
	if (b & 0x01){
	    burst_38kHz_of(562);
	    Clock_us::wait_us(562 * 3);
	}
	else{
	    burst_38kHz_of(562);
	    Clock_us::wait_us(562);
	}

	b >>= 1;
    }
}

static void transmit_one(Clock_us::counter_type time_first_burst_in_us, 
	      const NEC_message& msg)
{
    burst_38kHz_of(time_first_burst_in_us);
    Clock_us::wait_us(4500);

    NEC_transmit_byte(msg.address);
    NEC_transmit_byte(msg.inv_address);
    NEC_transmit_byte(msg.command);
    NEC_transmit_byte(msg.inv_command);

    burst_38kHz_of(562);

}


void transmit(Clock_us::counter_type time_first_burst_in_us, 
	      const NEC_message& msg)
{
    Clock_us::on();
//    for (uint8_t i = 0; i < 3; ++i){
	transmit_one(time_first_burst_in_us,  msg);
//	timer_wait_us(30000);
//    }
    Clock_us::off();
}





