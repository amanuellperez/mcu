// Copyright (C) 2019-2020 Manuel Perez 
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
#include <mcu_clock.h>
#include <atd_time.h>

void Main::run()
{
    while(1){
//	if (errno_)
//	    error();
//	else 
	    window_main();

	Micro::wait_ms(100);
    }
}


void Main::window_main()
{
    show_window_main();

    if (keyboard_.key<OK_KEY>().is_pressed())
	window_set_time();
}


void Main::show_window_main()
{
    print(System_clock::now_as_date_time());
}



