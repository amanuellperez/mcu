// Copyright (C) 2022-2024 Manuel Perez 
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

#ifndef __ATD_NAMES_H__
#define __ATD_NAMES_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Intento de hacer más expresivo el código.
 *	    ¡¡¡Experimental!!!
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    07/03/2022 Width, From, To, Row
 *    14/06/2024 ok/fail (experimental)
 *
 ****************************************************************************/
#include <cstdint>  // uint8_t
		    
namespace nm{// nm = name
// Números con significados
// ------------------------
// Las siguientes clases las defino para darle un significado a un número. 
// Ej: Cambiamos 'print(2, 3)' por 'print(2, atd::width(3))'. 
// Queda más claro que el 3 es un ancho y que el 2 es el número que queremos 
// imprimir.

// Width = es un número que significa un ancho
template <typename Int>
struct Width{
    Int x;
    Width(const Int& x0) : x{x0} { }

    template <typename Int2>
    Width(const Int2& x0) : x{x0} { }

    operator Int() const {return x;}
};


// Size = es un número que significa un número de objetos
template <typename Int>
struct Size{
    Int x;
    Size(const Int& x0) : x{x0} { }

    template <typename Int2>
    Size(const Int2& x0) : x{x0} { }

    operator Int() const {return x;}
};

// Row = número que significa 'fila'
template <typename Int>
struct Row{
    Int x;
    Row(const Int& x0) : x{x0} { }

    template <typename Int2>
    Row(const Int2& x0) : x{x0} { }

    operator Int() const {return x;}
};


// From = número (o puntero, o ???) que significa 'desde' donde empezamos
template <typename Int>
struct From{
    Int x;
    From(const Int& x0) : x{x0} { }

    template <typename Int2>
    From(const Int2& x0) : x{x0} { }

    operator Int() const {return x;}
};

// To = número (o puntero, o ???) que significa 'hasta' donde llegamos
template <typename Int>
struct To{
    Int x;
    To(const Int& x0) : x{x0} { }

    template <typename Int2>
    To(const Int2& x0) : x{x0} { }

    operator Int() const {return x;}
};


// Mensajes que devuelven las funciones
// ------------------------------------
// (RRR) ¿por qué no definirlos como enum?
//       Quedaría muy largo: if (funcion() == atd::Result::fail) return ...;
//
// DUDA: Meterlo en atd? atd::ok? atd::fail?
//
using Result = uint8_t;
static constexpr Result ok   = 0;
static constexpr Result fail = 1;

}// namespace  nm


#endif


