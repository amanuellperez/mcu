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
#include <avr_memory.h>
#include "not_generic.h"

namespace dev{

// TODO: incluir aqui dev_cols_glyhps.h!!! Son glyphs!!

// El nombre del namespace clasifica el tipo de glyphs. 
// Siempre estarán alineados a la derecha.
namespace glyphs_5x8{
constexpr const not_generic::Progmem_array<uint8_t, 8> bell PROGMEM = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0001110,
			    0b0001110,
			    0b0011111,
			    0b0000100,
			    0b0000000 };

constexpr const not_generic::Progmem_array<uint8_t, 8> arrow_up PROGMEM = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0000000 };


constexpr const not_generic::Progmem_array<uint8_t, 8> arrow_down PROGMEM = 
			  { 0b0000000,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0010101,
			    0b0001110,
			    0b0000100,
			    0b0000000};

constexpr const not_generic::Progmem_array<uint8_t, 8> heart_full PROGMEM = 
			  { 0b0000000,
			    0b0001010,
			    0b0011111,
			    0b0011111,
			    0b0001110,
			    0b0000100,
			    0b0000000,
			    0b0000000};

constexpr const not_generic::Progmem_array<uint8_t, 8> heart_empty PROGMEM = 
			  { 0b0000000,
			    0b0001010,
			    0b0010101,
			    0b0010001,
			    0b0001010,
			    0b0000100,
			    0b0000000,
			    0b0000000};


constexpr const not_generic::Progmem_array<uint8_t, 8> battery_empty PROGMEM = 
			  { 0b0001110,
			    0b0011011,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0011111 };

constexpr const not_generic::Progmem_array<uint8_t, 8> battery_half PROGMEM = 
			  { 0b0001110,
			    0b0011011,
			    0b0010001,
			    0b0010001,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111 };

constexpr const not_generic::Progmem_array<uint8_t, 8> battery_full PROGMEM = 
			  { 0b0001110,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111 };

constexpr const not_generic::Progmem_array<uint8_t, 8> musical_note PROGMEM = 
			  { 0b0000000,
			    0b0000011,
			    0b0001101,
			    0b0001001,
			    0b0001011,
			    0b0011011,
			    0b0011000,
			    0b0000000 };


constexpr const not_generic::Progmem_array<uint8_t, 8> padlock_close PROGMEM = 
			  { 0b0000000,
			    0b0001110,
			    0b0010001,
			    0b0010001,
			    0b0011111,
			    0b0011011,
			    0b0011011,
			    0b0011111 };

constexpr const not_generic::Progmem_array<uint8_t, 8> padlock_open PROGMEM = 
			  { 0b0000000,
			    0b0001110,
			    0b0010000,
			    0b0010000,
			    0b0011111,
			    0b0011011,
			    0b0011011,
			    0b0011111 };

constexpr const not_generic::Progmem_array<uint8_t, 8> man PROGMEM = 
			  { 0b0001110,
			    0b0001110,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0001010,
			    0b0010001 };

constexpr const not_generic::Progmem_array<uint8_t, 8> skull PROGMEM = 
			  { 0b0000000,
			    0b0001110,
			    0b0010101,
			    0b0011011,
			    0b0001110,
			    0b0001110,
			    0b0000000,
			    0b0000000 };


constexpr const not_generic::Progmem_array<uint8_t, 8> speaker_right PROGMEM = 
			  { 0b0000001,
			    0b0000011,
			    0b0001111,
			    0b0001111,
			    0b0001111,
			    0b0000011,
			    0b0000001,
			    0b0000000 };


constexpr const not_generic::Progmem_array<uint8_t, 8> plug PROGMEM = 
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
constexpr const not_generic::Progmem_array<uint8_t, 8> exponent2 PROGMEM = 
			  { 0b0000110,
			    0b0001001,
			    0b0000010,
			    0b0000100,
			    0b0001111,
			    0b0000000,
			    0b0000000,
			    0b0000000 };


constexpr const not_generic::Progmem_array<uint8_t, 8> exponent3 PROGMEM = 
			  { 0b0000110,
			    0b0000001,
			    0b0000010,
			    0b0000001,
			    0b0000110,
			    0b0000000,
			    0b0000000,
			    0b0000000 };


constexpr const not_generic::Progmem_array<uint8_t, 8> cuberoot PROGMEM = 
			  { 0b0011011,
			    0b0000110,
			    0b0001010,
			    0b0000110,
			    0b0011010,
			    0b0000010,
			    0b0000110,
			    0b0000010 };

constexpr const not_generic::Progmem_array<uint8_t, 8> pacman PROGMEM = 
			  { 0b0000000,
			    0b0001110,
			    0b0011011,
			    0b0011110,
			    0b0011100,
			    0b0011110,
			    0b0011111,
			    0b0001110 };

constexpr const not_generic::Progmem_array<uint8_t, 8> pacman_phantom PROGMEM = 
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
