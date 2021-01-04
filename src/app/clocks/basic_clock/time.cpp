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
#include "cfg.h"


void Main::print_time(atd::Generic_time<std::tm> t, uint8_t x0, uint8_t y0)
{
    lcd_.cursor_pos(x0, y0);
    atd::print_date(lcd_, t);

    lcd_.cursor_pos(x0, y0 + 1);
    atd::print_time(lcd_, t);

    lcd_.cursor_pos(x0 + 9, y0+1);
    atd::print_weekday<week_days_length>(lcd_, t, week_days);
}


void Main::print(const System_clock::time_point& t0)
{
    std::time_t sec = System_clock::to_time_t(t0);
    std::tm t;
    ::gmtime_r(&sec, &t);

    print_time(atd::Generic_time{t}, 0, 0);
}


// Decidimos cómo mostrar la fecha y la hora al usuario
void Main::get_time(atd::Generic_time<std::tm> t, 
	      uint8_t x0, uint8_t y0)
{
    print_time(t, x0, y0);

    dev::user_get_date(lcd_, keyboard_, t, x0, y0);
    dev::user_get_time(lcd_, keyboard_, t, x0, y0 + 1);
    dev::user_get_weekday<week_days_length>(lcd_, keyboard_, t, 
					    x0 + 9, y0 + 1,
					    week_days);

}


std::time_t Main::get_time(const std::time_t& t0, uint8_t x0, uint8_t y0)
{
    std::tm* mt = std::gmtime(&t0);
    
    atd::Generic_time<std::tm> t{*mt};

    get_time(t, x0, y0);

    return std::mktime(mt);
}


System_clock::time_point Main::user_get_time(const System_clock::time_point& t0,
              uint8_t x0, uint8_t y0)
{
    time_t t = std::chrono::system_clock::to_time_t(t0);
    t = get_time(t, x0, y0);

    return std::chrono::system_clock::from_time_t(t);
}
