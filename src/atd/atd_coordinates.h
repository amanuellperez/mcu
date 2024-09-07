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

#ifndef __ATD_COORDINATES_H__
#define __ATD_COORDINATES_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Hay un montón (demasiadas!) formas de referirse a una posición:
 *
 *  + Coordenadas geométricas o xy: (x,y)
 *    ¿Cuál es el origen del sistema de referencia? Hay que especificarlo de
 *    alguna otra forma.
 *
 *  + Coordenadas matriciales o ij: (i,j)
 *    En una matriz se elige el punto superior-izdo como origen. Los ejes son: 
 *    j = x; i = -y
 *    Posibles valores para una matriz m x n (rows x cols):
 *		    i = [0...m-1], j = [0...n-1]
 *    Otra posible forma de definirlas, en lugar de usar (i,j), sería 
 *    (row, col) pero al ser más largo de escribir mejor (i,j) (además, que es
 *    la notación habitual en matemátias)
 *
 *  + Cooredandas por páginas (= bloques horizontales) o pagecol: (page, col)
 *    Algunos displays (SDD1306) lo que hacen es agrupar las filas por
 *    bloques, llamados pages (imaginar que coges el display y lo cortas en
 *    tiras horizontales de 8 filas cada una, cada una es una página donde
 *    poder dibujar). 
 *
 *
 *  Aunque en la práctica todo es lo mismo (un nombre de la posición de un
 *  punto) como sería muy confuso cuando programas matrices hablar de (x,y) o
 *  al hacer geometría hablar de (i, j), hay que definir una clase diferente.
 *
 *  TODO: Las coordenadas (x,y) están definidas en geo_2d. ¿Traerlas aquí?
 *
 * HISTORIA
 *    Manuel Perez
 *    21/08/2024 PageCol
 *    07/09/2024 Coord_ij
 *
 ****************************************************************************/
#include <ostream>

#include "atd_concepts.h"

namespace atd{
/***************************************************************************
 *				PAGECOL
 ***************************************************************************/
// Descomponemos el plano en páginas horizontales
template <Type::Numeric Int>
struct PageCol{
    Int page;	
    Int col;	

    constexpr
    PageCol(const Int& page0, const Int& col0) : page{page0}, col{col0} { }

    constexpr PageCol& operator+=(const PageCol& x);
    constexpr PageCol& operator-=(const PageCol& x);
};


// operadores aritmeticos
// ----------------------
template <Type::Numeric Int>
constexpr PageCol<Int>& PageCol<Int>::operator+=(const PageCol& x)
{
    page += x.page;
    col  += x.col;

    return *this;
}

template <Type::Numeric Int>
constexpr PageCol<Int>& PageCol<Int>::operator-=(const PageCol& x)
{
    page -= x.page;
    col  -= x.col;

    return *this;
}

template <Type::Numeric Int>
inline constexpr PageCol<Int> 
    operator+(PageCol<Int> x, const PageCol<Int>& y)
{
    x += y;
    return x;
}

template <Type::Numeric Int>
inline constexpr PageCol<Int> 
    operator-(PageCol<Int> x, const PageCol<Int>& y)
{
    x -= y;
    return x;
}

// flujo de salida
// ---------------
template <Type::Numeric Int>
std::ostream& operator<<(std::ostream& out, const PageCol<Int>& p)
{ return out << '(' << p.page << ", " << p.col << ')'; }

// PageCol_rectangle
// -----------------
template <Type::Numeric Int>
struct PageCol_rectangle{
    using Point = PageCol<Int>;
    Point p0;	// extremo inferior-izdo
    Point p1;	// extremo superior-dcho

    // DUDA: ¿verificar que p1 > p0?
    constexpr
    PageCol_rectangle(const Point& p00, const Point& p10) 
		: p0{p00}, p1{p10} { }

    constexpr uint16_t size() const 
    { return (p1.col - p0.col + 1) * (p1.page - p0.page + 1); }
};


/***************************************************************************
 *				COORD_IJ
 ***************************************************************************/
// Coordenadas de matriz (i,j)
template <Type::Numeric Int>
struct Coord_ij{
    Int i;	
    Int j;	

    constexpr
    Coord_ij(const Int& i0, const Int& col0) : i{i0}, j{col0} { }

    constexpr Coord_ij& operator+=(const Coord_ij& x);
    constexpr Coord_ij& operator-=(const Coord_ij& x);
};


// operadores aritmeticos
// ----------------------
template <Type::Numeric Int>
constexpr Coord_ij<Int>& Coord_ij<Int>::operator+=(const Coord_ij& x)
{
    i += x.i;
    j  += x.j;

    return *this;
}

template <Type::Numeric Int>
constexpr Coord_ij<Int>& Coord_ij<Int>::operator-=(const Coord_ij& x)
{
    i -= x.i;
    j  -= x.j;

    return *this;
}

template <Type::Numeric Int>
inline constexpr Coord_ij<Int> 
    operator+(Coord_ij<Int> x, const Coord_ij<Int>& y)
{
    x += y;
    return x;
}

template <Type::Numeric Int>
inline constexpr Coord_ij<Int> 
    operator-(Coord_ij<Int> x, const Coord_ij<Int>& y)
{
    x -= y;
    return x;
}

// flujo de salida
// ---------------
template <Type::Numeric Int>
std::ostream& operator<<(std::ostream& out, const Coord_ij<Int>& p)
{ return out << '(' << p.i << ", " << p.j << ')'; }


} // namespace atd

#endif



