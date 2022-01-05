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
 *	Fundamental: para simplificar el código es fundamental que
 *	las abreviaturas estén ordenadas. De esa forma se puede traducir la
 *	key en str:
 *	    if (first_abb <= key <= last_abb)
 *		write(abb2str[key - first_abb]);
 *
 *	Además permite generalizar el código ya que de esta forma el programa
 *	no tiene ni idea de las abreviaturas que está usando (otra forma de
 *	implementarlo sería como con los comandos, con un switch, en cuyo caso
 *	el programa tendría que saber qué abreviaturas usa la calculadora).
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
// Caracteres especiales
    static constexpr uint8_t null    = 0;
    static constexpr uint8_t unknown = 255;

// Caracteres: corresponden a los códigos ASCII

// Command edition
    static constexpr uint8_t first_cmd = 128;
    static constexpr uint8_t ac        = 128;
    static constexpr uint8_t del       = 129;
    static constexpr uint8_t up        = 130;
    static constexpr uint8_t down      = 131;
    static constexpr uint8_t right     = 132;
    static constexpr uint8_t left      = 133;
    static constexpr uint8_t last_cmd  = 134;

// Abbrevations
    static constexpr uint8_t first_abb = 151;
    static constexpr uint8_t ans    = 151;
    static constexpr uint8_t ten_x  = 152; // 10^x
    static constexpr uint8_t sin    = 153;
    static constexpr uint8_t cos    = 154;
    static constexpr uint8_t tan    = 155;
    static constexpr uint8_t sexa   = 156; // º ' ''
    static constexpr uint8_t minus  = 157; //(-)
    static constexpr uint8_t ln     = 158;
    static constexpr uint8_t log    = 159;
    static constexpr uint8_t pow    = 160; // x^
    static constexpr uint8_t last_abb = 160;
 

// Symbols
    static constexpr uint8_t first_symbol = 180;
    static constexpr uint8_t sqrt         = 180;
    static constexpr uint8_t square       = 181; // x^2
    static constexpr uint8_t frac         = 182; // a/b
    static constexpr uint8_t inverse      = 183; // x^{-1}
    static constexpr uint8_t last_symbol  = 183;

// Instrucciones a la calculadora
    static constexpr uint8_t mode   = 200;
    static constexpr uint8_t m_plus = 201; // M+
    static constexpr uint8_t s2d    = 202; // S <=> D
    static constexpr uint8_t eng    = 203;
    static constexpr uint8_t rcl    = 204;
    static constexpr uint8_t calc   = 205;
    static constexpr uint8_t hyp    = 206;

// Teclas de cambio de significado:
    static constexpr uint8_t shift  = 220;
    static constexpr uint8_t alpha  = 221;

    // imprime como cadena el código correspondiente
    // (para depurar)
    static void print(std::ostream& out, uint8_t code);
};

inline constexpr const char* abb2str[] = {
    "Ans",
    "x10",
    "sin(",
    "cos(",
    "tan(",
    "º",
    "-",
    "ln(",
    "log(",
    "^",
};

#endif


