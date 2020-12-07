// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "avr_SPI.h"
#include "avr_pin.h"

#include <atd_bit.h>

// Observar que todas son funciones de configuración que se ejecutan una sola
// vez. No es imprescindible que sean las más eficientes.

namespace avr{

void SPI::cfg_pines_as_master()
{
    // Leer la nota pag 356 del libro de MAKE. Es fundamental poner SS a 1
    // antes que hacer un enable
    // Configuración de los pins
    Pin<SPI_num_pin_SCK>::as_output();
    Pin<SPI_num_pin_MOSI>::as_output();
    Pin<SPI_num_pin_MISO>::as_input_with_pullup();// no es necesario (ver table 23-1)
    Pin<SPI_num_pin_SS>::as_output(); 
    
    // Inicializamos SS
    // start off not selected (high)
    Pin<SPI_num_pin_SS>::write_one();	
}


void SPI::spi_mode(bool cpol, bool cpha)
{
    if (cpol == 0)
	atd::write_bit<CPOL>::to<0>::in(SPCR);
//	Register{SPCR}.write_zero_bit<CPOL>();
    else 
	atd::write_bit<CPOL>::to<1>::in(SPCR);
//	Register{SPCR}.write_one_bit<CPOL>();


    if (cpha == 0)
	atd::write_bit<CPHA>::to<0>::in(SPCR);
//	Register{SPCR}.write_zero_bit<CPHA>();
    else
	atd::write_bit<CPHA>::to<1>::in(SPCR);
//	Register{SPCR}.write_one_bit<CPHA>();
}


// De acuerdo con la tabla 23-5
void SPI::clock_speed_entre_2()
{// 100
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<0,0>::in(SPCR);
//    Register{SPSR}.write_one_bit<SPI2X>();
//    Register{SPSR}.write_zero_bit<SPR1, SPR0>(); <-- ERROR!!! no es SPSR
}

void SPI::clock_speed_entre_4()
{// 000
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<0,0>::in(SPCR);
//    Register{SPSR}.write_zero_bit<SPI2X>();
//    Register{SPSR}.write_zero_bit<SPR1, SPR0>();
}

void SPI::clock_speed_entre_8()
{// 101
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<0,1>::in(SPCR);
//    Register{SPSR}.write_one_bit<SPI2X>();
//    Register{SPSR}.write_zero_bit<SPR1>();
//    Register{SPSR}.write_one_bit<SPR0>();
}

void SPI::clock_speed_entre_16()
{// 001
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<0,1>::in(SPCR);
//    Register{SPSR}.write_zero_bit<SPI2X>();
//    Register{SPSR}.write_zero_bit<SPR1>();
//    Register{SPSR}.write_one_bit<SPR0>();
}

void SPI::clock_speed_entre_32()
{// 110
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<1,0>::in(SPCR);
//    Register{SPSR}.write_one_bit<SPI2X>();
//    Register{SPSR}.write_one_bit<SPR1>();
//    Register{SPSR}.write_zero_bit<SPR0>();
}

void SPI::clock_speed_entre_64()
{// 010
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<1,0>::in(SPCR);
//    Register{SPSR}.write_zero_bit<SPI2X>();
//    Register{SPSR}.write_one_bit<SPR1>();
//    Register{SPSR}.write_zero_bit<SPR0>();
}

void SPI::clock_speed_entre_128()
{// 011
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<1,1>::in(SPCR);
//    Register{SPSR}.write_zero_bit<SPI2X>();
//    Register{SPSR}.write_one_bit<SPR1,SPR0>();
}



} // namespace
