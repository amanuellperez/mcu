#include <ctype.h>

#include "main.h"
#include "lex.h"
#include "calc.tab.hpp"
#include "../buffer.h"	// read
#include "error.h"

int yylex()
{
    int c = static_cast<int>(Main::buffer.pop_front());

    if (c == '.' || isdigit(c)){ /* number */
	Main::buffer.push_front(c);
	if (!read(Main::buffer, yylval))
	    Main::set_yylex_read();

	return NUMBER;
    }

    return c;
}

void yyerror(const char* msg)
{
    msg_error(msg);

    if (Main::no_error())
	Main::set_yyerror();
}

