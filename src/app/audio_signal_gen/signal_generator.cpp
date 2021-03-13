// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

void Main::next_frequency()
{
    avr::Hertz old = freq_gen_;
    speaker_.next_frequency();
    freq_gen_ = speaker_.frequency();

    if (freq_gen_ - old < avr::Hertz{1}){
	if (freq_gen_ < speaker_.max_frequency()){
	    freq_gen_ += avr::Hertz{1};
	    next_frequency();
	}
    }

}

// por culpa del redondeo no decrementa de 1 en 1 cuando
// los números son pequeños, sino más lentamente.
// ¿merece la pena cambiarlo?
void Main::previous_frequency()
{
    avr::Hertz old = freq_gen_;

    speaker_.previous_frequency();
    freq_gen_ = speaker_.frequency();

    if (old - freq_gen_ < avr::Hertz{1}){
	if (freq_gen_ != speaker_.min_frequency()){
	    freq_gen_ -= avr::Hertz{1};
	    previous_frequency();
	}
    }
}



