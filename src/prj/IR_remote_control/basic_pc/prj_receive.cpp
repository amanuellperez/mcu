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
#include "dev_NEC_protocol.h"

bool Main::receive()
{
    avr::Enable_interrupts lock;

    avr::UART_basic::enable_interrupt_unread_data();
    Main::user_abort = false;

    bool res = pulse.receive<Train_cfg>(Main::user_abort);

    avr::UART_basic::disable_interrupt_unread_data();

    return res;
}


void Main::receive_menu()
{
    mode = Receive_cfg::print_pulses;

    receive_print_help();

    while (1){
	if (receive())
	    receive_print();

	else 
	    if (!receive_read_uart())
		return;

    }
}

void Main::receive_print()
{
    switch(mode){
	break; case Receive_cfg::print_pulses	    : print_pulses();
	break; case Receive_cfg::print_pulses_min  : print_pulses_min();
	break; case Receive_cfg::print_pulses_raw  : print_pulses_raw();
    }
}

void Main::receive_print_help()
{
    avr::UART_iostream uart;

    atd::print(uart, msg_receive_data_menu1);
    uart << (int) ir_receiver_pin;
    atd::print(uart, msg_receive_data_menu2);

}


bool Main::receive_read_uart()
{
    if (!avr::UART_basic::are_there_data_unread())
	return false;	// error de lógica

    avr::UART_iostream uart;

    char c{};
    uart >> c;

    switch(c){
	break; case '2': mode = Receive_cfg::print_pulses_min;
	break; case '3': mode = Receive_cfg::print_pulses_raw;

	break; case 'h': case 'H': receive_print_help();
	break; case 'e': case 'E': return false;

	break; default : mode = Receive_cfg::print_pulses;
    }

    return true;
}


void Main::print_pulses() const
{
    avr::UART_iostream uart;
    
    if (pulse.empty()){
	atd::print(uart, msg_no_data_received);
	return;
    }

    bool res = dev::NEC_protocol::print_verbose(uart, pulse);

    if (res){
	dev::NEC_message msg;
	atd::convert(pulse).into(msg);
	uart << "convert: " << msg << '\n';
    }

    if (res == false)
	print_raw_data();

}


void Main::print_pulses_min() const
{
    avr::UART_iostream uart;
    
    if (pulse.empty()){
	atd::print(uart, msg_no_data_received);
	return;
    }

    bool res = dev::NEC_protocol::print(uart, pulse);

    if (res == false)
	atd::print(uart, msg_unknown);


}
void Main::print_pulses_raw() const
{
    avr::UART_iostream uart;
    
    if (pulse.empty()){
	atd::print(uart, msg_no_data_received);
	return;
    }

    print_raw_data();
}




void Main::print_raw_data() const
{
    avr::UART_iostream uart;

    uart << '\n';
    atd::print(uart, msg_line);
    atd::print(uart, msg_print_raw_data_header1);
    uart << pulse.size() << '\n';

    if (pulse.full())
	atd::print(uart, msg_error_buffer_full);

    atd::print(uart, msg_polarity);
    uart << (int) pulse.polarity() << '\n';

    for (size_t i = 0; i < pulse.size(); ++i){
	uart <<  '(' << pulse[i].time_low << ", "
	     << pulse[i].time_high << ")\t= " << pulse[i].period();

	if (i % 2)
	    uart << "\t\t";
	else
	    uart << '\n';

    }
    uart << '\n';

    atd::print(uart, msg_total_number_of_pulses);
    uart << pulse.size() << '\n';
    atd::print(uart, msg_line);
}

