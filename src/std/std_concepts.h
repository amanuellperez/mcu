// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#pragma once

#ifndef __MCU_STD_CONCEPTS_H__
#define __MCU_STD_CONCEPTS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	El correspondiente a <concepts>
 *
 * HISTORIA
 *    A.Manuel L.Perez
 *    11/12/2022 Escrito
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_type_traits.h"

namespace STD{

template <typename T>
concept integral = is_integral_v<T>;

}// namespace

#endif


