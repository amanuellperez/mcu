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

// Conectar un teclado de 5 x 5
// El teclado es una matriz de pulsadores.
// Sacamos la salida por UART
#include "../../../dev_keypad.h"
#include "../../../dev_keyboard_code.h"

#include <avr_atmega.h>

namespace my_mcu = atmega;

// Este es el keypad barato que venden en internet de 4 x 3
//using Rows = dev::Keypad_rows<28, 27, 26, 25>;
//using Cols = dev::Keypad_cols<19, 18, 17>;

// Keypad de 5 x 5
using Rows = dev::Keypad_rows<28, 27, 26, 25, 24>;
using Cols = dev::Keypad_cols<19, 18, 17, 16, 15>;

using Keypad = dev::Keypad<Rows, Cols>;


struct Code: public dev::Keyboard_code_kascii
{
    static constexpr uint8_t AC = cancel;
    static constexpr uint8_t ANS = give_a_name1;

};

// Es el teclado inicial de la calculadora básica
constexpr uint8_t ascii_code[25] =
{'0', '.', '^', Code::ANS, '=',
 '1', '2', '3', '+', '-',
 '4', '5', '6', 'x', '/',
 '7', '8', '9', Code::del, Code::AC,
 's', '(', ')', Code::left, Code::right
};


using Keyboard = dev::Keyboard_keypad<Keypad>;


void test_keypad()
{
    my_mcu::UART_iostream uart;
    
    uart << "\n-----\n";
    uart << "Keypad\n";
    uart << "------\n\n";
    uart << "Pulsa una tecla:";

    Keypad keypad;

    while(1){
	while (!keypad.scan())
	    my_mcu::Micro::wait_ms(100);

	uart << "(" << (int) keypad.row() << ", " << (int) keypad.col() << ") = "
	    << (int) keypad.last_key() << '\n';
	my_mcu::Micro::wait_ms(100);
    }
}

void test_keyboard()
{
    my_mcu::UART_iostream uart;
    
    uart << "\n-----\n";
    uart << "Keyboard\n";
    uart << "------\n\n";
    uart << "Pulsa una tecla:";

    Keyboard keyboard{ascii_code};

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

	my_mcu::Micro::wait_ms(100); // debouncing
    }
}






int main()
{
// init_UART();
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
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



