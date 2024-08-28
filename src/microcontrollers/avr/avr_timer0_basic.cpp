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

#include "avr_timer0_basic.h"

namespace avr_{

// Table 19-10: Clock Select Bit Description
Timer0::Frequency_divisor Timer0::frequency_divisor()
{ 
    switch(atd::read_bits<CS02, CS01, CS00>::of(TCCR0B)){
	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<0,0,1>():
	    return Frequency_divisor::no_prescaling;

	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<0,1,0>():
	    return Frequency_divisor::divide_by_8;

	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<0,1,1>():
	    return Frequency_divisor::divide_by_64;

	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<1,0,0>():
	    return Frequency_divisor::divide_by_256;

	case atd::zero<uint8_t>::with_bits<CS02, CS01, CS00>::to<1,0,1>():
	    return Frequency_divisor::divide_by_1024;
    }

    // case 000
    return Frequency_divisor::no_clock_prescaling; 
}


//  Table 19-9: Waveform Generation Mode Bit Description
Timer0::Mode Timer0::mode()
{
    if (atd::read_bit<WGM02>::of(TCCR0B)){ // 1..
	switch(atd::read_bits<WGM01, WGM00>::of(TCCR0A)){ 
    //	case atd::zero<uint8_t>::with_bits<WGM01, WGM00>::to<0,0>(): 
    //	    return Mode::reserved;

	    case atd::zero<uint8_t>::with_bits<WGM01, WGM00>::to<0,1>():
		return Mode::PWM_phase_correct_top_OCRA;

    //	case atd::zero<uint8_t>::with_bits<WGM01, WGM00>::to<1,0>():
    //	    return Mode::reserved;

	    case atd::zero<uint8_t>::with_bits<WGM01, WGM00>::to<1,1>():
		return Mode::fast_PWM_top_OCRA;
	}
    } else { // 0..
	switch(atd::read_bits<WGM01, WGM00>::of(TCCR0A)){ 
	case atd::zero<uint8_t>::with_bits<WGM01, WGM00>::to<0,0>(): 
	    return Mode::normal;

	case atd::zero<uint8_t>::with_bits<WGM01, WGM00>::to<0,1>():
	    return Mode::PWM_phase_correct_top_0xFF;

	case atd::zero<uint8_t>::with_bits<WGM01, WGM00>::to<1,0>():
	    return Mode::CTC;

	case atd::zero<uint8_t>::with_bits<WGM01, WGM00>::to<1,1>():
	    return Mode::fast_PWM_top_0xFF;
	}
    }
    return Mode::reserved;
}

uint16_t Timer0::prescaler()
{
    switch(frequency_divisor()){
	break; case Frequency_divisor::no_clock_prescaling  : return 0;
	break; case Frequency_divisor::no_prescaling	    : return 1;
	break; case Frequency_divisor::divide_by_8	    : return 8;
	break; case Frequency_divisor::divide_by_64	    : return 64;
	break; case Frequency_divisor::divide_by_256	    : return 256;
	break; case Frequency_divisor::divide_by_1024	    : return 1024;
    }

    return 0;
}

// DUDA: ¿cómo gestionar los errores de programación?
void Timer0::prescaler(uint16_t prescaler_factor)
{
    switch (prescaler_factor){
	break; case 0   : off();
	break; case 8   : clock_frequency_divide_by_8();
	break; case 64  : clock_frequency_divide_by_64();
	break; case 256 : clock_frequency_divide_by_256();
	break; case 1024: clock_frequency_divide_by_1024();
	break; default  : clock_frequency_no_prescaling();
    }

}

}// namespace
 
