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

#ifndef __CHRONO_DEV_H__
#define __CHRONO_DEV_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Hardware connections
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    25/01/2021 v0.0
 *
 ****************************************************************************/
#include <stdint.h>
#include <avr_timer1_basic.h>

#include <dev_LCD_HD44780.h>
#include <dev_keyboard.h>
#include <dev_system_clock.h>

#include "chronometer.h"

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

// available: 15

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

using Keyboard_pins = dev::Keyboard_pins<23, 24, 25>;

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

using LCD_ostream = dev::LCD_ostream_1602<dev::LCD_HD44780<LCD_pins>>;


// keyboard
using namespace dev::Key_codes; // OK_KEY, UP_KEY, DOWN_KEY
using Keyboard_codes = dev::Keyboard_codes<OK_KEY, UP_KEY, DOWN_KEY>;
using Keyboard       = dev::Basic_keyboard<Keyboard_pins, Keyboard_codes>;

constexpr static uint16_t chronometer_timer_period_in_us = 64u;
using Chronometer                                        = dev::
    Chronometer_ms<avr::Timer1_CTC_mode, chronometer_timer_period_in_us, false>;

#endif

