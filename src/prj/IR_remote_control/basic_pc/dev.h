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

// En dev.h metemos todas las dependencias con el hardware.
// El resto del programa NO tiene que saber qué dispositivos concretos usamos. 
// Lo ideal es usar dispositivos genéricos en el resto, de esa forma se puede
// cambiar el hardware sin tocar el software.
#include <avr_interrupt.h>
#include <avr_timer1_generic.h>
#include <avr_UART_iostream.h>

// pins usados
// ------------
// using UART: pins 2 and 3
// available: 4-6
// VCC and GND: 7, 8
// available: 9-14
constexpr uint8_t sensor_pin = 15;
#define ISR_SENSOR_PIN ISR_PCINT_PIN15

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: available pins 27 and 28


// Hardware connections
// --------------------
// Voy a llamar Timer para saber que la interrupción del overflow es
// ISR_TIMER1_OVF (<-- ¿cómo generalizar esto? El problema son las macros!!!)
using Timer = dev::Generic_timer_counter<avr::Timer1>;
#define ISR_TIMER_OVF ISR_TIMER1_OVF

using Sensor= avr::Input_pin_without_pullup<sensor_pin>;


#endif


