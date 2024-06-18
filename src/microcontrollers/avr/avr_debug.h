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

#ifndef __AVR_DEBUG_H__
#define __AVR_DEBUG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para depurar el avr. Este archivo no se distribuye.
 *
 * HISTORIA
 *    Manuel Perez
 *    14/09/2023 Escrito
 *    14/06/2024 print_registers_adc
 *
 ****************************************************************************/
#include <ostream>


namespace avr_{

// Impresión de los registros del avr
// ----------------------------------
void print_registers_uart(std::ostream& out);

void print_registers_timer0(std::ostream& out);
void print_registers_timer1(std::ostream& out);
void print_registers_timer2(std::ostream& out);

void print_registers_adc(std::ostream& out);

}// namespace

 
#endif
