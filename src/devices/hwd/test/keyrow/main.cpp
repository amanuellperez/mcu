// Copyright (C) 2019-2021 Manuel Perez 
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

// Conectar 3 pulsadores a los pines indicados. La salida la mostramos por
// UART.
#include "../../dev_keyrow.h"

#include <avr_atmega.h>

// Microcontroller
// ----------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

// KeyRow
// ------
// pines a los que conectamos el teclado
using Keyrow_2pins = dev::Keyrow_pins<24, 25>;
using Keyrow_pins  = dev::Keyrow_pins<23, 24, 25>;

// código asociado a cada tecla del teclado
using namespace dev::Key_codes; // OK_KEY, UP_KEY, DOWN_KEY
using Keyrow_2codes = dev::Keyrow_codes<UP_KEY, DOWN_KEY>;
using Keyrow_codes  = dev::Keyrow_codes<OK_KEY, UP_KEY, DOWN_KEY>;

// dispositivos que conectamos
using Keyrow = dev::Basic_keyrow<Micro, Keyrow_pins, Keyrow_codes>;
//using Keyrow = dev::Basic_keyrow<Keyrow_2pins, Keyrow_2codes>;


void test_keyrow()
{
    UART_iostream uart;
    uart << "\n\nPress test\n";

    Keyrow keyrow;

    while (1){
	if (Keyrow::key<OK_KEY>().is_pressed())
	    uart << "enter\n" << std::flush;

	if (keyrow.key<UP_KEY>().is_pressed())
	    uart << "up\n" << std::flush;

	if (keyrow.key<DOWN_KEY>().is_pressed())
	    uart << "down\n" << std::flush;

	// CHECK_DONT_COMPILE
//	if (keyrow.key<100>().is_pressed())
//	  uart << "error no tiene que compilar\n" << std::flush;
	myu::Micro::wait_ms(100);
    }
}

void test_scan()
{
    UART_iostream uart;

    uart << "\n\nRead test\n";

    Keyrow keyrow;
    
    while(1){
	uint8_t c = keyrow.scan();
	switch(c){
	    case OK_KEY: uart << "ok\n"; break;
	    case UP_KEY: uart << "up\n"; break;
	    case DOWN_KEY: uart << "down\n"; break;
	    case NO_KEY: uart << "none\n"; break;
	}
	myu::Micro::wait_ms(100);
    }

}


int main()
{
// init_uart():
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

    uart << "\n\nKeyrow test\n"
	        "-------------\n";

    uart << "Menu:\n"
	    "[1]. Test press functions\n"
	    "[2]. Test scan function\n";

    char c{};
    uart >> c;

    if (c == '1')
	test_keyrow();
    else
	test_scan();
}



