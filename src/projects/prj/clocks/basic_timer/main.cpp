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

#include "main.h"

Main::Main()
{
// init_hardware();
    init_lcd();
    init_keyboard();
    init_chronometer();

    Micro::enable_interrupts();

// init_data();
    alarm_time = std::chrono::minutes{0};
}



void Main::init_lcd()
{
}

void Main::init_chronometer()
{
//    Chronometer::start(); // No tiene sentido que empiece encendido!!!
}


void Main::run()
{
    print_time();

    while(1){
//	if (errno_)
//	    error();
//	else 
	    window_main();

	Micro::wait_ms(100); // es un reloj de horno. Miro el teclado cada 100 ms
//	Micro::wait_ms(10); // es un cronometro que funciona a 1 ms. 
//		    // Actualizo el LCD cada 1 ms (si se elimina este mcu::Micro::wait_ms
//		    // el LCD se actualiza tan rápido que no se ve nada)
    }
}



int main()
{
    Main app;
    app.run();
}




