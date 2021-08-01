#include <ctype.h>

#include "main.h"
#include "lex.h"
#include "calc.tab.hpp"
#include "../buffer.h"	// read

int yylex()
{
    int c = static_cast<int>(Main::buffer.pop_front());

    if (c == '.' || isdigit(c)){ /* number */
	Main::buffer.push_front(c);
	if (!read(Main::buffer, yylval))
	    Main::set_yylex_read();

//Main::lcd.clear();
//atd::print(Main::lcd, yylval);
//wait_ms(500);
	return NUMBER;
    }

//Main::lcd.clear();
//Main::lcd << (char) c;
//wait_ms(500);
    return c;
}

void yyerror(const char* s)
{
    Main::lcd.screen().clear_row(1);
    Main::lcd.cursor_pos(0,1);
    Main::lcd << s;
    if (Main::no_error())
	Main::set_yyerror();
}

