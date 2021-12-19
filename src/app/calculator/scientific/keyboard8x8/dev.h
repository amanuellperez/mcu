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

#ifndef __DEV_H__
#define __DEV_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Hardware connections
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    12/12/2021 v0.0
 *
 ****************************************************************************/
#include <stdint.h>

#include <dev_keypad.h>

#include "../keycodes.h"


// pins usados
// ------------
// reset: pin 1
// UART for debugging purpose: pins 2 and 3
constexpr uint8_t KB_col6_pin = 4;
constexpr uint8_t KB_col7_pin = 5;

constexpr uint8_t KB_row0_pin = 6;
// VCC and GND: 7, 8
// los coloco al revés porque coinciden enfrente con el keyboard
constexpr uint8_t KB_row7_pin = 9;
constexpr uint8_t KB_row6_pin = 10;
constexpr uint8_t KB_row5_pin = 11;
constexpr uint8_t KB_row4_pin = 12;
constexpr uint8_t KB_row3_pin = 13;
constexpr uint8_t KB_row2_pin = 14;
constexpr uint8_t KB_row1_pin = 15;
// Using SPI: pins 16, 17, 18, 19
constexpr uint8_t SPI_SS_pin  = 16;
#define ISR_PCINT_PIN_SS ISR_PCINT_PIN16

// Alimentación y AREF: 20, 21, 22
constexpr uint8_t KB_col5_pin = 23;
constexpr uint8_t KB_col4_pin = 24;
constexpr uint8_t KB_col3_pin = 25;
constexpr uint8_t KB_col2_pin = 26;
// Not using TWI: pins 27 and 28
constexpr uint8_t KB_col1_pin = 27;
constexpr uint8_t KB_col0_pin = 28;



// Hardware connections
// --------------------
// keyboard
// --------
using KB_rows = dev::Keypad_rows<KB_row0_pin, 
		             KB_row1_pin, 
		             KB_row2_pin, 
		             KB_row3_pin, 
		             KB_row4_pin,
		             KB_row5_pin,
		             KB_row6_pin,
		             KB_row7_pin
			     >;

using KB_cols = dev::Keypad_cols<KB_col0_pin,
                             KB_col1_pin,
                             KB_col2_pin,
                             KB_col3_pin,
                             KB_col4_pin,
                             KB_col5_pin,
                             KB_col6_pin,
                             KB_col7_pin
			     >;

using Keypad = dev::Keypad<KB_rows, KB_cols>;


using Code = Sci_code;

// Es el teclado inicial de la calculadora básica
constexpr uint8_t key_code[64] =
{
Code::shift, Code::alpha,'?','?','?',Code::inverse,Code::mode,'?',
'?','?',Code::frac,Code::sqrt,Code::square,Code::pow,Code::log,Code::ln,
'?','?',Code::minus,Code::sexa,Code::hyp,Code::sin,Code::cos,Code::tan,
'?','?',Code::rcl,Code::eng,'(',')',Code::s2d,Code::m_plus,
'?','?','?',			'7','8','9',Code::del,Code::ac, 
'?',Code::up,'?',		'4','5','6','x','/',
Code::left,'?',Code::right,	'1','2','3','+','-',
'?', Code::down, '?',		'0', '.', Code::ten_x, Code::ans, '='
};


using Keyboard = dev::Keyboard_keypad<Keypad, Code>;


#endif

