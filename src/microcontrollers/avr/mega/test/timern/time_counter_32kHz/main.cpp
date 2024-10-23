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


#include "../../../mega_timer2.h" 
#include "../../../mega_debug.h"

#include "../../../mega_UART_iostream.h"
#include "../../../mega_cfg.h"
#include "../../../mega_pin.h"

// Microcontroller
// ---------------
namespace my_mcu = mega_;

// Pin conections
// --------------
static constexpr uint8_t led_pin = 14;

// Devices
// -------
using Pin2      = my_mcu::Pin<led_pin>;
using Counter2  = my_mcu::Time_counter2_32kHz_g<>;


// Global vbles
// ------------
volatile uint32_t nseconds = 0;

// Functions
// ---------
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}

void main_hello()
{
    my_mcu::UART_iostream uart;

    uart << "\n\nTime counter asyncrhonous test\n"
	        "-------------------------------\n"
		"Don't forget to connect a crystal of 32 kHz between pins "
		<< (int) Counter2::crystal_32_kHz_pin1
		<< " and "
		<< (int) Counter2::crystal_32_kHz_pin2
		<< "\n\n"
		   "Connect a LED in pin number " << (int) Pin2::number << '\n';

}

int main()
{
    init_uart();
    Pin2::as_output();
    
    my_mcu::enable_interrupts();

    main_hello();

    my_mcu::UART_iostream uart;
    
    if (Counter2::turn_on_with_overflow_to_count_1s() != 1)
	uart << "ERROR: can't start Counter 2\n";

    while(1){
	my_mcu::wait_ms(1000);
	{
	    my_mcu::Disable_interrupts lock;
	    uart << nseconds << " s\n";
	}
    }
}


// INTERRUPTS
// ----------
ISR_TIMER2_COMPA{
    Pin2::toggle();
    nseconds = nseconds + 1;
}
