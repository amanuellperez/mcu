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

#ifndef __ATD_BIT_MATRIX_H__
#define __ATD_BIT_MATRIX_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Un posible uso de una matriz de bits es usarlo como lienzo donde
 *	dibujar imágenes monocromas que luego mostraremos en displays.
 *
 * HISTORIA
 *    Manuel Perez
 *    29/08/2024 Bit_matrix_by_rows
 *
 ****************************************************************************/
#include "atd_cmath.h"	// div
#include "atd_bit.h"

namespace atd{

/***************************************************************************
 *			    BIT_MATRIX_BY_ROWS
 ***************************************************************************/
// El SDD1306 escribe bytes por columnas, mientras que el MAX7219 (tal como lo
// venden conectado al display) está por filas. 
// Por ello diferencio dos tipos de matrices: por filas y por columnas.
// La idea es que las funciones:
//	SDD1306::write(bit_matrix);
//	MAX7219::write(bit_matrix);
// sean lo más eficientes posibles. 
template <size_t nrows, size_t ncols>
class Bit_matrix_by_rows{
public:

    using data_type  = uint8_t; // esto es para documentar
    using index_type = uint16_t; 
    using Bit        = atd::Bit<data_type>;
    using const_Bit  = atd::Bit<const data_type>;

    static_assert(sizeof(data_type) == 1); // si se cambiar, revisar implementación
    static_assert(ncols % 8 == 0, "Only implemented multiples of 8");

// Dimensions
    constexpr size_t rows() const {return nrows;}
    constexpr size_t cols() const {return ncols;}

// Acceso aleatorio
    // lectura
    constexpr Bit operator()(index_type i, index_type j);
    constexpr const_Bit operator()(index_type i, index_type j) const;

// Funciones más eficientes 
    // Rellena toda la matriz con el valor b (0 ó 1 únicamente)
    void fill(uint8_t b);

private:
    // Internamente no almacenamos los bits como tales, sino que los agrupamos
    // en bloques por columnas. Si el data_type es uint8_t, los almacenamos
    // por bytes.
    static constexpr size_t ncols_in_blocks() {return ncols / 8*sizeof(data_type);}

    // observar el orden: 
    // row = 1: uint8_t, uint8_t, uint8_t, ... <-- los bits están agrupados
    //                                              por filas
    data_type data_[nrows][ncols_in_blocks()]; 

    // Dos formas de identificar un bit en una fila:
    //	1) j = indicando el número de bit
    //	2) (J, p) = indicando el número de byte J en el que se encuentra y el
    //		    bit p dentro de ese byte
    //	La relación es: j = 8 * J + p
    static constexpr std::pair<uint8_t, uint8_t> index(index_type j)
    { return div(j, index_type{8}); }
};

template <size_t nrows, size_t ncols>
constexpr 
auto Bit_matrix_by_rows<nrows, ncols>::
			operator()(index_type i, index_type j) -> Bit
{
    auto [J, p] = index(j);
    return bit(p).of(data_[i][J]);
}

template <size_t nrows, size_t ncols>
constexpr 
auto Bit_matrix_by_rows<nrows, ncols>::
			operator()(index_type i, index_type j) const -> const_Bit
{
    auto [J, p] = index(j);
    return bit(p).of(data_[i][J]);
}

template <size_t nrows, size_t ncols>
void Bit_matrix_by_rows<nrows, ncols>::fill(uint8_t b)
{
    if (b != 0)
	b = 0xFF; // esto supone que data_type == uint8_t

    for (uint8_t i = 0; i < rows(); ++i)
	for (uint8_t j = 0; j < ncols_in_blocks(); ++j)
	    data_[i][j] = b;
}


}// atd
 

#endif


