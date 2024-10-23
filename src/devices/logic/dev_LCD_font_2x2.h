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

#ifndef __DEV_LCD_FONT_2X2_H__
#define __DEV_LCD_FONT_2X2_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Big digits de 2 x 2
 *  
 *  TODO: llevarlo al directorio `rom`. 
 *	Generalizarlo como `rom_lcdnombre_de_letra_2x2.h`
 *	Meterlo en la ROM
 *
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    13/02/2022 Big_digit_2x2_t1: diseño mio.
 *
 ****************************************************************************/
#include "dev_LCD_big_digits.h" 

#include "not_generic.h"

namespace dev{

/***************************************************************************
 *			    FONT T1 2x2
 ***************************************************************************/
namespace big_digits{
constexpr uint8_t bricks2x2_t1_size = 8;
constexpr const uint8_t _bricks2x2_t1_size[8][bricks2x2_t1_size] PROGMEM = {
    { 0b00000011,
      0b00000011,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000},


    { 0b00000001,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000011,
      0b00000011},

    { 0b00011111,
      0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000},


    { 0b00011111,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00011111,
      0b00011111},

    { 0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011},


    { 0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00011111,
      0b00011111},


    { 0b00011111,
      0b00011111,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011},


    { 0b00011111,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00011111,
      0b00011111},

};


static inline not_generic::Progmem_biarray_view<uint8_t, 8, big_digits::bricks2x2_t1_size> 
				    bricks2x2_t1{big_digits::_bricks2x2_t1_size};

// Cada digit está construido de una serie de bricks. Aquí indicamos qué
// bricks necesita cada digit.
constexpr const uint8_t digits2x2_t1[10][4] /* PROGMEM */ = {
    // 0
    { 4, 6,
      4, 5},

    // 1
    { char_space, 4,
      char_space, 4},

    // 2
    { 0, 6,
      4, 3},

    // 3
    { 0, 6,
      1, 7},

    // 4
    { 4, 4,
      char_space, 6 },

    // 5
    { 4, 2,
      1, 7 }, 

    // 6
    { 4, 2, 
      4, 7},

    // 7
    { 0, 6,
      char_space, 4},

    // 8
    { 4, 6, 
      4, 7 },

    // 9
    { 4, 6,
      1, 7 }
};

} // namespace big_digits

/***************************************************************************
 *		     INTERFAZ DE ACCESO A LAS FONTS
 ***************************************************************************/
struct _Font_digit_2x2_t1{
    static constexpr uint8_t rows = 2;
    static constexpr uint8_t cols = 2;
    static constexpr uint8_t nbricks = big_digits::bricks2x2_t1_size;

    static auto brick(uint8_t i) { return big_digits::bricks2x2_t1[i];}
    static auto digit(uint8_t i, uint8_t j)
    {return big_digits::digits2x2_t1[i][j];}

    // is_digit(i,j).char_full()?
    static bool is_digit_char_full(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_full;}

    // is_digit(i,j).char_space()?
    static bool is_digit_char_space(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_space;}
};


// Alias
using Font_digit_2x2_t1= Big_digit<_Font_digit_2x2_t1>;

}// namespace

#endif
