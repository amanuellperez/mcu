#include <ctype.h>

#include "main.h"
#include "lex.h"
#include "calc.tab.hpp"


int yylex()
{
    int c = static_cast<char>(buffer_.getchar());
lcd.clear();
lcd << "c = [" << (char) c << ']';
wait_ms(1000);
    
    if (buffer_.bad()){
lcd.clear();
lcd << "bad";
wait_ms(1000);
	return 0;
}

    if (buffer_.eof()){
lcd.clear();
lcd << "eof";
wait_ms(1000);
	return 0;
    }

    if (c == '.' || isdigit(c)){ /* number */
	buffer_.unget_last_char();
	int n = buffer_.read(yylval);
	// TODO: check error
	
lcd.clear();
lcd << "read = [" << n << "] = \n[" << (int) yylval << ']';
wait_ms(1000);
	return NUMBER;
    }

    return c;
}

void yyerror(const char* s)
{
    lcd << "ERROR: " << s;
}

