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
 *    22/08/2023 CRC8 
 *
 ****************************************************************************/

#include <cstdint>
#include <span>
#include <iterator>

#include "atd_bit.h"

namespace atd{

// Calcula el CRC7 asociado a los bytes del array `data`
// Generador: g(x) = x^7 + x^3 + 1
// data = array con los bytes del número a calcular. data[0] = MSB
// (big-endian)
uint8_t CRC7(std::span<const uint8_t> data);


template <uint8_t g, 
	  bool reverse_data_bits, 
	  bool reverse_crc_bits,
	  std::forward_iterator It>
inline uint8_t CRC8_(It p, It pe)
{
    uint8_t crc = 0;

    for (; p != pe; ++p){
	if constexpr (reverse_data_bits)
	    crc ^= reverse_bits(*p);
	else
	    crc ^= *p;


	for (uint8_t j = 0; j < 8; ++j){
	    // if (most_significant_bit_of(crc)_is_one()) 
	    if (is_one_most_significant_bit_of(crc)){
		crc <<= 1;
		crc ^= g;
	    }
	    else
		crc <<= 1;
	}
    }

    if constexpr (reverse_crc_bits)
	return reverse_bits(crc);

    else
	return crc;
}



// Calcula el CRC8 asociado a los bytes del array `data`
// Generador: g(x) = x^8 + x^5 + x^4 + 1
// data = array con los bytes del número a calcular. data[0] = MSB
// (big-endian)
//
// Como el CRC es de 8 bits pero la función generadora usa 9 usamos el trick
// de eliminar el 9 bit del monomio x^8. 
//	Ejemplo: El polinomio generador usado por Maxim es 
//	    g(x) = x^8 + x^5 +  x^4 + 1 = 0b1 0011 0001 = 0x131 (9 bits!!!)
//	    En lugar de usar 0x131 abreviamos a 0x31 ya que todos los
//	    generadores de CRC8 son de la forma 0x1ab
//
template <uint8_t g, // función generadora
	  bool reverse_data_bits, 
	  bool reverse_crc_bits,
	  bool data_is_big_endian = true> // dice que data[0] = MSB
uint8_t CRC8(std::span<const uint8_t> data)
{
    if (data.empty())
	return 0;

    if constexpr (data_is_big_endian)
	return CRC8_<g, reverse_data_bits, reverse_crc_bits>
						    (data.begin(), data.end());
    else
	return CRC8_<g, reverse_data_bits, reverse_crc_bits>
						    (data.rbegin(), data.rend());


}   


// Calcula el CRC8 asociado a los bytes del array `data`
// Generador: g(x) = x^8 + x^5 + x^4 + 1
// data = array con los bytes del número a calcular. data[0] = MSB
// (big-endian)
inline uint8_t CRC8_Maxim(std::span<const uint8_t> data)
{ return CRC8<0x31, true, true, true>(data); }

inline uint8_t CRC8_Maxim(const uint8_t* data, size_t N)
{ 
    std::span<const uint8_t> d{data, N};
    return CRC8<0x31, true, true, true>(d.first(N)); 
}

}// namespace



#endif


