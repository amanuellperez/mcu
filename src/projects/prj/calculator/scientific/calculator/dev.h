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

#ifndef __DEV_SCIENTIFIC_H__
#define __DEV_SCIENTIFIC_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Hardware connections
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    05/08/2021 v0.0
 *
 ****************************************************************************/
#include <stdint.h>

#include "lcd.h"

#include "../../interface.h"
#include "spi_keyboard.h"

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
// available: 15

// Using SPI: pins 16, 17, 18, 19
constexpr uint8_t SPI_SS_pin  = 16;
// La frecuencia del master tiene que ser 4 veces más lenta que la del slave.
// El slave lo tenemos a 1MHz, luego el periodo máximo debe ser 4 us.
// De hecho, al principio probe con 2 us y no funcionaba bien.
// Con 4 funciona bien, garanticemos 8 us.
constexpr uint16_t period_in_us = 8;	

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: pins 27 and 28 available


// SPI selection
// -------------
// ¿Cómo gestionamos la selección de un dispositivo SPI?
// Como ahora solo tengo 1 dispositivo es sencillo, pero si tuviera más habria
// que pasar SPI_select<num_dispositivo>. 
// Dos formas de hacer la selección: implicitamente (en
// constructor/destructor) o explícitamente. De momento pruebo con la
// implícita.
struct SPI_select{
    SPI_select () { no_SS.write_zero();}
    ~SPI_select () { no_SS.write_one();}

    atmega::Output_pin<SPI_SS_pin> no_SS;
};

// Hardware connections
// --------------------
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<LCD_RS_pin>,
					dev::LCD_HD44780_RW<LCD_RW_pin>,
					dev::LCD_HD44780_E<LCD_E_pin>,
					dev::LCD_HD44780_D4<LCD_D4_pin, 
							    LCD_D5_pin, 
							    LCD_D6_pin, 
							    LCD_D7_pin>>;

// Interface
// ---------
using LCD       = LCD_calculator<my_mcu::Micro, LCD_pins>;
using Keyboard  = SPI_Keyboard<SPI_select, period_in_us>;
//using Interface = calc::Interface<LCD, Keyboard, Code, 20, '=', 50>;

using Interface_cfg = calc::Interface_cfg<LCD, Keyboard, Code, 20, Abb2str, '=', 50>;
using Interface = calc::Interface<Interface_cfg>;

using Buffer    = Interface::Buffer;

#endif

