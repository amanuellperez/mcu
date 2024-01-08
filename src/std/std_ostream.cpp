// Copyright (C) 2019-2020 Manuel Perez 
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

#include "std_ostream.h"

namespace STD{


ostream::sentry::sentry(ostream& out)
{
    if (!out.good()){
	out_ = 0;
	out.setstate(ios_base::failbit);
	return;
    }

    if (out.rdbuf() == 0){
	out_ = 0;
	out.setstate(ios_base::badbit);
	return;
    }

    if (out.tie())
	out.tie()->flush();

    out_ = &out;

}

// NOTAS: El standard habla de que si falla el flush se ponga failbit.
// El compilador gcc no hace eso sino que pone badbit si falla el flush o 
// no está definido rdbuf. Hago un compromiso: failbit si falla flush (no
// se ha conseguido ejecutar la operación correspondiente) y badbit si no
// está definido rdbuf() (ya que es un error irrecuperable).
ostream::sentry::~sentry()
{
    if (!out_)
	return;

    if (out_->good() and bool(out_->flags() & ios_base::unitbuf)){
	if (out_->rdbuf()){
	    if(out_->rdbuf()->pubsync() == -1)
		out_->setstate(ios_base::failbit);
	}else
	    out_->setstate(ios_base::badbit);
    }
}


ostream& ostream::put(char_type c)
{ 
    sentry sen{*this};

    if (sen){
	if (is_eof(rdbuf()->sputc(c)))
	    setstate(ios_base::badbit);

// Versión antigua: no imprimía '\0'. Borrarla cuando esté probada.
//	    char_type buf[2] = {c, '\0'};
//
//	    print_with_padding(buf);
    }

    return *this;
}



/// Escribe la cadena s[0..n) en el flujo.
ostream& ostream::write(const char_type* s, streamsize n)
{
    sentry sen{*this};

    if (sen){
	if (rdbuf()->sputn(s, n) != n)
	    setstate(ios_base::badbit);
    }

    return *this;
}


/// Writes uncommitted changes to the underlying output sequence.
ostream& ostream::flush()
{
    sentry sen{*this};

    if (sen){
	if (rdbuf()->pubsync() == -1)
	    setstate(ios_base::badbit);

    }

    return *this;
}


void ostream::print_with_padding(const char* c)
{
    if (width_ == 0)
	print(c);

    else{
	if (adjust_to_left())
	    print_left(c);
	else // observar que este caso incluye internal. (correcto???)
	    print_right(c);

	width(0);   // width es no sticky!!!
    }

}

void ostream::print_left(const char* c)
{
    if (print(c) == false)
	return;

    streamsize len = strlen(c);

    if (len < width())
	print_fill_char(width() - len);
}

void ostream::print_right(const char* c)
{
    streamsize len = strlen(c);

    if (len < width())
	if (print_fill_char(width() - len) == false)
	    return;

    print(c);
}

// imprime n caracteres de relleno
bool ostream::print_fill_char(streamsize n)
{
    for (streamsize i = 0; i < n; ++i){
	if (is_eof(rdbuf()->sputc(fill()))){
	    setstate(ios_base::badbit);
	    return false;
	}
    }

    return true;
}


bool ostream::print(const char* c)
{
    while(*c){
	if (is_eof(rdbuf()->sputc(*c))){
	    setstate(ios_base::badbit);
	    return false;
	}
	++c;
    }

    return true;
}



ostream& operator<<(ostream& out, const char* s) 
{ 
    ostream::sentry sen{out};

    if (sen)
	out.print_with_padding(s); 

    return out;
}


ostream& operator<<(ostream& out, char c)
{
    ostream::sentry sen{out};

    if (sen){
	const char s[2] = {c, '\0'};
	out.print_with_padding(s); 
    }

    return out;
}

ostream& operator<<(ostream& out, unsigned char c)
{
    ostream::sentry sen{out};

    if (sen){
	const char s[2] = {static_cast<char>(c), '\0'};
	out.print_with_padding(s); 
    }
    return out;
}

}// namespace


