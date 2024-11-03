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
#include "../../mega_import_avr.h"
#include "../../mega_UART.h"
#include <mcu_UART_iostream.h>


// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;



void test_cfg()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

    while(1){
	uart << "\n---------------\n";
	uart << "Variables cfg!\n";
	uart << "---------------\n";

	uart << "clock_frequency = " << myu::clock_frequency << '\n';
	char c{};
	uart >> c;
    }

}


int main()
{
    test_cfg();
}

