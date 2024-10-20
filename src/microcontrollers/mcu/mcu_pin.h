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

#ifndef __MCU_PIN_H__
#define __MCU_PIN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Cosas genéricas sobre pines de microcontroladores.	
 *
 * HISTORIA
 *    Manuel Perez
 *    24/06/2024 connected_to_VCC, connected_to_GND, floating
 *
 ****************************************************************************/

#include <cstdint>

namespace mcu{

// Precondicion: un microcontrolador no tiene más de 252 pins
// (si hubiera más basta con cambiar el tipo)
struct Pin_connection_type{
    static constexpr uint8_t to_VCC	= 253;
    static constexpr uint8_t to_GND	= 254;
    static constexpr uint8_t floating   = 255;
};


// Indica cómo se conecta un pin de un chip (NO del micro) 
template <uint8_t n>
struct Pin_connection {
    static constexpr uint8_t number = n;

    // No es un pin del micro
    static constexpr bool is_a_valid_pin()
    { return false; }

    // pero si es una conexión
    static constexpr bool is_a_valid_connection()
    { return n == Pin_connection_type::to_VCC or 
	     n == Pin_connection_type::to_GND or
	     n == Pin_connection_type::floating; }
};


// Esto opera como función, no como struct, por eso es `pin` y no `Pin`
// Idioma:
//	    if (pin<MS1>::is_connected_to_VCC()) ...
template <typename Pin>
    requires requires {Pin::number;}
struct pin
{
    static constexpr bool is_floating() 
    { return Pin::number == Pin_connection_type::floating;}

    static constexpr bool is_connected_to_VCC() 
    { return Pin::number == Pin_connection_type::to_VCC;}

    static constexpr bool is_connected_to_GND() 
    { return Pin::number == Pin_connection_type::to_GND;}
};


// Pin
// ---
// Esta clase está pensada para definir el Pin del micro de la siguiente
// forma:
//	    template <uint8_t n, typename Cfg>
//	    using Pin = mcu::Pin<private_::Pin<n, Cfg>>::type;
//
// Ver por ejemplo, el atmega4809 pin.
template <typename Pin_t,
	  bool is_pin = Pin_t::is_a_valid_pin(), 
	  bool pin_connection = (!Pin_t::is_a_valid_pin() and 
		    mcu::Pin_connection<Pin_t::number>::is_a_valid_connection())>
struct Pin;

template <typename Pin_t>
struct Pin<Pin_t, true, false>
{ using type = Pin_t; };

template <typename Pin_t>
struct Pin <Pin_t, false, true>
{ using type = mcu::Pin_connection<Pin_t::number>; };

template <typename Pin_t>
struct Pin <Pin_t, false, false>
{ static_assert(false, "Wrong pin number"); };

 

}// namespace


#endif


