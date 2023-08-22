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

#ifndef __ATD_CRC_H__
#define __ATD_CRC_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para calcular CRC
 *
 * HISTORIA
 *    Manuel Perez
 *    25/02/2023 CRC7 (experimentando)
 *
 ****************************************************************************/

#include <cstdint>
#include <span>


namespace atd{

// Calcula el CRC7 asociado a los bytes del array `data`
// Generador: g(x) = x^7 + x^3 + 1
// data = array con los bytes del número a calcular. data[0] = MSB
// (big-endian)
uint8_t CRC7(std::span<uint8_t> data);

// Calcula el CRC8 asociado a los bytes del array `data`
// Generador: g(x) = x^8 + x^5 + x^4 + 1
// data = array con los bytes del número a calcular. data[0] = MSB
// (big-endian)
uint8_t CRC8_Maxim(std::span<uint8_t> data);

}// namespace



#endif


