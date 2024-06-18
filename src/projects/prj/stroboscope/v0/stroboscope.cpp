// Copyright (C) 2021 Manuel Perez 
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

#include "stroboscope.h"
#include "cfg.h"

//// Si el micro está a 1MHz
// De momento no voy a dejar esta opción. Al comentar esta función dará un
// error de compilación si se intenta compilar en 1MHz
//void Stroboscope::generate_1MHz()
//{
//    if constexpr (freq_mcu == 1000000ul){
//
//    Timer::off();
//
//    if (freq_ <= 15){
//	uint16_t top  = 15625u / freq_ - 1; // 15625 = 1000000/64;
//	Timer::input_capture_register(top);
//	Timer::output_compare_register_A(8); // 500 us
//	Timer::set_clock_period_in_us<64>();
//    }
//    else{
//        uint16_t top =
//            static_cast<uint16_t>(freq_mcu / uint32_t{freq_}) - 1;
//	Timer::input_capture_register(top);
//	Timer::output_compare_register_A(500); // 500 us
//	Timer::set_clock_period_in_us<1>();
//    }
//    }
//}

void Stroboscope::generate_8MHz()
{
    if constexpr (freq_mcu == 8000000ul){
    Timer::off();

    if (freq_ <= 123){
	uint16_t top  = 31250 / freq_ - 1; // 31250 = 8000000/256;
	Timer::unsafe_input_capture_register(top);
	Timer::counter_type ocrA = top / duty_percent;
	Timer::unsafe_output_compare_register_A(ocrA);
	//Timer::output_compare_register_A(15); // 500 us!!!
	Timer::set_clock_period_in_us<32>();
    }
    else{
        uint16_t top =
            static_cast<uint16_t>(uint32_t{8000000ul} / uint32_t{freq_}) - 1;
	Timer::unsafe_input_capture_register(top);
	Timer::counter_type ocrA = top / duty_percent;
	Timer::unsafe_output_compare_register_A(ocrA); 
//	if (freq_ < 1000)
//	    Timer::output_compare_register_A(4000); // 500 us
//	else
//	    Timer::output_compare_register_A(1280); // 160 us

	Timer::set_clock_period_in_ns<125>();
    }

    }
}

void Stroboscope::generate(uint16_t freq)
{
    freq_ = bound(freq);
    on();
}


uint16_t Stroboscope::bound(uint16_t freq)
{
    if (freq > freq_max)
	return freq_max;

    return freq;
}

void Stroboscope::freq_plus(uint16_t x)
{
    if (freq_ + x <= freq_max)
	freq_ += x;
    else
	freq_ = freq_max;

    if (on_)
	generate();
}


void Stroboscope::freq_minus(uint16_t x)
{
    if (freq_ >= x + freq_min)
	freq_ -= x;
    else
	freq_ = freq_min;

    if (on_)
	generate();
}


