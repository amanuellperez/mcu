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

// Ideal para aprender cómo funciona SPI.
// Conectar el osciloscopio y jugar con la polaridad.

#include "../../../../avr_SPI.h"
#include "../../../../avr_pin.h"
#include "../../../../avr_UART_iostream.h"


namespace my_mcu = avr_;

using SPI = my_mcu::SPI_master;

constexpr uint16_t period_in_us = 8;	

int main() 
{
//// init_uart()
//    avr_::UART_iostream uart;
//    avr_::basic_cfg(uart);
//    uart.turn_on();


// init_SPI()
    // Probar a cambiar pol de 0 a 1.
    // Se ve muy bien con el osciloscopio que si pol == 0, el SCK es 0 cuando
    // esta idle, y si pol == 1 SCK == 1.
    // Probar a cambiar pha para ver cómo se comporta. Es muy visual.
    SPI::spi_mode(0,0);
    SPI::data_order_LSB();
    SPI::clock_period_in_us<period_in_us>();
    SPI::on();


//    uart << "\n\nMaster SPI\n"
//	    "----------\n"
//	    "Connect oscilloscope and measure SCK and MOSI pins\n";

    my_mcu::Output_pin<SPI::CS_pin_number> no_SS;
    no_SS.write_zero();
    while (1) {
	SPI::write(0xFF);   // esto pone a 1 MOSI durante 8 ciclos de reloj
//	SPI::write(0xAA);   // 
    }
}



