// Copyright (C) 2023 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

namespace mcu = atmega;
using Micro   = mcu::Micro;

using Clock = dev::Clock_s<Micro, mcu::Time_counter1_g>;

ISR_TIMER1_COMPA
{
    Clock::tick();
}



std::ostream& operator<<(std::ostream& out, const Clock::Date_time& t)
{
    
    atd::print(out, t.day(), nm::Width{2});
    out << '/';
    atd::print(out, t.month(), nm::Width{2});
    out << '/';
    atd::print(out, t.year(), nm::Width{2});
    out << ' ';

    atd::print(out, t.hours(), nm::Width{2});
    out << ':';
    atd::print(out, t.minutes(), nm::Width{2});
    out << ':';
    atd::print(out, t.seconds(), nm::Width{2});

    return out;
}

void run_test()
{
    mcu::UART_iostream uart;
    for (uint8_t i = 0; i < 10; ++i){
	if (!(i % 5))
	    uart << "-------------------\n";

	uart << Clock::now_as_date_time() << '\n';
	Micro::wait_ms(1000);
    }
}

void test_leap_year()
{
    mcu::UART_iostream uart;

    uart << "\n\nTest leap year\n";

    Clock::Date_time dt;
    dt.day(28);
    dt.month(2);
    dt.year(2000); // es bisiesto!!!
    dt.hours(23);
    dt.minutes(59);
    dt.seconds(55); 

    Clock::set(dt);

    run_test();
}


void test_not_leap_year()
{
    mcu::UART_iostream uart;

    uart << "\n\nTest not leap year\n";

    Clock::Date_time dt;
    dt.day(28);
    dt.month(2);
    dt.year(2001); // no es bisiesto!!!
    dt.hours(23);
    dt.minutes(59);
    dt.seconds(55); 

    Clock::set(dt);

    run_test();
}


void test_end_year()
{
    mcu::UART_iostream uart;

    uart << "\n\nTest end year\n";

    Clock::Date_time dt;
    dt.day(31);
    dt.month(12);
    dt.year(2022); // es bisiesto!!!
    dt.hours(23);
    dt.minutes(59);
    dt.seconds(55); 

    Clock::set(dt);

    run_test();

}

void test_clock()
{
    mcu::UART_iostream uart;

    uart << "\n\nClock test\n"
	        "----------\n";

    while(1){
	test_leap_year();
	test_not_leap_year();
	test_end_year();

    }
}


void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();
}



void init_timer()
{
    Clock::on();
    Micro::enable_interrupts();

}


int main()
{
    init_uart();
    init_timer();

    test_clock();
}


