// Copyright (C) 2023 Manuel Perez 
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

#ifndef __PRJ_DEV_H__
#define __PRJ_DEV_H__

#include <avr_atmega.h>
#include <dev_clocks.h>
#include <dev_push_button.h>

// MICROCONTROLLER
// ---------------
namespace mcu = atmega;
using Micro   = mcu::Micro;


// PIN
// ---
// reset: pin 1
// UART: pins 2 and 3

// available 4-6

// VCC and GND: 7, 8

// available: 9-13
static constexpr uint8_t button_pin = 14;
#define ISR_BUTTON_PIN ISR_PCINT_PIN14

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 3-26

// Not using TWI: available pins 27 and 28



// DEVICES
// -------

// Clock
using Time_counter = mcu::Time_counter2_32kHz_g<3000>;
using Clock	   = dev::Clock_s<Micro, Time_counter>;
#define ISR_CLOCK ISR_TIMER2_COMPA

//// Button
using Button = dev::Push_button<Micro, button_pin>;
#define ISR_BUTTON ISR_BUTTON_PIN 


#endif


