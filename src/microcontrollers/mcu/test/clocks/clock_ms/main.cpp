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

#include "../../../dev_clocks.h"
#include <avr_atmega.h>
#include <atd_time.h>

namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;

using Clock = dev::Clock_ms<Micro, my_mcu::Time_counter1_g>;



// Definimos el reloj del sistema
ISR_TIMER1_COMPA
{
    Clock::tick();
}




void print_time(my_mcu::UART_iostream& uart, const Clock::Time& t)
{
    uart << std::setw(2) << (int) t.hours << ':'
	 << std::setw(2) << (int) t.minutes << ':'
	 << std::setw(2) << (int) t.seconds << ':'
	 << std::setw(3) << (int) t.milliseconds << '\n';

}


void test_clock()
{
    my_mcu::UART_iostream uart;
//    using UART = my_mcu::UART_basic;

    uart << "\n\nClock test\n"
	        "----------\n"
		"Starting in 2 seconds:\n";

    Clock::turn_on();

    Clock::set(std::chrono::seconds{2});

    while(1){
	print_time(uart, Clock::now_as_time());

	my_mcu::Micro::wait_ms(100);

//	if(UART::are_there_data_unread()){
//	    char ans = static_cast<char>(UART::data_register());
//	    if(ans == 'h')
//		halt();
//
//	    print_menu();
//	}

    }
}


void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}


void init_timer()
{
    Clock::turn_on();
    Micro::enable_interrupts();
}


int main()
{
    init_uart();
    init_timer();

    test_clock();
}



