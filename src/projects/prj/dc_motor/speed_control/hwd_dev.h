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

#include "adev_motor.h"
#include "my_miniclock.h"

#include <avr_atmega.h>
#include <dev_L298N.h>


// cfg
// ---
					     
// microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;


// pin connections
// ---------------
// using UART: pins 2 and 3
// available: 4-6
// VCC and GND: 7, 8

// available: pins 9
static constexpr uint8_t speed_sensor_pin = 10;

// TODO: ¿cómo se puede generalizar esto? Es un incordio tener que mirar que
// la interrupción del pin 10 es PCINT0_vect.
//  Quiero:
//	(1) No tener que recordar la interrupción, poder escribir
//	    #define ISR_SPEED_SENSOR_PIN ISR_PIN(speed_sensor_pin)
//	    y mejor si no se usa una macro.
//
//	(2) Si, por error, no me doy cuenta e intento definir otra
//	    ISR(PCINT0_vect) correspondiente a otro pin, quiero que el
//	    compilador me lo diga (aunque daría error al compilar
//	    hwd_interrupt.cpp)
#define ISR_SPEED_SENSOR_PIN  ISR(PCINT0_vect)

// Timer0: genera señal PWM
static constexpr uint8_t ENA_pin	  = 11; 

// El pin 12 del Timer0 no lo usamos
static constexpr uint8_t IN1_pin = 12;
static constexpr uint8_t IN2_pin = 13;
// No tengo implementado el L298N para que solo se pueda conectar un motor.
// Como esto es un experimento, solo necesito un motor. ¿Merecería la pena
// modificar el L298N para que, si no se le pasa la configuración de uno de
// los puentes H no incluya el código correspondiente? No creo, ya que si en
// una aplicación usas un L298N que tiene 2 puentes H es porque quieres usar 2
// puentes H, no uno (???)
//static constexpr uint8_t IN3_pin = mcu::Pin_connection::floating;
//static constexpr uint8_t IN4_pin = mcu::Pin_connection::floating;
static constexpr uint8_t ENB_pin = ENA_pin;
static constexpr uint8_t IN3_pin = IN1_pin;
static constexpr uint8_t IN4_pin = IN2_pin;

// available pins: 14, 15
// Not using SPI: available pins 16, 17, 18, 19

// Alimentación y AREF: 20, 21, 22

// available: 23-26
// Not using TWI: available pins 27 and 28

// MICRO devs
// ----------
// Necesitamos 2 timers: uno para generar una señal PWM y controlar la
// velocidad del motor, y otro para medir tiempos.
// El Timer0 no lo podemos usar con Miniclock_ms para medir el tiempo ya que
// solo mediría hasta 255 ms. Para no complicar el código, introduciendo un
// reloj (aunque,... tampoco es tan complicado; mejor usar un reloj?) uso el
// Timer1 para medir el tiempo en milisegundos, de esa forma podemos medir
// perfectamente hasta 1 segundo (1'000 ms), dejando el Timer0 como generador
// de PWM.
using PWM_pinA = myu::PWM0_pin<ENA_pin>;
using PWM_pinB = PWM_pinA; // = mcu::Pin_connection::floating;

using Miniclock_ms = my::Miniclock_ms<Micro, myu::Time_counter1>;

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

using Motor = adev::DC_Motor<L298N>;

using Speed_sensor_pin = Micro::Pin<speed_sensor_pin>;

#endif

