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
#include "../../../../avr_SPI_generic.h"
#include "../../../../avr_pin.h"
#include "../../../../avr_UART_iostream.h"


using SPI = avr_::SPI_master_g;

// OJO: clave periodo a 8 us. Si pongo a 2 us al slave no le da tiempo a leer
// y se lee basura y pierden datos.
constexpr uint16_t periodo_en_us = 8;	
constexpr uint16_t npin_SS = SPI::SS_pin_number;	


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
        char c{};
        uart >> c;
        uart << c << " ---> ";

	avr_::Output_pin<npin_SS> no_SS;
	no_SS.write_zero();
	uint8_t res = SPI::write(uint8_t{c});
	uart << static_cast<char>(res) << '\n';

	no_SS.write_one();
	avr_::wait_ms(1000);

    }
}



