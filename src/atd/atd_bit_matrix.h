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
 *	El problema es que hay 4 formas diferentes de almacenar los uint8_t
 *	que componen los bits de la matriz:
 *
 *	    Bitmatrix_row_1bit: filas de 1 bit , uint8_t por filas
 *	    Bitmatrix_row_8bit: filas de 8 bits, uint8_t por filas
 *	    Bitmatrix_col_1bit: columnas 1 bit , uint8_t por columnas
 *	    Bitmatrix_col_8bit: columnas 8 bits, uint8_t por columnas
 *
 *	(ver pag 27, proleg I)
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    29/08/2024 Bitmatrix_row_1bit
 *
 ****************************************************************************/
#include <iterator>

#include "atd_cmath.h"	// div
#include "atd_math.h"	// ceil_division
#include "atd_bit.h"
#include "atd_coordinates.h"	// Coord_ij

namespace atd{

/***************************************************************************
 *			    BITMATRIX_ROW_1BIT
 ***************************************************************************/
// El SDD1306 escribe bytes por columnas, mientras que el MAX7219 (tal como lo
// venden conectado al display) está por filas. 
// Por ello diferencio dos tipos de matrices: por filas y por columnas.
// La idea es que las funciones:
//	SDD1306::write(bit_matrix);
//	MAX7219::write(bit_matrix);
// sean lo más eficientes posibles. 
//
// (???) En la práctica los displays con 4 MAX7219 que he comprado se pueden
// colocar por columnas, permitiendo escribir las letras como en el SDD1306
// siendo más práctico (para hacer scrolls). Mejor usar Bitmatrix_col_1bit?
template <size_t nrows, size_t ncols>
class Bitmatrix_row_1bit{
public:
// Types
    using data_type  = uint8_t; // esto es para documentar
    using index_type = uint16_t; 
    using Bit        = atd::Bit<data_type>;
    using const_Bit  = atd::Bit<const data_type>;
    using Coord_ij   = atd::Coord_ij<index_type>;

// iterator = data_type* <-- NO, ya que de iterar sería sobre los bits, no
//			         sobre los bytes.
// Byte iterators
    using byte_iterator         = data_type*; 
    using const_byte_iterator   = const data_type*; 
    using reverse_byte_iterator = std::reverse_iterator<byte_iterator>;
    using const_reverse_byte_iterator 
				= std::reverse_iterator<const_byte_iterator>;

    static_assert(sizeof(data_type) == 1); // si se cambiar, revisar implementación

// Bit matrix interface
// --------------------
// Dimensions
    static constexpr size_t rows() {return nrows;}
    static constexpr size_t cols() {return ncols;}

// Acceso aleatorio
    // lectura
    constexpr Bit operator()(index_type i, index_type j);
    constexpr const_Bit operator()(index_type i, index_type j) const;

    // Escritura (es más eficiente write_byte)
    void write(data_type x, index_type i, index_type j);

// Funciones más eficientes 
    // Rellena toda la matriz con el valor b (0 ó 1 únicamente)
    void fill(uint8_t b);

    void clear() {fill(0);}

// Implementation interface: byte interface
// ----------------------------------------
// Byte dimensions
    static constexpr size_t rows_in_bytes() {return nrows; }
    static constexpr size_t cols_in_bytes() 
    {return ceil_division(ncols, 8*sizeof(data_type));}

    // Escribe el byte indicado en (i, j)
    void write_byte(data_type x, index_type I, index_type J);
    data_type read_byte(index_type I, index_type J) const;

    // Imaginar que sobre esta matriz de bits, ponemos un grid de bytes
    // (colocados por columnas). Esta función devuelve la coordenada del bit
    // p0 ajustada a ese grid.
    constexpr static std::pair<index_type, index_type>
				byte_coordinates_of(const Coord_ij& p0);

// Acceso por columnas 
    // Iteradores: begin/end por columnas en bytes (!!!)
    byte_iterator row_begin(index_type i);  // byte donde empieza la columna j
    byte_iterator row_end(index_type i);    // byte + 1 donde acaba la columna j
					
    const_byte_iterator row_begin(index_type i) const; // byte donde empieza la columna j
    const_byte_iterator row_end(index_type i) const;	// byte + 1 donde acaba la columna j
							
    // Reverse iterators: rbegin/rend por columnas en bytes (!!!)
    reverse_byte_iterator rrow_begin(index_type i); 
    reverse_byte_iterator rrow_end(index_type i);	
					
    const_reverse_byte_iterator rrow_begin(index_type i) const; 
    const_reverse_byte_iterator rrow_end(index_type i) const;
								
    // tamaño en bytes de la columna j
    static constexpr index_type row_size(index_type i);

private:
// Data
    // observar el orden: 
    // row = 1: uint8_t, uint8_t, uint8_t, ... <-- los bits están agrupados
    //                                              por filas
    data_type data_[nrows][cols_in_bytes()]; 

    // Dos formas de identificar un bit en una fila:
    //	1) j = indicando el número de bit
    //	2) (J, p) = indicando el número de byte J en el que se encuentra y el
    //		    bit p dentro de ese byte
    //	La relación es: j = 8 * J + p
    static constexpr std::pair<index_type, index_type> index(index_type j)
    { return div(j, index_type{8}); }
};

// (RRR) ¿por qué necesito r?
//	 Almacenamos en bytes los datos:
//
//	76543210 76543210 76543210  <-- esta es la posición p en el byte
//	-------- -------- --------  <-- valor de los bytes
//	01234567 01234567 01234567  <-- indice r
//	01234567 89..               <-- indice j
//
//	Observar que cuando decimos que j == 0 nos referimos realmente al
//	valor p = 0
template <size_t nrows, size_t ncols>
constexpr 
auto Bitmatrix_row_1bit<nrows, ncols>::
			operator()(index_type i, index_type j) -> Bit
{
    auto [J, r] = index(j);
    uint8_t p = 7 - r;	// 8 = cada byte tiene 8 bits, con p + r + 1 = 8 
    return bit(p).of(data_[i][J]);
}

template <size_t nrows, size_t ncols>
constexpr 
auto Bitmatrix_row_1bit<nrows, ncols>::
			operator()(index_type i, index_type j) const -> const_Bit
{
    auto [J, r] = index(j);
    uint8_t p = 7 - r;	// 8 = cada byte tiene 8 bits
    return bit(p).of(data_[i][J]);
}


template <size_t nrows, size_t ncols>
inline void
Bitmatrix_row_1bit<nrows, ncols>::
    write_byte(data_type x, index_type I, index_type J)
{ 
    data_[I][J] = x;
}

template <size_t nrows, size_t ncols>
inline auto
Bitmatrix_row_1bit<nrows, ncols>::
    read_byte(index_type I, index_type J) const -> data_type
{ return data_[I][J]; }

template <size_t nrows, size_t ncols>
inline
void Bitmatrix_row_1bit<nrows, ncols>::
    write(data_type x, index_type i, index_type j)
{ in(data_[i]).bit(j).write_byte_reverse_order(x); }



template <size_t nrows, size_t ncols>
inline constexpr auto
Bitmatrix_row_1bit<nrows, ncols>::byte_coordinates_of(const Coord_ij& p0) 
		    -> std::pair<index_type, index_type>
{ return { p0.i, p0.j / 8}; }


template <size_t nrows, size_t ncols>
inline 
auto 
Bitmatrix_row_1bit<nrows, ncols>::row_begin(index_type i) -> byte_iterator
{ return &(data_[i][0]); }


template <size_t nrows, size_t ncols>
inline 
auto Bitmatrix_row_1bit<nrows, ncols>::row_end(index_type i) -> byte_iterator
{ return row_begin(i + 1); }

template <size_t nrows, size_t ncols>
inline 
auto 
Bitmatrix_row_1bit<nrows, ncols>::row_begin(index_type i) const 
							-> const_byte_iterator
{ return &(data_[i][0]); }


template <size_t nrows, size_t ncols>
inline 
auto Bitmatrix_row_1bit<nrows, ncols>::row_end(index_type i) const
						    -> const_byte_iterator
{ return row_begin(i + 1); }


template <size_t nrows, size_t ncols>
inline 
auto 
Bitmatrix_row_1bit<nrows, ncols>::rrow_begin(index_type i) -> reverse_byte_iterator
{ return reverse_byte_iterator{row_end(i)}; }


template <size_t nrows, size_t ncols>
inline 
auto Bitmatrix_row_1bit<nrows, ncols>::rrow_end(index_type i) -> reverse_byte_iterator
{ return reverse_byte_iterator{row_begin(i)}; }

template <size_t nrows, size_t ncols>
inline 
auto 
Bitmatrix_row_1bit<nrows, ncols>::rrow_begin(index_type i) const 
							-> const_reverse_byte_iterator
{ return const_reverse_byte_iterator{row_end(i)}; }


template <size_t nrows, size_t ncols>
inline 
auto Bitmatrix_row_1bit<nrows, ncols>::rrow_end(index_type i) const
						    -> const_reverse_byte_iterator
{ return const_reverse_byte_iterator{row_begin(i)}; }
					


template <size_t nrows, size_t ncols>
inline 
constexpr auto 
Bitmatrix_row_1bit<nrows, ncols>::row_size(index_type i) -> index_type
{return rows_in_bytes();}


template <size_t nrows, size_t ncols>
void Bitmatrix_row_1bit<nrows, ncols>::fill(uint8_t b)
{
    if (b != 0)
	b = 0xFF; // esto supone que data_type == uint8_t

    for (uint8_t i = 0; i < rows(); ++i)
	for (uint8_t j = 0; j < cols_in_bytes(); ++j)
	    data_[i][j] = b;
}

/***************************************************************************
 *			    BITMATRIX_COL_1BIT
 ***************************************************************************/
// Matriz de nrows x ncols bits. 
// Internamente se almacenan en bloques por columnas.
// Lo podemos concebir como que descomponemos la matriz de bits en páginas
// horizontales, cada página de 8 bits de alto. El número de páginas que hay
// son nrows / 8
//
// Suministro dos tipos de interfaces:
//  1) Interfaz de matrices de bits. Este interfaz es igual para todos los
//     Bitmatrix_...
//
//  2) Interfaz que depende de la implementación: interfaz de bytes.
//     Si suministro diferentes matrices de bits es para que se puedan copiar
//     de la forma más rápida posible a diferentes displays monocromáticos.
template <size_t nrows, size_t ncols>
class Bitmatrix_col_1bit{
public:
// Types
    using data_type  = uint8_t; // esto es para documentar
    using index_type = uint16_t; 
    using Bit        = atd::Bit<data_type>;
    using const_Bit  = atd::Bit<const data_type>;
    using Coord_ij   = atd::Coord_ij<index_type>;

// iterator = data_type* <-- NO, ya que de iterar sería sobre los bits, no
//			         sobre los bytes.
// Byte iterators
    using byte_iterator         = data_type*; 
    using const_byte_iterator   = const data_type*; 
    using reverse_byte_iterator = std::reverse_iterator<byte_iterator>;
    using const_reverse_byte_iterator 
				= std::reverse_iterator<const_byte_iterator>;

    static_assert(sizeof(data_type) == 1); // si se cambiar, revisar implementación


// Bit matrix interface
// --------------------
// Dimensions
// Es una matriz de bits ==> convenio no pongo el `bit` en `bit_rows`
    static constexpr size_t rows() {return nrows;}
    static constexpr size_t cols() {return ncols;}

// Acceso aleatorio
    // lectura
    constexpr Bit operator()(index_type i, index_type j);
    constexpr const_Bit operator()(index_type i, index_type j) const;

    // Escritura (es más eficiente write_byte)
    void write(data_type x, index_type i, index_type j);

// Funciones más eficientes 
    // Rellena toda la matriz con el valor b (0 ó 1 únicamente)
    void fill(uint8_t b);

    void clear() {fill(0);}

// Implementation interface: byte interface
// ----------------------------------------
// La idea es imaginar poner un grid de bytes (por columnas) sobre la matriz
// de bits.
// Byte dimensions
    static constexpr size_t rows_in_bytes() 
	{return ceil_division(nrows, 8*sizeof(data_type));}
    static constexpr size_t cols_in_bytes() {return ncols;}

    // CUIDADO: las coordenadas (i,j) son las coordenadas de la matriz de
    // bytes, no de la matriz de bits. Por eso no le paso Coord_ij
    void write_byte(data_type x, index_type I, index_type J);
    data_type read_byte(index_type I, index_type J) const;

    // Imaginar que sobre esta matriz de bits, ponemos un grid de bytes
    // (colocados por columnas). Esta función devuelve la coordenada del bit
    // p0 ajustada a ese grid.
    constexpr static std::pair<index_type, index_type>
				byte_coordinates_of(const Coord_ij& p0);

// Acceso por columnas 
    // Iteradores: begin/end por columnas en bytes (!!!)
    byte_iterator col_begin(index_type j);  // byte donde empieza la columna j
    byte_iterator col_end(index_type j);    // byte + 1 donde acaba la columna j
					
    const_byte_iterator col_begin(index_type j) const; // byte donde empieza la columna j
    const_byte_iterator col_end(index_type j) const;	// byte + 1 donde acaba la columna j
							
    // Reverse iterators: rbegin/rend por columnas en bytes (!!!)
    reverse_byte_iterator rcol_begin(index_type j); 
    reverse_byte_iterator rcol_end(index_type j);	
					
    const_reverse_byte_iterator rcol_begin(index_type j) const; 
    const_reverse_byte_iterator rcol_end(index_type j) const;
								
    // tamaño en bytes de la columna j
    static constexpr index_type col_size(index_type j);


private:


    // observar el orden: 
    // row = 1: uint8_t, uint8_t, uint8_t, ... <-- los bits están agrupados
    //                                              por filas
    data_type data_[cols_in_bytes()][rows_in_bytes()]; 

    // Dos formas de identificar un bit en una columna:
    //	1) i = indicando el número de bit
    //	2) (I, p) = indicando el número de byte I en el que se encuentra y el
    //		    bit p dentro de ese byte
    //	La relación es: i = 8 * I + p
    static constexpr std::pair<index_type, index_type> index(index_type i)
    { return div(i, index_type{8}); }
};

template <size_t nrows, size_t ncols>
inline 
constexpr 
auto Bitmatrix_col_1bit<nrows, ncols>::
			operator()(index_type i, index_type j) -> Bit
{
    auto [J, p] = index(i);
    return bit(p).of(data_[j][J]);
}

template <size_t nrows, size_t ncols>
inline 
constexpr 
auto Bitmatrix_col_1bit<nrows, ncols>::
			operator()(index_type i, index_type j) const -> const_Bit
{
    auto [J, p] = index(i);
    return bit(p).of(data_[j][J]);
}


template <size_t nrows, size_t ncols>
inline constexpr auto
Bitmatrix_col_1bit<nrows, ncols>::byte_coordinates_of(const Coord_ij& p0) 
		    -> std::pair<index_type, index_type>
{ return { p0.i / 8 , p0.j}; }


template <size_t nrows, size_t ncols>
inline void
Bitmatrix_col_1bit<nrows, ncols>::
    write(data_type x, index_type i, index_type j)
{ in(data_[j]).bit(i).write_byte_same_order(x); }


template <size_t nrows, size_t ncols>
inline void
Bitmatrix_col_1bit<nrows, ncols>::
    write_byte(data_type x, index_type I, index_type J)
{ data_[J][I] = x; }

template <size_t nrows, size_t ncols>
inline auto
Bitmatrix_col_1bit<nrows, ncols>::
    read_byte(index_type I, index_type J) const -> data_type
{ return data_[J][I]; }

template <size_t nrows, size_t ncols>
inline 
auto 
Bitmatrix_col_1bit<nrows, ncols>::col_begin(index_type j) -> byte_iterator
{ return &(data_[j][0]); }


template <size_t nrows, size_t ncols>
inline 
auto Bitmatrix_col_1bit<nrows, ncols>::col_end(index_type j) -> byte_iterator
{ return col_begin(j + 1); }

template <size_t nrows, size_t ncols>
inline 
auto 
Bitmatrix_col_1bit<nrows, ncols>::col_begin(index_type j) const 
							-> const_byte_iterator
{ return &(data_[j][0]); }


template <size_t nrows, size_t ncols>
inline 
auto Bitmatrix_col_1bit<nrows, ncols>::col_end(index_type j) const
						    -> const_byte_iterator
{ return col_begin(j + 1); }


template <size_t nrows, size_t ncols>
inline 
auto 
Bitmatrix_col_1bit<nrows, ncols>::rcol_begin(index_type j) -> reverse_byte_iterator
{ return reverse_byte_iterator{col_end(j)}; }


template <size_t nrows, size_t ncols>
inline 
auto Bitmatrix_col_1bit<nrows, ncols>::rcol_end(index_type j) -> reverse_byte_iterator
{ return reverse_byte_iterator{col_begin(j)}; }

template <size_t nrows, size_t ncols>
inline 
auto 
Bitmatrix_col_1bit<nrows, ncols>::rcol_begin(index_type j) const 
							-> const_reverse_byte_iterator
{ return const_reverse_byte_iterator{col_end(j)}; }


template <size_t nrows, size_t ncols>
inline 
auto Bitmatrix_col_1bit<nrows, ncols>::rcol_end(index_type j) const
						    -> const_reverse_byte_iterator
{ return const_reverse_byte_iterator{col_begin(j)}; }
					


template <size_t nrows, size_t ncols>
inline 
constexpr auto 
Bitmatrix_col_1bit<nrows, ncols>::col_size(index_type j) -> index_type
{return rows_in_bytes();}


template <size_t nrows, size_t ncols>
void Bitmatrix_col_1bit<nrows, ncols>::fill(uint8_t b)
{
    if (b != 0)
	b = 0xFF; // esto supone que data_type == uint8_t

    for (uint8_t i = 0; i < cols(); ++i)
	for (uint8_t j = 0; j < rows_in_bytes(); ++j)
	    data_[i][j] = b;
}

}// atd
 

#endif


