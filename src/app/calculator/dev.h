// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *    16/07/2021 v0.0
 *
 ****************************************************************************/
#include <stdint.h>

#include <dev_LCD_HD44780.h>
#include "keyboard_5x5.h"


// pins usados
// ------------
// UART for debugging purpose: pins 2 and 3
constexpr uint8_t LCD_RS_pin = 4;
constexpr uint8_t LCD_RW_pin = 5;
constexpr uint8_t LCD_E_pin  = 6;
// VCC and GND: 7, 8
// available: 9 y 10
constexpr uint8_t LCD_D4_pin = 11;
constexpr uint8_t LCD_D5_pin = 12;
constexpr uint8_t LCD_D6_pin = 13;
constexpr uint8_t LCD_D7_pin = 14;

// los coloco al revés porque coinciden enfrente con el keyboard
// Not using SPI: pins 16, 17, 18, 19
constexpr uint8_t KB_col4_pin = 15;
constexpr uint8_t KB_col3_pin = 16;
constexpr uint8_t KB_col2_pin = 17;
constexpr uint8_t KB_col1_pin = 18;
constexpr uint8_t KB_col0_pin = 19;

// Alimentación y AREF: 20, 21, 22

// Not using TWI: pins 27 and 28
constexpr uint8_t KB_row4_pin = 24;
constexpr uint8_t KB_row3_pin = 25;
constexpr uint8_t KB_row2_pin = 26;
constexpr uint8_t KB_row1_pin = 27;
constexpr uint8_t KB_row0_pin = 28;



// Hardware connections
// --------------------
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<LCD_RS_pin>,
					dev::LCD_HD44780_RW<LCD_RW_pin>,
					dev::LCD_HD44780_E<LCD_E_pin>,
					dev::LCD_HD44780_D4<LCD_D4_pin, 
							    LCD_D5_pin, 
							    LCD_D6_pin, 
							    LCD_D7_pin>>;

using LCD = dev::LCD_ostream_1602<dev::LCD_HD44780<LCD_pins>>;


// keyboard
using KB_rows = Keyboard_5_rows<KB_row0_pin, 
		             KB_row1_pin, 
		             KB_row2_pin, 
		             KB_row3_pin, 
		             KB_row4_pin>;

using KB_cols = Keyboard_5_cols<KB_col0_pin,
                             KB_col1_pin,
                             KB_col2_pin,
                             KB_col3_pin,
                             KB_col4_pin>;

using Keyboard = Keyboard_5x5<KB_rows, KB_cols>;

#endif

