// Copyright (C) 2020 Manuel Perez 
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
#ifndef ATD_RATIO_H
#define ATD_RATIO_H


/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de std::ratio
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    31/03/2020 is_static_ratio
 *    31/01/2021 ratio_inverse
 *    05/03/2021 is_power_of_ten, ratio_exponent_of_power_of_ten
 *    17/10/2023 Type::Static_ratio
 *
 ****************************************************************************/
#include <type_traits>
#include <ratio>

#include "atd_math.h"

namespace atd{

// is_static_ratio
// --------
// bool is_static_ratio(typename T)
// {
//	if (is_<T>_a_specialization_of<ratio>)
//	    return true;
//	else
//	    return false;
// }
template <typename T>
struct is_static_ratio : std::false_type { };

template <intmax_t N, intmax_t D>
struct is_static_ratio<std::ratio<N, D>> : std::true_type { };

template <typename T>
inline constexpr bool is_static_ratio_v = is_static_ratio<T>::value;
 

// ratio_inverse
// -------------
// ratio_inverse(num/den) == den/num
template <typename r>
using ratio_inverse = std::ratio<r::den, r::num>;



// ratio_is_power_of_ten
// ---------------------
// ratio_is_power_of_ten<q>:
//  if (q == 10^n) return true;
//  else           return false;
template <typename q>
struct _ratio_is_power_of_ten: 
    public std::bool_constant<(q::num == 1 and is_power_of_ten(q::den))
		or	 (is_power_of_ten(q::num) and q::den == 1)>{};

template <typename q>
inline constexpr bool ratio_is_power_of_ten = _ratio_is_power_of_ten<q>::value;


// ratio_exponent_of_power_of_ten
// -------------------------------
// ratio_exponent_of_power_of_ten<10^n>:
//  return n;
namespace impl_of{
template <typename q>
struct ratio_exponent_of_power_of_ten
    : public std::integral_constant<int,
          (q::num == 1? -exponent_of_power_of_ten(q::den):
          exponent_of_power_of_ten(q::num))> {
    static_assert(ratio_is_power_of_ten<q>);
};
}// impl_of
 
template <typename q>
inline constexpr int ratio_exponent_of_power_of_ten =
    impl_of::ratio_exponent_of_power_of_ten<q>::value;

}// namespace atd


// Static_ratio
// ------------
namespace Type{
template <typename T>
concept Static_ratio = atd::is_static_ratio_v<T>;
}// namespace Type
 
#endif


