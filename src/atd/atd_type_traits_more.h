// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __TYPE_TRAITS_MORE_H__
#define __TYPE_TRAITS_MORE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traits de tipos no básicos.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    18/03/2021 v0.0
 *
 ****************************************************************************/

#include "atd_type_traits_basic.h"
#include "atd_decimal.h"

namespace atd{
// same_type_with_double_bits_
template<typename Rep, int N>
struct same_type_with_double_bits_<Decimal<Rep, N>>{
    using type = Decimal<same_type_with_double_bits<Rep>, N>;
};
}// namespace


#endif


