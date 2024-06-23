// Copyright (C) 2024 Manuel Perez 
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

#include "../../dev_A4988.h"
#include <avr_atmega.h>

#include <atd_magnitude.h>


// Microcontroller
// ---------------
// myu = my_mcu (???) <-- today, I like myu! Tomorrow... :?
namespace myu = atmega;
using Micro   = myu::Micro;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------
// static constexpr uint8_t NO_ENABLE = mcu::Pin::floating;
static constexpr uint8_t DIR_pin = 14;
static constexpr uint8_t STEP_pin = 15;
using STEP_SWG_pin = myu::SWG1_pin<STEP_pin>;

// Devices
// -------
using A4988_STEP_AND_DIR = dev::A4988_STEP_AND_DIR<STEP_SWG_pin, DIR_pin>;
using A4988_pins = dev::A4988_pins<A4988_STEP_AND_DIR>;

using A4988 = dev::A4988_basic<Micro, A4988_pins>;


// Functions
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nA4988 test\n"
	        "----------\n"
		"IMPORTANT: connect an electrolytic capacitor of 100 uF (minimum 47 uF)!!!\n"
		"Connections:\n"
		"\tDIR = " << (int) DIR_pin << 
		"; STEP = " << (int) STEP_pin << 
		"\n\n";
}

void test_step()
{
    myu::UART_iostream uart;
    uart << "\nFrequency (in Hz): ";
    uint32_t freq;
    uart >> freq;

    if (freq == 0)
	return;

    uart << "Number of steps: ";
    A4988::NSteps_t nsteps;
    uart >> nsteps;

    A4988::step(atd::Hertz<uint32_t>{freq}, nsteps);
}


// Main
// ----
int main() 
{
    init_uart();
    A4988::init();

    hello();

    Micro::enable_interrupts();
    myu::UART_iostream uart;

    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. step\n";

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '1': test_step();

	    break; default: uart << "What???\n";

	}
    }
}


ISR_TIMER1_CAPT{
    STEP_SWG_pin::handle_interrupt();
}
