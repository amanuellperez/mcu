// Copyright (C) 2019-2023 Manuel Perez 
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

#include "../../../mega_UART.h"
#include <mcu_UART_iostream.h>

#include "../../../mega_pin.h"
#include <avr_time.h>
#include "../../../mega_sleep.h"
#include "../../../mega_debug.h"
#include "../../../mega_timer2.h"


// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;


// Pin connections
// ---------------
static constexpr uint8_t led_pin = 14;

// Hwd devices
// -----------
using Counter = myu::Time_counter2_32kHz_g<1000>;
using Pin     = myu::Pin<led_pin>;


// Terminal
// --------
UART_iostream uart; // análogo a std::cout/std::cin

						
// Functions
// ---------
void init_uart()
{
    UART_iostream::init();

    uart.turn_on();
}


void main_hello()
{
    uart << "\nTest uart with sleep power save mode\n"
	      "------------------------------------\n"
	      "END\n";
}


void init()
{
    init_uart();
    Pin::as_output();
    Counter::turn_on_with_overflow_to_count_1s();
    myu::enable_interrupts();
    myu::sleep_mode(myu::Sleep::mode::power_save);

}

void print()
{
    uart << "1234567890\n"
	    "Can you see numbers from 1 to 0?\n"
	    "END\n";
}

void flush()
{
    myu::print_registers_uart(uart);
    while (!myu::UART_basic::is_transmit_complete())
    {;}
    // Datasheet: The TXC0 Flag bit is automatically cleared
    // when a transmit complete interrupt is executed, 
    // or it can be cleared by writing a one to its bit location.
    
    atd::write_bit<TXC0>::to<1>::in(UCSR0A);


}


int main() 
{
    init();

    main_hello();


    while (1) {
	flush();    // comment this line. The program generates garbage

	myu::sleep();

	print();
    }

}


ISR_TIMER2_COMPA
{
    Pin::toggle(); // para saber cuándo se llama a la interrupción
}
