// Copyright (C) 2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __KEYCODES_H__
#define __KEYCODES_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Extensión del código ASCII que voy a usar en la
 *  calculadora.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    13/12/2021 v0.0
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t 
#include <ostream>

// No heredo de Keyboard_code_kascii ya que tengo muchas teclas especiales.
// Creo un teclado especial para la calculadora
struct Sci_code
{
    static constexpr uint8_t null   = 0;

    static constexpr uint8_t ac     = 128;
    static constexpr uint8_t ans    = 129;
    static constexpr uint8_t del    = 130;
    static constexpr uint8_t up     = 131;
    static constexpr uint8_t down   = 132;
    static constexpr uint8_t right  = 133;
    static constexpr uint8_t left   = 134;
    static constexpr uint8_t ten_x  = 135; // 10^x
    static constexpr uint8_t m_plus = 136; // M+
    static constexpr uint8_t s2d    = 137; // S <=> D
    static constexpr uint8_t eng    = 138;
    static constexpr uint8_t rcl    = 139;
    static constexpr uint8_t sin    = 140;
    static constexpr uint8_t cos    = 141;
    static constexpr uint8_t tan    = 142;
    static constexpr uint8_t hyp    = 143;
    static constexpr uint8_t sexa   = 144; // º ' ''
    static constexpr uint8_t minus  = 145; //(-)
    static constexpr uint8_t ln     = 146;
    static constexpr uint8_t log    = 147;
    static constexpr uint8_t pow    = 148; // x^
    static constexpr uint8_t square = 149; // x^2
    static constexpr uint8_t root   = 150;
    static constexpr uint8_t frac   = 151; // a/b 
    static constexpr uint8_t inverse= 152; // x^{-1}
    static constexpr uint8_t calc   = 153;
    static constexpr uint8_t shift  = 154;
    static constexpr uint8_t alpha  = 155;
    static constexpr uint8_t mode   = 156;
    static constexpr uint8_t sqrt   = 157;
    
    static constexpr uint8_t unknown = 255;

    // imprime como cadena el código correspondiente
    static void print(std::ostream& out, uint8_t code);
};


#endif


