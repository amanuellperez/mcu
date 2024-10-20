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

#ifndef __MEGA0_H__
#define __MEGA0_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Built-in devices que suministra cada micro de la familia mega0	
 *
 * HISTORIA
 *    Manuel Perez
 *    19/10/2024 Empezando...
 *
 ****************************************************************************/
#include <avr_private.h>   // importamos todo avr_

#include "mega0_pin.h"


/***************************************************************************
 *			    ATMEGA4809 de 40 pins
 ***************************************************************************/
namespace atmega4809_40{
    using namespace avr_;

    template <uint8_t n>
    using Pin = mega0_::Pin<n, mega0_::cfg::pins_40>;
}


#endif


