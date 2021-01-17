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

#include "main.h"

void Main::run()
{
    lcd_.clear();

    while(1){
	// TODO: if (errno_) error();
	if (sensor_.error()) {
	    lcd_.clear();
	    lcd_ << "Sensor error";
	} else if (rtc_.error()) {
	    lcd_.clear();
	    lcd_ << "RTC error";
	}
	else
	    window_main();

	wait_ms(100);
    }
}

void Main::window_main()
{
    show_window_main();

    if (user_press_change_time())
    {
	wait_user_release_change_time();
	window_set_time();
    }
}

void Main::show_window_main()
{
    RTC::Time_point t0;
    rtc_.read(t0);
    atd::Generic_time<RTC::Time_point> t{t0};
    
    lcd_.cursor_pos(0, 0);
    lcd_ << std::setw(2) << t.hours() << ':'
	<< std::setw(2) << t.minutes();

    Sensor::Celsius T;
    Sensor::Pascal P;
    Sensor::Relative_humidity H;
    sensor_.T_and_P_and_H(T, P, H);
    Sensor::Hectopascal hP{P};

    lcd_.cursor_pos(8, 0);
    lcd_ << T << ' ' << lcd_symbol::of("º") << 'C';
    lcd_.cursor_pos(0, 1);
    lcd_ << H << "% ";
    lcd_ << hP << " hPa ";
}



