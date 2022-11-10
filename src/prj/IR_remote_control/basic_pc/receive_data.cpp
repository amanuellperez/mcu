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
#include "IR_NEC_protocol.h"


void Main::receive_data() const
{
    avr::UART_iostream uart;
    
    if (pulse.size == 0){
	uart << "No data received.\n";
	return;
    }

    bool res = print_NEC_protocol(uart, pulse);

    if (res == false)
	print_raw_data();

}


void Main::receive_min_data() const
{
    avr::UART_iostream uart;
    
    if (pulse.size == 0){
	uart << "No data received.\n";
	return;
    }

    bool res = print_min_NEC_protocol(uart, pulse);

    if (res == false)
	uart << "???\n";


}
void Main::receive_raw_data() const
{
    avr::UART_iostream uart;
    
    if (pulse.size == 0){
	uart << "No data received.\n";
	return;
    }

    print_raw_data();
}




void Main::print_raw_data() const
{
    avr::UART_iostream uart;

    uart << "\n\n------------------------------\n"
	 << "RAW data: (time_low, time_high) = period\n"
	 << "num pulses = " << pulse.size << '\n';

    if (pulse.full())
	uart << "ERROR: se ha llenado el buffer de datos.\n"
	     << "Modificar el programa aumentando `max_num_data`.\n";



    for (size_t i = 0; i < pulse.size; ++i){
	uart <<  '(' << pulse[i].time_low << ", "
	     << pulse[i].time_high << ")\t= " << pulse[i].period();

	if (i % 2)
	    uart << "\t\t";
	else
	    uart << '\n';



    }
    uart << '\n';

    uart << "\nTotal number of pulses: " << pulse.size << '\n'
	 << "------------------------------\n";
}

