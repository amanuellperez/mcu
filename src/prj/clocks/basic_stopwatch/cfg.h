// Copyright (C) 2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __CFG_H__
#define __CFG_H__

#include <stdint.h>

// periodo, en ms, en el que tardamos en repetir un bucle main
// (realmente es el tiempo de espera entre una iteración del main y la
// siguiente)
constexpr static uint16_t period_main_clock_ms = 50;

// Divisor de la frecuencia del main para esperar a que acabe el bouncing.
// Esto es, esperamos keyboard_clock_imax * period_main_clock_ms milisegundos
// hasta mirar de nuevo el teclado después de haber pulsado una tecla.
constexpr static uint8_t keyboard_clock_imax = 2; // 2 * 50 = 100


#endif


