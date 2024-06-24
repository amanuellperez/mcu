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

struct Pin {
    using number_type = uint8_t;

// Precondicion: un microcontrolador no tiene más de 252 pins
// (si hubiera más basta con cambiar el tipo)
    static constexpr number_type connected_to_VCC = 253;
    static constexpr number_type connected_to_GND = 254;
    static constexpr number_type floating         = 255;
};


}// namespace


#endif


