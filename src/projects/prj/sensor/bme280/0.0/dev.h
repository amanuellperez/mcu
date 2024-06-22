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

#ifndef __SENSOR_DEV_H__
#define __SENSOR_DEV_H__

#include <dev_HD44780.h>
#include <dev_BME280_basic.h>

#include <avr_atmega.h>
#include <dev_TWI_master.h>

// Microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;

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

// available: 23, 24, 25, 26

// using TWI: pins 27 and 28
static constexpr uint8_t TWI_buffer_size = 100; 
using TWI_master_cfg = dev::TWI_master_cfg<Micro, 
                                           my_mcu::TWI_basic,
					   TWI_buffer_size>;

using TWI = dev::TWI_master<TWI_master_cfg>;
static constexpr int TWI_frecuency = 50; // kHz
static constexpr TWI::Address twi_sensor_address = 0x76;



// Hardware connections
// --------------------
// LCD
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
using LCD_ostream      = dev::LCD_ostream_1602<Generic_LCD_1602>;
using lcd_symbol       = dev::HD44780_charset_A00;

// Sensor
using Sensor = dev::BME280_TWI<TWI, twi_sensor_address>;


#endif

