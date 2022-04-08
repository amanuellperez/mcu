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
#include <dev_system_clock.h>
#include <atd_time.h>

// Window: main
// ------------
void Main::window_main()
{
    show_window_main();

    if (keyboard_.key<OK_KEY>().is_pressed())
	window_set_time(System_clock::now());
}


void Main::show_window_main()
{
    print(System_clock::now(), 2, 0);
}



// Window: set_time
// ----------------
void Main::window_set_time(System_clock::time_point t0)
{
    scr_.clear();

    t0 = user_get_time(t0, 2, 0);
    wait_release_key();

    System_clock::set(t0);
}


