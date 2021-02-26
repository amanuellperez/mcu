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

#ifndef __STATIC_H__
#define __STATIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Algoritmos en tiempo de compilación.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    18/01/2020 static_array
 *    26/02/2021 Reestructurado.
 *
 ****************************************************************************/

namespace atd{

/*!
 *  \brief  Static array.
 *
 */
template <typename T, T... args>
struct static_array{
    using size_type = std::size_t;

    static constexpr size_type size = sizeof...(args);
    static constexpr T data[size] = {args...};

    constexpr T operator[](size_type i) const {return data[i];}
};


}// namespace

#endif




