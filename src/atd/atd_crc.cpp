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
#include "atd_crc.h"

#include <cstdint>

#include "atd_bit.h"
#include "atd_type_traits.h"

namespace atd{

uint8_t CRC7(std::span<const uint8_t> data)
{
    // g(x) = x^7 + x^3 + 1 = 0b10001001 = 0x89
    constexpr uint8_t g = 0x89; 

    if (data.empty())
	return 0;
    
    uint8_t crc = 0;

    for (uint8_t i = 0; i < data.size(); ++i){
	crc ^= data[i];

	for (uint8_t j = 0; j < 8; ++j){
	    // if (most_significant_bit_of(crc)_is_one()) 
	    if (is_one_most_significant_bit_of(crc))
		crc ^= g;

	    crc <<= 1;
	}
    }

    return crc >> 1;
}   



}// namespace





