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


// En dev.h metemos todas las dependencias con el hardware.
// El resto del programa NO tiene que saber qué dispositivos concretos usamos. 
// Lo ideal es usar dispositivos genéricos en el resto, de esa forma se puede
// cambiar el hardware sin tocar el software.
#include <mega.h>
#include <mcu_one_wire.h>
#include <dev_DS18B20.h>

// microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;


// pin connections
// ---------------
// using UART: pins 2 and 3
// available: 4-6
// VCC and GND: 7, 8
// available: 9-14

constexpr uint8_t one_wire_pin = 15;

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: available pins 27 and 28

// PROTOCOLS
// =========

// One wire protocol
// -----------------
using Cfg = mcu::One_wire_cfg<myu::Micro, one_wire_pin>;
using One_wire = mcu::One_wire<Cfg>;
using Search = mcu::One_wire_search<Cfg>;


// UART
// ----
using UART = mcu::UART_iostream<myu::UART_8bits>;


// Hwd Devices
// -----------
using Watchdog = myu::Watchdog;
using Sensor   = dev::DS18B20<Micro, One_wire>;


// CFG
// ---
static constexpr uint16_t sensor_timeout_ms = 3000;  // 3 s
#endif


