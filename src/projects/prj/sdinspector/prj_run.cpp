// Copyright (C) 2023 Manuel Perez 
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
#include "dev_print.h"	// print_line
			
char Main::main_menu()
{
    UART_iostream uart;

    uart << '\n';
    print_line(uart);
    if (sector.is_invalid())
	atd::print(uart, msg_main_no_sector_load);

    else {
	atd::print(uart, msg_main_sector_load);
	uart << sector.address;
    }

    uart << '\n';
    print_line(uart);

    atd::print(uart, msg_main_menu);
    if (sector.is_valid())
	atd::print(uart, msg_main_menu2);

    char ans{};
    uart >> ans;


    return ans;
}

void Main::run_command_invalid_sector(char cmd)
{
    switch(cmd){
	break; case '1': read_status();
	break; case '2': load_sector();
    }
}

void Main::run_command_valid_sector(char cmd)
{
    switch(cmd){
	break; case '1': read_status();
	break; case '2': load_sector();
	break; case '3': print_sector();
	break; case '4': print_sector_fromto();
	break; case '5': edit_sector(true);
	break; case '6': edit_sector(false);
	break; case '7': flush_sector();
    }
}

// (RRR) Como el menu es pequeño hago un mix para reutilizar código.
//       Si fuera un poco más liado sería mejor crear dos menus diferentes y
//       que el código quedara más claro:
//	    if (sector.is_invalid())
//		main_menu_invalid_sector();
//	    else
//		main_menu_valid_sector();
void Main::run()
{
    while(1){
	char cmd = main_menu();
	if (sector.is_invalid())
	    run_command_invalid_sector(cmd);

	else
	    run_command_valid_sector(cmd);
    }
}


