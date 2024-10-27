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

#ifndef __DEV_LCD_FONT_2X1_H__
#define __DEV_LCD_FONT_2X1_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Big digits de 2 x 1
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    13/02/2022 Big_digit_2x1_t1: diseño mio (aunque no hay muchas más
 *                                 posibilidades).
 *
 ****************************************************************************/
#include <atd_rom.h>


namespace dev{

/***************************************************************************
 *			    FONT T1 2x1
 ***************************************************************************/
namespace big_digits{
using ROM_read = MCU::ROM_read;

constexpr uint8_t bricks2x1_t1_size = 8;

static constexpr 
atd::ROM_biarray<uint8_t, 8, bricks2x1_t1_size, ROM_read> bricks2x1_t1 
	PROGMEM = {
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,

      0b00011111,
      0b00010000,
      0b00010000,
      0b00010000,
      0b00010000,
      0b00010000,
      0b00010000,
      0b00011111,

      0b00011111,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,


      0b00011111,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00011111,


      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00000001,
      0b00011111,


      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00011111,


      0b00011111,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00011111,


      0b00011111,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001,
      0b00010001
};



// Cada digit está construido de una serie de bricks. Aquí indicamos qué
// bricks necesita cada digit.
constexpr const uint8_t digits2x1_t1[10][2] /* PROGMEM */ = {
    // 0
    { 7, 5 },

    // 1
    { 0, 0 },

    // 2
    {2, 1 },

    // 3
    { 3, 4 },

    // 4
    {5, 0 },

    // 5
    { 1, 4 },

    // 6
    { 3, 5 },

    // 7
    { 2, 0 },

    // 8
    { 6, 5 },

    // 9
    { 6, 4}

};

} // namespace big_digits

/***************************************************************************
 *		     INTERFAZ DE ACCESO A LAS FONTS
 ***************************************************************************/
namespace impl_of{

struct Font_digit_2x1_t1{
    static constexpr uint8_t rows = 2;
    static constexpr uint8_t cols = 1;
    static constexpr uint8_t nbricks = big_digits::bricks2x1_t1_size;

    static auto brick(uint8_t i) { return big_digits::bricks2x1_t1.row(i);}
    static auto digit(uint8_t i, uint8_t j)
    {return big_digits::digits2x1_t1[i][j];}

    // is_digit(i,j).char_full()?
    static bool is_digit_char_full(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_full;}

    // is_digit(i,j).char_space()?
    static bool is_digit_char_space(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_space;}
};
}// impl_of

// Alias
using Font_digit_2x1_t1= Big_digit<impl_of::Font_digit_2x1_t1>;


/***************************************************************************
 *			    FONT T2 2x1
 ***************************************************************************/
namespace big_digits{
constexpr uint8_t bricks2x1_t2_size = 8;

static constexpr 
atd::ROM_biarray<uint8_t, 8, bricks2x1_t2_size, ROM_read> bricks2x1_t2
	PROGMEM = {
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,

      0b00011111,
      0b00011111,
      0b00011000,
      0b00011000,
      0b00011000,
      0b00011000,
      0b00011111,
      0b00011111,

      0b00011111,
      0b00011111,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,


      0b00011111,
      0b00011111,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00011111,
      0b00011111,


      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00000011,
      0b00011111,
      0b00011111,


      0b00011011,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011111,
      0b00011111,


      0b00011111,
      0b00011111,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011111,
      0b00011111,


      0b00011111,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011011,
      0b00011011
};


// Cada digit está construido de una serie de bricks. Aquí indicamos qué
// bricks necesita cada digit.
constexpr const uint8_t digits2x1_t2[10][2] /* PROGMEM */ = {
    // 0
    { 7, 5 },

    // 1
    { 0, 0 },

    // 2
    {2, 1 },

    // 3
    { 3, 4 },

    // 4
    {5, 0 },

    // 5
    { 1, 4 },

    // 6
    { 3, 5 },

    // 7
    { 2, 0 },

    // 8
    { 6, 5 },

    // 9
    { 6, 4}

};

} // namespace big_digits

/***************************************************************************
 *		     INTERFAZ DE ACCESO A LAS FONTS
 ***************************************************************************/
namespace impl_of {
struct Font_digit_2x1_t2{
    static constexpr uint8_t rows = 2;
    static constexpr uint8_t cols = 1;
    static constexpr uint8_t nbricks = big_digits::bricks2x1_t2_size;

    static auto brick(uint8_t i) { return big_digits::bricks2x1_t2.row(i);}
    static auto digit(uint8_t i, uint8_t j)
    {return big_digits::digits2x1_t2[i][j];}

    // is_digit(i,j).char_full()?
    static bool is_digit_char_full(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_full;}

    // is_digit(i,j).char_space()?
    static bool is_digit_char_space(uint8_t i, uint8_t j)
    {return digit(i, j) == big_digits::char_space;}

};
}// impl_of

// Alias
using Font_digit_2x1_t2= Big_digit<impl_of::Font_digit_2x1_t2>;

}// namespace

#endif
