// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
// El teclado es una matriz de pulsadores. En las filas conectamos diodos para
// evitar cortocircuitos, y en las columnas al conectar los pines los
// configuramos con pullup resistor.
// Sacamos la salida por UART
#include <avr_UART.h>
#include "../keyboard_5x5.h"


using Rows = Keyboard_5_rows<28, 27, 26, 25, 24>;
using Cols = Keyboard_5_cols<19, 18, 17, 16, 15>;
using Keyboard = Keyboard_5x5<Rows, Cols>;

int main()
{
// init_UART();
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();
    
    uart << "\n----------\n";
    uart << "Keyboard " << (int)Keyboard::nrows() << "x"
         << (int)Keyboard::ncols() << "\n";
    uart << "------------\n\n";
    uart << "Press a key\n";

    Keyboard keyboard;

    while(1){
	while (!keyboard.scan())
	    wait_ms(100);

	uart << "(" << (int) keyboard.row << ", " << (int) keyboard.col << ") = "
	    << (int) keyboard.last_key() << '\n';
	wait_ms(100);
    }
}



