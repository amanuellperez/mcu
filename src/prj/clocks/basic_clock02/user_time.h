// Copyright (C) 2022 Manuel Perez 
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

#pragma once

#ifndef __USER_TIME_H__
#define __USER_TIME_H__

// Generic functions
// No son del todo genéricas ya que depende del tamaño de la 
// fuente hay que retocar dónde colocar los segundos.

#include "main.h"

template <typename Font, typename Screen>
void print(Screen& scr,
           //atd::Generic_time_view<std::tm> t,
           const atd::Date_time& t,
           uint8_t x0, uint8_t y0)
{
    scr.cursor_pos(x0, y0);
    Font::print_number(scr, t.hours(), nm::Width{2});
    Font::print_colon(scr);
    Font::print_number(scr, t.minutes(), nm::Width{2});

    scr.cursor_pos(x0 + 4 * Font::cols  + 1, y0 + 1);
    scr.print(t.seconds(), nm::Width{2});
}

template <typename Font, typename Screen, typename Keyboard>
void user_get_time(Screen& scr, Keyboard& key, 
                   // atd::Generic_time_view<std::tm> t,
		   atd::Date_time& t,
                   uint8_t x0, uint8_t y0)
{
    print<Font>(scr, t, x0, y0);	
    t.hours(user_choose_number_circular(scr, key).pos(x0, y0)
					.max(23)
					.template choose2<Font>(t.hours()));

    // +1 por los ':'
    t.minutes(user_choose_number_circular(scr, key)
                  .pos(x0 + 2 * Font::cols + 1, y0)
                  .max(59)
                  .template choose2<Font>(t.minutes()));

    t.seconds(user_choose_number_circular(scr, key)
                  .pos(x0 + 4 * Font::cols  + 1, y0 + 1)
                  .max(59)
                  .choose2(t.seconds()));

}



#endif



