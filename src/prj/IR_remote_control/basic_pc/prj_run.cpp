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

void Main::print_run_menu()
{
    avr::UART_iostream uart;
    atd::print(uart, msg_run_menu1);
    uart << (int) ir_transmitter_pin;
    atd::print(uart, msg_run_menu2);
}


void Main::run()
{
    while (1){
	print_run_menu();
	
	avr::UART_iostream uart;
	char c{};
	uart >> c;
	switch (c){
	    break; case '1' : generate_38kHz();
	    break; case '2' : transmit_data();
	    break; case '3' : replay();
	    break; case '4' : research_remote_control();
	    break; default  : receive_data_menu();
	}
    }
}

void Main::generate_38kHz()
{
    SWG::generate_38kHz_on();

    avr::UART_iostream uart;
    atd::print(uart, msg_generate_38kHz_on_msg);
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
	    break; case Work_mode::print_pulses	    : print_pulses();
	    break; case Work_mode::print_pulses_min : print_pulses_min();
	    break; case Work_mode::print_pulses_raw : print_pulses_raw();
	}
	
    }
}

void Main::print_instructions()
{
    avr::UART_iostream uart;

    atd::print(uart, msg_receive_data_menu1);
    uart << (int) ir_receiver_pin;
    atd::print(uart, msg_receive_data_menu2);

    mode = Work_mode::print_pulses;
}


void Main::choose_mode_operation()
{
    if (avr::UART_basic::are_there_data_unread()){
	avr::UART_iostream uart;

	char c{};
	uart >> c;

	switch(c){
	    break; case '2': mode = Work_mode::print_pulses_min;
	    break; case '3': mode = Work_mode::print_pulses_raw;
	    break; case 'h': mode = Work_mode::help;
	    break; default : mode = Work_mode::print_pulses;
	}


    }
}


void Main::replay()
{
    avr::UART_iostream uart;
    atd::print(uart, msg_menu_replay);

    pulse.receive();

    if (pulse.empty()){
	atd::print(uart, msg_fail);
	uart << ". ";
	atd::print(uart, msg_no_data_received);
	return;
    }

    print_raw_data();
    atd::print(uart, msg_ok);
    uart << '\n';
    atd::print(uart, msg_press_a_key_to_replay);

    char c{};
    uart >> c;
    if (SWG::transmit(pulse))
	atd::print(uart, msg_ok);

    else
	atd::print(uart, msg_fail);

    uart << '\n';

}

