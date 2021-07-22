// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "dev.h"
#include <atd_buffer.h>
#include <stdio.h> // sscanf 

extern LCD lcd;	// TODO borrame

#undef getchar

// DUDA: no hereda las funciones de atd::Buffer, teniendo que volverlas a
// escribir para darle visibilidad. ¿Por qué?
template <size_t N>
class Stream_buffer : public atd::Buffer<char, N>{
public:
    using Parent = atd::Buffer<char, N>;

    using value_type	    = Parent::value_type;
    using pointer           = Parent::pointer;
    using const_pointer     = Parent::const_pointer;
    using reference         = Parent::reference;
    using const_reference   = Parent::const_reference;
    using size_type         = Parent::size_type;
    using difference_type   = Parent::difference_type;
    using iterator          = Parent::iterator;
    using const_iterator    = Parent::const_iterator;


    constexpr Stream_buffer(): Parent{} { init(); }
    constexpr Stream_buffer(std::initializer_list<value_type> v) : Parent{v}
    { init(); }

    /// Appends the given element to the end of the container.
    /// Es responsabilidad del usuario garantizar que size() != max_size()
    constexpr void push_back(char x) {Parent::push_back(x);}

    /// Removes the last element of the container.
    /// Es responsabilidad del usuario garantizar que size() > 0
    constexpr void pop_back() {Parent::pop_back();}

    /// Erases all elements from the container.
    constexpr void clear() {Parent::clear(); reset();}

    // iterators
    constexpr iterator begin()  {return Parent::begin();}
    constexpr iterator end()  {return Parent::end();}

    constexpr const_iterator begin() const  {return Parent::begin();}
    constexpr const_iterator end() const  {return Parent::end();}

    char getchar()
    {
	if (p_ != pe_)
	    return *p_++;

	return 0;
    }

    void unget_last_char()
    {
	if (p_ != begin())
	    --p_;
    }
    
    /// Le añade un '\0' al final del buffer, para poderlo leer con sscanf
    /// o como cadena de C.
    void write_endl()
    {
	if (p_ == pe_)
	    --p_;

	 *p_ = '\0';

    }

    void read(double* x)
    {	
	if (p_ != pe_){
	    int n = sscanf(p_, "%lf", x); 
	    if (n != EOF)
		p_ += n;
	    // else error() TODO
	}

	else
	    *x = 2.222;  // error() TODO
    }

    uint8_t push_back(const char* str);

    bool eof() const {return p_ == pe_;}

    /// Prepara el buffer para empezar a leerlo o escribirlo
    void reset() { p_ = begin(); }


    // capacity:
    constexpr bool empty() const  {return Parent::empty();}
    constexpr size_type size() const  {return Parent::size();}
    constexpr size_type max_size() const  {return Parent::max_size();}

    // Los datos válidos serán de [0, size()) = [begin, end)
    constexpr pointer data() {return Parent::data();}
    constexpr const_pointer data() const {return Parent::data();}

private:
    pointer p0_, pe_; // zona de memoria del buffer [p0_, pe_)
		      // CUIDADO: no confundirlo con [begin, end) que ese 
		      // es el buffer escrito (end != pe_ en general!!!)
    pointer p_; // posición actual

    void init()
    {
	p0_ = data();
	pe_ = p0_ + max_size();

	reset();

    }

    constexpr void push_back_char(char x) {Parent::push_back(x);}
};


template <size_t N>
void print(LCD& lcd, const Stream_buffer<N>& buf)
{
    for (auto x: buf)
	lcd << x;
}



/// Devuelve el número de caracteres añadidos a 'buf'.
template <size_t N>
uint8_t Stream_buffer<N>::push_back(const char* str)
{
    uint8_t i = 0;
    
    while (*str and p_ != pe_){
	push_back_char(*str);
	++str;
	++p_;
    }

    return i;
}


#endif


