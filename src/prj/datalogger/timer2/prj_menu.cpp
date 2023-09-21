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

#include "prj_main.h"

void Main::menu_main()
{
    uart << "\nMain menu\n"
	      "---------\n"
	      "1. Sensor resolution\n"
	      "2. Time measure\n"; 

    char opt{};
    uart >> opt;

    switch (opt){
	break; case '1': menu_sensor();
	break; case '2': menu_time();
	break; default : uart << "Unknown option\n";
    }
}


void Main::menu_sensor()
{
    uart << "Choose resolution: 9, 10, 11 or 12 bits: ";
    uint16_t res = 0;
    uart >> res;

    if (res < 9 or res > 12){
	uart << "Wrong resolution\n";
	return;
    }

    uart << "Changing resolution to " << res << " bits ... ";
    auto result = sensor_resolution(res);
    print_result(uart, result);
}

void Main::menu_time()
{
    uart << "Choose time between measurements (in seconds): ";

    uint32_t t = 1;
    uart >> t;

    if (t == 0){
	uart << "Can't be 0\n";
	return;
    }

    incr_alarm_ = std::chrono::seconds{t};
    uart << "Taking data every " << t << " seconds\n";

    Clock::set(0);
    update_alarm();
}


