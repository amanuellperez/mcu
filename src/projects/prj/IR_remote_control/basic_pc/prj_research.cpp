// Copyright (C) 2022 Manuel Perez 
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

#include "prj_main.h"
#include "dev_NEC_remote.h"

constexpr uint8_t cmd_length_ = 20;


/***************************************************************************
 *			    research_remote_control
 ***************************************************************************/
void Main::research_remote_control()
{
    UART uart;

    uart << '\n';
    atd::print(uart, msg_research_remote_control_help);

    char cmd[cmd_length_];
    cmd[0] = 'a';

    while(cmd[0] != '\0')
	research_remote_control_read(cmd);
}


void Main::research_remote_control_read(char* cmd)
{
    UART uart;

    cmd[0] = '\0'; // getline no inicializa a 0 en caso de no recibir nada
		   
    if (uart.getline(cmd, cmd_length_, '\r')){
	if (cmd[0] == '\0')
	    return;

	uart << cmd << '\t'; // DUDA: ¿mejor hacer echo en el terminal que use?
			     
	if (!receive())
	    return; 

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
    UART uart;
    uart << '\n';
    atd::print(uart, msg_test_remote_control);

    using RC = dev::Remote_control_Elegoo;
    RC rc;

    // TODO: a progmem. Dar opción de elegir el control remoto a probar.
    uart << "Comandos entre " << (int) rc.first << " y " << (int) rc.last << '\n';

    RC::Command_type i = rc.null;
    uart >> i;


    while(i != rc.null){
	uart << "cmd escrito = " << i << '\n';
	if (rc.transmit<Miniclock_us, SWG>(i))
	    uart << "OK\n";
	else 
	    uart << "ERROR\n"; // TODO: progmem
				   
	i = rc.null;
	uart >> i;
    }
}


