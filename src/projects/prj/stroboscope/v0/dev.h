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

#ifndef __STROBOSCOPE_DEV_H__
#define __STROBOSCOPE_DEV_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Hardware connections
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    25/04/2021 v0.0
 *
 ****************************************************************************/
#include <stdint.h>

#include <dev_LCD_screen.h>
#include <dev_HD44780.h>
#include <dev_keyrow.h>
#include "stroboscope.h"

#include <avr_atmega.h>
namespace my_mcu = atmega;

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

constexpr uint8_t stroboscope_pin = 15; // Timer1

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

using Keyrow_pins = dev::Keyrow_pins<23, 24, 25>;

// available: 26

// Not using TWI: available pins 27 and 28



// Hardware connections
// --------------------
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<LCD_RS_pin>,
					dev::LCD_HD44780_RW<LCD_RW_pin>,
					dev::LCD_HD44780_E<LCD_E_pin>,
					dev::LCD_HD44780_D4<LCD_D4_pin, 
							    LCD_D5_pin, 
							    LCD_D6_pin, 
							    LCD_D7_pin>>;

// LCD
// ---
using LCD_1602         = dev::LCD_HD44780_1602<LCD_pins>;
using Generic_LCD_1602 = dev::Generic_LCD<LCD_1602>;
using Screen_1602      = dev::LCD_screen_1602<Generic_LCD_1602>;
using LCD              = Screen_1602;

// keyrow
using namespace dev::Key_codes; // UP_KEY, DOWN_KEY
using Keyrow_codes = dev::Keyrow_codes<OK_KEY, UP_KEY, DOWN_KEY>;
using Keyrow       = dev::Basic_keyrow<Keyrow_pins, Keyrow_codes>;


// Stroboscope
// incluido en "stroboscope.h"


#endif

