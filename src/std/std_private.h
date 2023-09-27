// Copyright (C) 2023 Manuel Perez 
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

#ifndef __MCU_STD_PRIVATE_H__
#define __MCU_STD_PRIVATE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Cosas genéricas usadas en la implementación de std.
 *
 * HISTORIA
 *    Manuel Perez
 *    26/09/2023 dereferenceable
 *
 ****************************************************************************/
#include "std_config.h"
// Este header es común a todo. 
// Que no incluya ningún otro archivo para evitar dependencias circulares.
namespace STD{

namespace private_{

template<typename T>
using add_reference = T&;

template<typename T>
concept is_referenceable = requires { typename add_reference<T>; };

template<typename T>
concept dereferenceable = 
	requires(T& t)
	{
	    { *t } -> is_referenceable;
	};



}// namespace private_
}// namespace STD
#endif


