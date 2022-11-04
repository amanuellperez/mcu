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


void Main::print_data() const
{
    avr::UART_iostream uart;
    
    if (num_data < 2){
	uart << "No se han recibido datos.\n";
	return;
    }

    bool res = false;

    if (is_NEC_protocol(data, num_data))
	res = print_NEC_protocol(uart, data, num_data);

    if (res == false)
	print_raw_data();


}



void Main::print_raw_data() const
{
    avr::UART_iostream uart;

    uart << "------------------------------\n"
	 << "RAW data:\n"
	 << "num_data = " << num_data << '\n';

    if (num_data == max_num_data)
	uart << "ERROR: se ha llenado el buffer de datos.\n"
	     << "Modificar el programa aumentando `max_num_data`.\n";



    for (int8_t i = 0; i < num_data; ++i){
	if (i % 2)
	    uart <<  data[i] << "); ";
	else {
	    uart << "(" << data[i] << ", ";
	}

	if (i != 0 and (i % 7) == 0)
	    uart << '\n';
    }
    uart << '\n';

    uart << "Total number of pulses: " << num_data / 2 << '\n'
	 << "------------------------------\n";
}

