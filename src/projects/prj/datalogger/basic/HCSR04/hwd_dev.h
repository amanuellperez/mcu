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
#include <avr_atmega.h>
#include <mcu_clock.h>
#include <mcu_miniclock.h>
#include <dev_HCSR04.h>

// cfg
// ---
static constexpr uint16_t timeout_ms = 3000; // esperamos máximo 3 segundos
					     
// microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;


// pin connections
// ---------------
// using UART: pins 2 and 3
// available: 4-6
// VCC and GND: 7, 8

// crystal of 32kHz: pins 9, 10

// available: 11-14

constexpr uint8_t trigger_pin = 13;
constexpr uint8_t echo_pin    = 14;

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: available pins 27 and 28

// uart
// ----
class UART : public my_mcu::UART_iostream{
public:
// Name? Realmente es empty_get_area(), aunque el standard usa más el nombre
// de `ignore`
    void empty_read_buffer();
};
#define ISR_UART_RX ISR_USART_RX

inline void UART::empty_read_buffer()
{
    char c{};

    while(is_there_something_to_read())
	*(this) >> c;
}

// DEVICES
// -------
// Timers
// ------
// Timer0: sin usar
// Timer1: Miniclock_us	    (cualquier device lo puede usar)
using Miniclock_us = mcu::Miniclock_us<my_mcu::Micro, my_mcu::Time_counter1>;

// Timer2: Clock_s	    (propiedad de Clock_s. No lo puede usar nadie más)
using Time_counter = my_mcu::Time_counter2_32kHz_g<timeout_ms>;

// Clock
using Clock	   = mcu::Clock_s<Micro, Time_counter>;
#define ISR_CLOCK ISR_TIMER2_COMPA

// Sensor medición de distancia
using HCSR04_cfg   = dev::HCSR04_cfg<Micro, Miniclock_us,
				     trigger_pin, echo_pin>;
using Sensor       = dev::HCSR04<HCSR04_cfg>;


#endif


