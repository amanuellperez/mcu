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


void Main::print_time(atd::Date_time_view<RTC::Time_point> t, uint8_t x0, uint8_t y0)
{
    lcd_.cursor_pos(x0, y0);
    atd::print_date(lcd_, t);

    lcd_.cursor_pos(x0, y0 + 1);
    atd::print_time(lcd_, t);

    lcd_.cursor_pos(x0 + 9, y0+1);
    atd::print_weekday<week_days_length>(lcd_, t, week_days);
}


// Decidimos cómo mostrar la fecha y la hora al usuario
void Main::user_get_time(atd::Date_time_view<RTC::Time_point> t, uint8_t x0, uint8_t y0)
{
    dev::user_get_date(lcd_, keyboard_, t, x0, y0);
    dev::user_get_weekday<week_days_length>(lcd_, keyboard_, t, 
					    x0 + 9, y0 + 1,
					    week_days);
    dev::user_get_time(lcd_, keyboard_, t, x0, y0 + 1);
}



