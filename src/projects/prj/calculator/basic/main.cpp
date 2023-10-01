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

// Conectar un teclado de 5 x 5
// El teclado es una matriz de pulsadores. En las filas conectamos diodos para
// evitar cortocircuitos, y en las columnas al conectar los pines los
// configuramos con pullup resistor.
// Sacamos la salida por un LCD
#include "main.h"
#include "../interface.h"
#include "../buffer.h"

#include "lex.h"
#include "calc.tab.hpp"
#include "cfg.h"

LCD Main::lcd;

Main::Main():keyboard_{key_code}
{
// init data

// init_hardware():
    init_lcd();
    init_keyboard();

}


// En este caso voy a gestionar el LCD directamente
void Main::init_lcd()
{
    lcd.terminal().stop_brcorner(true);// I'm not going to use it as a terminal
    lcd.terminal().nowrap(); 
}

// para depurar
void Main::print_buffer()
{
    lcd.clear();
    int i = 0;
    auto p = buffer.begin();
    lcd << '[';
    for (; i < 10 and p != buffer.end(); ++i, ++p)
	lcd << *p;

    lcd << "]\n";
    mcu::Micro::wait_ms(1000);
}

void Main::print_result()
{
    lcd.terminal().clear_row(1);
    lcd.cursor_pos(lcd.cols() - 1, 1);
    lcd.terminal().print_align_to_the_right<double_ndigits>(result);
}

void Main::print_msg_error()
{
    switch(error_){
	case Error::no: return;

	case Error::yylex_read:
	    lcd.clear();
	    lcd << "yylex_read error";
	    break;

	case Error::yyerror: return; // este se imprime en yyerror()
				     // lo mantengo para depurar
    }
}

void Main::run()
{
    clear_error();

    Interface interface{lcd, keyboard_, Abb2str{}};
    interface.initial_screen();

    while(1){
	interface.getline(buffer);
	clear_error();
//	print_buffer(); // debug
	yyparse();

	if (no_error())
	    print_result();

	else
	    print_msg_error();

    }
}

         

int main()
{
    Main app;
    app.run();
}


