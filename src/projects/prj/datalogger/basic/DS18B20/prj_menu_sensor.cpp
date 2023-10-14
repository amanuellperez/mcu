// Copyright (C) 2023 Manuel Perez 
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

#include "../prj_main.h"
#include "hwd_ds18b20.h"

static void menu_sensor(std::iostream& out, Sensor& sensor)
{
    out << "Choose resolution: 9, 10, 11 or 12 bits: ";
    uint16_t res = 0;
    out >> res;

    if (res < 9 or res > 12){
	out << "Wrong resolution\n";
	return;
    }

    out << "\nChanging resolution to " << res << " bits ... ";
    auto result = sensor_resolution(sensor, res);
    print_result(out, result);
}

void Main::print_menu_sensor()
{
    uart << "2. Sensor resolution\n";
}

void Main::menu_sensor(char option)
{
    switch (option){
	break; case '2': ::menu_sensor(uart, sensor_);
	break; default : uart << "Unknown option\n";
    }
}

