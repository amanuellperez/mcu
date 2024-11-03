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

#include "../../mega_watchdog.h" 
#include "../../mega_cfg.h"
#include "../../mega_pin.h"
#include "../../mega_micro.h"
#include "../../mega_UART.h"
#include <mcu_UART_iostream.h>
#include <atd_ostream.h>

// microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

// pin connections
// ---------------
constexpr uint8_t npin = 15;
using Pin = myu::Pin<npin>;


// Hwd Devices
// -----------
using Watchdog = myu::Watchdog;


/***************************************************************************
 *				    MAIN
 ***************************************************************************/
// Global vbles
// ------------
volatile bool counter;

// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}


int main()
{
    init_uart();
    counter = false;

// menu
    UART_iostream uart;
    uart << "\n\nWatchdog test\n"
	        "-------------\n"
		"Connect oscilloscope to pin " << (int) Pin::number << "\n"
		"(my test, at 5V, show an error from 16 to 20% of the generated period)\n";


    while (1){
	uart << "\nInterrupt mode test\n"
	          "-------------------\n"
		"Change pin every:\n"
		"\t1. 16 ms\n"
		"\t2. 32 ms\n"
		"\t3. 64 ms\n"
		"\t4. 125 ms\n"
		"\t5. 250 ms\n"
		"\t6. 500 ms\n"
		"\t7. 1 s\n"
		"\t8. 2 s\n"
		"\t9. 4 s\n"
		"\tA. 8 s\n";
	
	char opt{};
	uart >> opt;
	switch(opt){
	    break; case '1': Watchdog::interrupt_mode<Watchdog::Period::T16_ms>();
	    break; case '2': Watchdog::interrupt_mode<Watchdog::Period::T32_ms>();
	    break; case '3': Watchdog::interrupt_mode<Watchdog::Period::T64_ms>();
	    break; case '4': Watchdog::interrupt_mode<Watchdog::Period::T125_ms>();
	    break; case '5': Watchdog::interrupt_mode<Watchdog::Period::T250_ms>();
	    break; case '6': Watchdog::interrupt_mode<Watchdog::Period::T500_ms>();
	    break; case '7': Watchdog::interrupt_mode<Watchdog::Period::T1_s>();
	    break; case '8': Watchdog::interrupt_mode<Watchdog::Period::T2_s>();
	    break; case '9': Watchdog::interrupt_mode<Watchdog::Period::T4_s>();
	    break; case 'a':
		   case 'A': Watchdog::interrupt_mode<Watchdog::Period::T8_s>();
	    break; default:  Watchdog::interrupt_mode<Watchdog::Period::T1_s>();
	}

	myu::enable_interrupts();

	uart << "Press a key to stop the watchdog\n";
	uart >> opt;
	Watchdog::stop();
	}

}


ISR_WDT{
    if (counter)
	Pin::write_zero();
    else
	Pin::write_one();

    counter = !counter;

}

