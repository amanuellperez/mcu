// Copyright (C) 2020-2021 Manuel Perez 
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

#ifndef __ATD_BCD_H__
#define __ATD_BCD_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Interfaz para manejo de tipo BCD.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    29/04/2020 v0.0. Versión mínima. Experimental.
 *    08/03/2021 Counter_BCD2
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t

namespace atd{

/// Convierte un número 'x' de 2 cifras escrito en BCD a binario.
/// El orden del BCD tiene que ser el humano: 'decenas|unidades'
template <typename Int>
inline Int BCD2int(const Int& x)
{
    Int u = (x & 0x0F);		// unidades
    Int d = (x & 0xF0) >> 4;	// decenas

    return Int{10}*d + u;
}

/// Convierte un número 'x' de 2 cifras escrito en binario a BCD.
/// El orden del BCD tiene que ser el humano: 'decenas|unidades'
template <typename Int>
inline Int int2BCD(const Int& x)
{
    Int u = x % Int{10};    // unidades
    Int d = (x-u)/Int{10};  // decenas

    return (d << 4) | u;
}



/*!
 *  \brief  Contador BCD.
 *
 *  La diferencia con usar un 'int' en lugar de este contador para contar
 *  números es que un int siempre va de 0 a 9, 10 a 19, ... 90 a 99.
 *  Este BCD se puede elegir qué números. Por ejemplo:
 *	    00, 01, 02, 03,
 *	    10, 11, 12, 13,
 *	    20, 21, 22, 23
 *  En este ejemplo d1d0_max = 23;
 *	    
 * 
 *  El 2 del nombre (Counter_BCD2) es porque es un contador de dos dígitos: d1d0
 *
 */
struct __Counter_BCD2{
    uint8_t d1 : 4;
    uint8_t d0 : 4;

    uint8_t d1_max : 4;
    uint8_t d0_max : 4;
};


class Counter_BCD2{
public:
// constructor
    Counter_BCD2(uint8_t d1_max, uint8_t d0_max);

    /// reset sin cambiar los valores máximos de d1d0.
    void reset();

    /// Añadimos uno al contador.
    void tick();
    
// cfg
    /// Definimos valor máximo.
    void max(uint8_t d1_max, uint8_t d0_max);

// valor del contador
    /// Dígit number 0
    uint8_t d0() const {return nticks_.d0;}

    /// Dígit number 2
    uint8_t d1() const {return nticks_.d1;}

    uint8_t as_uint() const {return nticks_.d1*uint8_t{10} + nticks_.d0;}

    bool is_zero() const {return nticks_.d0 == 0 and nticks_.d1 == 0;}
    bool is_max() const
    {
        return nticks_.d0 == nticks_.d0_max and nticks_.d1 == nticks_.d1_max;
    }

  private:
    __Counter_BCD2 nticks_;
};





}// namespace

#endif



