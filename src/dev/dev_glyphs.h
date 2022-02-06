// Copyright (C) 2022 A.Manuel L.Perez 
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

#ifndef __DEV_GLYPHS_H__
#define __DEV_GLYPHS_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Definición de diferentes glyphs
 *
 *  - COMENTARIOS: Los glyphs son independientes de cualquier dispositivo. Se
 *	clasifican por el número de pixeles que tienen: 5 x 8, 5 x 10, 8 x 8,
 *	...
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    06/02/2022 Escrito
 *
 ****************************************************************************/

namespace dev{

// TODO: incluir aqui dev_cols_glyhps.h!!! Son glyphs!!

// El nombre del namespace clasifica el tipo de glyphs. 
// Siempre estarán alineados a la derecha.
namespace glyphs_5x8{
constexpr const char bell[8] = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0001110,
			    0b0001110,
			    0b0011111,
			    0b0000100,
			    0b0000000 };

constexpr const char arrow_up[8] = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0000000 };


constexpr const char arrow_down[8] = 
			  { 0b0000000,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0010101,
			    0b0001110,
			    0b0000100,
			    0b0000000};

constexpr const char heart_full[8] = 
			  { 0b0000000,
			    0b0001010,
			    0b0011111,
			    0b0011111,
			    0b0001110,
			    0b0000100,
			    0b0000000,
			    0b0000000};

constexpr const char heart_empty[8] = 
			  { 0b0000000,
			    0b0001010,
			    0b0010101,
			    0b0010001,
			    0b0001010,
			    0b0000100,
			    0b0000000,
			    0b0000000};


constexpr const char battery_empty[8] = 
			  { 0b0001110,
			    0b0011011,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0011111 };

constexpr const char battery_half[8] = 
			  { 0b0001110,
			    0b0011011,
			    0b0010001,
			    0b0010001,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111 };

constexpr const char battery_full[8] = 
			  { 0b0001110,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111 };

constexpr const char musical_note[8] = 
			  { 0b0000000,
			    0b0000011,
			    0b0001101,
			    0b0001001,
			    0b0001011,
			    0b0011011,
			    0b0011000,
			    0b0000000 };


constexpr const char padlock_close[8] = 
			  { 0b0000000,
			    0b0001110,
			    0b0010001,
			    0b0010001,
			    0b0011111,
			    0b0011011,
			    0b0011011,
			    0b0011111 };

constexpr const char padlock_open[8] = 
			  { 0b0000000,
			    0b0001110,
			    0b0010000,
			    0b0010000,
			    0b0011111,
			    0b0011011,
			    0b0011011,
			    0b0011111 };

constexpr const char man[8] = 
			  { 0b0001110,
			    0b0001110,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0001010,
			    0b0010001 };

constexpr const char skull[8] = 
			  { 0b0000000,
			    0b0001110,
			    0b0010101,
			    0b0011011,
			    0b0001110,
			    0b0001110,
			    0b0000000,
			    0b0000000 };


constexpr const char speaker_right[8] = 
			  { 0b0000001,
			    0b0000011,
			    0b0001111,
			    0b0001111,
			    0b0001111,
			    0b0000011,
			    0b0000001,
			    0b0000000 };


constexpr const char plug[8] = 
			  { 0b0001010,
			    0b0001010,
			    0b0011111,
			    0b0010001,
			    0b0010001,
			    0b0001110,
			    0b0000100,
			    0b0000100 };


// Al principio lo llamé 'square', pero eso entra en conflicto con la
// función square de C. Además, se pueden crear símbolos 3, 4, ... cuyos
// nombres serían exponent3, exponent4, ... 
constexpr const char exponent2[8] = 
			  { 0b0000110,
			    0b0001001,
			    0b0000010,
			    0b0000100,
			    0b0001111,
			    0b0000000,
			    0b0000000,
			    0b0000000 };


constexpr const char exponent3[8] = 
			  { 0b0000110,
			    0b0000001,
			    0b0000010,
			    0b0000001,
			    0b0000110,
			    0b0000000,
			    0b0000000,
			    0b0000000 };


constexpr const char cuberoot[8] = 
			  { 0b0011011,
			    0b0000110,
			    0b0001010,
			    0b0000110,
			    0b0011010,
			    0b0000010,
			    0b0000110,
			    0b0000010 };

constexpr const char pacman[8] = 
			  { 0b0000000,
			    0b0001110,
			    0b0011011,
			    0b0011110,
			    0b0011100,
			    0b0011110,
			    0b0011111,
			    0b0001110 };

constexpr const char pacman_phantom[8] = 
			  { 0b0000000,
			    0b0001110,
			    0b0011111,
			    0b0010101,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0010101 };



}// namespace


}// namespace
#endif
