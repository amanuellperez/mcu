// Copyright (C) 2022 A.Manuel L.Perez 
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

#include "../../dev_miniclocks.h"
#include <avr_atmega.h>

// Devices
// -------
namespace mcu = atmega;

constexpr uint8_t test_pin = 14;

using Pin = mcu::Output_pin<test_pin>;

using Miniclock0_us = dev::Miniclock_us<mcu::Timer0_generic_counter>;
using Miniclock0_ms = dev::Miniclock_ms<mcu::Timer0_generic_counter>;

using Miniclock1_us = dev::Miniclock_us<mcu::Timer1_generic_counter>;
using Miniclock1_ms = dev::Miniclock_ms<mcu::Timer1_generic_counter>;

// Functions
// ---------
enum class Cfg{ 
    miniclock0_us, miniclock0_ms,
    miniclock1_us, miniclock1_ms
};

void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();
}


template <typename Miniclock_us>
void generate_square_wave_us(Pin& pin, uint16_t time_us)
{
    while (1){
	Miniclock_us::start();
	while (Miniclock_us::read() < time_us){
	    // This takes a lot of time ==> this function can't generate
	    // a square wave of microseconds!!!
	    if (mcu::UART_basic::are_there_data_unread())
		return;
	}
	pin.toggle();
    }
}

template <typename Miniclock_ms>
void generate_square_wave_ms(Pin& pin, uint16_t time_ms)
{
    while (1){
	Miniclock_ms::start();
	while (Miniclock_ms::read() < time_ms){
	    if (mcu::UART_basic::are_there_data_unread())
		return;
	}
	pin.toggle();
    }
}


void print_menu()
{
    mcu::UART_iostream uart;

    uart << "1. Miniclock0_us\n"
	    "2. Miniclock0_ms\n"
	    "3. Miniclock1_us\n"
	    "4. Miniclock1_ms\n"
	    "Other key to continue\n";
}

void ask_clock(Cfg& cfg)
{
    mcu::UART_iostream uart;

    char c{};
    uart >> c;
    
    switch(c){
	break; case '1':  cfg = Cfg::miniclock0_us;
	break; case '2':  cfg = Cfg::miniclock0_ms;
	break; case '3':  cfg = Cfg::miniclock1_us;
	break; case '4':  cfg = Cfg::miniclock1_ms;
    }
}


template <typename Miniclock_ms>
void generate_ms(Pin& pin)
{
    mcu::UART_iostream uart;
    uint16_t time_ms;

    uart << "Write semiperiod, in ms, of square wave to generate:\n"
	    "(max value of " << (uint32_t) Miniclock_ms::counter_max() << ')';
    uart >> time_ms;
    uart << "\nGenerating a square wave of " << (2*time_ms) << " ms\n";
    print_menu();

    generate_square_wave_ms<Miniclock_ms>(pin, time_ms);
}


template <typename Miniclock_us>
void generate_us(Pin& pin)
{
    mcu::UART_iostream uart;
    uint16_t time_us;

    uart << "\nWARNING: in microseconds this test doesn't work correctly\n"
	    "(which is normal, look at the code)\n\n";
    uart << "Write semiperiod, in us, of square wave to generate:\n"
	    "(max value of " << (uint32_t) Miniclock_us::counter_max() << ')';
    uart >> time_us;
    uart << "\nGenerating a square wave of " << (2*time_us) << " us\n";
    print_menu();

    generate_square_wave_us<Miniclock_us>(pin, time_us);
}

int main()
{
    Pin pin;

    init_uart();
    Miniclock0_ms::init();

    mcu::UART_iostream uart;
    Cfg cfg = Cfg::miniclock0_ms;

    uart << "\n\nMiniclock test\n"
	        "--------------\n"
		"Connect oscilloscope to pin " << (int) pin.number << '\n';
    while(1){
	print_menu();
	ask_clock(cfg);

	switch(cfg){
	    break; case Cfg::miniclock0_us: generate_us<Miniclock0_us>(pin);
	    break; case Cfg::miniclock0_ms: generate_ms<Miniclock0_ms>(pin);
	    break; case Cfg::miniclock1_us: generate_us<Miniclock1_us>(pin);
	    break; case Cfg::miniclock1_ms: generate_ms<Miniclock1_ms>(pin);
	}
    }
}




