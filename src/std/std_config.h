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

#ifndef __MCU_STD_CONFIG_H__
#define __MCU_STD_CONFIG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	En principio no quiero tener que distinguir entre
 *	si uso la STL estandard o la mía. Por ello, voy a meter todo este
 *	código, que tiene que ser estandar dentro de std.
 *
 *	Problema:
 *	1.- Para probarlo entra en conflicto mi std con el std estandar.
 *	Necesito meterlo en un espacio de nombres diferente (mtd).
 *	2.- ¿Qué pasa si en el futuro se ve que es mala idea usar std?
 *	Cambiando esta macro, cambia todo el código (aunque no el código del
 *	cliente).
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    16/07/2019 v0.0
 *    08/10/2023 throw_exception
 *    04/10/2024 Al intentar llevar STD::atd_ a atd fallan las pruebas en
 *               atd/pc_test por culpa de la variable STD. Por ello, opto por
 *               definir STD en mi cpp_environment para poder ejecutar las
 *               pruebas en atd/pc_test.
 *
 ****************************************************************************/
#if !defined STD

    // mtd or std?
    #if defined __AVR__ || __AVR
    #   define STD std

    #elif defined(__GNUC__) || defined(__clang__)
    #   define STD mtd

    #else
    # error "Unknown compiler"
    #endif

#endif

// Cfg
// ---
// (RRR) De momento no lanzo excepciones en mi código, pero el estandar
//	 habla de lanzar excepciones. ¿Cómo escribir el código de manera que
//	 esté de acuerdo con el estandard en caso de poder lanzar excepciones?
//
//	 La variable `can_throw_exceptions` va a determinar si se pueden
//	 lanzar o no. En el código en lugar de escribir `throw exception`
//	 escribiré `throw_exception` que dependiendo del valor de esta
//	 variable lanzará o no una excepción.
constexpr bool can_throw_exceptions = false;

template <typename Exception, typename... Args>
inline 
constexpr void throw_exception(Args... args)
{
    if constexpr (can_throw_exceptions)
	throw Exception{args...};

    else {
// DUDA: ¿gestionamos las excepciones en los micros? Podemos usar un errno o
// exception_happened. 
// De hecho se puede crear una enumeración con los tipos de excepciones que
// lanza el standard y marcarlos en exception_type. (un valor que sea
// 'no_exception' para saber que no se ha lanzado excepciones).
//	exception_type = T;
	return;
    }
}

#endif


