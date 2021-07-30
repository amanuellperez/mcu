#include <ctype.h>

#include "main.h"
#include "lex.h"
#include "calc.tab.hpp"


int yylex()
{
    int c = static_cast<int>(Main::buffer.pop_front());

    if (c == '.' || isdigit(c)){ /* number */
	Main::buffer.push_front(c);
	read(Main::buffer, yylval);
	// TODO: check error
	return NUMBER;
    }

    return c;
}

void yyerror(const char* s)
{
    Main::lcd.screen().clear_row(1);
    Main::lcd.cursor_pos(0,1);
    Main::lcd << s;
    Main::error = true;
}

