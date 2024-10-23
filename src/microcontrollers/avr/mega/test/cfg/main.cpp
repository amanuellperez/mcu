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

// Probamos el UART.
// Conectar el FTDI y abrir screen. Lo que se escriba en teclado se envia
// al microcontrolador que lo devuelve, con lo que lo vemos en pantalla.
#include "../../mega_private.h"
#include "../../mega_UART_iostream.h"





void test_cfg()
{
    mega_::UART_iostream uart;
    mega_::basic_cfg(uart);
    uart.turn_on();

    while(1){
	uart << "\n---------------\n";
	uart << "Variables cfg!\n";
	uart << "---------------\n";

	uart << "clock_frequency = " << mega_::clock_frequency << '\n';
	char c{};
	uart >> c;
    }

}


int main()
{
    test_cfg();
}

