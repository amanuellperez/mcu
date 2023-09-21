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

void Main::hello()
{
    uart << "\n\nTemperature datalogger\n"
	        "----------------------\n"
	    "Connect DS18B20 to pin " << (int) one_wire_pin 
					<< " with a 4.7k pull-up resistor\n"
	    "and a cristal of 32kHz in pins TOSC1 and TOSC2\n\n";
}



void Main::run()
{
    if (show_main_menu_){
	Micro::Disable_interrupts lock;

	menu_main();
	show_main_menu_ = false;
    }

    else {
	Clock::time_point t = atomic_now();

	if (t  >= time_next_alarm_){
	    print_temperature(uart);
	    update_alarm();
	}
    }

    // uart.flush(); <-- fundamental en power-save mode
    Micro::sleep(); // Se despierta cada segundo 
}


int main()
{
    Main app;

    app.hello();
    Micro::wait_ms(500); // Según la datasheet se necesita 1 segundo para que 
			 // el cristal de 32kHz se estabilice. (ver modo
			 // asyncrhono, Timer2)
    while (1){
	app.run();
    }
}



