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

#pragma once
#ifndef __SENSOR_CFG_H__
#define __SENSOR_CFG_H__

#include <stdint.h>

constexpr uint8_t time_wait_release_key = 100; // in ms

// ¿Cuánto tiempo esperamos para hacer la siguiente iteración?
// Aquí hay que tener en cuenta:
// 1. Este tiempo marca cada cuanto muestreamos el teclado. Si ponemos 800 ms,
//    el teclado funciona mal ya que se tarda mucho en mirarlo.
// 2. El LCD de 3.3V si se hace el polling cada 100 ms se ven mal los dígitos
//    según cambian. Para evitar el parpadeo muestro cada
//    'sensor_update_every' veces la window_main.
constexpr uint16_t time_polling = 100;
constexpr uint8_t sensor_update_every = 1000/time_polling;

// Dias de la semana
constexpr const char* week_days = "DoLuMaMiJuViSa";
constexpr const uint8_t week_days_length = 2;

#endif


