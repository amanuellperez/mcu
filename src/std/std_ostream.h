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
#ifndef __MCU_STD_OSTREAM_H__
#define __MCU_STD_OSTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Implementación mínima del sistema de flujos.
 *
 *  - COMENTARIOS: Iré implementando las cosas según las vaya necesitando.
 *
 *  - HISTORIA:
 *	Manuel Perez- 24/09/2019 v0.0
 *
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_streambuf.h"
//#include "std_string.h"
#include "std_sizeof_txt.h"
#include "std_ios.h"
#include "std_cstdio.h"	    // int_to_cstring

namespace STD{

class ostream : virtual public basic_ios {
public:
    // Tipos propios
    using char_type	= basic_ios::char_type;
    using int_type	= basic_ios::int_type;
    using traits_type	= basic_ios::traits_type;

    // construcción-destrucción
    explicit ostream(streambuf* sb)
    { init(sb); }

    // DUDA: al no existir los operadores new/delete no se puede definir como
    // virtual el destructor, sino la clase hija genera un error. ¿Algún
    // problema?
    ~ostream() {flush();}


    // Formatted output
    // ----------------
    ostream& operator<<(short x)
    { return *this << static_cast<int>(x); }

    ostream& operator<<(unsigned short x)
    {return *this << static_cast<unsigned int>(x);}

    ostream& operator<<(int x) 
    {return operator_print<int>(x);}

    ostream& operator<<(unsigned int x) 
    {return operator_print<unsigned int>(x);}

    ostream& operator<<(long x)
    {return operator_print<long>(x);}

    ostream& operator<<(unsigned long x)
    {return operator_print<unsigned long>(x);}

    ostream& operator<<(long long x)
    {return operator_print<long long>(x);}

    ostream& operator<<(unsigned long long x)
    {return operator_print<unsigned long long>(x);}

    friend ostream& operator<<(ostream& out, char s);
    friend ostream& operator<<(ostream& out, unsigned char s);
//    friend ostream& operator<<(ostream& out, signed char s);

    friend ostream& operator<<(ostream& out, const char* s);

    // Manipuladores
    ostream& operator<<(ostream& (*pf)(ostream&)) { return pf(*this); }
    ostream& operator<<(basic_ios& (*pf)(basic_ios&)) 
    { 
	pf(*this);
	return *this;
    }

    ostream& operator<<(ios_base& (*pf)(ios_base&))
    { 
	pf(*this);
	return *this;
    }


    // Unformatted output
    // ------------------
    /// Escribe el caracter c en el flujo.
    ostream& put(char_type c);
    
    /// Escribe la cadena s[0..n) en el flujo.
    ostream& write(const char_type* s, streamsize n);

    /// Writes uncommitted changes to the underlying output sequence.
    ostream& flush();

    // prefix/suffix
    // -------------
    class sentry;

protected:
    ostream(const ostream&) = delete;
    ostream(ostream&&)	    = delete;

private:
    // Funciones ayuda
    // ---------------
    // Imprime la cadena. Vuelca todos los caracteres de c al streambuffer.
    // precondicion: se ha llamado al centinela
    bool print(const char* c);

    // Imprime la cadena c añadiendo el padding que corresponda dado por
    // width().
    // precondicion: se ha llamado al centinela
    void print_with_padding(const char* c);

    // Añade el padding a la izquierda de c.
    // precondicion: se ha llamado al centinela
    void print_left(const char* c);

    // Añade el padding a la derecha de c.
    // precondicion: se ha llamado al centinela
    void print_right(const char* c);

    // Imprime n caracteres de relleno
    // precondicion: se ha llamado al centinela
    bool print_fill_char(streamsize n);

    // implementación del operator<<
    template <typename Int>
    ostream& operator_print(const Int& x);

};


// El centinela va a garantizar que el flujo esté bien construido (que esté
// definido rdbuf()). De esta forma las funciones que lo llaman no tienen que
// andar preocupandose de esto.
class ostream::sentry{
public:
    explicit sentry(ostream& out);
    ~sentry();

    explicit operator bool() const {return (out_ != 0);}

    sentry(const sentry&) = delete;
    sentry& operator=(const sentry&) = delete;

private:
    // El standard explícitamente habla de tener un bool ok_; lo cual es un 
    // error: no debería de hablar de cómo se implementa la clase!!!
    ostream* out_;
};


template <typename Int>
ostream& ostream::operator_print(const Int& x)
{
    sentry sen{*this};

    if (sen){
	// El +1 es para el '\0'
	char buffer[__Num_caracteres_max<Int>+1];

        char* p = __int_to_cstring(
            buffer, buffer + __Num_caracteres_max<Int>, x);

	buffer[__Num_caracteres_max<Int>] = '\0';

        print_with_padding(p);
    }

    return *this;
}



ostream& operator<<(ostream& out, char c);
ostream& operator<<(ostream& out, unsigned char c);
//ostream& operator<<(ostream& out, signed char c);

// Versiones antiguas: no se pueden basar en `put` que es unformatted.
// Borrarlas cuando estén probadas.
//inline ostream& operator<<(ostream& out, char c)
//{
//    out.put(c);
//
//    return out;
//}
//
//inline ostream& operator<<(ostream& out, unsigned char c)
//{
//    out.put(c);	// TODO: ¿por qué char == unsigned char!!! No tiene por qué!!!
//
//    return out;
//}


//inline ostream& operator<<(ostream& out, signed char c)
//{
//    out.put(c);
//
//    return out;
//}



ostream& operator<<(ostream& out, const char* s);


// Manipuladores
inline ostream& flush(ostream& out)
{
    out.flush();

    return out;
}



}// namespace

#endif

