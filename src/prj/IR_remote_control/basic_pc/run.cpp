// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

void Main::run()
{
    print_instructions();

    while (1){
	::receive_pulses(pulse);
	choose_mode_operation();
	switch(mode){
	    break; case Work_mode::help		    : print_instructions();
	    break; case Work_mode::receive_data	    : receive_data();
	    break; case Work_mode::receive_min_data : receive_min_data();
	    break; case Work_mode::receive_raw_data : receive_raw_data();
	    break; case Work_mode::transmit_data    : transmit_data();
	}
	
    }
}

void Main::print_instructions()
{
    avr::UART_iostream uart;

    uart << "\n\nConnect the IR receiver to pin " << (int) ir_receiver_pin 
	 << ", point the TV remote to it and read the data.\n";

    uart << "Menu\n"
	    "----\n"
	    "h. Show this menu\n"
	    "1. Receive data (default)\n"
	    "2. Receive data (minimal version)\n"
	    "3. Receive and print RAW data\n"
	    "4. Transmit data\n\n";

    mode = Work_mode::receive_data;
}


void Main::choose_mode_operation()
{
    if (avr::UART_basic::are_there_data_unread()){
	avr::UART_iostream uart;

	char c{};
	uart >> c;

	switch(c){
	    break; case '2': mode = Work_mode::receive_min_data;
	    break; case '3': mode = Work_mode::receive_raw_data;
	    break; case '4': mode = Work_mode::transmit_data;
	    break; case 'h': mode = Work_mode::help;
	    break; default : mode = Work_mode::receive_data;
	}


    }
}

