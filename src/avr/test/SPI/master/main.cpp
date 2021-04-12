// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../../avr_SPI_basic.h"
#include "../../../avr_pin.h"
#include "../../../avr_UART_iostream.h"


using SPI = avr::SPI_master;

constexpr uint16_t periodo_en_us = 2;	
constexpr uint16_t npin_SS = avr::SPI_num_pin_SS;	


int main() 
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

    avr::Output_pin<npin_SS> no_SS;

// init_SPI()
    SPI::on<periodo_en_us>();
    SPI::spi_mode(0,0);
    SPI::data_order_MSB();


    uart << "\n\nMaster SPI\n"
	    "----------\n"
	    "Escribe y lo transmito al slave:\n";

    while (1) {
//        char c{};
//        uart >> c;
//        uart << c;

	for (uint8_t i = 0; i < 255; ++i){
	    no_SS.write_zero();
	    // SPI::write(std::byte{c});
	    //SPI::write(std::byte{0x1F});
	    uart << int(i) << '\n';
	    SPI::write(std::byte{i});
	    no_SS.write_one();
	    wait_ms(300);
	}
    }
}



