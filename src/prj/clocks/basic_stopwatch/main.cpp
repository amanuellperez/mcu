// Copyright (C) 2021 A.Manuel L.Perez 
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

Main::Main() : ktimer_{keyboard_clock_imax}
{
// init_hardware():
    init_lcd();
    init_keyboard();
    init_stopwatch();
}


void Main::init_keyboard()
{
    ktimer_.off();
}


void Main::init_lcd()
{
    Font::load(scr_);
}

void Main::init_stopwatch()
{
    Stopwatch::init();
}

void Main::run()
{
    print_time();

    while(1){
//	if (errno_)
//	    error();
//	else 
	    window_main();

	mcu::wait_ms(period_main_clock_ms);
	ktimer_.tick();
    }
}



int main()
{
    Main app;
    app.run();
}




