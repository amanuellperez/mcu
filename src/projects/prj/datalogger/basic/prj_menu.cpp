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

void Main::menu_options()
{
    while (1) {
	uart << "\nMain menu\n"
		  "---------\n"
		  "0. Start\n"
		  "1. Time measure\n";
		  print_menu_sensor();

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '0': return;
	    break; case '1': menu_time();
	    break; default : menu_sensor(opt);
	}
    }
}



void Main::menu_time()
{
    uart << "Choose time between measurements\n  (xx h[ours] yy m[inutes] zz s[econds]): ";

    time_point t0{};
    bool res = read_time(uart, t0);

    if (res == false){
	uart << "Wrong time";
	return;
    }

    uint32_t t_incr = t0.time_since_epoch().count();
    if (t_incr == 0){
	uart << "Time can't be 0\n";
	return;
    }

    change_time_settings(0, t_incr);

    uart << "\nTaking data every ";
    print_time(uart, t0); 
    uart << '\n';
}


