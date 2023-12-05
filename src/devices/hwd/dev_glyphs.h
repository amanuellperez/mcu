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
 *    Manuel Perez
 *    06/02/2022 Escrito
 *
 *  TODO: moverlo a dev1
 ****************************************************************************/
#include <avr_memory.h>
#include <atd_memory.h>
#include "not_generic.h" // TODO: Este archivo no es genérico por dos motivos:
			 // (1) Aparece la macro PROGMEM (¿es genérica?)
			 // (2) Hay que parametrizarlo todo por la función
			 //	Micro::ROM_read que dependerá del
			 //	microcontrolador.
			 //
			 // ¿Cómo podemos "generalizar" esto?
			 //
			 // Solución 1
			 // ----------
			 // En lugar de distribuir este archivo
			 // distribuimos un archivo de texto con los gliphs.
			 // Ejemplo:
			 //	bell = 0b00000
			 //	       0b00010 ...
			 //
			 // fichero que llamamos glyphs.rom (o .pgm???)
			 // A partir de este fichero usando un script
			 // generamos este archivo de cabecera:
			 //	$ rom2cpp atmega32 glyphs.rom
			 // que genera el fichero adecuado dependiendo de la
			 // arquitectura usada.
			 //
			 // Solución 2
			 // ----------
			 // ??? ¿Cómo hacerlo usando C++ sin usar scripts ni
			 // macros?

namespace dev{

// TODO: incluir aqui dev_cols_glyhps.h!!! Son glyphs!!

using ROM_read = not_generic::ROM_read;

// El nombre del namespace clasifica el tipo de glyphs. 
// Siempre estarán alineados a la derecha.
namespace glyphs_5x8{

template <typename Read>
using Glyph = atd::ROM_array<uint8_t, 8, Read>;

constexpr const Glyph<ROM_read> bell PROGMEM = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0001110,
			    0b0001110,
			    0b0011111,
			    0b0000100,
			    0b0000000 };

constexpr const Glyph<ROM_read> arrow_up PROGMEM = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0000000 };


constexpr const Glyph<ROM_read> arrow_down PROGMEM = 
			  { 0b0000000,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0010101,
			    0b0001110,
			    0b0000100,
			    0b0000000};

constexpr const Glyph<ROM_read> heart_full PROGMEM = 
			  { 0b0000000,
			    0b0001010,
			    0b0011111,
			    0b0011111,
			    0b0001110,
			    0b0000100,
			    0b0000000,
			    0b0000000};

constexpr const Glyph<ROM_read> heart_empty PROGMEM = 
			  { 0b0000000,
			    0b0001010,
			    0b0010101,
			    0b0010001,
			    0b0001010,
			    0b0000100,
			    0b0000000,
			    0b0000000};


constexpr const Glyph<ROM_read> battery_empty PROGMEM = 
			  { 0b0001110,
			    0b0011011,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0011111 };

constexpr const Glyph<ROM_read> battery_half PROGMEM = 
			  { 0b0001110,
			    0b0011011,
			    0b0010001,
			    0b0010001,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111 };

constexpr const Glyph<ROM_read> battery_full PROGMEM = 
			  { 0b0001110,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111 };

constexpr const Glyph<ROM_read> musical_note PROGMEM = 
			  { 0b0000000,
			    0b0000011,
			    0b0001101,
			    0b0001001,
			    0b0001011,
			    0b0011011,
			    0b0011000,
			    0b0000000 };


constexpr const Glyph<ROM_read> padlock_close PROGMEM = 
			  { 0b0000000,
			    0b0001110,
			    0b0010001,
			    0b0010001,
			    0b0011111,
			    0b0011011,
			    0b0011011,
			    0b0011111 };

constexpr const Glyph<ROM_read> padlock_open PROGMEM = 
			  { 0b0000000,
			    0b0001110,
			    0b0010000,
			    0b0010000,
			    0b0011111,
			    0b0011011,
			    0b0011011,
			    0b0011111 };

constexpr const Glyph<ROM_read> man PROGMEM = 
			  { 0b0001110,
			    0b0001110,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0001010,
			    0b0010001 };

constexpr const Glyph<ROM_read> skull PROGMEM = 
			  { 0b0000000,
			    0b0001110,
			    0b0010101,
			    0b0011011,
			    0b0001110,
			    0b0001110,
			    0b0000000,
			    0b0000000 };


constexpr const Glyph<ROM_read> speaker_right PROGMEM = 
			  { 0b0000001,
			    0b0000011,
			    0b0001111,
			    0b0001111,
			    0b0001111,
			    0b0000011,
			    0b0000001,
			    0b0000000 };


constexpr const Glyph<ROM_read> plug PROGMEM = 
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
constexpr const Glyph<ROM_read> exponent2 PROGMEM = 
			  { 0b0000110,
			    0b0001001,
			    0b0000010,
			    0b0000100,
			    0b0001111,
			    0b0000000,
			    0b0000000,
			    0b0000000 };


constexpr const Glyph<ROM_read> exponent3 PROGMEM = 
			  { 0b0000110,
			    0b0000001,
			    0b0000010,
			    0b0000001,
			    0b0000110,
			    0b0000000,
			    0b0000000,
			    0b0000000 };


constexpr const Glyph<ROM_read> cuberoot PROGMEM = 
			  { 0b0011011,
			    0b0000110,
			    0b0001010,
			    0b0000110,
			    0b0011010,
			    0b0000010,
			    0b0000110,
			    0b0000010 };

constexpr const Glyph<ROM_read> pacman PROGMEM = 
			  { 0b0000000,
			    0b0001110,
			    0b0011011,
			    0b0011110,
			    0b0011100,
			    0b0011110,
			    0b0011111,
			    0b0001110 };

constexpr const Glyph<ROM_read> pacman_phantom PROGMEM = 
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
