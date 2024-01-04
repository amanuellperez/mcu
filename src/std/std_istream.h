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

#pragma once
#ifndef __MCU_STD_ISTREAM_H__
#define __MCU_STD_ISTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Implementación mínima del sistema de flujos.
 *
 *  - COMENTARIOS: Iré implementando las cosas según las vaya necesitando.
 *	En principio lo implemento sin excepciones.
 *
 *  - HISTORIA:
 *	Manuel Perez- 26/09/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_streambuf.h"
#include "std_ios.h"
#include "std_limits.h"

#include <ctype.h>	// isdigit

#include "std_ostream.h" // para definir iostream
    
namespace STD{

class istream : virtual public basic_ios{
public:
    // Tipos propios
    using char_type	= basic_ios::char_type;
    using traits_type	= basic_ios::traits_type;
    using int_type	= basic_ios::int_type;


    // constructor/destructor
    // ----------------------
    explicit istream(streambuf* sb) : gcount_{0} 
    { init(sb); }


    // DUDA: al no existir los operadores new/delete no se puede definir como
    // virtual el destructor, sino la clase hija genera un error. ¿Algún
    // problema?
    // virtual ~istream() {}

    // Formatted input
    // ---------------
    // Hay que definir numeric_limits<short> para poder definir estas
    // funciones.
//    istream& operator>>(short & n);
//    istream& operator>>(unsigned short & n);

    istream& operator>>(int& n);
    istream& operator>>(unsigned int& n);

    istream& operator>>(long& n);
    istream& operator>>(unsigned long& n);

    istream& operator>>(long long& n);
    istream& operator>>(unsigned long long& n);

    // Unformatted input
    // -----------------
    /// Returns the number of characters extracted by the last unformatted
    /// input member function called for the object.
    streamsize gcount() const {return gcount_;}

    /// Lee un caracter si está disponible, en caso contrario devuelve eof.
    /// Consume el caracter.
    int_type get();

    /// Reads one character and stores it to 'c' if available.
    /// Otherwise leaves 'c' unmodified and sets failbit...
    /// Consume el caracter.
    istream& get(char_type& c);
 
    /// Reads characters and stores them into the successive locations of the
    /// character array whose first element is pointed to by s. Read max n-1 
    /// characters, or until eof, or hasta que se lee delim. Este caracter
    /// delim no se consume ni se almacena en s.
    /// Consume los caracteres.
    istream& get(char_type* s, streamsize n, char_type delim);

    istream& get(char_type* s, streamsize n)
    {return get(s, n, '\n');}

    /// Es la misma función que get salvo con la diferencia de que mientras
    /// que get no consume el caracter 'delim', esta función sí que lo
    /// consume. Como lo consume lo incluye en la cuenta de gcount().
    /// Consume los caracteres.
    istream& getline(char_type* s, streamsize n, char_type delim);

    istream& getline(char_type* s, streamsize n)
    {return getline(s, n, '\n');}

    /// Ignora n caracteres como mucho o hasta encontrar delim o eof.
    /// En caso de encontrar delim es consumido.
    istream& ignore(streamsize n = 1, int_type delim = traits_type::eof());

    /// Extrae n caracteres del flujo como máximo. 
    /// Se extraen caracteres hasta que o bien se leen n caracteres o bien se
    /// alcanza end of file.
    istream& read(char_type* s, streamsize n);

    // streamsize readsome(char_type* s, streamsize n);

    // Manipuladores
    // -------------
    istream& operator>>(istream& (*pf)(istream&)) { return pf(*this); }
    istream& operator>>(basic_ios& (*pf)(basic_ios&)) 
    { 
	pf(*this);
	return *this;
    }

    istream& operator>>(ios_base& (*pf)(ios_base&))
    { 
	pf(*this);
	return *this;
    }


    // prefix/suffix
    // -------------
    class sentry;


protected:
    istream(const istream&) = delete;
    istream(istream&&)	    = delete;

    template <typename IntReturn, typename Int>
    IntReturn bounded_static_cast(const Int& r);

private:
    // Datos
    // -----
    streamsize gcount_;


    // Funciones de ayuda
    // ------------------
    // Indica si d es un dígito (del 0 al 9).
    static bool is_digit(int_type d)
    {return isdigit(traits_type::to_char_type(d));}

    // TODO: esto depende claramente del tipo de caracter que usamos.
    static char_type ascii_to_digit(int_type d)
    { return traits_type::to_char_type(d) - '0'; }

    // Si no se lee un número devuelve 0 (de acuerdo al standard) y 
    // state == failbit.
    template <typename Int>
    Int read();

    template <typename Int>
    Int read_unsigned();


    // Implementación de la lectura de un número sin signo.
    template <typename Int>
    Int read_number();

    // Implementación de get/getline.
    int_type get_unguarded(char_type* s, streamsize n, char_type delim);

    template <typename Char>
    friend istream& _operator_read(istream& in, Char& c);

};



// Implementación de operator>>(char)
template <typename Char>
istream& _operator_read(istream& in, Char& c)
{
    istream::int_type ret = in.get();

    if (!in.is_eof(ret))
	c = static_cast<Char>(ret);

    return in;
}

inline istream& operator>>(istream& in, char& c)
{ return _operator_read<char>(in, c); }

inline istream& operator>>(istream& in, unsigned char& c)
{ return _operator_read<unsigned char>(in, c); }

inline istream& operator>>(istream& in, signed char& c)
{ return _operator_read<signed char>(in, c); }


class istream::sentry{
public:
    using traits_type = istream::traits_type;
    using char_type   = istream::char_type;
    using int_type    = istream::int_type;

    explicit sentry(istream& in, bool noskipws = false);

    sentry(const sentry&) = delete;
    sentry& operator=(const sentry&) = delete;
    ~sentry() {}

    explicit operator bool() const {return ok_;}

private:
    bool ok_;

    bool is_whitespace(int_type d);
    void discard_ws(istream& in);
};


template <typename IntReturn, typename Int>
IntReturn istream::bounded_static_cast(const Int& r)
{
    if (r < numeric_limits<IntReturn>::min()){
	setstate(ios_base::failbit);
	return numeric_limits<IntReturn>::min();

    } else if (r > numeric_limits<IntReturn>::max()){
	setstate(ios_base::failbit);
	return numeric_limits<IntReturn>::max();

    } else
	return static_cast<IntReturn>(r);
}


template <typename Int>
Int istream::read_number()
{
//traza(">>> read_number()\n\r");
    Int r = 0;
    int_type d = rdbuf()->sgetc();

    while (!is_eof(d) and is_digit(d)){
//traza("    c = ["); traza(traits_type::to_char_type(d)); traza("]\r\n");
	r *= 10u;
	r += ascii_to_digit(d);

	d = rdbuf()->snextc();
    }

    if (is_eof(d))
	setstate(ios_base::eofbit);

    return r;
}

template <typename Int>
Int istream::read()
{
    int_type d = rdbuf()->sgetc();
    char_type c = traits_type::to_char_type(d);

    Int sign = 1;	// +1 ó -1
    if (c == '+'){
	// sign = Int{1};
	d = rdbuf()->snextc();
    }
    else if (c == '-'){
	sign = Int{-1};
	d = rdbuf()->snextc();
    }

    else if (!is_digit(d)){
	setstate(ios_base::failbit);
	return 0;
    }

    return sign*read_number<Int>();

}

template <typename Int>
Int istream::read_unsigned()
{
    int_type d = rdbuf()->sgetc();
    char_type c = traits_type::to_char_type(d);

    if (c == '+')   // DUDA: un unsigned ¿tiene sentido ponerle el +???
	d = rdbuf()->snextc();

    else if (!is_digit(d)){
	setstate(ios_base::failbit);
	return 0;
    }

    return read_number<Int>();
}


/*!
 *  \brief  iostream
 *
 */
class iostream : public istream, 
		 public ostream {
public:
    using char_type   = istream::char_type;
    using int_type    = istream::int_type;
    using traits_type = istream::traits_type;


    // Constructor
    explicit iostream(streambuf* sb) : istream{sb}, ostream{sb} {}

    // virtual ~iostream() {}

protected:
    iostream(const iostream&) = delete;
    // iostream(iostream&&) ...

    iostream& operator=(const iostream&) = delete;
    // iostream& operator=(iostream&&) ...
    // void swap(iostream&) ...
};


}// namespace

#endif

