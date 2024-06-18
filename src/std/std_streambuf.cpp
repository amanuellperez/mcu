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

#include "std_streambuf.h"

namespace STD{

streamsize streambuf::in_avail()
{
    streamsize ret = egptr() - gptr();
    
    if (ret)
	return ret;
    else
	return showmanyc();
}

streambuf::int_type streambuf::snextc() 
{
    if (!is_eof(sbumpc()))
	return sgetc();

    return traits_type::eof();
}

streambuf::int_type streambuf::sbumpc() 
{
    if (gptr() != egptr()){
	int_type ret = traits_type::to_int_type(*gptr());
	gbump(1);
	return ret;
    }
	
    return uflow();
}

/// Lee el caracter actual. No avanza the input sequence 
/// (no consume el caracter).
streambuf::int_type streambuf::sgetc() 
{
    if (gptr() != egptr())
	return traits_type::to_int_type(*gptr());
	
    return underflow();
}


streambuf::int_type streambuf::sputbackc(char_type c)
{
    if ((gptr() > eback()) and traits_type::eq(c, gptr()[-1])){
	gbump(-1);
	return traits_type::to_int_type(*gptr());
    }

    return pbackfail(traits_type::to_int_type(c));
}


streambuf::int_type streambuf::sungetc()
{
    if (gptr() > eback()){
	gbump(-1);
	return traits_type::to_int_type(*gptr());
    }

    return pbackfail();
}


streambuf::int_type streambuf::sputc(char_type c) 
{ 
    if (pptr() == epptr())
	return overflow(traits_type::to_int_type(c)); 
    
    *pptr() = c;
    pbump(1);

    return traits_type::to_int_type(c);
}

streamsize streambuf::xsgetn(char_type* s, streamsize N)
{
    streamsize leidos = 0;
    
    while (N > 0){
	streamsize n = vacia_get_area(s, N);
	s += n;
	N -= leidos;
	leidos += n;

	if (N > 0){
	    int_type c = underflow();

	    if (is_eof(c))
		return leidos;
	}

    }

    return leidos;
}

streambuf::int_type streambuf::uflow() 
{
    if (is_eof(underflow()))
	return traits_type::eof();

    int_type ret = traits_type::to_int_type(*gptr());
    gbump(1);
    return ret;
}


streamsize streambuf::xsputn(const char_type* s, streamsize n)
{ 
    streamsize escritos = 0;

    while (escritos < n){
	streamsize len = llena_put_area(s, n - escritos);
	s += len;
	escritos += len;

	if (escritos < n){
	    int_type c = overflow(traits_type::to_int_type(*s));
	    
	    if (is_eof(c))
		return escritos;

	    ++s;
	    ++escritos;
	}
    }

    return escritos;
}


streamsize streambuf::llena_put_area(const char_type* s, streamsize N)
{
    streamsize buf_len = epptr() - pptr();

    streamsize n = min(buf_len, N);
    traits_type::copy(pptr(), s, n);
    pbump(n);

    return n;
}

// Vacia la get area en el array s[0..N). Vacía máximo N caracteres.
// Devuelve el número de caracteres escritos en s.
streamsize streambuf::vacia_get_area(char_type* s, streamsize N)
{
    streamsize buf_len = egptr() - gptr();

    streamsize n = min(buf_len, N);
    traits_type::copy(s, gptr(), n);
    gbump(n);

    return n;
}

}// namespace


