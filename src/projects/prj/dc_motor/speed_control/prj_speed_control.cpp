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

#include "prj_main.h"
#include <pli_iostream.h>

Main::Direction Main::ask_direction()
{
    char dir = pli::ask_char(uart, "Direction (+/-): ");

    if (dir == '-')
	return Direction::negative;

    return Direction::positive;
}

void Main::turn_speed_control()
{
    int16_t rpm = pli::ask<int16_t>(uart, "Speed (in rpm): ");

    auto direction = ask_direction();

//    Speed_control_motor::turn(direction, rpm);
    uint8_t p = 20; // TODO: por qué empezar en 20?

    atd::Float16 RPM{rpm};
    for (; p <= 100; p += 5){
	Motor::turn(direction, p);
	Micro::wait_ms(10); // TODO: por que 10 ms? inercia
	auto speed = Speedmeter::measure_speed_rpm();
	uart << "p = " << atd::Percentage{p} << " -> " 
	     << speed << " rpm\n";
	if (speed > atd::Float16{rpm})
	    return;
    }

}

