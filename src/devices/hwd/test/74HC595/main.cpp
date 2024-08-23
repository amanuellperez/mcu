// Copyright (C) 2019-2021 Manuel Perez 
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

#include "../../dev_74HC595.h"
#include <avr_atmega.h>

namespace my_mcu = atmega;

// dev
constexpr uint16_t period_in_us = 2;	
using SPI = atmega::SPI_master;

using Register_pins =
    dev::Register_74HC595_pins<dev::Register_74HC595_RCLK<12>,
                               dev::Register_74HC595_NO_SRCLR<16>>;

using Register = dev::Register_74HC595_SPI<Register_pins>;


// src
void sweep(Register& reg)
{
    for (uint8_t p = 1; p != 0; p <<= 1){
	reg.write(p);
	my_mcu::Micro::wait_ms(100);
    }

    my_mcu::Micro::wait_ms(100);
    reg.clear();
}


void counter(Register& reg)
{
    for (uint8_t i = 0; i < 255; ++i){
	reg.write(i);
	my_mcu::Micro::wait_ms(500);
    }

    reg.write(255);
    my_mcu::Micro::wait_ms(500);

}


int main() 
{
// init_SPI()
    SPI::spi_mode(0,0);
    SPI::data_order_LSB();
    SPI::clock_period_in_us<period_in_us>();
    SPI::turn_on();

//    select_register();

    Register reg;
    reg.buffer_clear();

    while(1){
	//sweep(reg);
	counter(reg);
    }
}


