// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

void Main::run()
{
    init_rtc_clock();

    while(1){
	if (rtc.error())
	    error();
	else 
	    window_main();

	wait_ms(100);
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
    RTC::Clock t;
    rtc.read(t);
    print(t);
}


void Main::print(const RTC::Clock& t)
{
    atd::const_Generic_time<RTC::Clock> gt{t};

    lcd_.cursor_pos(0,0);
    atd::print_date(lcd_, gt);

    lcd_.cursor_pos(0,1);
    atd::print_time(lcd_, gt);
}


void Main::error()
{
    lcd_.clear();
    lcd_ << "RTC error";
}
