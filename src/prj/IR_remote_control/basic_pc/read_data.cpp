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

#include "main.h"
#include <algorithm>

static volatile Timer::counter_type buffer[Main::max_num_data];
static volatile int8_t i = -1;
static volatile bool end_of_reading = false;

static void reset_data()
{
    i = -1;
    end_of_reading = false;
}

void Main::read_data()
{
    reset_data();

    avr::enable_all_interrupts();
    Timer::enable_overflow_interrupt();
    avr::Interrupt::enable_pin<sensor_pin>();

    Timer::on<1>();	    // leemos microsegundos
    
    while (end_of_reading == false and i < max_num_data)
    { ; }

    avr::disable_all_interrupts();

    Timer::off(); 

    num_data = i;
    std::copy_n(buffer, num_data, data);
}


ISR_SENSOR_PIN {
    if (i >= 0)
	buffer[i] = Timer::value();

    Timer::reset();
    i = i + 1;
}



ISR_TIMER_OVF {
    if (i != -1)
	end_of_reading = true;
}


