// Copyright (C) 2022-2024 Manuel Perez 
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

#ifndef __MEGA_MICRO_H__
#define __MEGA_MICRO_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones globales para todos los micros de la familia mega
 *
 * HISTORIA
 *    Manuel Perez
 *    12/12/2022 Escrito
 *
 ****************************************************************************/
#include <avr_micro.h>

#include "mega_sleep_hwd.h"
#include "mega_pin_hwd.h"

namespace mega_{

/***************************************************************************
 *			    MICRO
 ***************************************************************************/
template <typename Cfg_pins>
struct Micro : avr_::protected_::Micro {
// Constructor
    Micro() = delete;

// Cfg
// ---
    static constexpr auto clock_frequency_in_hz = F_CPU;

// Pins
// ----
    template <uint8_t n>
    using Pin = hwd::Pin<n, Cfg_pins>;

// Interrupts
// ----------
    template <uint8_t n>
    using Enable_change_level_interrupt 
				    = hwd::Enable_change_level_interrupt<n>;

// Sleep
// -----
    using Sleep = hwd::Sleep;

    // Definimos el modo del sleep
    static void sleep_mode(Sleep::mode mode) {hwd::sleep_mode(mode);}

    // Dormimos. 
    // Precondicion: haber definido el modo con `sleep_mode`.
    static void sleep() {hwd::sleep();}

    // Equivalente a:
    //	    sleep_mode(mode);
    //	    sleep();
    static void sleep(Sleep::mode mode) {hwd::sleep(mode);}

};// Micro


} // namespace
#endif
