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

#include "prj_main.h"
#include "square_wave.h"

void Main::run()
{
    while (1){
	avr::UART_iostream uart;
	uart << "\nMenu\n"
		"----\n"
		"1. Generate 38kHz on pin " << ir_transmitter_pin << '\n'
	     << "2. Transmit data\n"
		"3. Replay\n"
		"4. Receive data\n";
	
	char c{};
	uart >> c;
	switch (c){
	    break; case '1' : generate_38kHz();
	    break; case '2' : transmit_data();
	    break; case '3' : replay();
	    break; default  : receive_data_menu();
	}
    }
}

void Main::generate_38kHz()
{
    SWG::generate_38kHz_on();

    avr::UART_iostream uart;
    uart << "Generating 38kHz. Press a key to stop.\n";
    char c{};
    uart >> c;

    SWG::generate_38kHz_off();
}


void Main::receive_data_menu()
{
    print_instructions();

    while (1){
	pulse.receive();
	choose_mode_operation();
	switch(mode){
	    break; case Work_mode::help		    : print_instructions();
	    break; case Work_mode::receive_data	    : receive_data();
	    break; case Work_mode::receive_min_data : receive_min_data();
	    break; case Work_mode::receive_raw_data : receive_raw_data();
	}
	
    }
}

void Main::print_instructions()
{
    avr::UART_iostream uart;

    uart << "\n\nConnect the IR receiver to pin " << (int) ir_receiver_pin 
	 << ", point the TV remote to it and read the data.\n";

    uart << "\nMenu\n"
	    "----\n"
	    "h. Show this menu\n"
	    "1. Receive data (default)\n"
	    "2. Receive data (minimal version)\n"
	    "3. Receive and print RAW data\n\n";

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
	    break; case 'h': mode = Work_mode::help;
	    break; default : mode = Work_mode::receive_data;
	}


    }
}


void Main::replay()
{
    avr::UART_iostream uart;
    uart << "\nReplay\n";
//	    "------\n"
//	    "Press a button of your remote control ... ";

    pulse.receive();

    if (pulse.empty()){
	uart << "FAIL. No data received.\n";
	return;
    }

    print_raw_data();
    uart << "OK\n"
	    "Press a key to replay ... ";

    char c{};
    uart >> c;
    if (SWG::transmit(pulse))
	uart << "OK\n";

    else
	uart << "FAIL\n";

}

