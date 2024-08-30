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
 * HISTORIA
 *    Manuel Perez
 *    30/08/2024 Primeros experimentos
 *
 ****************************************************************************/
#include <type_traits>

#include "atd_bit_matrix.h"

namespace atd{

// Escribe el caracter c en (i, j) usando la fuente Font
template <typename Font, size_t nrows, size_t ncols, typename PageCol>
void write(Bit_matrix_by_cols<nrows, ncols>& m, const PageCol& pos, char ic)
    requires requires 
	    {	Font::is_by_columns; 
		Font::is_ASCII_font;
		std::is_same_v<typename Bit_matrix_by_cols<nrows, ncols>::PageCol, PageCol>;
	    }
{
    auto letter = Font::glyph.row(Font::index(ic));
    auto c = letter.begin();
    for (uint8_t j = 0; j < Font::cols; ++j){
	for (uint8_t npage = 0; 
		    npage < Font::col_in_bytes and c != letter.end(); 
		    ++npage, ++c){
	    m.write_byte(*c, pos + PageCol{npage, j});
	}
    }
}


}// atd

#endif


