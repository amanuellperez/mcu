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

// Ejemplo básico de uso del Timer como contador
#include "../../../mega_cfg.h"
#include "../../../mega_UART.h"
#include <mcu_UART_iostream.h>
#include "../../../mega_timer2_basic.h"
#include <avr_time.h>

#include <atd_ostream.h>
#include <stdlib.h>
#include <std_type_traits.h>

// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART>;

// Hwd devices
// -----------
using Timer = myu::Timer2;

// Cfg
// ---
constexpr uint16_t period_in_us = 1024;

// Global variables
// ----------------
volatile uint32_t counter = 0;


// FUNCTIONS
// ---------
void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();
}

void print_register(const char* name, uint8_t value)
{
    UART_iostream uart;
    uart << name << "\t= ";
    atd::print_int_as_hex(uart, value);
    uart << '\n';
}

void print_registers()
{
    UART_iostream uart;
    print_register("TCCR2A", TCCR2A);
    print_register("TCCR2B", TCCR2B);
    print_register("TCNT2", TCNT2);
    print_register("OCR2A", OCR2A);
    print_register("OCR2B", OCR2B);
    print_register("TIMSK2", TIMSK2);
    print_register("TIFR2", TIFR2);
    print_register("ASSR", ASSR);
    print_register("GTCCR", GTCCR);

    uart << "\nPress a key to continue\n";
    char c{};
    uart >> c;
}

int main()
{
    init_uart();

    UART_iostream uart;
    uart << "\nTimer2 counter test\n"
	      "-------------------\n";

// clock_init();
    Timer::enable_overflow_interrupt(); 
    Timer::normal_mode();
    Timer::clock_frequency_divide_by_1024();  // arranca el timer
    print_registers();

// start:
    myu::enable_interrupts();
					      


    while(1){
	Timer::counter_type v;
	uint32_t c{};
	{
	    myu::Disable_interrupts di;
	    v = Timer::counter();
	    c = counter;
	}

	uint64_t t_us = (c*(Timer::max() + 1)+ v)*period_in_us;
	uint64_t t_s = t_us / uint64_t{1'000'000};
	uart << t_s << " s\n";

	myu::wait_ms(1000);
    }
}



ISR_TIMER2_OVF
{
    counter = counter + 1;
}


