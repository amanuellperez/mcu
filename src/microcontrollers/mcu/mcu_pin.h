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

// Indica cómo se conecta un pin de un chip (NO del micro) 
struct Pin_connection {
    using number_type = uint8_t;

// Precondicion: un microcontrolador no tiene más de 252 pins
// (si hubiera más basta con cambiar el tipo)
    static constexpr number_type to_VCC	    = 253;
    static constexpr number_type to_GND	    = 254;
    static constexpr number_type floating   = 255;

    template <number_type n>
    static constexpr bool is_valid()
    { return n == to_VCC or 
	     n == to_GND or
	     n == floating; }
};


// Esto opera como función, no como struct, por eso es `pin` y no `Pin`
// Idioma:
//	    if (pin<MS1>::is_connected_to_VCC()) ...
template <typename Pin>
    requires requires {Pin::number;}
struct pin
{
    static constexpr bool is_floating() 
    { return Pin::number == Pin_connection::floating;}

    static constexpr bool is_connected_to_VCC() 
    { return Pin::number == Pin_connection::to_VCC;}

    static constexpr bool is_connected_to_GND() 
    { return Pin::number == Pin_connection::to_GND;}
};

}// namespace


#endif


