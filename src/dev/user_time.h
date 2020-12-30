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

#pragma once

#ifndef __USER_TIME_H__
#define __USER_TIME_H__
/****************************************************************************
 *
 * - DESCRIPCION: Funciones para gestionar time.h mediante dispositivos.
 *
 * - HISTORIA:
 *  A.Manuel L.Perez
 *	01/09/2019 v0.0
 *	21/12/2019 Cambio interfaz (elimino tm por time_t).
 *	05/01/2020 Añado chrono::time_point.
 *	27/12/2020 Generalizando para poder usarlo con RTC.
 *	30/12/2020 Hacemos la elección de los números circular (pasamos de 00
 *	           a 59 segundos)
 *
 ****************************************************************************/
#include <cstdint>
#include <chrono>
#include <atd_time.h>
#include "user_choose_number.h"


namespace dev{

/// El usuario de la aplicación fija el tiempo a través de un teclado
/// y un LCD.
/// t0: parámetro de entrada/salida. De entrada es el tiempo inicial a mostrar
/// en el LCD. De salida es el tiempo seleccionado por el usuario.
// LCD: pantalla con acceso aleatorio (cursor_pos) e iostream (operator<<)
// Keyboard: left, right, ok
template <typename LCD, typename Keyboard, typename Date_time>
void user_get_time(LCD& lcd,
                   Keyboard& key,
                   atd::Generic_time<Date_time>& t,
                   uint8_t x0, uint8_t y0)
{
    lcd.cursor_pos(x0, y0);
    atd::print_date(lcd, t);

    lcd.cursor_pos(x0, y0 + 1);
    atd::print_time(lcd, t);

    t.day(user_choose_number_circular(lcd, key).pos(x0, y0)
					.between(1, 31)
					.choose2(t.day()));

    t.month(user_choose_number_circular(lcd, key).pos(x0 + 3, y0)
				       .between(1, 12)
				       .choose2(t.month()));

    t.year(user_choose_number_circular(lcd, key).pos(x0 + 6, y0)
					.choose4(t.year()));

    t.hours(user_choose_number_circular(lcd, key).pos(x0, y0 + 1)
					.max(23)
					.choose2(t.hours()));

    t.minutes(user_choose_number_circular(lcd, key).pos(x0 + 3, y0 + 1)
					.max(59)
					.choose2(t.minutes()));

    t.seconds(user_choose_number_circular(lcd, key).pos(x0 + 6, y0 + 1)
					.max(59)
					.choose2(t.seconds()));

}


template <typename LCD, typename Keyboard>
std::time_t user_get_time(
    LCD& lcd, Keyboard& key, const std::time_t& t0, uint8_t x0, uint8_t y0)
{
    std::tm* mt = std::gmtime(&t0);
    
    atd::Generic_time<std::tm> t{*mt};
    user_get_time(lcd, key, t, x0, y0);

    return std::mktime(mt);
}


/// El usuario de la aplicación fija el tiempo a través de un teclado
/// y un LCD.
/// t0: parámetro de entrada/salida. De entrada es el tiempo inicial a mostrar
/// en el LCD. De salida es el tiempo seleccionado por el usuario.
// LCD: pantalla con acceso aleatorio (cursor_pos) e iostream (operator<<)
// Keyboard: left, right, ok
template <typename LCD, typename Keyboard, typename Clock, typename Duration>
std::chrono::time_point<Clock, Duration> user_get_time(LCD& lcd,
                                         Keyboard& key,
                                         const std::chrono::time_point<Clock, Duration>& t0,
                                         uint8_t x0, uint8_t y0)
{
    time_t t = std::chrono::system_clock::to_time_t(t0);
    t = user_get_time(lcd, key, t, x0, y0);
    return std::chrono::system_clock::from_time_t(t);
}


}// namespace

#endif


