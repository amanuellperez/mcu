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

#include "../../avr_signal_generator.h"
#include "../../avr_time.h"
#include "../../avr_types.h"


using Signal_gen = avr::Signal_generator;
constexpr uint16_t period_in_us = 1;

int main()
{
    Signal_gen::on<period_in_us>();
    

    while(1){
	Signal_gen::ch1_square_wave(avr::Hertz{50});
	wait_ms(5000);

	Signal_gen::ch1_off();
	wait_ms(5000);

    }
}



