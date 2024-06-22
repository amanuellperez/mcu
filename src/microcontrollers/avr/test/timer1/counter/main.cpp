// Copyright (C) 2019-2020 Manuel Perez 
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

#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_UART_iostream.h"
#include "../../../avr_timer1_basic.h"
#include "../../../avr_time.h"
#include "../../../avr_interrupt.h"
#include "../../../avr_micro.h"


#include <atd_ostream.h>
#include <stdlib.h>
#include <std_type_traits.h>

namespace my_mcu = avr_;

using Timer = my_mcu::Timer1;

constexpr uint16_t period_in_us = 1024;

volatile uint32_t counter = 0;

ISR_TIMER1_OVF
{ counter = counter + 1; }

 
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
    my_mcu::UART_iostream uart;
    uart << us_to_time_in_days(time_en_us) << "\n\r";
}


int main()
{
// init_uart();
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();

// init_timer();
    Timer::normal_mode();
    Timer::pin_A_disconnected(); // (???) creo que es innecesario 
    Timer::pin_B_disconnected();
    Timer::clock_frequency_divide_by_1024();
    Timer::enable_overflow_interrupt();
    my_mcu::enable_interrupts();

    while(1){
	Timer::counter_type v;
	uint32_t c;
	{// lo más atómico posible
	    my_mcu::Disable_interrupts l;
	    v = Timer::unsafe_counter();
	    c = counter;
	}
	
	uint64_t t_us = (c*(uint64_t{Timer::max()} + 1)+ v)*period_in_us;
        uart << '(' << c << "*" << (uint64_t{Timer::max()} + 1) << "+" << v << ")*"
             << period_in_us << " = " << t_us << " us = ";

        print (t_us);
	my_mcu::wait_ms(1000);
    }
}



