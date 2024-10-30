// Copyright (C) 2024 Manuel Perez 
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

#ifndef __AVR_PRIVATE_H__
#define __AVR_PRIVATE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Todo lo definido aquí es para ser incluido en los namespaces de los
 *	micros correspondientes (atmega328p, atmega4809...)
 *
 *	Para facilitar esa inclusión aquí defino todos los archivos de
 *	cabecera de este directorio.
 *
 *	No se incluyen los archivos que dependen de F_CPU. 
 *
 * HISTORIA
 *    Manuel Perez
 *    19/10/2024 Escrito
 *
 ****************************************************************************/

#include <avr_interrupt.h>
#include <avr_memory.h>
#include <avr_types.h>
#include <avr_constants.h>
#include <avr_time.h>
#include <avr_micro.h>


#endif


