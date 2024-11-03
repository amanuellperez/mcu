// Copyright (C) 2022 Manuel Perez 
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

#include "../../mcu_miniclock.h"
#include <avr_atmega.h>

// Devices
// -------
namespace myu = atmega;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

constexpr uint8_t test_pin = 14;

using Pin = myu::Pin<test_pin>;

using Miniclock0_us = mcu::Miniclock_us<myu::Micro, myu::Time_counter0>;
using Miniclock0_ms = mcu::Miniclock_ms<myu::Micro, myu::Time_counter0>;

using Miniclock1_us = mcu::Miniclock_us<myu::Micro, myu::Time_counter1>;
using Miniclock1_ms = mcu::Miniclock_ms<myu::Micro, myu::Time_counter1>;


// Functions
// ---------
enum class Cfg{ 
    miniclock0_us, miniclock0_ms,
    miniclock1_us, miniclock1_ms
};

void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}


template <typename Miniclock, typename Pin>
void generate_square_wave_no_use_wait( typename Miniclock::counter_type time_ms_or_us)
{
    UART_iostream uart;
    uart << "Not using Miniclock::wait() function\n";

    while (1){
	Miniclock::start();
	while (Miniclock::time() < time_ms_or_us){
	    // This takes a lot of time in us ==> this function can't generate
	    // a square wave of microseconds!!!
	    if (myu::UART_basic::are_there_unread_data())
		return;
	}
	Pin::toggle();
    }
}



template <typename Miniclock, typename Pin>
void generate_square_wave_use_wait(typename Miniclock::counter_type time_ms_or_us)
{
    UART_iostream uart;
    uart << "Using Miniclock::wait() function\n";

    while (1){
	Miniclock::start();
	Miniclock::wait(time_ms_or_us);
	Pin::toggle();

	if (myu::UART_basic::are_there_unread_data())
	    return;
    }
}



void print_menu()
{
    UART_iostream uart;

    uart << "\n----------------\n"
	    "1. Miniclock0_us\n"
	    "2. Miniclock0_ms\n"
	    "3. Miniclock1_us\n"
	    "4. Miniclock1_ms\n"
	    "5. Use/don't use wait function\n"
	    "Other key to continue\n"
	    "----------------\n\n";
}

void ask_clock(Cfg& cfg, bool& use_wait)
{
    UART_iostream uart;

    char c{};
    uart >> c;
    
    switch(c){
	break; case '1':  cfg = Cfg::miniclock0_us;
	break; case '2':  cfg = Cfg::miniclock0_ms;
	break; case '3':  cfg = Cfg::miniclock1_us;
	break; case '4':  cfg = Cfg::miniclock1_ms;
	break; case '5':  use_wait = !use_wait;
    }
}


template <typename Miniclock_ms, typename Pin>
void generate_ms(bool use_wait)
{
    UART_iostream uart;
    uint16_t time_ms;

    uart << "\nWrite semiperiod, in ms, of square wave to generate:\n"
	    "(max value of " << (uint32_t) Miniclock_ms::counter_max() << ')';
    uart >> time_ms;
    uart << "\nGenerating a square wave of " << (2*time_ms) << " ms\n";


    print_menu();

    if (use_wait)
	generate_square_wave_no_use_wait<Miniclock_ms, Pin>(time_ms);

    else
	generate_square_wave_use_wait<Miniclock_ms, Pin>(time_ms);
}


template <typename Miniclock_us, typename Pin>
void generate_us(bool use_wait)
{
    UART_iostream uart;
    uint16_t time_us;

    uart << "\nWARNING: in microseconds this test doesn't work correctly\n"
	    "(which is normal, look at the code)\n\n";
    uart << "Write semiperiod, in us, of square wave to generate:\n"
	    "(max value of " << (uint32_t) Miniclock_us::counter_max() << ')';
    uart >> time_us;
    uart << "\nGenerating a square wave of " << (2*time_us) << " us\n";

    print_menu();

    if (use_wait)
	generate_square_wave_no_use_wait<Miniclock_us, Pin>(time_us);

    else
	generate_square_wave_use_wait<Miniclock_us, Pin>(time_us);
}

int main()
{
    Pin::as_output();

    init_uart();
    Miniclock0_ms::init();

    UART_iostream uart;
    Cfg cfg = Cfg::miniclock0_ms;
    bool use_wait = false;

    uart << "\n\nMiniclock test\n"
	        "--------------\n"
		"Connect oscilloscope to pin " << (int) Pin::number << '\n';
    while(1){
	print_menu();
	ask_clock(cfg, use_wait);

	switch(cfg){
	    break; case Cfg::miniclock0_us: 
			generate_us<Miniclock0_us, Pin>(use_wait);

	    break; case Cfg::miniclock0_ms: 
			generate_ms<Miniclock0_ms, Pin>(use_wait);

	    break; case Cfg::miniclock1_us: 
			generate_us<Miniclock1_us, Pin>(use_wait);

	    break; case Cfg::miniclock1_ms: 
			generate_ms<Miniclock1_ms, Pin>(use_wait);
	}
    }
}




