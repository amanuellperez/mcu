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


void Main::hello()
{
    uart << "\n\nCar 2 wheels experiment\n"
	        "-----------------------\n"
		"Connections:\n"
		"\tIN1 = " << (int) IN1_pin << 
		"; IN2 = " << (int) IN2_pin << 
		"; IN3 = " << (int) IN3_pin << 
		"; IN4 = " << (int) IN4_pin << 
		"\n\tENA = " << (int) ENA_pin <<
		"; ENB = " << (int) ENB_pin 
		<< "\n\n";
}


void Main::lateral_movement()
{
// TODO: esto con 4 ruedas fijo que funciona, pero ¿cómo hacerlo con 2?
    for (uint8_t i = 0; i < 10; ++i){
	Car::forward_turn_right();
	Micro::wait_ms(100);
	Car::forward_turn_left();
	Micro::wait_ms(100);
    }
    Car::stop();
}


void Main::run()
{
// main_menu():
    uart << "\nMenu\n"
	      "----\n"
	      "0. Stop car\n"
	      "1. Forward\n"
	      "2. Backward\n"
	      "3. Turn right\n"
	      "4. Turn left\n"
	      "5. Turn right while gradually increasing speed\n"
	      "6. Turn left while gradually increasing speed\n"
	      "7. Lateral movement\n";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case '0': Car::stop();
	break; case '1': Car::forward(50);
	break; case '2': Car::backward(50);
	break; case '3': Car::forward_turn_right();
	break; case '4': Car::forward_turn_left();
	break; case '7': lateral_movement();

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


