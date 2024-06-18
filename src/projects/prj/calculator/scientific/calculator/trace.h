// Copyright (C) 2022 Manuel Perez 
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

#ifndef __TRACE_H__
#define __TRACE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Depuremos la calculadora
 *    En caso de estar definida TRACES_ON se va llevando un log de lo que va
 *    haciendo la calculadora vía UART.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    06/01/2022 Escrito
 *
 ****************************************************************************/
#include <app_trace.h>
#include "dev.h"

#ifdef TRACES_ON
void trace(const Buffer& buffer);

#else

inline void trace(const Buffer& buffer){}

#endif




#endif


