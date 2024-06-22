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

// Microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;


// Devices
// -------
using Time_counter = my_mcu::Time_counter2_32kHz_g<3000>;
using Clock2 = dev::Clock_s<Micro, Time_counter>;


// Functions
// ---------
template <typename Date_time>
std::ostream& operator<<(std::ostream& out, const Date_time& t)
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


template <typename Clock>
void run_test()
{
    my_mcu::UART_iostream uart;

    for (uint8_t i = 0; i < 10; ++i){
	if (!(i % 5))
	    uart << "-------------------\n\t";

	uart << Clock::now_as_date_time() << '\n';
	Micro::wait_ms(1000);
    }
}

template <typename Clock>
void test_leap_year()
{
    my_mcu::UART_iostream uart;

    uart << "\n\nTest leap year\n";

    typename Clock::Date_time dt;
    dt.day(28);
    dt.month(2);
    dt.year(2000); // es bisiesto!!!
    dt.hours(23);
    dt.minutes(59);
    dt.seconds(55); 

    Clock::set(dt);

    run_test<Clock>();
}

template <typename Clock>
void test_not_leap_year()
{
    my_mcu::UART_iostream uart;

    uart << "\n\nTest not leap year\n";

    typename Clock::Date_time dt;
    dt.day(28);
    dt.month(2);
    dt.year(2001); // no es bisiesto!!!
    dt.hours(23);
    dt.minutes(59);
    dt.seconds(55); 

    Clock::set(dt);

    run_test<Clock>();
}


template <typename Clock>
void test_end_year()
{
    my_mcu::UART_iostream uart;

    uart << "\n\nTest end year\n";

    typename Clock::Date_time dt;
    dt.day(31);
    dt.month(12);
    dt.year(2022); // es bisiesto!!!
    dt.hours(23);
    dt.minutes(59);
    dt.seconds(55); 

    Clock::set(dt);

    run_test<Clock>();

}




void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}



template <typename Clock>
void test(const char* name)
{
    my_mcu::UART_iostream uart;
    uart << "\n\n" << name << " test\n"
		"----------\n";
    test_leap_year<Clock>();
    test_not_leap_year<Clock>();
    test_end_year<Clock>();
}


int main()
{
    init_uart();
    Micro::enable_interrupts();

    if (Clock2::turn_on() == false){
	my_mcu::UART_iostream uart;
	uart << "ERROR: Clock2::turn_on fail!!!\n";
    }

    while(1){
	test<Clock2>("Clock2");
    }

}



// Interrupts
// ----------
ISR_TIMER2_COMPA
{
    Clock2::tick();
    if (Clock2::is_new_second()){
	my_mcu::UART_iostream uart;
	uart << "tick2: ";
    }
}

