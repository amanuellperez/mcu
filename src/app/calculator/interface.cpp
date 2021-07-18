// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

// Conectar un teclado de 5 x 5
// El teclado es una matriz de pulsadores. En las filas conectamos diodos para
// evitar cortocircuitos, y en las columnas al conectar los pines los
// configuramos con pullup resistor.
// Sacamos la salida por un LCD
#include "interface.h"
#include "buffer.h"

void Interface::DEL_command()
{
    lcd_ << "DEL!!!";
}

void Interface::AC_command()
{
    lcd_.clear();
    // buffer_.reset():
//    ibuf_ = 0;
}

void Interface::to_the_left_command()
{
    lcd_ << "to the left cmd";
}

void Interface::to_the_right_command()
{
    lcd_ << "to the right cmd";
}

void Interface::getline(Buffer& buffer)
{
// init LCD
    lcd_.clear();
    lcd_.cursor_on();

// read
    buffer.clear();

    while (buffer.size() != buffer.max_size()){
	uint8_t key = keyboard_.getkey();
	if (key == key_return)
	    return;

	if (key_commands[key].is_command())
	    key_commands[key].execute_command(this);

	else {
	    const char* p = key_strings[key_commands[key].str_id];
	    lcd_ << p;
	    push_back(buffer, p);
	}


	wait_ms(debouncing_time);
    }
}



