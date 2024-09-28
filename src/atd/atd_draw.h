// Copyright (C) 2024 Manuel Perez 
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

#ifndef __ATD_DRAW_H__
#define __ATD_DRAW_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones de dibujo
 *
 *	El lienzo sobre el que vamos a dibujar es una matriz de colores.
 *	Los colores podrán ser monocromáticos puros (0/1), monocromaticos de
 *	intensidad (0/255), o colores de verdad (r,g,b) (o la representación
 *	que se quiera).
 *
 * TODO/DUDA:
 *	En los bucles estoy usando `uint8_t` por ser el tipo básico del
 *	atmega, pero otros micros no tienen pueden ser de 16 bits y no de 8
 *	bits. Por defecto, la regla es usar siempre `int`. ¿Dejar uint8_t o
 *	cambiarlo a int?
 *
 * HISTORIA
 *    Manuel Perez
 *    30/08/2024 Primeros experimentos
 *
 ****************************************************************************/
#include <type_traits>

#include "atd_bit_matrix.h"

namespace atd{

// Bitmatrix_col_1bit
// ------------------
// Escribe el caracter c en el byte más próximo al bit (i, j) 
// usando la fuente Font.
// precondition: (i, j) son unsigned ==> i >= 0 and j >= 0
//		(¿modificarlo para que admita signed? De momento no)
template <typename Font, size_t NR, size_t NC>
void write(Bitmatrix_col_1bit<NR, NC>& m, 
	    const typename Bitmatrix_col_1bit<NR, NC>::Coord_ij& p0, 
	    char ic)
    requires requires 
	    {	Font::is_by_columns; 
		Font::is_turned_to_the_right;
		Font::is_ASCII_font;
	    }
{
    using Index = typename Bitmatrix_col_1bit<NR, NC>::index_type;

    if (p0.i >= m.rows() or p0.j >=  m.cols())
	return;

    auto letter = Font::glyph.row(Font::index(ic));
    auto c = letter.begin();

    // nrows = número de filas de la Font a escribir en la matriz
    // nrows0 = ceil_division(m.rows() - p0.i, 8) ???
    Index nrows0 = (m.rows() - p0.i) / 8;
    if ((m.rows() - p0.i) % 8 != 0)
	++nrows0; 

    Index nrows_in_bytes = std::min<Index>(Font::rows_in_bytes, nrows0);
    Index ncols = std::min<Index>(Font::cols, m.cols() - p0.j);

    // CUIDADO: los glyphs de las letras giradas a la derecha, empiezan por la
    // primera columna ABAJO, y luego van subiendo hasta llegar a la primera
    // columna ARRIBA (van de (n..0] las j)
    for (uint8_t j = 0; j < ncols; ++j){
	uint8_t I = 0; // dibujamos la fila I de la Font contando desde ABAJO
	// saltamos la parte de abajo del glyph que queda fuera del bitmatrix
	for (; I < Font::rows_in_bytes - nrows_in_bytes; ++I)
	    ++c;   

	for (; I < Font::rows_in_bytes; ++I, ++c)
	    m.write(*c, p0.i + 8 * (Font::rows_in_bytes - 1 - I), p0.j + j);

    }
}


// Bitmatrix_row_1bit
// ------------------
// TODO: permitir que se pueda escribir en cualquier (i,j) como
// Bitmatrix_col_1bit
// Escribe el caracter c en (i, j) usando la fuente Font
template <typename Font, size_t nrows, size_t ncols>
void write(Bitmatrix_row_1bit<nrows, ncols>& m, 
	    const typename Bitmatrix_row_1bit<nrows, ncols>::Coord_ij& p0,
	    char ic)
    requires requires 
	    {	Font::is_by_rows; 
		Font::is_looking_from_the_front;
		Font::is_ASCII_font;
	    }
{
    using Index = typename Bitmatrix_row_1bit<nrows, ncols>::index_type;

    auto [I0, J0] = m.byte_coordinates_of(p0);

    if (I0 >= m.rows_in_bytes() or J0 >=  m.cols_in_bytes())
	return;

    auto letter = Font::glyph.row(Font::index(ic));
    auto c = letter.begin();

    Index I = std::min<Index>(Font::rows_in_bytes, m.rows_in_bytes() - I0);
    Index J = std::min<Index>(Font::cols_in_bytes, m.cols_in_bytes() - J0);

    for (uint8_t i = 0; i < I; ++i){
	// saltamos la parte de abajo del glyph que queda fuera del bitmatrix
	uint8_t j = 0;
	for (; j < J; ++j, ++c)
	    m.write_byte(*c, I0 + i, J0 + j);

	for (; j < Font::cols_in_bytes- J; ++j)
	    ++c;   

    }
}




}// atd

#endif


