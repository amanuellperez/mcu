// Copyright (C) 2024 Manuel Perez 
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
#include <pli_iostream.h>

void Main::hello()
{
    uart << "\n\nDC motor speed control\n"
	        "----------------------\n"
		"Connections:\n"
		"\tIN1 = " << (int) IN1_pin << 
		"; IN2 = " << (int) IN2_pin << 
//		"; IN3 = " << (int) IN3_pin << 
//		"; IN4 = " << (int) IN4_pin << 
		"\n\tENA = " << (int) ENA_pin <<
//		"; ENB = " << (int) ENB_pin 
		"\nSpeed sensor pin = " << (int) speed_sensor_pin <<
		"\n\nN = " << (int) speed_sensor_N <<
		"\n\n";

}


void Main::run()
{
// main_menu():
    uart << "\nMenu\n"
	      "----\n"
	      "0. Stop car\n"
	      "1. Turn\n"
	      "2. Measure speed\n"
	      "3. Automatic table (percentage, speed)\n";

    char opt{};
    uart >> opt;
    uart << '\n';

    switch (opt){
	break; case '0': Motor::stop();
	break; case '1': turn();
	break; case '2': measure_speed();
	break; case '3': table_speed();

	break; default : uart << "What?\n";
    }
    
}

int main() 
{
    Main app;

    app.hello();

    while (1){
	app.run();
    }
}


