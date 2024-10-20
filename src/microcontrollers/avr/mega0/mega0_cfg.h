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

#ifndef __MEGA0_CFG_H__
#define __MEGA0_CFG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Configuración de la familia de micros mega-0 avr.	
 *
 * HISTORIA
 *    Manuel Perez
 *    18/10/2024 Empezando...
 *		 DUDAS: creo (???) que los registros de todos los micros de
 *		 una misma familia se llaman igual. Por ello basta con definir
 *		 una única configuración para todos ellos comunes (salvo en
 *		 pines, que hay que clasificarlos por número de pins).
 *
 ****************************************************************************/

#include <type_traits>

#include <avr/io.h>

namespace mega0_{
namespace cfg{

// CONFIGURACIÓN DE LOS PINES
// --------------------------
// La idea es numerar los pines de acuerdo a su posición en el chip
// y no de acuerdo a los puertos B, C y D.

// impl_of::pins
// -------------
// Cosas comunes a todos los mega0
namespace impl_of{
struct pins{
    static constexpr auto pullupen_mask = PORT_PULLUPEN_bm;
};
}// impl_of


// pins_40
// -------
// Pines del atmega4809 de 40 pins
struct pins_40 : impl_of::pins{

template <uint8_t n>
static constexpr 
bool is_a_valid_pin(){
    return 
    (n == 1) or (n == 2) or (n == 3) or (n == 4)
    or (n == 7) or (n == 8) or (n ==  9) 
    or (n == 10) or (n == 11) or (n == 12) or (n == 13) or (n == 14) or (n == 15) 
    or (n == 16) or (n == 19) 
    or (n == 20) or (n == 21) or (n == 22) or (n == 23) or (n == 24) or (n == 25) 
    or (n == 26) or (n == 27) or (n == 28) or (n == 29) 
    or (n == 33) or (n == 34) or (n == 35) 
    or (n == 36) or (n == 37) or (n == 38) or (n == 39) or (n == 40);
}

template <uint8_t n>
static constexpr 
auto PORT()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return &PORTC;
    if constexpr (n == 2) return &PORTC;
    if constexpr (n == 3) return &PORTC;
    if constexpr (n == 4) return &PORTC;
    if constexpr (n == 7) return &PORTC;
    if constexpr (n == 8) return &PORTC;

    if constexpr (n ==  9) return &PORTD;
    if constexpr (n == 10) return &PORTD;
    if constexpr (n == 11) return &PORTD;
    if constexpr (n == 12) return &PORTD;
    if constexpr (n == 13) return &PORTD;
    if constexpr (n == 14) return &PORTD;
    if constexpr (n == 15) return &PORTD;
    if constexpr (n == 16) return &PORTD;

    if constexpr (n == 19) return &PORTE;
    if constexpr (n == 20) return &PORTE;
    if constexpr (n == 21) return &PORTE;
    if constexpr (n == 22) return &PORTE;

    if constexpr (n == 23) return &PORTF;
    if constexpr (n == 24) return &PORTF;
    if constexpr (n == 25) return &PORTF;
    if constexpr (n == 26) return &PORTF;
    if constexpr (n == 27) return &PORTF;
    if constexpr (n == 28) return &PORTF;
    if constexpr (n == 29) return &PORTF;

    if constexpr (n == 33) return &PORTA;
    if constexpr (n == 34) return &PORTA;
    if constexpr (n == 35) return &PORTA;
    if constexpr (n == 36) return &PORTA;
    if constexpr (n == 37) return &PORTA;
    if constexpr (n == 38) return &PORTA;
    if constexpr (n == 39) return &PORTA;
    if constexpr (n == 40) return &PORTA;
}


template <uint8_t n>
static constexpr 
auto bitmask()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return PIN0_bm;
    if constexpr (n == 2) return PIN1_bm;
    if constexpr (n == 3) return PIN2_bm;
    if constexpr (n == 4) return PIN3_bm;
    if constexpr (n == 7) return PIN4_bm;
    if constexpr (n == 8) return PIN5_bm;

    if constexpr (n ==  9) return PIN0_bm;
    if constexpr (n == 10) return PIN1_bm;
    if constexpr (n == 11) return PIN2_bm;
    if constexpr (n == 12) return PIN3_bm;
    if constexpr (n == 13) return PIN4_bm;
    if constexpr (n == 14) return PIN5_bm;
    if constexpr (n == 15) return PIN6_bm;
    if constexpr (n == 16) return PIN7_bm;

    if constexpr (n == 19) return PIN0_bm;
    if constexpr (n == 20) return PIN1_bm;
    if constexpr (n == 21) return PIN2_bm;
    if constexpr (n == 22) return PIN3_bm;

    if constexpr (n == 23) return PIN0_bm;
    if constexpr (n == 24) return PIN1_bm;
    if constexpr (n == 25) return PIN2_bm;
    if constexpr (n == 26) return PIN3_bm;
    if constexpr (n == 27) return PIN4_bm;
    if constexpr (n == 28) return PIN5_bm;
    if constexpr (n == 29) return PIN6_bm;

    if constexpr (n == 33) return PIN0_bm;
    if constexpr (n == 34) return PIN1_bm;
    if constexpr (n == 35) return PIN2_bm;
    if constexpr (n == 36) return PIN3_bm;
    if constexpr (n == 37) return PIN4_bm;
    if constexpr (n == 38) return PIN5_bm;
    if constexpr (n == 39) return PIN6_bm;
    if constexpr (n == 40) return PIN7_bm;
}

};

}// namespace cfg
 
}// namespace mega0_
 

#endif


