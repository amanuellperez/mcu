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
#include "dev_square_wave.h"

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
	    break; case '2' : send_message();
	    break; case '3' : replay();
	    break; case '4' : research_remote_control();
	    break; case '5' : clone_remote_control();
	    break; case '6' : test_remote_control();
	    break; default  : receive_menu();
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




void Main::replay()
{
    avr::UART_iostream uart;
    atd::print(uart, msg_menu_replay);

    // pulse.receive<Train_cfg>();
    if (!receive())
	return; 

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

