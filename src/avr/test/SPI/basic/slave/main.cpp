// Copyright (C) 2021 Manuel Perez 
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

#include "../../../../avr_atmega328p_cfg.h"
#include "../../../../avr_SPI_basic.h"
#include "../../../../avr_interrupt.h"
#include "../../../../avr_UART_iostream.h"


using SPI = avr_::SPI_slave;

constexpr uint16_t periodo_en_us = 2;	
constexpr uint16_t npin_SS = avr_::SPI_num_pin_SS;	


int main() 
{
// init_uart()
    avr_::UART_iostream uart;
    avr_::basic_cfg(uart);
    uart.on();


// init_SPI()
    SPI::on();
    SPI::spi_mode(0,0);
    SPI::data_order_LSB();


    uart << "\n\nSlave SPI\n"
	    "----------\n"
	    "Recibiendo: ";

    while (1) {
	SPI::wait_transmission_complete();
	std::byte data = SPI::data_register();
	uart << std::to_integer<uint16_t>(data) << '\n';

//	if (data != std::byte{0}){
//	    std::byte ndata{0};
//	    {
//                avr_::Interrupts_lock lock;
//                ndata = data;
//                data  = std::byte{0};
//	    }
//
////	    if (ndata == std::byte{0x56})
////		uart << "OK\n";
////	    else
////		uart << "ERROR\n";
////	    uart << "ndata = " << int(ndata) << '\n';
////	    uart << "'" << std::to_integer<char>(ndata) << "'";
//	    uart << static_cast<uint16_t>(ndata) << '\n';
//	}
////	wait_ms(100);

    }
}


//ISR_SPI_STC{
//    data = SPI::data_register();
//}
//

