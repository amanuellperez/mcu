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
 *    20/08/2023 CRC8 (de Maxim)
 *  
 */

#include "atd_crc.h"

#include <cstdint>

#include "atd_bit.h"
#include "atd_type_traits.h"

#include <iostream> // TODO: borrame, depurar
		    //
namespace atd{

uint8_t CRC7(std::span<uint8_t> data)
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


// IDEAS
//  1. Para que entre el generador en 1 byte no incluimos el bit de x^8.
//     
uint8_t CRC8_Maxim(std::span<uint8_t> data)
{
    // g(x) = x^8 + x^5 +  x^4 + 1 = 0b1 0011 0001 = 0x131 (9 bits!!!)
    constexpr uint8_t g = 0x31; 

    if (data.empty())
	return 0;
    
    uint8_t crc = 0;

    for (uint8_t i = 0; i < data.size(); ++i){
	crc ^= data[i];

std::cerr << "\ncrc = [0x" << std::hex << (int) crc << "]\n";
	for (uint8_t j = 0; j < 8; ++j){
	    // if (most_significant_bit_of(crc)_is_one()) 
	    if (is_one_most_significant_bit_of(crc)){
		crc <<= 1;
		crc ^= g;
	    }
	    else
		crc <<= 1;
std::cerr << "crc = [0x" << std::hex << (int) crc << "]\n";
	}
    }

    return crc;
}   



}// namespace





