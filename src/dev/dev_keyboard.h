// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __DEV_KEYBOARD_H__
#define __DEV_KEYBOARD_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Teclado vulgar y corriente.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    07/01/2020 v0.0 (segundo intento: introduzco codificación de caracteres)
 *
 ****************************************************************************/
#include <dev_push_button.h>

namespace dev{

template <uint8_t pin_button1, uint8_t char1,
	  uint8_t pin_button2, uint8_t char2,
	  uint8_t pin_button3, uint8_t char3>
struct Keyboard3{

    dev::Push_button<pin_button1> b1;
    dev::Push_button<pin_button2> b2;
    dev::Push_button<pin_button3> b3;

    static constexpr uint8_t c1 = char1;
    static constexpr uint8_t c2 = char2;
    static constexpr uint8_t c3 = char3;


    template <uint8_t c>
    static constexpr auto code2button(){
	if constexpr (c == c1)
	    return Push_button<pin_button1>{};

	else if constexpr (c == c2)
	    return Push_button<pin_button2>{};

	else if constexpr (c == c3)
	    return Push_button<pin_button3>{};

    }


    template <uint8_t c>
    static constexpr auto key(){
	return code2button<c>();
    }
};


}// namespace

#endif
