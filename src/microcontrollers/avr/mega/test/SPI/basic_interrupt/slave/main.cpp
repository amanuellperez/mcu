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

#include "../../../../mega_registers.h"
#include "../../../../mega_interrupt.h"
#include "../../../../mega_SPI_hal.h"
#include "../../../../mega_UART_hal.h"
#include <mcu_UART_iostream.h>
#include "../../../../mega_micro.h"

// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::hal::UART_8bits>;

using SPI = myu::hal::SPI_slave;


static volatile uint8_t data {0};

int main() 
{
// init_uart()
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();


// init_SPI()
    SPI::turn_on();
    SPI::mode(0,0);
    SPI::data_order_LSB();
    SPI::interrupt_enable();
    myu::enable_interrupts();

    uart << "\n\nSlave SPI\n"
	    "----------\n"
	    "Nos limitamos a recibir bytes enviados por el master.\n"
	    "Recibimos datos usando interrupciones.\n"
	    "No enviamos nada, lo que se traduce en que el SPI slave devuelve\n"
	    "al master el byte recibido.\n"
	    "Recibiendo: ";

    while (1) {
	if (data != uint8_t{0}){
	    uint8_t ndata{0};
	    {
                myu::Disable_interrupts lock;
                ndata = data;
                data  = uint8_t{0};
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


