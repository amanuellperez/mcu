// Copyright (C) 2022 Manuel Perez 
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
#include <mcu_miniclock.h>
#include <dev_square_wave.h>

// microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro = my_mcu::Micro;


// pin connections
// ---------------
// using UART: pins 2 and 3
// available: 4-6
// VCC and GND: 7, 8
// available: 9-11
constexpr uint8_t speaker_pin = 12;

// available: 13-15

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: available pins 27 and 28


// Devices
// -------
// Uso: Timer0 como SWG0_g
//	Timer1 como Miniclock
using Miniclock1_ms = mcu::Miniclock_ms<my_mcu::Micro, my_mcu::Time_counter1>;

using SWG0_ms = dev::Square_wave_generator< my_mcu::Square_wave_generator0_g
					  , my_mcu::Output_pin<speaker_pin>
					  , Miniclock1_ms>;


// UART
// ----
// Interfaz static para leer/escribir en UART
// Como vamos a llamar a funciones del traductor `UART_basic`
// hay que implementarlo como una clase. Si no habría valido con un
// using UART = my_mcu::UART_iostream;
class UART : public my_mcu::UART_iostream {
public:
    static void init();

    static bool are_there_unread_data()
    { return my_mcu::UART_basic::are_there_unread_data();}

    static void enable_interrupt_unread_data()
    {my_mcu::UART_basic::enable_interrupt_unread_data();}

    static void disable_interrupt_unread_data()
    {my_mcu::UART_basic::disable_interrupt_unread_data();}

};

#endif


