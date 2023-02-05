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

#include "std_istream.h"


namespace STD{
//istream& istream::operator>>(short& n)
//{
//    sentry sen{*this, false};
//
//    if (sen){
//	long r = read<long>();
//	n = bounded_static_cast<short>(r);
//    }
//
//    return *this;
//}


istream& istream::operator>>(int& n)
{
//traza("\r\n>>> operator>>(int)\r\n");
    sentry sen{*this, false};

    if (sen){
	long r = read<long>();
	n = bounded_static_cast<int>(r);
    }

    return *this;
}


istream& istream::operator>>(long& n)
{
//traza("\r\n>>> operator>>(long)\r\n");
    sentry sen{*this, false};

    if (sen)
	n = read<long>();

    return *this;
}

istream& istream::operator>>(long long& n)
{
//traza("\r\n>>> operator>>(long)\r\n");
    sentry sen{*this, false};

    if (sen)
	n = read<long long>();

    return *this;
}

//istream& istream::operator>>(unsigned short& n)
//{
//    sentry sen{*this, false};
//
//    if (sen){
//	long r = read_unsigned<unsigned long>();
//	n = bounded_static_cast<unsigned short>(r);
//    }
//
//    return *this;
//}

istream& istream::operator>>(unsigned int& n)
{
//traza("\r\n>>> operator>>(unsigned int)\r\n");
    sentry sen{*this, false};

    if (sen){
	unsigned long r = read_unsigned<unsigned long>();
	n = bounded_static_cast<unsigned int>(r);
    }
//traza("\r\n<<< saliendo operator>>(unsigned int)\r\n");
    return *this;
}


istream& istream::operator>>(unsigned long& n)
{
//traza("operator>>(unsigned long)\r\n");
    sentry sen{*this, false};

    if (sen)
	n = read_unsigned<unsigned long>();

    return *this;
}



istream& istream::operator>>(unsigned long long& n)
{
//traza("operator>>(unsigned long)\r\n");
    sentry sen{*this, false};

    if (sen)
	n = read_unsigned<unsigned long long>();

    return *this;
}



istream::int_type istream::get()
{
    sentry sen{*this, true};

    int_type d = traits_type::eof();
    gcount_ = 0;

    if (sen){
	d = rdbuf()->sbumpc();

	if (is_eof(d))
	    setstate(ios_base::eofbit);
	else
	    gcount_ = 1;
    }

    if (gcount_ == 0)
	setstate(ios_base::failbit);


    return d;
}

istream& istream::get(char_type& c)
{ 
    int_type ret = get();

    if (!is_eof(ret))
	c = traits_type::to_char_type(ret);

    return *this;
}

// Es la implementación de get/getline.
// Da por supuesto que se garantizan las precondiciones del sentry (sentry ==
// true) y no gestiona los errores. Eso se lo deja a get/getline.
// precondición: gcount_ =  0
// Esta función no consume el delimitador.
istream::int_type istream::get_unguarded(char_type* s, streamsize n, char_type delim)
{
    int_type idelim = traits_type::to_int_type(delim);
    int_type d = rdbuf()->sgetc();

    while (gcount_ + 1 < n
	    and !is_eof(d)
	    and !eq(d, idelim)){

	s[gcount_] = traits_type::to_char_type(d);
	++gcount_;
	d = rdbuf()->snextc();
    }

    if (gcount_ > 0) // el standard dice explicitamente que sea > 0
	s[gcount_] = '\0';

    if (is_eof(d))
	setstate(ios_base::eofbit);

    return d;
}


istream& istream::get(char_type* s, streamsize n, char_type delim)
{
    sentry sen{*this, true};

    gcount_ = 0;

    if (sen)
	get_unguarded(s, n, delim);

    if (gcount_ == 0)
	setstate(ios_base::failbit);


    return *this;
}

// Observar que la diferencia entre get y getline es el que consuma o no el
// delimitador y la gestión de errores.
istream& istream::getline(char_type* s, streamsize n, char_type delim)
{
    sentry sen{*this, true};

    gcount_ = 0;

    if (sen){
	int_type d = get_unguarded(s, n, delim);

	if (!eof()){
	    if (eq(d, delim)){
		rdbuf()->sbumpc();
		++gcount_;
	    }
	    else
		setstate(ios_base::failbit);
	}
    }

    if (gcount_ == 0)
	setstate(ios_base::failbit);


    return *this;
}

istream& istream::ignore(streamsize n, int_type delim)
{
    sentry sen{*this, true};

    gcount_ = 0;

    if (sen){
	int_type d = rdbuf()->sgetc();   

	while (gcount_ < n and !is_eof(d) and !eq(d, delim)){
	    d = rdbuf()->snextc();   
	    ++gcount_;
	}

	if (!eof()){
	    rdbuf()->sbumpc();
	    ++gcount_;
	}
	else
	    setstate(ios_base::eofbit);
    }

    return *this;
}



istream& istream::read(char_type* s, streamsize n)
{
    sentry sen{*this, true};

    gcount_ = 0;

    if (sen){
	gcount_ = rdbuf()->sgetn(s, n);

	if (gcount_ != n)
	    setstate(ios_base::eofbit | ios_base::failbit);
    }

    return *this;
}




istream::sentry::sentry(istream& in, bool noskipws)
{
    if (!in.good()){
	in.setstate(ios_base::failbit);
	ok_ = false;
	return;
    }

    ok_ = true;

    if (in.tie())
	in.tie()->flush();

    if (!noskipws and bool(in.flags() & ios_base::skipws))
	discard_ws(in);
}


bool istream::sentry::is_whitespace(int_type d)
{
    char_type c = traits_type::to_char_type(d);
    return (traits_type::eq(c, ' ') or traits_type::eq(c, '\t') or
	    traits_type::eq(c, '\n'));
}

// Escrita leyendo el standard.
void istream::sentry::discard_ws(istream& in)
{
    int_type d = in.rdbuf()->sgetc();
    while (!is_eof(d) and is_whitespace(d))
	d = in.rdbuf()->snextc();

    if (is_eof(d))
	in.setstate(failbit | eofbit);
}




}// namespace


