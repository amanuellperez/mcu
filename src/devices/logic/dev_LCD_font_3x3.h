// Copyright (C) 2022 Manuel Perez 
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

#ifndef __DEV_LCD_FONT_3X3_H__
#define __DEV_LCD_FONT_3X3_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Fuente tipo 1 para el LCD.
 *
 *    Quedaría mejor llamar a la fuente: Courier_3x3, o Times_New_Roman_3x3, 
 *    en lugar de Big_digit_3x3_t1. Pero es un
 *    tipo de letra que he visto en internet y no se de dónde la habrán sacado.
 *    Si no, sería interesante ponerle el nombre del que la diseñó.
 *    ¿Alguien sabe su nombre? ¿Para bautizarla así en lugar de 't1'?
 *  
 *  - HISTORIA:
 *    Manuel Perez
 *    09/02/2022 Big_digit_3x3_t1: el formato lo baso en uno visto en internet.
 *
 ****************************************************************************/
#include "dev_LCD_big_digits.h" 

#include <avr_memory.h>
#include "not_generic.h"

namespace dev{

namespace big_digits{

/***************************************************************************
 *			    FONT T1 3x3
 ***************************************************************************/
// Para el display de 4 filas (cada digit es de 3 x 3)
constexpr uint8_t bricks3x3_t1_size = 8;
constexpr const uint8_t _bricks3x3_t1[8][bricks3x3_t1_size] PROGMEM = {
    { 0b00000011,
      0b00000111,
      0b00001111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },

    { 0b00011000,
      0b00011100,
      0b00011110,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },
    
    { 0b00011111,
      0b00001111,
      0b00000111,
      0b00000011,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },

    { 0b00011111,
      0b00011110,
      0b00011100,
      0b00011000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },

    { 0b00000011,
      0b00000111,
      0b00001111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111 },

    { 0b00011000,
      0b00011100,
      0b00011110,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111 },

    { 0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },

    { 0b00001110,  // decimal point
      0b00001110,
      0b00001110,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },
};

// DUDA: el definir static esta variable hace que al hacer un avr-size
// se generen 2 bytes en memoria no inicializada (sección .bss) ¿por qué?
static inline not_generic::Progmem_biarray_view<uint8_t, 8, big_digits::bricks3x3_t1_size> 
				    bricks3x3_t1{big_digits::_bricks3x3_t1};


// DUDA: ¿merece la pena meter esto en PROGMEM? 
// Mientras que los brick solo se usan una vez, cuando se cargan en el LCD,
// los digits se van a estar leyendo continuamente, cada vez que se quiera
// dibujar un digit. 
// Cada digit está construido de una serie de bricks. Aquí indicamos qué
// bricks necesita cada digit.
constexpr const uint8_t digits3x3_t1[10][9] /* PROGMEM */ = {
    // 0
    { 4, 6, 5,	
      char_full, char_space, char_full, 
      2, 6, 3 },

    // 1
    { 0, char_full, char_space,
      char_space, char_full, char_space,
      6, 6, 6 },

    // 2
    { 0, 6, 5,
      4, 6, 3, 
      6, 6, 6},

    // 3
    { 6, 6, 5,
      char_space, 6, char_full,
      6 , 6, 3},

    // 4
    { char_full, char_space, char_full,
      2, 6, char_full, 
      char_space, char_space, 6},

    // 5
    { char_full, 6, 6,
      2, 6, 5,
      2, 6, 3 },

    // 6
    { 4, 6, 1, 
      char_full, 6, 5,
      2, 6, 3 },

    // 7
    { 6, 6, char_full,
      char_space, 4, 3,
      char_space, 6, char_space},

    // 8
    { 4, 6, 5,
      char_full, 6, char_full,
      2, 6, 3},

    // 9
    { 4, 6, 5,
      2, 6, char_full,
      2, 6, 3}
};


} // namespace big_digits


/***************************************************************************
 *		     INTERFAZ DE ACCESO A LAS FONTS
 ***************************************************************************/
// (RRR) Esta estructura tiene dos finalidades:
//       1. Define el tipo de fuente. La clase Big_digit queda parametrizada
//          por la fuente. (esta es la razón principal por la que inicialmente
//          la definí).
//       2. Encapsular cómo se almacenan los bricks y los digits. ¿en RAM? ¿en
//          PROGMEM?
struct _Font_digit_3x3_t1{
    static constexpr uint8_t rows = 3;
    static constexpr uint8_t cols = 3;
    static constexpr uint8_t nbricks = big_digits::bricks3x3_t1_size;

    static auto brick(uint8_t i) { return big_digits::bricks3x3_t1[i];}
    static auto digit(uint8_t i, uint8_t j)
    {return big_digits::digits3x3_t1[i][j];}

    // is_digit(i,j).char_full()?
    static bool is_digit_char_full(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_full;}

    // is_digit(i,j).char_space()?
    static bool is_digit_char_space(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_space;}
};


// Alias
using Font_digit_3x3_t1 = Big_digit<_Font_digit_3x3_t1>;

} //namespace
#endif
