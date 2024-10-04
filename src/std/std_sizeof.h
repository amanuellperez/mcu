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
#ifndef __ATD_SIZEOF_H__
#define __ATD_SIZEOF_H__
/****************************************************************************
 *
 * DESCRIPCION
 *  Necesito Num_caracteres_max para crear los ostream.
 *
 * HISTORIA
 *    Manuel Perez
 *    08/02/2013 Escrito
 *    11/10/2019 Completo y simplifico cosas
 *    04/10/2024 Metido en atd_ para llevarlo a atd
 *
 ****************************************************************************/


#include "std_config.h"

#include <stdint.h> // uint8_t, ...
#include "std_type_traits.h"

namespace STD{
namespace atd_{

/// Al no poder usar string, cuando se quiere convertir un entero a
/// cadena hay que reservar la memoria necesaria:
/// 
///   // OJO: recordar sumarle 1 para el '\0'.
///   char buffer[Sizeof_txt_v<int>+1]; 
///
///   itoa(x, buffer, 10);  // almacenamos en buffer 'x' como cadena.
// Número de caracteres necesarios para escribir T como cadena de caracteres
// (como texto, y sin incluir el signo + en los unsigned, solo el número. Los
// signed tienen un caracter más para poder incluir el signo -).
// Devuelve el número de caracteres en función del número de bytes:
//	size = 8bits    unsigned	max = 255	num_max = 3
//	size = 8b       signed		max = -128	num_max = 4
//	size = 16b      unsigned	max = 65536	num_max = 5
//	size = 16b	signed		max = -32768	num_max = 6
//	size = 32b      unsigned	max = 4294967296 num_max = 10
//	size = 32b      signed		max = 4294967296 num_max = 11
//	size = 64b      unsigned	max = ...	 num_max = 20
//	size = 64b      signed		max = ...	 num_max = 21
//
//  Observar que el número de caracteres_h es diferente para signed que
//  unsigned. Por simplificar la implementación devuelvo el mayor de los 2.
//  El _h es para indicar que es un helper. La clase a usar es Sizeof_txt.


template <uint8_t sz>
inline constexpr uint8_t __max_number_of_digits()
{
    if constexpr (sz == 1) return 3;
    else if constexpr (sz == 2) return 5;
    else if constexpr (sz == 4) return 10;
    else if constexpr (sz == 8) return 20;
    else return 0;  // TODO: que de error.
}



template <typename Int, bool signo>
struct Max_number_of_digits_of_h
    : public 
      integral_constant<Int, __max_number_of_digits<sizeof(Int)>()> 
{ };

template <typename Int>
struct Max_number_of_digits_of_h<Int, true>
    : public 
      integral_constant<Int, __max_number_of_digits<sizeof(Int)>() + 1> 
{ };

/// Devuelve el número de caracteres que cómo máximo ocupa el Int como cadena
/// de C.
template <typename Int>
inline constexpr int Max_number_of_digits_of
    = Max_number_of_digits_of_h<Int, is_signed_v<Int>>::value;


}// namespace atd_
}// namespace STD



#endif


