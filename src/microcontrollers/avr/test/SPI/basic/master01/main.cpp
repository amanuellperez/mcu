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
#include "../../../../avr_SPI.h"
#include "../../../../avr_pin.h"
#include "../../../../avr_UART_iostream.h"


using SPI = avr_::SPI_master_g;

// OJO: clave periodo a 8 us. Si pongo a 2 us al slave no le da tiempo a leer
// y se lee basura y pierden datos.
constexpr uint16_t periodo_en_us = 8;	
constexpr uint16_t npin_SS = SPI::SS_pin_number;	

void SPI_write(uint8_t x)
{
    avr_::UART_iostream uart;

    avr_::Output_pin<npin_SS> no_SS;
    no_SS.write_zero();
    SPI::write(uint8_t{x});
    uart << static_cast<uint16_t>(x) << '\n';
    no_SS.write_one();
    avr_::wait_ms(1000);
}

int main() 
{
// init_uart()
    avr_::UART_iostream uart;
    avr_::basic_cfg(uart);
    uart.turn_on();


// init_SPI()
    SPI::spi_mode(0,0);
    SPI::data_order_LSB();
    SPI::clock_period_in_us<periodo_en_us>();
    SPI::on();


    uart << "\n\nMaster SPI\n"
	    "----------\n"
	    "Escribe y lo transmito al slave:\n";

    while (1) {
//        char c{};
//        uart >> c;
//        uart << c;

//	for (uint8_t i = 0; i < 255; ++i){
//	    no_SS.write_zero();
//	    // SPI::write(uint8_t{c});
//	    //SPI::write(uint8_t{0x1F});
//	    uart << int(i) << '\n';
//	    SPI::write(uint8_t{i});
//	    no_SS.write_one();
//	    wait_ms(300);
//	}
	for (uint8_t i = 0; i <= 255; ++i){
//	    no_SS.write_zero();
//	    uart << static_cast<uint16_t>(i) << '\n';
//	    SPI::write(uint8_t{i});
//	    no_SS.write_one();
	    SPI_write(1);
	    SPI_write(2);
	    SPI_write(4);
	    SPI_write(8);
	    SPI_write(16);
	    SPI_write(32);
	    SPI_write(64);
	    SPI_write(128);
	}
    }
}



