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

#ifndef __DEV_LCD_FONT_1X3_H__
#define __DEV_LCD_FONT_2X3_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Fuentes de 2 x 3
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    15/02/2022 Big_digit_2x3_t1: el formato lo baso en uno visto en internet.
 *
 ****************************************************************************/
#include "dev_LCD_big_digits.h" 

#include <avr_memory.h>
#include "not_generic.h"

namespace dev{

namespace big_digits{

/***************************************************************************
 *			    FONT T1 2x3
 ***************************************************************************/
constexpr uint8_t bricks2x3_t1_size = 8;
constexpr const uint8_t _bricks2x3_t1[8][bricks2x3_t1_size] PROGMEM = {
    { 0b00000111,
      0b00001111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111 },

    { 0b00011100,
      0b00011110,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111 },
 
    { 0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00001111,
      0b00000111 },

    { 0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011110,
      0b00011100 },

    { 0b00011111,
      0b00011111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },


    { 0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00011111,
      0b00011111,
      0b00011111 },

    { 0b00011111,
      0b00011111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00011111,
      0b00011111 },

    { 0b00000111,
      0b00001111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 }
};

// DUDA: el definir static esta variable hace que al hacer un avr-size
// se generen 2 bytes en memoria no inicializada (sección .bss) ¿por qué?
static inline not_generic::Progmem_biarray_view<uint8_t, 8, big_digits::bricks2x3_t1_size> 
				    bricks2x3_t1{big_digits::_bricks2x3_t1};


// DUDA: ¿merece la pena meter esto en PROGMEM? 
// Mientras que los brick solo se usan una vez, cuando se cargan en el LCD,
// los digits se van a estar leyendo continuamente, cada vez que se quiera
// dibujar un digit. 
// Cada digit está construido de una serie de bricks. Aquí indicamos qué
// bricks necesita cada digit.
constexpr const uint8_t digits2x3_t1[10][6] /* PROGMEM */ = {
    // 0
    { 0, 4, 1,
      2, 5, 3 }, 

    // 1
    { 7, 1, char_space,
      5, char_full, 5 },

    // 2
    { 6, 6, 1,
      2, 5, 5 },

    // 3
    { 4, 6, 1,
      5, 5, 3 }, 

    // 4
    { 2, 5, char_full,
      char_space, char_space, char_full },

    // 5
    { 2, 6, 6,
      5, 5, 3 },

    // 6
    { 0, 6, 6,
      2, 5, 3 },

    // 7
    { 7, 4, 1,
      char_space, char_space, char_full },

    // 8
    { 0, 6, 1,
      2, 5, 3 },

    // 9
    { 0, 6, 1,
      char_space, char_space, char_full }
};


} // namespace big_digits


/***************************************************************************
 *		     INTERFAZ DE ACCESO A LAS FONTS
 ***************************************************************************/
struct _Font_digit_2x3_t1{
    static constexpr uint8_t rows = 2;
    static constexpr uint8_t cols = 3;
    static constexpr uint8_t nbricks = big_digits::bricks2x3_t1_size;

    static auto brick(uint8_t i) { return big_digits::bricks2x3_t1[i];}
    static auto digit(uint8_t i, uint8_t j)
    {return big_digits::digits2x3_t1[i][j];}

    // is_digit(i,j).char_full()?
    static bool is_digit_char_full(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_full;}

    // is_digit(i,j).char_space()?
    static bool is_digit_char_space(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_space;}
};


// Alias
using Font_digit_2x3_t1 = Big_digit<_Font_digit_2x3_t1>;

} //namespace
#endif
