// Copyright (C) 2019-2024 Manuel Perez 
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
#ifndef __ROM_FONT_BASIC_5x8_H__
#define __ROM_FONT_BASIC_5x8_H__

/****************************************************************************
 *
 * DESCRIPCION
 *	Tipo de letra básica 
 *
 * HISTORIA
 *    Manuel Perez
 *    11/08/2024 Traido de dev.
 *
 ****************************************************************************/
#include <atd_rom.h>
// #include <avr_memory.h> <-- hay que incluirlo antes de este archivo

namespace rom{


namespace font_basic_5x8{

using ROM_read = MCU::ROM_read;

// No incluyo todos los códigos ascii, sino a partir del 32:
constexpr uint8_t font_index0 = 32; 

constexpr uint8_t font_cols   = 5;  // número de columnas que tiene cada font
constexpr uint8_t font_nchars = 96; // número de caracteres: 96 códigos ASCII

// Tabla con los caracteres ASCII
// Como queremos poder hacer un scroll de izquierda a derecha (o al revés)
// en lugar de escribir las letras por filas las escribimos por columnas.
constexpr 
atd::ROM_biarray<uint8_t, font_nchars, font_cols, ROM_read> font
     PROGMEM = { 
0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,//  space 
0b00000000,0b00000000,0b11110010,0b00000000,0b00000000,//  ! 
0b00000000,0b11100000,0b00000000,0b11100000,0b00000000,//  " 
0b00101000,0b11111110,0b00101000,0b11111110,0b00101000,//  # 
0b00100100,0b01010100,0b11111110,0b01010100,0b01001000,//  $ 
0b11000100,0b11001000,0b00010000,0b00100110,0b01000110,//  % 
0b01101100,0b10010010,0b10101010,0b01000100,0b00001010,//  & 
0b00000000,0b10100000,0b11000000,0b00000000,0b00000000,//  ' 
0b00000000,0b00111000,0b01000100,0b10000010,0b00000000,//  ( 
0b00000000,0b10000010,0b01000100,0b00111000,0b00000000,//  ) 
0b00101000,0b00010000,0b01111100,0b00010000,0b00101000,//  *
0b00010000,0b00010000,0b01111100,0b00010000,0b00010000,//  + 
0b00000000,0b00001010,0b00001100,0b00000000,0b00000000,//  , 
0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,//  - 
0b00000000,0b00000110,0b00000110,0b00000000,0b00000000,//  . 
0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,//  / 
0b01111100,0b10001010,0b10010010,0b10100010,0b01111100,//  0 
0b00000000,0b01000010,0b11111110,0b00000010,0b00000000,//  1 
0b01000010,0b10000110,0b10001010,0b10010010,0b01100010,//  2 
0b10000100,0b10000010,0b10100010,0b11010010,0b10001100,//  3 
0b00011000,0b00101000,0b01001000,0b11111110,0b00001000,//  4 
0b11100100,0b10100010,0b10100010,0b10100010,0b10011100,//  5 
0b00111100,0b01010010,0b10010010,0b10010010,0b00001100,//  6 
0b11000000,0b10001110,0b10010000,0b10100000,0b11000000,//  7 
0b01101100,0b10010010,0b10010010,0b10010010,0b01101100,//  8 
0b01100000,0b10010010,0b10010010,0b10010100,0b01111000,//  9 
0b00000000,0b00110110,0b00110110,0b00000000,0b00000000,//  : 
0b00000000,0b01101010,0b01101100,0b00000000,0b00000000,//  ; 
0b00010000,0b00101000,0b01000100,0b10000010,0b00000000,//  < 
0b00101000,0b00101000,0b00101000,0b00101000,0b00101000,//  = 
0b00000000,0b10000010,0b01000100,0b00101000,0b00010000,//  > 
0b01000000,0b10000000,0b10001010,0b10010000,0b01100000,//  ? 
0b01001100,0b10010010,0b10011110,0b10000010,0b01111100,//  @ 
0b01111110,0b10001000,0b10001000,0b10001000,0b01111110,//  A 
0b11111110,0b10010010,0b10010010,0b10010010,0b01111100,//  B 
0b01111100,0b10000010,0b10000010,0b10000010,0b01000100,//  C 
0b11111110,0b10000010,0b10000010,0b10000010,0b01111100,//  D 
0b11111110,0b10010010,0b10010010,0b10010010,0b10000010,//  E 
0b11111110,0b10010000,0b10010000,0b10010000,0b10000000,//  F 
0b01111100,0b10000010,0b10010010,0b10010010,0b01011100,//  G 
0b11111110,0b00010000,0b00010000,0b00010000,0b11111110,//  H 
0b10000010,0b10000010,0b11111110,0b10000010,0b10000010,//  I 
0b00001100,0b10000010,0b10000010,0b11111100,0b10000000,//  J 
0b11111110,0b00010000,0b00101000,0b01000100,0b10000010,//  K 
0b11111110,0b00000010,0b00000010,0b00000010,0b00000010,//  L 
0b11111110,0b01000000,0b00110000,0b01000000,0b11111110,//  M 
0b11111110,0b00100000,0b00010000,0b00001000,0b11111110,//  N 
0b11111100,0b00000010,0b00011100,0b00000010,0b11111100,//  W 
0b11000110,0b00101000,0b00010000,0b00101000,0b11000110,//  1 
0b11000000,0b00100000,0b00011110,0b00100000,0b11000000,//  Y 
0b10000110,0b10001010,0b10010010,0b10100010,0b11000010,//  Z 
0b11111110,0b10000010,0b10000010,0b00000000,0b00000000,//  [ 
0b01000000,0b00100000,0b00010000,0b00001000,0b00000100,//  ^
0b00000000,0b00000000,0b10000010,0b10000010,0b11111110,//  ] 
0b00100000,0b01000000,0b10000000,0b01000000,0b00100000,//  ^ 
0b00000010,0b00000010,0b00000010,0b00000010,0b00000010,//  _ 
0b00000000,0b10000000,0b01000000,0b00100000,0b00000000,//  ` 
0b00000100,0b00101010,0b00101010,0b00101010,0b00011110,//  a 
0b11111110,0b00010010,0b00100010,0b00100010,0b00011100,//  b 
0b00011100,0b00100010,0b00100010,0b00100010,0b00000100,//  c 
0b00011100,0b00100010,0b00100010,0b00010010,0b11111110,//  d 
0b00011100,0b00101010,0b00101010,0b00101010,0b00011000,//  e 
0b00010000,0b01111110,0b10010000,0b10000000,0b01000000,//  f 
0b00110000,0b01001010,0b01001010,0b01001010,0b01111100,//  g 
0b11111110,0b00010000,0b00100000,0b00100000,0b00011110,//  h 
0b00000000,0b00100010,0b10111110,0b00000010,0b00000000,//  i 
0b00000100,0b00000010,0b00100010,0b10111100,0b00000000,//  j 
0b11111110,0b00001000,0b00010100,0b00100010,0b00000000,//  k 
0b00000000,0b10000010,0b11111110,0b00000010,0b00000000,//  l 
0b00011110,0b00100000,0b00010000,0b00100000,0b00011110,//  m 
0b00111110,0b00010000,0b00100000,0b00100000,0b00011110,//  n 
0b00011100,0b00100010,0b00100010,0b00100010,0b00011100,//  o 
0b00111110,0b00101000,0b00101000,0b00101000,0b00010000,//  p 
0b00010000,0b00101000,0b00101000,0b00111110,0b00000000,//  q 
0b00111110,0b00010000,0b00100000,0b00100000,0b00010000,//  r 
0b00010010,0b00101010,0b00101010,0b00101010,0b00000100,//  s 
0b00100000,0b11111100,0b00100010,0b00000010,0b00000100,//  t 
0b00111100,0b00000010,0b00000010,0b00000100,0b00111110,//  u 
0b00111000,0b00000100,0b00000010,0b00000100,0b00111000,//  v 
0b00111100,0b00000010,0b00001100,0b00000010,0b00111100,//  w 
0b00100010,0b00010100,0b00001000,0b00010100,0b00100010,//  x 
0b00110000,0b00001010,0b00001010,0b00001010,0b00111100,//  y 
0b00100010,0b00100110,0b00101010,0b00110010,0b00100010,//  z 
0b00000000,0b00010000,0b01101100,0b10000010,0b00000000,//  { 
0b00000000,0b00000000,0b11111110,0b00000000,0b00000000,//  /
0b00000000,0b10000010,0b01101100,0b00010000,0b00000000,//  } 
0b00010000,0b00100000,0b00100000,0b00010000,0b00100000,//  ~ 
0b00000000,0b01010101,0b10101010,0b01010101,0b10101010 //  unknown 
}; // ascii_characters[96]


} // font_basic_5x8
}// namespace rom

#endif
