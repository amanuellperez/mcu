// Copyright (C) 2021 Manuel Perez 
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
#include "cfg.h"
#include <user_time.h>

void Main::init_time(RTC::Time_point& t)
{
    atd::Generic_time_view<RTC::Time_point> gt{t};

    gt.day(1);
    gt.month(1);
    gt.year(2021);

    gt.hours(12);
    gt.minutes(0);
    gt.seconds(0);

    // Que no se olvide encenderlo!!!
    t.clock_on =  true;
}


// precondition: que el LCD esté funcionando. 
void Main::init_rtc_clock()
{
    RTC::Time_point t;
    rtc_.read(t);    

    if (!t.clock_on){
	t.clock_on = true;
	rtc_.write(t);
//	lcd_.clear();
//	lcd_ << "BME280 v1.0";  // presentación y sirve para depurar
//	wait_ms(200);
//	init_time(t);
//	window_set_time(t);
    }
}



void Main::window_set_time()
{
    RTC::Time_point t;
    rtc_.read(t);

    window_set_time(t);
}


void Main::window_set_time(RTC::Time_point& t)
{
    atd::Generic_time_view<RTC::Time_point> gt{t};

    lcd_.clear();

    print_datetime(gt, 0, 0);
    user_get_datetime(gt, 0, 0);

    rtc_.write(t);

    wait_release_key();

}



void Main::print_datetime(atd::Generic_time_view<RTC::Time_point> t, uint8_t x0, uint8_t y0)
{
    lcd_.cursor_pos(x0, y0);
    atd::print_date(lcd_, t);

    lcd_.cursor_pos(x0, y0 + 1);
    atd::print_time(lcd_, t);

    lcd_.cursor_pos(x0 + 9, y0+1);
    atd::print_weekday<week_days_length>(lcd_, t, week_days);
}


// Decidimos cómo mostrar la fecha y la hora al usuario
void Main::user_get_datetime(atd::Generic_time_view<RTC::Time_point> t, uint8_t x0, uint8_t y0)
{
    dev::user_get_date(lcd_, keyboard_, t, x0, y0);
    dev::user_get_weekday<week_days_length>(lcd_, keyboard_, t, 
					    x0 + 9, y0 + 1,
					    week_days);
    dev::user_get_time(lcd_, keyboard_, t, x0, y0 + 1);
}



