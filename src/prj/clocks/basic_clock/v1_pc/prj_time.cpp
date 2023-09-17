// Copyright (C) 2023 Manuel Perez 
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

#include "prj_main.h"

void Main::print_time()
{
//    print_datetime(uart, Clock::now_as_date_time());
    uart << "NUEVO\n";
    for (int i = 1; i < 40; ++i)
	uart << i << ';';

    uart << "FIN\n";
    while(!mcu::UART_basic::is_ready_to_transmit()) 
	    ;   
//    uart << "por que no imprime el segundo retorno de carro?\n";
    Micro::wait_ms(1);
}

