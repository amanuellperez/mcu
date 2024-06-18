// Copyright (C) 2019-2020 Manuel Perez 
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
#ifndef __MCU_STD_IOS_TYPES_H__
#define __MCU_STD_IOS_TYPES_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Implementación mínima del sistema de flujos.
 *
 *  - COMENTARIOS: Iré implementando las cosas según las vaya necesitando.
 *
 *  - HISTORIA:
 *	Manuel Perez- 24/09/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_cstddef.h"

namespace STD{
// std_ios.h
using streamsize = ptrdiff_t; // Signed integral type
}// namespace

#endif

