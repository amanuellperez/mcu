// Copyright (C) 2021-2022 Manuel Perez 
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

#ifndef __INSTCODES_H__
#define __INSTCODES_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Códigos de instrucción.
 *
 *    Un error de concepción es concebir los códigos ASCII como códigos de
 *    caracter. Los códigos ASCII realmente son códigos de instrucciones. 
 *    El ascii 'a' significa 'imprime la letra a'. Con esta filosofía defino
 *    los siguientes códigos de instrucción.
 *	
 *    Fundamental: para simplificar el código es fundamental que
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
 *    Manuel Perez
 *    13/12/2021 v0.0
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t 
#include <ostream>

#include <avr_atmega.h>

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
    static constexpr uint8_t frac         = 181; // a/b
    static constexpr uint8_t inverse      = 182; // x^{-1}
    static constexpr uint8_t last_symbol  = 182;

// New symbols: el LCD trae una charset por defecto, pudiendo añadir
// 8 símbolos más. Los 'new symbols' son estos símbolos más.
    static constexpr uint8_t first_new_symbol = 190;
    static constexpr uint8_t square           = 190; // x^2
    static constexpr uint8_t last_new_symbol  = 190;


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

namespace __progmem{

constexpr const char abb1[] PROGMEM = "Ans";
constexpr const char abb2[] PROGMEM = "x10";
constexpr const char abb3[] PROGMEM = "sin(";
constexpr const char abb4[] PROGMEM = "cos(";
constexpr const char abb5[] PROGMEM = "tan(";
constexpr const char abb6[] PROGMEM = "º";
constexpr const char abb7[] PROGMEM = "-";
constexpr const char abb8[] PROGMEM = "ln(";
constexpr const char abb9[] PROGMEM = "log(";
constexpr const char abb10[] PROGMEM = "^";

using Abb2str_array = 
atmega::ROM_string_array<Sci_code::last_abb - Sci_code::first_abb + 1>;

constexpr Abb2str_array abb2str PROGMEM = {
    abb1, abb2, abb3, abb4, abb5,
    abb6, abb7, abb8, abb9, abb10
};
}// namespace

// Es el casi-equivalente a `const char*` (¿`const char* const`?) pero
// almacenando la memoria en PROGMEM
struct Abb2str{
    atmega::Element_ROM_string_array<__progmem::Abb2str_array::size()> 
	    operator[](size_t i) const { return __progmem::abb2str[i];}

    // Devuelve la longitud máxima de las cadenas almacenadas
    // TODO: ¿cómo calcularla automáticamente?
    static constexpr uint8_t max_size() {return  10;}
};



//extern const char* abb2str[];
//constexpr const char* abb2str[] = {
//    "Ans",
//    "x10",
//    "sin(",
//    "cos(",
//    "tan(",
//    "º",
//    "-",
//    "ln(",
//    "log(",
//    "^",
//};

#endif


