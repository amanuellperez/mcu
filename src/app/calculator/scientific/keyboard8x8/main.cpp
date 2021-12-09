// Copyright (C) 2021 A.Manuel L.Perez 
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
/****************************************************************************
 *
 *  - DESCRIPCION: Prueba de teclado 8x8
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    09/12/2021 v0.0
 *
 ****************************************************************************/
// Conectar un teclado de 8 x 8
// El teclado es una matriz de pulsadores.
// Sacamos la salida por UART
#include <avr_UART.h>
#include <dev_keypad.h>
#include <dev_keyboard_code.h>

// Keypad de 5 x 5
// No usar: UART (pines 2, 3) y SPI (pines 16-19)
using Rows = dev::Keypad_rows<6, 15, 14, 13, 12, 11, 10, 9>;
using Cols = dev::Keypad_cols<28, 27, 26, 25, 24, 23, 4, 5>;

using Keypad = dev::Keypad<Rows, Cols>;


struct Code: public dev::Keyboard_code_kascii
{
    static constexpr uint8_t AC = cancel;
    static constexpr uint8_t ANS = give_a_name1;

};

// Es el teclado inicial de la calculadora básica
constexpr uint8_t ascii_code[64] =
{
'1','2','3','4','5','6','7','8',
'9','0','a','b','c','d','e','f',
'g','h','i','j','k','l','m','n',
'o','p','q','r','s','t','u','v',
'w','x','y','z','A','B','C','D', 
'E','F','G','H','I','J','K','L',
'M','N','O','P','Q','R','S','T',
Code::ANS, Code::del, Code::AC, Code::left, Code::right
,'(', ')', '+'
};


using Keyboard = dev::Keyboard_keypad<Keypad, Code>;


void test_keypad()
{
    avr::UART_iostream uart;
    
    uart << "\n-----\n";
    uart << "Keypad\n";
    uart << "------\n\n";
    uart << "Pulsa una tecla:";

    Keypad keypad;

    while(1){
	while (!keypad.scan())
	    wait_ms(100);

	uart << "(" << (int) keypad.row() << ", " << (int) keypad.col() << ") = "
	    << (int) keypad.last_key() << '\n';
	wait_ms(100);
    }
}

void test_keyboard()
{
    avr::UART_iostream uart;
    
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

	wait_ms(100); // debouncing
    }
}






int main()
{
// init_UART();
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();
 
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



