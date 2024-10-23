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

#ifndef __AVR_WATCHDOG_H__
#define __AVR_WATCHDOG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del Watchdog 
 *
 * REFERENCIAS
 *  + Datasheet
 *  + AVR132: Using the Enhanced Watchdog Timer
 *
 * HISTORIA
 *    Manuel Perez
 *    06/09/2023 Versión mínima.
 *
 ****************************************************************************/
#include <avr/io.h> // registros: DDRB... PORT...
#include <atd_bit.h>

#include "mega_interrupt.h"

namespace mega_{

class Watchdog{
public:
// TYPES
    // Table 15-2
    enum Period
    { T16_ms = 0x00,
      T32_ms = 0x01,
      T64_ms = 0x02,
      T125_ms = 0x03,
      T250_ms = 0x04,
      T500_ms = 0x05,
      T1_s   = 0x06,
      T2_s   = 0x07,
      T4_s   = 0x20,
      T8_s   = 0x21};


// MODES
// DUDA: No tengo claro la forma de llamar a las funciones cuando definimos
// los modes de funcionamiento. Por ejemplo, para configurar el Watchdog en
// interrupt mode qué nombre es mejor?:
//	    Watchdog::interrupt_mode()  <-- voy a probar con este ahora
//	    Watchdog::mode_interrupt(); <-- este estilo lo use con los Timers
//	    Watchdog::set_interrupt_mode();
//
    static void stop();

    template <uint8_t preescaler_mask>
    static void interrupt_mode();

    template <uint8_t preescaler_mask>
    static void system_reset_mode();

    template <uint8_t preescaler_mask>
    static void interrupt_and_system_reset_mode();


private:
    static void reset() {asm volatile("wdr");}

    template <uint8_t interrupt_mode_mask, 
	      uint8_t reset_mode_mask,
	      uint8_t preescaler_mask>
    static void cfg();

};


// Table 15-1
// La forma de programar el Watchdog tiene un protocolo.
// Copio el ejemplo dado en la overview 15.8.2
template <uint8_t interrupt_mode_mask, 
	  uint8_t reset_mode_mask,
	  uint8_t preescaler_mask>
inline void Watchdog::cfg()
{ 
    Disable_interrupts lock;
    reset();

    atd::write_bits<WDCE, WDE>::to<1,1>::in(WDTCSR); 

    // OJO: hay que igualarlo!!! 
    // No funciona solo modificar los bits correspondientes.
    WDTCSR = interrupt_mode_mask | reset_mode_mask | preescaler_mask;
}

// Table 15-1
// ----------
inline void Watchdog::stop()
{ 
    Disable_interrupts lock;
    reset();

    atd::write_bit<WDRF>::to<0>::in(MCUSR); 

    atd::write_bits<WDCE, WDE>::to<1,1>::in(WDTCSR); 

    WDTCSR = 0x00;
}


template <uint8_t preescaler_mask>
inline void Watchdog::interrupt_mode()
{ cfg<0x40, 0x00, preescaler_mask>(); }

template <uint8_t preescaler_mask>
inline void Watchdog::system_reset_mode()
{ cfg<0x00, 0x08, preescaler_mask>(); }

template <uint8_t preescaler_mask>
inline void Watchdog::interrupt_and_system_reset_mode()
{ cfg<0x40, 0x08, preescaler_mask>(); }






}// namespace
 

#endif



