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

#include "avr_SPI_basic.h"
#include "avr_pin.h"

#include <atd_bit.h>

// Observar que todas son funciones de configuración que se ejecutan una sola
// vez. No es imprescindible que sean las más eficientes.

namespace avr{

void SPI_master::init()
{
    // Configuración de los pins
    Pin<SPI_num_pin_SCK>::as_output();
    Pin<SPI_num_pin_MOSI>::as_output();
//    Pin<SPI_num_pin_MISO>::as_input_without_pullup();
    Pin<SPI_num_pin_SS>::as_output(); // fundamental para que no sea slave
    
////    // Inicializamos SS
//// Leer la nota pag 356 del libro de MAKE. Es fundamental poner SS a 1
//// antes que hacer un enable
////    // start off not selected (high)
//    Pin<SPI_num_pin_SS>::write_one();	
}


void SPI_slave::init()
{
    // Configuración de los pins (table 23-1). 
//    Pin<SPI_num_pin_SCK>::as_input_without_pullup();
//    Pin<SPI_num_pin_MOSI>::as_input_without_pullup();
    Pin<SPI_num_pin_MISO>::as_output();
//    Pin<SPI_num_pin_SS>::as_input_without_pullup(); 
}


void SPI_basic::spi_mode(bool cpol, bool cpha)
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
void SPI_basic::clock_speed_divide_by_2()
{// 100
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<0,0>::in(SPCR);
}

void SPI_basic::clock_speed_divide_by_4()
{// 000
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<0,0>::in(SPCR);
}

void SPI_basic::clock_speed_divide_by_8()
{// 101
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<0,1>::in(SPCR);
}

void SPI_basic::clock_speed_divide_by_16()
{// 001
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<0,1>::in(SPCR);
}

void SPI_basic::clock_speed_divide_by_32()
{// 110
    atd::write_bit<SPI2X>::to<1>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<1,0>::in(SPCR);
}

void SPI_basic::clock_speed_divide_by_64()
{// 010
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<1,0>::in(SPCR);
}

void SPI_basic::clock_speed_divide_by_128()
{// 011
    atd::write_bit<SPI2X>::to<0>::in(SPSR);
    atd::write_bit<SPR1, SPR0>::to<1,1>::in(SPCR);
}



} // namespace
