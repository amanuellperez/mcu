#include <ctype.h>

#include "main.h"
#include "lex.h"
#include "calc.tab.hpp"


int yylex()
{
    int c = static_cast<char>(buffer_.getchar());
    
    if (buffer_.eof())
	return 0;

    if (c == '.' || isdigit(c)){ /* number */
	buffer_.unget_last_char();
	buffer_.read(&yylval);
	// TODO: check error
	
lcd.clear();
lcd << "yylex.yyval =\n[" << (int) yylval << ']';
wait_ms(1000);
	return NUMBER;
    }

    return c;
}

void yyerror(const char* s)
{
    lcd << "ERROR: " << s;
}

