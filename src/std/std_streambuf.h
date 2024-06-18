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
#ifndef __MCU_STD_STREAMBUF_H__
#define __MCU_STD_STREAMBUF_H__
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

#include "std_ios_types.h"
#include "std_char_traits.h"	
#include "std_algorithm.h"  // min


namespace STD{

class streambuf{
public:
    // Tipos propios
    using char_type	= char_traits<char>::char_type;
    using int_type	= char_traits<char>::int_type;
//    using pos_type	= char_traits<char>::pos_type;
//    using off_type	= char_traits<char>::off_type;
    using traits_type	= char_traits<char>;


    // Destrucción
    // DUDA: al no existir los operadores new/delete no se puede definir como
    // virtual el destructor, sino la clase hija genera un error. ¿Algún
    // problema?
    // virtual ~streambuf() {}


    // Buffer and positioning
    // ----------------------
    /// Es flush().
    int pubsync() {return sync();}

    // Get area
    // --------
    /// If a read position is available, returns egptr() - gptr(). Otherwise
    /// returns showmanyc().
    streamsize in_avail();

    /// Advances the input sequence by one character and reads one character.
    /// (Observar que el caracter actual al que apuntabamos lo descartamos
    /// leyendo el siguiente caracter, pero dejando ese caracter en el buffer
    /// en gptr()).
    int_type snextc();

    /// Reads one character and advances the input sequence by one character
    /// (consume el caracter). Básicamente esta función es: read_a_character!
    int_type sbumpc();

    /// Lee el caracter actual. No avanza the input sequence 
    /// (no consume el caracter).
    int_type sgetc();
    
    /// Reads n characters from the input sequence and stores them into s.
    /// The characters are read as if by repeated call of sbumpc. Esto es, 
    /// consume n caracteres. Consume los n caracteres.
    streamsize sgetn(char_type* s, streamsize n)
    {return xsgetn(s, n);}


    // Putback
    // -------
    /// Puts back a character back to the get area.
    int_type sputbackc(char_type c);

    /// Moves the next pointer in the input sequence back by one.
    int_type sungetc();


    // Put area
    // --------
    /// Escribimos un caracter en el buffer. 
    int_type sputc(char_type c);

    /// Escribe los caracteres s[0..n) en el buffer.
    streamsize sputn(const char_type* s, streamsize n)
    { return xsputn(s, n); }



protected:
    // Construcción
    // ------------
    streambuf() : eback_{0}, gptr_{0}, egptr_{0},
		  pbase_{0}, pptr_{0}, epptr_{0}
    { }

    streambuf(const streambuf&)		    = default;
    streambuf& operator=(const streambuf&)  = default;

    // void swap(streambuf&);

    // Get area access
    // ---------------
    // La get area es: [gptr(), egptr())
    char_type* eback() const { return eback_;}
    char_type* gptr()  const { return gptr_; }
    char_type* egptr() const { return egptr_;}

    // Adds n to the next pointer for the input sequence
    void gbump(int n) { gptr_ += n;}

    // Definimos la get area
    void setg(char_type* eback0, char_type* gptr0, char_type* egptr0);


    // Put area access
    // ---------------
    // La put area es: [pbase(), pptr())
    char_type* pbase() const {return pbase_;}
    char_type* pptr()  const {return pptr_;}
    char_type* epptr() const {return epptr_;}


    // Adds n to the next pointer for the output sequence
    void pbump(int n) {pptr_ += n;}

    // Definimos la put area
    void setp(char_type* pbase0, char_type* epptr0);

    // -------------------
    // Funciones virtuales
    // -------------------
    
    // Buffer management
    // -----------------
    // Synchronizes the controlled sequences with the arrays (vamos, es
    // flush()). That is, if pbase() is non-null the characters [pbase(),
    // pptr()) are writen to the controlled sequence. The pointers may be
    // reset as appropiate.
    //
    // Returns: -1 on failure (cada clase derivada determinará qué considera
    // fallo).
    virtual int sync() {return 0;}
    
    // Get area
    // --------
    // Obtains the number of characters available for input in the associated
    // input sequence if known.
    virtual streamsize showmanyc() {return 0;}


    // Reads max N characters from the input sequence and stores them into s.
    // The characters are read as if by repeated call of sbumpc.
    // Returns: the number of characters assigned to s.
    virtual streamsize xsgetn(char_type* s, streamsize N);


    // Ensures that at least one character is avaible in the input area by
    // updating the pointers to the input area and reading more data in from
    // the input sequence. Returns the value of that character on success or
    // traits_type::eof() on failure.
    // (Esta función no consume el caracter)
    virtual int_type underflow()
    {return traits_type::eof();}


    // Idéntica a underflow salvo que esta función avanza un caracter el valor
    // de gptr() (consume un caracter).
    virtual int_type uflow();

    // Putback
    // -------
    // Puts a character back into the input sequence, posibly modifying the 
    // input sequence.
    virtual int_type pbackfail(int_type c = traits_type::eof())
    {return traits_type::eof();}


    // Put area
    // --------
    // Ensures that there is space at the put area for at least one character
    // by saving some initial subsequence of characters starting at pbase() to
    // the output sequence and updating the poinnters to the put area (if
    // needed). If c is not eof() it is either put to the put area or directly
    // saved to the output sequence.
    //
    // Returns: traits::eof() if the function fails, or some other unspecified 
    // value to indicate success (typically returns c to indicate success, 
    // salvo cuando c == traits::eof() en cuyo caso devuelve traits::not_eof(c))
    virtual int_type overflow(int_type c = traits_type::eof())
    { return traits_type::eof();}

    // Writes up to n characters to the output sequence as if by repeated
    // calls to sputc(c).
    // Returns: the number of characters written.
    virtual streamsize xsputn(const char_type* s, streamsize n);


    // Funciones ayuda
    // ---------------
    bool is_eof(int_type c) const
    { return traits_type::eq_int_type(c, traits_type::eof());}

private:
    // Get area: [eback_, egptr_)
    // La zona que queda por leer es [gptr_, egptr_)
    char_type* eback_;
    char_type* gptr_;
    char_type* egptr_;

    // Put area: [pbase_, epptr_)
    // pptr_ = marca la posición donde queremos escribir el siguiente byte.
    char_type* pbase_;
    char_type* pptr_;
    char_type* epptr_;

    // Funciones de ayuda
    // ------------------
    // Vuelca el máximo número de caracteres de la cadena s[0..N) en la put
    // area, actualizandola. Devuelve el número de caracteres escritos.
    streamsize llena_put_area(const char_type* s, streamsize N);

    // Vacia la get area en el array s[0..N). Vacía máximo N caracteres.
    // Devuelve el número de caracteres escritos en s.
    streamsize vacia_get_area(char_type* s, streamsize N);


};




inline void
streambuf::setp(char_type* pbase0, char_type* epptr0)
{
    pbase_ = pbase0;
    pptr_  = pbase0;
    epptr_ = epptr0;
}


inline void
streambuf::setg(char_type* eback0, char_type* gptr0, char_type* egptr0)
{
    eback_ = eback0;
    gptr_  = gptr0;
    egptr_ = egptr0;
}





}// namespace

#endif

