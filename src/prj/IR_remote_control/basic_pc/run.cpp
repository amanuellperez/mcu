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
    avr::UART_iostream uart;

    uart << "Conecta el IR receiver al pin " << sensor_pin 
	 << ", apunta el mando del TV y leamos datos.\n";

    while (1){
	read_data();
	print_data();
    }
}

void Main::print_data()
{
    avr::UART_iostream uart;
    
    if (num_data < 2){
	uart << "No se han recibido datos.\n";
	return;
    }

    if (is_equal(data[0], 9000) and is_equal(data[1], 4500))
	print_nec_protocol();

    else
	print_raw_data();


}

// precondition: sabemos que data[0] == 9000, y data[1] == 4500
void Main::print_nec_protocol()
{
    avr::UART_iostream uart;
    
    uart << "NEC\n"
	    "---\n";

    if (num_data != 33 * 2){ // 1 pulso preámbulo + 4 bytes
	uart << "Error: recibidos " << ((num_data - 2)/ 2) << " bits,\n"
	     << "pero se esperan 32 bits (4 bytes)\n"
	     << "num_data = [" << num_data << "]\n";

	print_raw_data();
    }


}


void Main::print_raw_data()
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

