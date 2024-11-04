// Copyright (C) 2021 Manuel Perez 
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



#include <stdio.h>
#include <ctype.h>

#include "symbol.h"
#include "calc.tab.hpp"
#include "main.h"
#include "error.h"
#include "../../buffer.h"	// read

#include <mega.h>

int yylex()
{
    ctrace << ">>> yylex: buffer = ";
    ctrace << "run: free ram =  [" << atmega::Micro::bytes_of_free_ram() << "] bytes\n";
    trace(Main::buffer);

    int c = static_cast<int>(Main::buffer.pop_front());

    ctrace << "ylex(" << (char) c << ") ... ";
    if (c == '.' || isdigit(c)){ /* number */
	Main::buffer.push_front(c);
	if (!read(Main::buffer, yylval.value))
	    Main::set_yylex_read();

	ctrace << "NUMBER = [" << (int) yylval.value << "]\n";
	ctrace << "<<< yylex: buffer = ";
	trace(Main::buffer);
	return NUMBER;
    }


    if (isalpha(c)){
// pop_front_alnumstring(buffer, string):
	
	Main::buffer.push_front(c);
	char str[100]; // TODO: parametrizar
	char* p = str;
	while (!Main::buffer.empty() and isalpha(Main::buffer[0])){
	    *p++ = Main::buffer.pop_front();
	}
	*p = '\0';

	Symbol* s = Main::symbol[str];
	if (s == nullptr)
	    return UNDEF;

	yylval.symbol = s;

	return s->type;
    }

    ctrace << "[" << (char) c << "]?\n";
    return c;

}


void yyerror(const char* msg)
{
ctrace << "yyerror(" << msg << ")\n";
    msg_error(msg);

    if (Main::no_error())
	Main::set_yyerror();
}

