#include <ctype.h>

#include "main.h"
#include "lex.h"
#include "calc.tab.hpp"


int yylex()
{
    int c = static_cast<int>(buffer_.pop_front());

    if (c == '.' || isdigit(c)){ /* number */
	buffer_.push_front(c);
	// int n = read(buffer_, yylval);
	read(buffer_, yylval);
	// TODO: check error
	return NUMBER;
    }

    return c;
}

void yyerror(const char* s)
{
    lcd << "ERROR: " << s;
}

