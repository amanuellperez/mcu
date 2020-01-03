// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ATD_TYPE_TRAITS_H__
#define __ATD_TYPE_TRAITS_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Ampliación de type_traits.h
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez- 27/08/2019 Less_than,
 *					Pertenece_al_intervalo_cerrado
 *
 ****************************************************************************/
#include <type_traits>

namespace atd{


/// ¿x es menor que y?
template <typename Int, Int x, Int y>
struct Less_than: public std::bool_constant<x < y>{};


// ¿x pertenece al intervalo cerrado [a,b]?
template <typename Int, Int x, Int a, Int b>
struct Pertenece_al_intervalo_cerrado
    : public std::bool_constant< 
				a <= x and x <= b
				> {};



// Tipo que permite hacer:
// static_assert(always_false<T>::value, "aqui no debe de llegar");
template <typename T>
struct always_false_type : std::false_type { };

template <typename T>
inline constexpr bool always_false_v = always_false_type<T>::value;

}// namespace

#endif

