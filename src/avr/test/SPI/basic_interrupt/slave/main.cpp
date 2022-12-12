// Copyright (C) 2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
#include "../../../../dev_interrupt.h"
#include "../../../../avr_micro.h"


using SPI = avr_::SPI_slave;

constexpr uint16_t npin_SS = avr_::SPI_num_pin_SS;	

static volatile std::byte data {0};

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
    SPI::interrupt_enable();
    avr_::enable_interrupts();

    uart << "\n\nSlave SPI\n"
	    "----------\n"
	    "Nos limitamos a recibir bytes enviados por el master.\n"
	    "Recibimos datos usando interrupciones.\n"
	    "No enviamos nada, lo que se traduce en que el SPI slave devuelve\n"
	    "al master el byte recibido.\n"
	    "Recibiendo: ";

    while (1) {
	if (data != std::byte{0}){
	    std::byte ndata{0};
	    {
                dev::Disable_interrupts<avr_::Micro> lock;
                ndata = data;
                data  = std::byte{0};
	    }

	    uart << static_cast<uint16_t>(ndata) << '\n';
	}
    }
}


// ----------
// Interrupts
// ----------
ISR_SPI_STC{
    data = SPI::data_register();
}


