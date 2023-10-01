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

#include "main.h"
#include <atd_eng_magnitude.h>

void Main::init_signal_generator(uint16_t period_in_us)
{
    sw_period_ = period_in_us;

    switch(period_in_us){
	case 1: speaker_.on<1>(); break;
	case 8: speaker_.on<8>(); break;
	case 64: speaker_.on<64>(); break;
	case 256: speaker_.on<256>(); break;
	case 1024: speaker_.on<1024>(); break;

    }

    speaker_.frequency(freq_gen_);
    freq_gen_ = speaker_.frequency();
}


void Main::turn_on()
{
    on_ = true;

    speaker_.ch1_on();
}

void Main::turn_off()
{
    on_ = false;

    speaker_.ch1_off();
}


void Main::multiply_frequency_by(const Scalar& a)
{
    if (a * freq_gen_ < speaker_.max_frequency()){
	speaker_.frequency(a*freq_gen_);
	freq_gen_ = speaker_.frequency();
    }
}

void Main::divide_frequency_by(const Scalar& a)
{
    if (freq_gen_ / a > speaker_.min_frequency()){
	speaker_.frequency(freq_gen_ / a);
	freq_gen_ = speaker_.frequency();
    }
}

void Main::next_frequency(const avr::Frequency& fadd)
{
    avr::Frequency next_freq = freq_gen_ + fadd;

    if (next_freq > speaker_.max_frequency())
	return;

    while (freq_gen_ < next_freq){
	speaker_.next_frequency();
	freq_gen_ = speaker_.frequency();
    }
}

void Main::previous_frequency(const avr::Frequency& fsubstract)
{
    avr::Frequency next_freq;
    if (freq_gen_ > fsubstract)
	next_freq = freq_gen_ - fsubstract;
    else
	next_freq = freq_gen_;

    if (next_freq < speaker_.min_frequency())
	return;

    while (freq_gen_ > next_freq){
	speaker_.previous_frequency();
	freq_gen_ = speaker_.frequency();
    }

}


void Main::print_without_decimals(LCD& out, const avr::Frequency& f)
{
    out << atd::to_integer<uint16_t>(f.internal_value());
    atd::print_unit(out, f);

}

void Main::print(LCD& out, const avr::Frequency& f)
{
    if (f < 1_kHz)
	print_without_decimals(out, f);
    else
	out << f;

}



