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


void Main::main_hello()
{
    uart << "\n\nBasic RTC\n"
	        "---------\n"
		"Remember to connect external crystal of 32 kHz\n\n";

    Micro::wait_ms(10); // para que haga el flush del hello
}

void Main::run()
{
    main_hello();

    Micro::wait_ms(500); // Según la datasheet se necesita 1 segundo para que 
			 // el cristal de 32kHz se estabilice. (ver modo
			 // asyncrhono, Timer2)

    while (1) {
	Micro::sleep();
//	Micro::wait_ms(1);

	print_time();
    }
}


int main()
{
    Main app;
    app.run();
}

