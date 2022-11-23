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
#include "IR_NEC_data.h"

constexpr uint8_t cmd_length_ = 20;


/***************************************************************************
 *			    research_remote_control
 ***************************************************************************/
void Main::research_remote_control()
{
    avr::UART_iostream uart;

    uart << '\n';
    atd::print(uart, msg_research_remote_control_help);

    char cmd[cmd_length_];
    cmd[0] = 'a';

    while(cmd[0] != '\0')
	research_remote_control_read(cmd);
}


void Main::research_remote_control_read(char* cmd)
{
    avr::UART_iostream uart;

    cmd[0] = '\0'; // getline no inicializa a 0 en caso de no recibir nada
		   
    if (uart.getline(cmd, cmd_length_, '\r')){
	if (cmd[0] == '\0')
	    return;

	uart << cmd << '\t'; // DUDA: ¿mejor hacer echo en el terminal que use?
			     
	pulse.receive();
	print_pulses_min();
    }

    else {
	uart.clear();
	atd::print(uart, msg_error_reading_line);
	cmd[0] = '\0';	// error
    }

}


/***************************************************************************
 *			    test_remote_control
 ***************************************************************************/
void Main::test_remote_control()
{
    avr::UART_iostream uart;
    uart << '\n';
    atd::print(uart, msg_test_remote_control);

    using RC = Remote_control<Elegoo>;

    // TODO: a progmem. Dar opción de elegir el control remoto a probar.
    // TODO: Mejor que NEC_transmitter quedaría:
    //	    Remote_control<Clock_us, SWG>::transmit(index);
    //	    y ya el Remote_control sabe si el protocolo a usar es NEC o RC5,
    //	    los comandos y demás. Sería muy sencillo de usar.
    uart << "Comandos entre " << (int) RC::first << " y " << (int) RC::last << '\n';

    RC::index_type i = RC::null;
    uart >> i;


    while(i != RC::null){
	uart << "cmd escrito = " << i << '\n';
	if (RC::first <= i and i <= RC::last){
	    uart << "OK\n";
	    NEC_protocol::transmit<Clock_us, SWG>(RC::first_burst, RC::cmd[i]);
	}
	else 
	    uart << "Error cmd\n"; // TODO: progmem
				   
	i = RC::null;
	uart >> i;
    }
}


