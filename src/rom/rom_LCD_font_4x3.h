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

#ifndef __DEV_LCD_FONT_4X3_H__
#define __DEV_LCD_FONT_4X3_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Fuente tipo 1 para el LCD.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    15/02/2022 Big_digit_4x3_t1: diseño mío.
 *
 ****************************************************************************/
#include <atd_rom.h>

namespace dev{

namespace big_digits{

using ROM_read = MCU::ROM_read;

/***************************************************************************
 *			    FONT T1 4x3
 ***************************************************************************/
// Para el display de 4 filas (cada digit es de 3 x 3)
constexpr uint8_t bricks4x3_t1_size = 8;
static constexpr 
atd::ROM_biarray<uint8_t, 8, bricks4x3_t1_size, ROM_read> bricks4x3_t1
	PROGMEM = {
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000011,
      0b00000111,
      0b00001111,
      0b00011111 ,

      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00011000,
      0b00011100,
      0b00011110,
      0b00011111 ,
    
      0b00011111,
      0b00001111,
      0b00000111,
      0b00000011,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 ,

      0b00011111,
      0b00011110,
      0b00011100,
      0b00011000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 ,


      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00001111,
      0b00000111,
      0b00000011 ,

      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011110,
      0b00011100,
      0b00011000 ,

      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 ,

      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00011111,
      0b00011111,
      0b00011111,
      0b00011111 
};


// DUDA: ¿merece la pena meter esto en PROGMEM? 
// Mientras que los brick solo se usan una vez, cuando se cargan en el LCD,
// los digits se van a estar leyendo continuamente, cada vez que se quiera
// dibujar un digit. 
// Cada digit está construido de una serie de bricks. Aquí indicamos qué
// bricks necesita cada digit.
constexpr const uint8_t digits4x3_t1[10][12] /* PROGMEM */ = {
    // 0
    { 0, 7, 1,
      char_full, char_space, char_full,
      char_full, char_space, char_full, 
      4, 7, 5},

    // 1
    { 0, 7, char_space, 
      char_space, char_full, char_space,
      char_space, char_full, char_space,
      7, char_full, 7 },

    // 2
    { 0, 7, 1, 
      char_space, char_space, char_full,
      char_full, 6, 3,
      4, 7, 1 },

    // 3
    { 0, 7, 1, 
      char_space, char_space, char_full,
      char_space, 6, char_full,
      0, 7, 5 },

    // 4
    { 0, char_space, 1,
      char_full, char_space, char_full,
      2, 6, char_full, 
      char_space, char_space, 5 },

    // 5
    { 0, 7, 1,
      char_full, char_space, char_space,
      2, 6, char_full, 
      0, 7, 5 },

    // 6
    { 0, 7, 1, 
      char_full, char_space, char_space,
      char_full, 6, char_full,
      4, 7, 5 },

    // 7
    { 0, 7, 1,
      char_space, char_space, char_full, 
      char_space, char_space, char_full, 
      char_space, char_space, 5 },

    // 8
    { 0, 7, 1, 
      char_full, char_space, char_full,
      char_full, 6, char_full,
      4, 7, 5 },

    // 9
    { 0, 7, 1,
      char_full, char_space, char_full,
      2, 6, char_full, 
      0, 7, 5 }
};


} // namespace big_digits


/***************************************************************************
 *		     INTERFAZ DE ACCESO A LAS FONTS
 ***************************************************************************/
namespace impl_of{
struct Font_digit_4x3_t1{
    static constexpr uint8_t rows = 4;
    static constexpr uint8_t cols = 3;
    static constexpr uint8_t nbricks = big_digits::bricks4x3_t1_size;

    static auto brick(uint8_t i) { return big_digits::bricks4x3_t1.row(i);}
    static auto digit(uint8_t i, uint8_t j)
    {return big_digits::digits4x3_t1[i][j];}

    // is_digit(i,j).char_full()?
    static bool is_digit_char_full(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_full;}

    // is_digit(i,j).char_space()?
    static bool is_digit_char_space(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_space;}
};
}// impl_of

// Alias
using Font_digit_4x3_t1 = Big_digit<impl_of::Font_digit_4x3_t1>;

} //namespace
#endif
