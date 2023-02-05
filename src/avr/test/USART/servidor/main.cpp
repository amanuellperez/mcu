// Copyright (C) 2019-2020 Manuel Perez 
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

//  Este programa dialoga con el programa test_cliente04.cpp
//
//  Enviamos un fichero desde el AVR al pc (es el paso previo para hacer un
//  termómetro logger)
//
#include "../../../avr_time.h"
#include "../../../avr_UART_iostream.h"
#include <atd_ascii.h>

using namespace avr_;
using atd::ASCII;


bool get_ack(UART_iostream& uart)
{
    char c;
    if (!uart.get(c)){
	uart.clear();
	uart << "Error al recibir datos\n\r";
	return false;
    }

    if (c != ASCII::ACK){
	uart << "Error: en lugar de recibir ACK se recibe ["
	     << static_cast<int>(c) << "]\r\n";
	return false;
    }

    return true;
}


int main() 
{
    UART_iostream uart;
    avr_::basic_cfg(uart);
    uart.on();


    // ¡¡el nombre_programa tiene que ir en flash memory!!!
    const char id[] = "termometro logger";

    uint8_t n = 0;
    while(1){
	++n;
    
	// header
	uart << ASCII::SOH;

	if (!get_ack(uart))
	    continue;

	uart << id;

	if (!get_ack(uart))
	    continue;

	// Datos
	uart << ASCII::STX;
	uart << "En un lugar de la mancha... (este es mi fichero)\n\r";
	uart << "Enviado " << n << " veces\r\n";
	uart << ASCII::ETX;

	uart << ASCII::EOT;

	wait_ms(1000);
    }

    return 0;
}


