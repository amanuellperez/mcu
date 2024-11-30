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

#include "mega_SPI_hwd.h"

#include <atd_bit.h>

// Observar que todas son funciones de configuración que se ejecutan una sola
// vez. No es imprescindible que sean las más eficientes.

namespace mega_{
namespace hwd{


void SPI::spi_mode(uint8_t cpol, uint8_t cpha)
{
    if (cpol == 0)
	atd::write_bit<CPOL>::to<0>::in(SPCR);
    else 
	atd::write_bit<CPOL>::to<1>::in(SPCR);


    if (cpha == 0)
	atd::write_bit<CPHA>::to<0>::in(SPCR);
    else
	atd::write_bit<CPHA>::to<1>::in(SPCR);
}


// De acuerdo con la tabla 23-5
void SPI::clock_frequency_divide_by_2()
{// 100
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bits<SPR1, SPR0>::to<0,0>::in(SPCR);
}

void SPI::clock_frequency_divide_by_4()
{// 000
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bits<SPR1, SPR0>::to<0,0>::in(SPCR);
}

void SPI::clock_frequency_divide_by_8()
{// 101
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bits<SPR1, SPR0>::to<0,1>::in(SPCR);
}

void SPI::clock_frequency_divide_by_16()
{// 001
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bits<SPR1, SPR0>::to<0,1>::in(SPCR);
}

void SPI::clock_frequency_divide_by_32()
{// 110
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bits<SPR1, SPR0>::to<1,0>::in(SPCR);
}

void SPI::clock_frequency_divide_by_64()
{// 010
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bits<SPR1, SPR0>::to<1,0>::in(SPCR);
}

void SPI::clock_frequency_divide_by_128()
{// 011
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bits<SPR1, SPR0>::to<1,1>::in(SPCR);
}



} // namespace
} // namespace
