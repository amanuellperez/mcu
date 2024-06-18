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
#ifndef __MCU_STD_BASIC_IOS_H__
#define __MCU_STD_BASIC_IOS_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Implementación mínima del sistema de flujos.
 *
 *  - COMENTARIOS: Iré implementando las cosas según las vaya necesitando.
 *
 *  - HISTORIA:
 *	Manuel Perez- 01/10/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"
//#include "std_string.h"
#include "std_streambuf.h"
#include "std_ios_base.h"
#include "std_iosfwd.h"

namespace STD{

class basic_ios : public ios_base {
public:
    // Tipos propios
    using char_type	= char;
    using traits_type	= char_traits<char>;
    using int_type	= int;

    // Constructor/destructor
    // ----------------------
    explicit basic_ios(streambuf* sb)
	: ios_base(), streambuf_{0}, ostream_tie_{0}, fill_{' '}
    { init(sb);}

    basic_ios(const basic_ios&)		    = delete;
    basic_ios& operator=(const basic_ios&)  = delete;

    // Flags functions
    // ---------------
    explicit operator bool() const { return !fail();}
    bool operator!() const {return fail();}
    iostate rdstate() const {return streambuf_state_;}
    void clear(iostate state = goodbit);
    void setstate(iostate state) {clear(rdstate() | state);}

    bool good() const {return rdstate() == 0;}
    bool eof() const {return (rdstate() & eofbit) != 0;}
    bool fail() const {return (rdstate() & (failbit | badbit )) != 0;}
    bool bad() const {return (rdstate() & badbit) != 0;}


    // Members
    // -------
    /// Returns an output sequence that is tied to (synchronized with) the
    //sequence controlled by the stream buffer.
    ostream* tie() const;

    /// Sets the currente tied stream to os. Returns the tied stream before
    /// the operation.
    ostream* tie(ostream* os);

    streambuf* rdbuf() const {return streambuf_;}
    streambuf* rdbuf(streambuf* sb);

    /// Returns the character used to pad (fill) an output conversion to the
    /// specified field width.
    char_type fill() const {return fill_;}

    /// Postcondition: traits::eq(c, fill()).
    /// Returns: The previous value of fill().
    char_type fill(char_type c);

protected:
    // Constructor
    // -----------
    // ¿Cómo construir un iostream?
    // Como la herencia es virtual (para que iostream no tenga duplicada esta
    // clase via istream/ostream), es necesario que las clases hijas llamen al
    // constructor de basic_ios explícitamente. Para evitar esto hacemos lo
    // siguiente:
    //	1.- Definimos este constructor por defecto protected, que es el que
    //	llaman las clases hijas (iostream por ejemplo).
    //	2.- Definimos la función init(sb) que es la función que llaman
    //	istream/ostream en su constructor. De esta forma invertimos el orden
    //	de construcción que por defecto impone C++. C++ construye: 1º
    //	basic_ios (la clase padre común a iostream via istream/ostream) y
    //	luego istream/ostream. Lo que hacemos es dar un constructor que no hace
    //	nada y en el constructor de istream/ostream llamamos a init,
    //	construyendo de verdad basic_ios (de tal forma que realmente lo
    //	construimos después de istream/ostream y no antes).
    basic_ios()
	: ios_base(), streambuf_{0}, ostream_tie_{0}, fill_{' '}
    {}

    void init(streambuf* sb);

private:
    // Datos
    streambuf* streambuf_;
    ostream* ostream_tie_;
    char_type fill_;
};




}// namespace

#endif


