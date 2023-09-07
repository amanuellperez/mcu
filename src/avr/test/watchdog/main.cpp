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

#include "../../avr_atmega328p_cfg.h"
#include "../../avr_pin.h"
#include "../../avr_micro.h"
#include "../../avr_UART_iostream.h"
#include <atd_ostream.h>

// microcontroller
// ---------------
namespace mcu = avr_;
using Micro = mcu::Micro;
#include "../../avr_watchdog.h" // TODO: orden

// pin connections
// ---------------
constexpr uint8_t npin = 15;
using Pin = mcu::Pin<npin>;


// Hwd Devices
// -----------
using Watchdog = mcu::Watchdog;


/***************************************************************************
 *				    MAIN
 ***************************************************************************/
// Global vbles
// ------------
volatile uint32_t counter;

// Functions
// ---------
void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();
}


int main()
{
    init_uart();
    counter = 0;

// menu
    mcu::UART_iostream uart;
    uart << "\n\nWatchdog test\n"
	        "-------------\n";

    Watchdog::interrupt_mode<Watchdog::Period::T1_s>();
    Micro::enable_interrupts();

    while (1){
	    ;
	}

}


ISR_WDT{
    counter = counter + 1;
    mcu::UART_iostream uart;
    uart << counter << '\n';

}

