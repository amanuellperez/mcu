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
 *
 ****************************************************************************/
#include <cstdint>
#include <atd_time.h>
#include "user_choose_number.h"


namespace dev{

/// El usuario de la aplicación fija el tiempo a través de un teclado
/// y un LCD.
/// t0: parámetro de entrada/salida. De entrada es el tiempo inicial a mostrar
/// en el LCD. De salida es el tiempo seleccionado por el usuario.
// LCD: pantalla con acceso aleatorio (cursor_pos) e iostream (operator<<)
// Teclado: left, right, ok
template <typename LCD, typename Teclado>
std::time_t user_get_time(
    LCD& lcd, Teclado& key, const std::time_t& t0, uint8_t x0, uint8_t y0)
{
    std::tm* t = std::gmtime(&t0);

    lcd.cursor_pos(x0, y0);
    lcd << atd::only_date(*t);

    lcd.cursor_pos(x0, y0 + 1);
    lcd << atd::only_time(*t);

    t->tm_mday = User_choose_number{lcd, key}.pos(x0, y0)
					.between(1, 31)
					.choose2(t->tm_mday);

    // el month empieza en 0, de ahí el +-1
    t->tm_mon = User_choose_number{lcd, key}.pos(x0 + 3, y0)
				       .between(1, 12)
				       .choose2(t->tm_mon + 1);
    --t->tm_mon;   

    // el año se especifica desde 1900
    t->tm_year = User_choose_number{lcd, key}.pos(x0 + 6, y0)
					.choose4(t->tm_year + 1900);
    t->tm_year -= 1900;


    t->tm_hour = User_choose_number{lcd, key}.pos(x0, y0 + 1)
					.max(23)
					.choose2(t->tm_hour);

    t->tm_min = User_choose_number{lcd, key}.pos(x0 + 3, y0 + 1)
					.max(59)
					.choose2(t->tm_min);

    t->tm_sec = User_choose_number{lcd, key}.pos(x0 + 6, y0 + 1)
					.max(59)
					.choose2(t->tm_sec);

    return std::mktime(t);
}


}// namespace

#endif


