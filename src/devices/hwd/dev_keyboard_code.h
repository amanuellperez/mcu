// Copyright (C) 2021 Manuel Perez 
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

#ifndef __DEV_KEYBOARD_CODE_H__
#define __DEV_KEYBOARD_CODE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Códigos ASCII extendido. Incluye right/left arrow,
 *    cancel...
 *  
 *    No he encontrado ningún standard que defina los códigos usados
 *    right/left arrow y demás. Así que defino mi propio código. 
 *
 *    En principio, como es para teclados pequeños (botones
 *    up/down/right/left/ok) o como mucho para una calculadora no necesito
 *    incluir letras acentuadas y demás. Voy a hacer una extensión propia de
 *    ASCII, llamada KASCII (keyboard-ascii). De esta forma, si más adelante
 *    necesito otra codificación o hay una standard bastará con cambiar el
 *    nombre del código usado en el programa.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    01/08/2021 v0.0
 *
 ****************************************************************************/

namespace dev{

struct Keyboard_code_kascii{
// standard ASCII
    static constexpr uint8_t null  = 0;
    static constexpr uint8_t enter = '\n';
    static constexpr uint8_t cancel = 24;
    static constexpr uint8_t escape = 26;
    static constexpr uint8_t del    = 127;


// extensión propia
    static constexpr uint8_t up    = 200;
    static constexpr uint8_t down  = 201;
    static constexpr uint8_t right = 202;
    static constexpr uint8_t left  = 203;

// Caracteres a ser usados por la aplicación a su gusto
// Ver test como ejemplo.
    static constexpr uint8_t give_a_name1 = 251;
    static constexpr uint8_t give_a_name2 = 252;
    static constexpr uint8_t give_a_name3 = 253;
    static constexpr uint8_t give_a_name4 = 254;
    static constexpr uint8_t give_a_name5 = 255;
};


}// namespace

#endif


