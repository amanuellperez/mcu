// Copyright (C) 2019-2024 Manuel Perez 
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

#include "../../../mega_cfg_hwd.h"
#include "../../../mega_UART_hal.h"
#include <mcu_UART_iostream.h>
#include "../../../mega_timer1_hwd.h"
#include <avr_time.h>
#include "../../../mega_interrupt.h"
#include "../../../mega_micro.h"
#include "../../../mega_debug_hwd.h"


#include <atd_ostream.h>
#include <stdlib.h>
#include <std_type_traits.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace myu = mega_;
namespace hwd = mega_::hwd;
using UART_iostream = mcu::UART_iostream<myu::hal::UART_8bits>;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;


// Devices
// -------
using Timer = hwd::Timer1;

// Global vbles
// ------------
constexpr uint16_t period_in_us = 1024;

// Interrupt vbles
// ---------------
volatile uint32_t counter = 0;


// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}
 
void init_timer_clock()
{
    Timer::normal_mode();
    Timer::clock_frequency_divide_by_1024();
    Timer::enable_overflow_interrupt();
}


void hello()
{
    UART_iostream uart;
    uart << "\n\nTimer1 counter test\n"
	        "--------------------\n"
		"Connections: none\n";
}


struct time_in_days{
    uint16_t us; // microseconds: [0, 999]
    uint16_t ms; // miliseconds : [0, 999]
    uint8_t sec; // seconds     : [0, 59]
    uint8_t min; // minutes     : [0, 59]
    uint8_t hour;// hours	   : [0, 23]
    uint16_t day;// days	   : [0, 65535]
};

constexpr uint64_t num_microsegundos_tiene_1dia = 86400000000UL;

time_in_days us_to_time_in_days(uint64_t time_in_us)
{
    time_in_days t;
    t.day = time_in_us / num_microsegundos_tiene_1dia;

    uint32_t us = time_in_us % num_microsegundos_tiene_1dia;

    ldiv_t d = ldiv(us, 1000UL);

    t.us = d.rem;
    d = ldiv(d.quot, 1000UL);
    t.ms = d.rem;
    d = ldiv(d.quot, 60UL);	// Aqui quot ya entra en un uint32_t.
    t.sec = d.rem;
    d = ldiv(d.quot, 60UL);
    t.min = d.rem;
    d = ldiv(d.quot, 24UL);
    t.hour = d.rem;

    return t;
}

std::ostream& operator<<(std::ostream& out, const time_in_days& t)
{
    return out << t.day << "d "
	        << atd::write_as_int(t.hour) << ':'
		<< atd::write_as_int(t.min) << ':'
		<< atd::write_as_int(t.sec) << '.'
		<< t.ms << ':'
		<< t.us;

}

void print(uint64_t time_en_us)
{
    UART_iostream uart;
    uart << us_to_time_in_days(time_en_us) << "\n\r";
}

void test_clock()
{
    init_timer_clock();

    UART_iostream uart;
    while (1){
	Timer::counter_type v;
	uint32_t c;
	{// lo más atómico posible
	    myu::Disable_interrupts l;
	    v = Timer::unsafe_counter();
	    c = counter;
	}
	
	uint64_t t_us = (c*(uint64_t{Timer::max()} + 1)+ v)*period_in_us;
        uart << '(' << c << "*" << (uint64_t{Timer::max()} + 1) << "+" << v << ")*"
             << period_in_us << " = " << t_us << " us = ";

        print (t_us);
	myu::wait_ms(1000);
    }
}

void test_flag_interrupt()
{
    UART_iostream uart;

    myu::disable_interrupts();

    Timer::normal_mode();
    Timer::unsafe_counter(0);	// es safe por haber llamado a disable_interrupts
    Timer::enable_overflow_interrupt();

    Test test{uart};
    uart << '\n';

    CHECK_TRUE(test, Timer::overflow_interrupt_is_set() == false, 
					"Timer::overflow_interrupt_is_set");

    Timer::clock_frequency_no_prescaling(); // turn on
    
    while (!Timer::overflow_interrupt_is_set()) ;

    uart << "\nOVERFLOW INTERRUPT SET!\n";

    // comprobemos que no ha cambiado
    CHECK_TRUE(test, Timer::overflow_interrupt_is_set() == true, 
					"Timer::overflow_interrupt_is_set");

    Timer::clear_overflow_interrupt();
    CHECK_TRUE(test, Timer::overflow_interrupt_is_set() == false, 
					"Timer::clear_overflow_interrupt");

    Timer::off();
}

int main()
{
    init_uart();
    myu::enable_interrupts();

    hello();
    UART_iostream uart;

    while(1){
	uart << "\n\nMenu\n"
	            "----\n"
		    "1. Clock\n"
		    "2. Test flag interrupt\n";
	char opt{};
	uart >> opt;

	switch(opt){
	    break; case '2': test_flag_interrupt();
	    break; default: test_clock();
	}
    }
}


// ISRs
// ----
ISR_TIMER1_OVF
{ counter = counter + 1; }

