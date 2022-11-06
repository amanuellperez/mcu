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

#if F_CPU==8000000UL
#pragma GCC warning "Micro in 8MHz: remember to execute `make set_fast_fuse`"
#endif

// En dev.h metemos todas las dependencias con el hardware.
// El resto del programa NO tiene que saber qué dispositivos concretos usamos. 
// Lo ideal es usar dispositivos genéricos en el resto, de esa forma se puede
// cambiar el hardware sin tocar el software.
#include <avr_interrupt.h>
#include <avr_timer0_generic.h>
#include <avr_timer1_generic.h>
#include <avr_UART_iostream.h>

// pins usados
// ------------
// using UART: pins 2 and 3
// available: 4-6
// VCC and GND: 7, 8
// available: 9-13
constexpr uint8_t ir_transmitter_pin = 14;

constexpr uint8_t ir_receiver_pin = 15;
#define ISR_RECEIVER_PIN ISR_PCINT_PIN15

// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: available pins 27 and 28


// Hardware connections
// --------------------
// Necesito contar tiempos hasta 2300 us, por eso no vale el Timer0.
using Receiver_timer = dev::Generic_timer_counter<avr::Timer1>;
#define ISR_RECEIVER_TIMER_OVF ISR_TIMER1_OVF

// Necesitamos generar una señal de 38kHz con este timer, lo que supone un
// semiperiodo de 13 us, menor del valor máximo de 255 us de este timer.
// NOTA: se puede usar el timer1 ya que usamos interrupciones distintas.
using Transmit_timer = dev::Generic_timer_counter<avr::Timer0>;
#define ISR_TRANSMIT_TIMER_OVF ISR_TIMER0_COMPA

using IR_receiver    = avr::Input_pin_without_pullup<ir_receiver_pin>;
using IR_transmitter = avr::Output_pin<ir_transmitter_pin>;


#endif


