// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

// Conectar 3 pulsadores a los pines indicados. La salida la mostramos por
// UART.
#include <avr_UART.h>
#include "../../dev_keyboard.h"

#include <avr_time.h>

// pines a los que conectamos el teclado
using Keyboard_2pins = dev::Keyboard_pins<24, 25>;
using Keyboard_pins  = dev::Keyboard_pins<23, 24, 25>;

// código asociado a cada tecla del teclado
using namespace dev::Key_codes; // OK_KEY, UP_KEY, DOWN_KEY
using Keyboard_2codes = dev::Keyboard_codes<UP_KEY, DOWN_KEY>;
using Keyboard_codes  = dev::Keyboard_codes<OK_KEY, UP_KEY, DOWN_KEY>;

// dispositivos que conectamos
using Keyboard = dev::Basic_keyboard<Keyboard_pins, Keyboard_codes>;
//using Keyboard = dev::Basic_keyboard<Keyboard_2pins, Keyboard_2codes>;


void test_keyboard()
{
    avr::UART_iostream uart;
    uart << "\n\nPress test\n";

    Keyboard keyboard;

    while (1){
	if (Keyboard::key<OK_KEY>().is_pressed())
	    uart << "enter\n" << std::flush;

	if (keyboard.key<UP_KEY>().is_pressed())
	    uart << "up\n" << std::flush;

	if (keyboard.key<DOWN_KEY>().is_pressed())
	    uart << "down\n" << std::flush;

	// CHECK_DONT_COMPILE
//	if (keyboard.key<100>().is_pressed())
//	  uart << "error no tiene que compilar\n" << std::flush;
	wait_ms(100);
    }
}

void test_scan()
{
    avr::UART_iostream uart;

    uart << "\n\nRead test\n";

    Keyboard keyboard;
    
    while(1){
	uint8_t c = keyboard.scan();
	switch(c){
	    case OK_KEY: uart << "ok\n"; break;
	    case UP_KEY: uart << "up\n"; break;
	    case DOWN_KEY: uart << "down\n"; break;
	    case NO_KEY: uart << "none\n"; break;
	}
	wait_ms(100);
    }

}


int main()
{
// init_uart():
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

    uart << "\n\nKeyboard test\n"
	        "-------------\n";

    uart << "Menu:\n"
	    "[1]. Test press functions\n"
	    "[2]. Test scan function\n";

    char c{};
    uart >> c;

    if (c == '1')
	test_keyboard();
    else
	test_scan();
}



