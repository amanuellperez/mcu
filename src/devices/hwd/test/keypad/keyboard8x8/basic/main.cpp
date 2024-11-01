// Copyright (C) 2021 Manuel Perez 
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
/****************************************************************************
 *
 *  - DESCRIPCION: Prueba de teclado 8x8
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    09/12/2021 v0.0
 *
 ****************************************************************************/
// Conectar un teclado de 8 x 8
// El teclado es una matriz de pulsadores.
// Sacamos la salida por UART
#include <avr_atmega.h>
#include "../dev.h"

// Microcontroller
// ---------------
namespace myu = atmega;
using UART = myu::UART;
using UART_iostream = mcu::UART_iostream<UART>;

void test_keypad()
{
    UART_iostream uart;
    
    uart << "\n-----\n";
    uart << "Keypad\n";
    uart << "------\n\n";
    uart << "Pulsa una tecla:";

    Keypad keypad;

    while(1){
	while (!keypad.scan())
	    myu::Micro::wait_ms(100);

	uart << "(" << (int) keypad.row() << ", " << (int) keypad.col() << ") = "
	    << (int) keypad.last_key() << '\n';
	myu::Micro::wait_ms(100);
    }
}

void test_keyboard()
{
    UART_iostream uart;
    
    uart << "\n-----\n";
    uart << "Keyboard\n";
    uart << "------\n\n";
    uart << "Pulsa una tecla:";

    Keyboard keyboard{key_code};

    while(1){
	uint8_t c = keyboard.getchar();
	switch(c){
	    case Code::del: uart << "DEL"; break;
	    case Code::AC: uart << "AC"; break;
	    case Code::left: uart << "left"; break;
	    case Code::right: uart << "right"; break;
	    case Code::ANS: uart << "ANS"; break;
	    default: uart << c; break;
	}

	myu::Micro::wait_ms(100); // debouncing
    }
}






int main()
{
// init_UART();
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();
 
    uart << "\n------------\n";
    uart << "Keypad" << (int)Keypad::nrows() << "x"
         << (int)Keypad::ncols() << "\n";
    uart << "------------\n\n";
    uart << "1. Test keypad\n"
	    "2. Test keyboard_keypad\n";

    char c{};
    uart >> c;
    if (c == '1')
	test_keypad();

    else
	test_keyboard();

}



