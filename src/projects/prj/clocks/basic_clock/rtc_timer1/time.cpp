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

// Esta función depende del `locale`: estoy eligiendo escribir las fechas en
// el formato de España.
// Con std::format y std::chrono quedaría mejor, pero hay que implementarlo.
void Main::print(const Date_time& t, uint8_t x0, uint8_t y0)
{
    lcd_.cursor_pos(x0, y0);

    // atd::print_date(lcd_, t);
    atd::print(lcd_, t.day(), nm::Width{2});
    atd::print(lcd_, '/');
    atd::print(lcd_, t.month(), nm::Width{2});
    atd::print(lcd_, '/');
    atd::print(lcd_, t.year(), nm::Width{2});

    lcd_.cursor_pos(x0, y0 + 1);
    //atd::print_time(lcd_, t);
    atd::print(lcd_, t.hours(), nm::Width{2});
    atd::print(lcd_, ':');
    atd::print(lcd_, t.minutes(), nm::Width{2});
    atd::print(lcd_, ':');
    atd::print(lcd_, t.seconds(), nm::Width{2});

    lcd_.cursor_pos(x0 + 9, y0+1);
    atd::print_weekday<week_days_length>(lcd_, t, week_days);
}




// Decidimos cómo mostrar la fecha y la hora al usuario
void Main::user_get_time(Date_time& t, uint8_t x0, uint8_t y0)
{
    print(t, x0, y0);	
    dev::user_get_date(lcd_, keyboard_, t, x0, y0);
    dev::user_get_time(lcd_, keyboard_, t, x0, y0 + 1);
//    dev::user_get_weekday<week_days_length>(lcd_, keyboard_, t, 
//					    x0 + 9, y0 + 1,
//					    week_days);

}


