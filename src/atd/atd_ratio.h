// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
#ifndef ATD_RATIO_H
#define ATD_RATIO_H


/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de std::ratio
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    31/03/2020 is_ratio
 *
 ****************************************************************************/
#include <type_traits>
#include <ratio>

namespace atd{

// bool is_ratio(typename T)
// {
//	if (is_<T>_a_specialization_of<ratio>)
//	    return true;
//	else
//	    return false;
// }
template <typename T>
struct is_ratio : std::false_type { };

template <intmax_t N, intmax_t D>
struct is_ratio<std::ratio<N, D>> : std::true_type { };

}// namespace

#endif


