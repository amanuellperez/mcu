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

#include "mega_timer1_hwd.h"

namespace mega_{
namespace hwd{

// Table 20-7: Clock Select Bit Description
Timer1::Frequency_divisor Timer1::frequency_divisor()
{ 
    switch(atd::read_bits<CS12, CS11, CS10>::of(TCCR1B)){
	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<0,0,1>():
	    return Frequency_divisor::no_prescaling;

	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<0,1,0>():
	    return Frequency_divisor::divide_by_8;

	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<0,1,1>():
	    return Frequency_divisor::divide_by_64;

	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<1,0,0>():
	    return Frequency_divisor::divide_by_256;

	case atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<1,0,1>():
	    return Frequency_divisor::divide_by_1024;
    }

    // case 000
    return Frequency_divisor::no_clock_source;
}

// Table 20-6: Waveform Generation Mode Bit Description
Timer1::Mode Timer1::mode()
{
    switch(atd::read_bits<WGM13, WGM12>::of(TCCR1B)){ 
	break; case // 00..
	    atd::zero<uint8_t>::with_bits<WGM13, WGM12>::to<0,0>():
	    switch(atd::read_bits<WGM11, WGM10>::of(TCCR1A)){ 
		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<0,0>():
		return Mode::normal;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<0,1>():
		return Mode::PWM_phase_correct_top_0x00FF;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<1,0>():
		return Mode::PWM_phase_correct_top_0x01FF;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<1,1>():
		return Mode::PWM_phase_correct_top_0x03FF;
	    }

	break; case // 01..
	    atd::zero<uint8_t>::with_bits<WGM13, WGM12>::to<0,1>():
	    switch(atd::read_bits<WGM11, WGM10>::of(TCCR1A)){ 
		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<0,0>():
		return Mode::CTC_top_OCRA;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<0,1>():
		return Mode::fast_PWM_top_0x00FF;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<1,0>():
		return Mode::fast_PWM_top_0x01FF;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<1,1>():
		return Mode::fast_PWM_top_0x03FF;
	    }

	break; case // 10..
	    atd::zero<uint8_t>::with_bits<WGM13, WGM12>::to<1,0>():
	    switch(atd::read_bits<WGM11, WGM10>::of(TCCR1A)){ 
		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<0,0>():
		return Mode::PWM_phase_and_frequency_correct_top_ICR;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<0,1>():
		return Mode::PWM_phase_and_frequency_correct_top_OCRA;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<1,0>():
		return Mode::PWM_phase_correct_top_ICR;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<1,1>():
		return Mode::PWM_phase_correct_top_OCRA;
	    }

	break; case // 11..
	    atd::zero<uint8_t>::with_bits<WGM13, WGM12>::to<1,1>():
	    switch(atd::read_bits<WGM11, WGM10>::of(TCCR1A)){ 
		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<0,0>():
		return Mode::CTC_top_ICR;

//		break; case 
//		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<0,1>():
//		return Mode::reserved;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<1,0>():
		return Mode::fast_PWM_top_ICR;

		break; case 
		atd::zero<uint8_t>::with_bits<WGM11, WGM10>::to<1,1>():
		return Mode::fast_PWM_top_OCRA;
	    }
    }
    
    return Mode::reserved;
}


// DUDA: ¿cómo gestionar los errores de programación?
void Timer1::prescaler(uint16_t prescaler_factor)
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

uint16_t Timer1::prescaler()
{
    switch(frequency_divisor()){
	break; case Frequency_divisor::no_clock_source      : return 0;
	break; case Frequency_divisor::no_prescaling	    : return 1;
	break; case Frequency_divisor::divide_by_8	    : return 8;
	break; case Frequency_divisor::divide_by_64	    : return 64;
	break; case Frequency_divisor::divide_by_256	    : return 256;
	break; case Frequency_divisor::divide_by_1024	    : return 1024;
    }

    return 0;
}


}// namespace
}// namespace
 
