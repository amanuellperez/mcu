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
#include "pli_iostream.h"

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


void Main::lateral_movement(bool automatic, 
			    atd::Sign sign, uint8_t right, uint8_t left)
{
    Car::move_opposite(sign, right, left);

    Micro::wait_ms(100);
    if (!automatic){
	Car::stop();
	pli::press_key(uart);
    }
}

void Main::lateral_movement()
{
    uart << "\nResearching lateral movement\n";
//    bool automatic = ask_yesno(uart, "Automatic");

    for (uint8_t i = 0; i < 10; ++i){
	Car::move(atd::Sign::negative, 0, 50);
    Micro::wait_ms(150);
    Car::stop();
    pli::press_key(uart);

	Car::backward(50);
    Micro::wait_ms(150);
    Car::stop();
    pli::press_key(uart);

	Car::move(atd::Sign::negative, 50, 0);
    Micro::wait_ms(150);
    Car::stop();
    pli::press_key(uart);
	Car::forward(50);

    Micro::wait_ms(150);
    Car::stop();
    pli::press_key(uart);
//	lateral_movement(automatic, atd::Sign::positive, 0, 50);
//	lateral_movement(automatic, atd::Sign::positive, 50, 0);
//	lateral_movement(automatic, atd::Sign::negative, 0, 50);
//	lateral_movement(automatic, atd::Sign::negative, 50, 0);
    }

    Car::stop();
}


void Main::interactive_test()
{
    uart << "\nInteractive test:\n";

    uart << "Forward/backward?(+/-) ";

    auto sign = atd::Sign::positive;
    char ans{};
    uart >> ans;
    if (ans == '-')
	sign = atd::Sign::negative;


    uint8_t right = pli::ask_uint8_t(uart, "\nRight percentage: ");
    if (right == 255)
	return;
    
    uint8_t left = pli::ask_uint8_t(uart, "Left percentage: ");
    if (left == 255)
	return;

    bool sense = pli::ask_yesno(uart, "Same direction of wheels rotation");
    if (sense)
	Car::move(sign, right, left);
    else
	Car::move_opposite(sign, right, left);

    pli::press_key_to("stop", uart);

    Car::stop();

}


void Main::driving_in_circles()
{
    uart << "\nDriving in circles:\n";

    uart << "Press '+' to increase radius or other letter to end\n";
    
    pli::press_key_to_continue(uart);

    uint8_t left = 100;
    for (uint8_t right = 30; right < 100; right += 5)
    {
	uart << right << '/' << left << '\n';
	Car::forward_turn_right(right, left);
	char opt{};
	uart >> opt;
	if (opt != '+'){
	    Car::stop();
	    return;
	}
    }

    Car::stop();

}

void Main::wheel_working()
{
    uart << "\nDoes the 2 wheels are similar?\n";

    uart << "Right test:\n"
	    "take in your hand the car an look at the right wheel\n";

    for (uint8_t i = 0; i < 6; ++i){
	Car::right_wheel(atd::Sign::positive, 50);
	Micro::wait_ms(150);
	Car::stop();
	pli::press_key(uart);
    }

    uart << "Left test:\n"
	    "look now at the left wheel\n";

    for (uint8_t i = 0; i < 6; ++i){
	Car::left_wheel(atd::Sign::positive, 50);
	Micro::wait_ms(150);
	Car::stop();
	pli::press_key(uart);
    }

}

void Main::run()
{
// main_menu():
    uart << "\nMenu\n"
	      "----\n"
	      "0. Stop car\n"
	      "1. Forward\n"
	      "2. Backward\n"
	      "3. Turn right (forward)\n"
	      "4. Turn left (forward)\n"
	      "5. Interactive test\n"
	      "6. Drive in circles\n"
	      "7. Lateral movement\n"
	      "8. How does the wheels works?\n";

    char opt{};
    uart >> opt;

    switch (opt){
	break; case '0': Car::stop();
	break; case '1': Car::forward(50);
	break; case '2': Car::backward(50);
	break; case '3': Car::forward_turn_right(0, 50);
	break; case '4': Car::forward_turn_left();
	break; case '5': interactive_test();
	break; case '6': driving_in_circles();
	break; case '7': lateral_movement();
	break; case '8': wheel_working();

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


