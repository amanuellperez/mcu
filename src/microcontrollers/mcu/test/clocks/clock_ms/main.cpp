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

#include "../../../mcu_clock.h"
#include <avr_atmega.h>
#include <atd_time.h>

namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART>;


using Clock = mcu::Clock_ms<Micro, myu::Time_counter1>;



// Definimos el reloj del sistema
ISR_TIMER1_COMPA
{
    Clock::tick();
}




void print_time(UART_iostream& uart, const Clock::Time& t)
{
    uart << std::setw(2) << (int) t.hours << ':'
	 << std::setw(2) << (int) t.minutes << ':'
	 << std::setw(2) << (int) t.seconds << ':'
	 << std::setw(3) << (int) t.milliseconds << '\n';

}


void test_clock()
{
    UART_iostream uart;
//    using UART = myu::UART_basic;

    uart << "\n\nClock test\n"
	        "----------\n"
		"Starting in 2 seconds:\n";

    Clock::turn_on();

    Clock::set(std::chrono::seconds{2});

    while(1){
	print_time(uart, Clock::now_as_time());

	myu::Micro::wait_ms(100);

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
    UART_iostream uart;
    myu::UART_basic_cfg();
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



