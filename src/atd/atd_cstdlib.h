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

#ifndef __ATD_CSTDLIB_H__
#define __ATD_CSTDLIB_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de cstdlib
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    24/07/2021 atoi
 *
 ****************************************************************************/

namespace atd{

// Equivalente a std::atoi pero con 'char' como argumento.
template <typename Int>
inline Int to_int(char c) {return c - '0';}


}// namespace

#endif


