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

#include "../../../dev_push_button.h"
#include "../../../dev_led.h"
#include <avr_atmega.h>

// Microcontroller
// ---------------
namespace mcu = atmega;
using Micro = mcu::Micro;

// Pin connections
// ---------------
static constexpr uint8_t led_pin    = 13;
static constexpr uint8_t button_pin = 14;
#define ISR_BUTTON ISR_PCINT_PIN14

// Devices
// -------
using LED    = dev::LED<Micro, led_pin>;
using Button = dev::Push_button<Micro, button_pin>;

// Global vbles
// ------------
// Terminal
mcu::UART_iostream uart;

// Functions
// ---------
void init_uart()
{
    mcu::basic_cfg(uart);
    uart.turn_on();
}

void main_hello()
{
    uart << "\n\nPush button test\n"
	        "----------------\n"
		"Connect a push button in pin " << (int) button_pin 
		<< " and a LED in pin " << (int) led_pin << "\n\n";

}

void normal_test()
{
    uart << "\nNormal test\n"
	    "-----------\n"
	    "When the button is pressed the LED turns on\n"
	    "and when it is released the LED turns off.\n"
	    "Press a key to end test.\n";

    while (1){
	if (Button::is_pressed())
	    LED::turn_on();
	else 
	    LED::turn_off();

	if (uart.is_there_something_to_read()){
	    char c{}; // uart.empty_buffer()???
	    uart >> c;
	    return; 
	}
	Micro::wait_ms(100);
    }
}

void test_interrupts()
{
    while(1){
	uart << "\nTest interrupts\n"
	        "---------------\n"
		"1. Enable interrupts\n"
		"2. Disable interrupts\n"
		"3. Return main menu\n";

	char opt{};
	uart >> opt;
	switch (opt){
	    break; case '1': Button::enable_interrupt();
			    uart << "Interrupts enabled.\n"
				    "When you press the button the LED "
				    "has to change state.\n\n";

	    break; case '2': Button::disable_interrupt();
			    LED::turn_off();
			    uart << "Interrupts disabled."
				    "If you press now the button nothing "
				    "happens.\n\n";
	    break; case '3': return;
	    break; default: uart << "Unknown option.\n";
	}
    }
}

void init()
{
    init_uart();
    Button::init();
    LED::init();
    Micro::enable_interrupts();
}

// Main
// ----
int main()
{
    init();

    main_hello();

    while(1){
	uart << "\nMenu\n"
	        "----\n"
		"1. Normal test\n"
		"2. Test interrupts\n";

	char opt{};
	uart >> opt;
	switch(opt){
	    break; case '1': normal_test();
	    break; case '2': test_interrupts();
	    break; default: uart << "Unknown option\n";
	}

    }
}


ISR_BUTTON{
    uart << "Interrupt\n";
    LED::toggle();
}
