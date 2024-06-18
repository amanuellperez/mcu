// Copyright (C) 2024 Manuel Perez 
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

#ifndef __HWD_DEV_H__
#define __HWD_DEV_H__


// En dev.h metemos todas las dependencias con el hardware.
// El resto del programa NO tiene que saber qué dispositivos concretos usamos. 
// Lo ideal es usar dispositivos genéricos en el resto, de esa forma se puede
// cambiar el hardware sin tocar el software.

#include "car_2wheels.h"

#include <avr_atmega.h>
#include <dev_L298N.h>


// cfg
// ---
					     
// microcontroller
// ---------------
namespace mcu = atmega;
using Micro   = mcu::Micro;


// pin connections
// ---------------
// using UART: pins 2 and 3
// available: 4-6
// VCC and GND: 7, 8

// available: pins 9, 10

static constexpr uint8_t IN1_pin = 12;
static constexpr uint8_t IN2_pin = 11;
static constexpr uint8_t IN3_pin = 13;
static constexpr uint8_t IN4_pin = 14;

// Timer1
static constexpr uint8_t ENA_pin = 15;
static constexpr uint8_t ENB_pin = 16;

using PWM_pinA = mcu::PWM1_pin<ENA_pin>;
using PWM_pinB = mcu::PWM1_pin<ENB_pin>;

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: available pins 27 and 28

// PROTOCOLS
// ---------

// uart
// ----
// constexpr uint32_t baud_rate = 9'600;  // es el valor por defecto

// DEVICES
// -------
using L298_pinA = dev::L298N_pin_cfg<PWM_pinA, IN1_pin, IN2_pin>;
using L298_pinB = dev::L298N_pin_cfg<PWM_pinB, IN3_pin, IN4_pin>;

using L298N = dev::L298N_basic<Micro, L298_pinA, L298_pinB>;

using Car = adev::Car_2_wheels<L298N>;

#endif


