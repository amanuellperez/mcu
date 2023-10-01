// Copyright (C) 2019-2022 Manuel Perez 
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

#ifndef __DEV_USER_TIME_H__
#define __DEV_USER_TIME_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para gestionar time.h mediante dispositivos.
 *
 * HISTORIA
 *  Manuel Perez
 *	01/09/2019 v0.0
 *	21/12/2019 Cambio interfaz (elimino tm por time_t).
 *	05/01/2020 Añado chrono::time_point.
 *	27/12/2020 Generalizando para poder usarlo con RTC.
 *	30/12/2020 Hacemos la elección de los números circular (pasamos de 00
 *	           a 59 segundos)
 *	07/04/2022 Que funcione con Fonts.
 *	           NOTA: este archivo sobra. Es mejor que cada aplicación
 *	           concreta implemente todas estas funciones ya que hay muchas
 *	           formas de hacerlo.
 *
 *	07/01/2023 En lugar de trabajar con Date_time_view<T> uso el interfaz
 *		   genérico definido por la clase atd::Date_time 
 *
 ****************************************************************************/
#include <cstdint>
#include <chrono>
#include <atd_time.h>
#include "user_choose_number.h"
#include "user_choose_string.h"


namespace dev{

template <uint8_t names_length,
          typename Screen, typename Keyboard,
          typename Date_time>
void user_get_weekday(Screen& scr,
                      Keyboard& key,
                      //atd::Date_time_view<Date_time>& t,
		      Date_time& t,
                      uint8_t x0,
                      uint8_t y0,
                      const char* names_weekday)
{
    t.weekday(user_choose_string_circular<names_length>(scr, key, 
			atd::Array_const_nstrings{names_weekday, names_length})
		.pos(x0, y0)
		.show(t.weekday()));
}

template <typename Font = Font_digit_default,
          typename Screen, typename Keyboard,
          typename Date_time>
void user_get_date(Screen& scr,
                   Keyboard& key,
                   //atd::Date_time_view<Date_time>& t,
		   Date_time& t,
                   uint8_t x0,
                   uint8_t y0)
{
    t.day(user_choose_number_circular(scr, key).pos(x0, y0)
					.between(1, 31)
					.template choose2<Font>(t.day()));

    t.month(user_choose_number_circular(scr, key)
                .pos(x0 + 2 * Font::cols + 1, y0)
                .between(1, 12)
                .template choose2<Font>(t.month()));

    t.year(user_choose_number_circular(scr, key)
               .pos(x0 + 4 * Font::cols + 2, y0)
               .template choose4<Font>(t.year()));
}

template <typename Font = Font_digit_default,
          typename Screen, typename Keyboard,
          typename Date_time>
void user_get_time(Screen& scr,
                   Keyboard& key,
                   //atd::Date_time_view<Date_time>& t,
		   Date_time& t,
                   uint8_t x0,
                   uint8_t y0)
{
    t.hours(user_choose_number_circular(scr, key).pos(x0, y0)
					.max(23)
					.template choose2<Font>(t.hours()));

    // +1 por los ':'
    t.minutes(user_choose_number_circular(scr, key)
                  .pos(x0 + 2 * Font::cols + 1, y0)
                  .max(59)
                  .template choose2<Font>(t.minutes()));

    t.seconds(user_choose_number_circular(scr, key)
                  .pos(x0 + 4 * Font::cols  + 2, y0)
                  .max(59)
                  .template choose2<Font>(t.seconds()));
}

template <typename Font = Font_digit_default,
          typename Screen,
          typename Date_time>
void print_time(Screen& scr,
                //atd::Date_time_view<Date_time>& t,
                const Date_time& t,
                uint8_t x0,
                uint8_t y0)
{
    scr.cursor_pos(x0, y0);
    Font::print_number(scr, t.hours(), nm::Width{2});
    Font::print_colon(scr);
    Font::print_number(scr, t.minutes(), nm::Width{2});
    Font::print_colon(scr);
    Font::print_number(scr, t.seconds(), nm::Width{2});
}

template <typename Font = Font_digit_default,
          typename Screen,
          typename Date_time>
void print_date(Screen& scr,
                //atd::Date_time_view<Date_time>& t,
                const Date_time& t,
                uint8_t x0,
                uint8_t y0)
{
    scr.cursor_pos(x0, y0);
    Font::print_number(scr, t.day(), nm::Width{2});
    Font::print_colon(scr); // TODO: print_slash (con big_digit dificil)
    Font::print_number(scr, t.month(), nm::Width{2});
    Font::print_colon(scr); // TODO: print_slash
    Font::print_number(scr, t.year(), nm::Width{4});
}

}// namespace

#endif


