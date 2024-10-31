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

#ifndef __MEGA0_MICRO_H__
#define __MEGA0_MICRO_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	mega0::Micro	
 *
 * HISTORIA
 *    Manuel Perez
 *    27/10/2024 Escrito
 *
 ****************************************************************************/
#include <avr_micro.h>

// #include "mega_sleep.h" TODO
#include "mega0_pin.h"

namespace mega0_{

/***************************************************************************
 *			    MICRO
 ***************************************************************************/
template <typename Cfg_pins>
struct Micro : avr_::protected_::Micro {
    // interfaz static
    Micro() = delete;

// Pins
// ----
    template <uint8_t n>
    using Pin = mega0_::Pin<n, Cfg_pins>;


// Sleep
// -----
//    // Definimos el modo del sleep
//    static void sleep_mode(Sleep::mode mode) {mega_::sleep_mode(mode);}
//
//    // Dormimos. 
//    // Precondicion: haber definido el modo con `sleep_mode`.
//    static void sleep() {mega_::sleep();}
//
//    // Equivalente a:
//    //	    sleep_mode(mode);
//    //	    sleep();
//    static void sleep(Sleep::mode mode) {mega_::sleep(mode);}

};// Micro


} // namespace

#endif


