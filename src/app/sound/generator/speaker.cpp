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

void Main::init_speaker()
{
    speaker_.on<timer_period_in_us>();
}


void Main::turn_on()
{
    on_ = true;

    update_frequency();
    speaker_.ch1_on();
}

void Main::turn_off()
{
    on_ = false;

    speaker_.ch1_off();
}

void Main::freq_gen_add(uint8_t x)
{
    freq_gen_ += avr::Hertz{x};

    if (on_)
	update_frequency();
}

void Main::freq_gen_substract(uint8_t x)
{
    freq_gen_ -= avr::Hertz{x};

    if (on_)
	update_frequency();
}

void Main::update_frequency()
{
    speaker_.frequency(freq_gen_);
    freq_gen_ = speaker_.frequency();
}
