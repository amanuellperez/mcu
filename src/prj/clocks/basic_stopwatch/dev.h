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

#ifndef __DEV_H__
#define __DEV_H__

#include <stdint.h>
#include <avr_timer1_generic.h>

#include <dev_LCD_HD44780.h>
#include <dev_LCD_font_2x2.h>
#include <dev_LCD_font_2x3.h>
#include <dev_LCD_font_4x3.h>
#include <dev_keyrow.h>
#include "stopwatch.h"

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

// available: 23
using Keyrow_pins = dev::Keyrow_pins<24, 25>;

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
using Screen	       = Screen_1602;
// Choose font to use:
using Font             = dev::Font_digit_2x2_t1; 
//using Font             = dev::Font_digit_2x3_t1; 
//using Font             = dev::Font_digit_4x3_t1;  //<-- esta fallando. 
//							hay que depurar esta
//							fuente


// keyrow
// ------
static constexpr uint8_t NO_KEY    = 0;
static constexpr uint8_t STOP_KEY  = 1;
static constexpr uint8_t START_KEY = 2;
using Keyrow_codes = dev::Keyrow_codes<START_KEY, STOP_KEY>;
using Keyboard	   = dev::Basic_keyrow<Keyrow_pins, Keyrow_codes>;


// Stopwatch
// ---------
using Timer = avr::Generic_timer1;
constexpr static uint16_t timer_period_in_us = 64u;
using Stopwatch = my::Stopwatch<Timer, timer_period_in_us>;


#endif

