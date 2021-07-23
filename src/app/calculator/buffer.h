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
#include <ostream>

#undef getchar



// Devuelve las unidades de x
inline int units(int x) { return x % 10; }

// TODO: a double.h
inline std::ostream& operator<<(std::ostream& out, double x)
{
    int i = static_cast<int>(x);
    out << i;

    x -= i;
    // CUIDADO: la parte entera del double es máximo 2^15 - 1= 32767!!!
    // TODO: parametrizarlo
    i = static_cast<int>(x*10000.0); // max. 4 cifras

    // TODO: mejorarlo
    while (i != 0 and units(i) == 0)
	i /= 10;

    if (i != 0)
	out << '.' << i;

    return out;
}
// TODO: a double.h? ponerlo en un .cpp
inline double convert_to_decimal(int x)
{
    double y = static_cast<double>(x);


    while (y > 0){
	y /= 10;
    }

    return y;
}





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

    enum class State{good, bad, scanf_error, read_eof};

    constexpr static const char decimal_point = '.';

    constexpr Stream_buffer(): Parent{} { init(); }
    constexpr Stream_buffer(std::initializer_list<value_type> v) : Parent{v}
    { init(); }

// Funciones de escritura: 
//     Escribimos al final del buffer.
    /// Appends the given element to the end of the container.
    /// Es responsabilidad del usuario garantizar que size() != max_size()
    constexpr void push_back(char x) {Parent::push_back(x);}

    /// Devuelve el número de caracteres añadidos a 'buf'.
    constexpr uint8_t push_back(const char* str);

    /// Removes the last element of the container.
    /// Es responsabilidad del usuario garantizar que size() > 0
    constexpr void pop_back() {Parent::pop_back();}

// Funciones de lectura:
//    Leemos desde el principio (de izda a dcha). Primero el caracter 0, luego
//    el 1, ... (justo al revés que la escritura).

    /// Extraemos un caracter del buffer.
    char getchar();

    /// Devolvemos el último caracter leído.
    void unget_last_char();

    /// Devuelve el número de caracteres extraídos del buffer.
    uint8_t read(int& x);

    /// Devuelve el número de caracteres extraídos del buffer.
    uint8_t read(double& x);

// Reset
    /// Erases all elements from the container.
    constexpr void clear() {Parent::clear(); reset();}

    /// Prepara el buffer para empezar a leerlo.
    constexpr void reset() { p_ = Parent::begin(); state_ = State::good;}

// Iterators
    constexpr iterator begin()  {return p_;}
    constexpr iterator end()  {return Parent::end();}

    constexpr const_iterator begin() const  {return p_;}
    constexpr const_iterator end() const  {return Parent::end();}

    
// state
    bool eof() const {return p_ == end();}
    bool good() const {return state_ == State::good;}
    bool bad() const {return state_ == State::bad;}
    bool read_error() const {return state_ == State::scanf_error 
				or state_ == State::read_eof;}




    // capacity:
    constexpr bool empty() const  {return Parent::empty();}
    constexpr size_type size() const  {return Parent::size();}
    constexpr size_type max_size() const  {return Parent::max_size();}

    // Los datos válidos serán de [0, size()) = [begin, end)
    constexpr pointer data() {return Parent::data();}
    constexpr const_pointer data() const {return Parent::data();}

private:
    pointer m0_, me_; // zona de memoria del buffer [m0_, me_)
		      // CUIDADO: no confundirlo con [begin, end) que ese 
		      // es el buffer escrito (end != me_ en general!!!)
    pointer p_; // posición actual

    // State
    State state_;

    void init()
    {
	m0_ = data();
	me_ = m0_ + max_size();

	reset();

    }

    constexpr void push_back_char(char x) {Parent::push_back(x);}

};


template <size_t N>
inline std::ostream& operator<<(std::ostream& out, const Stream_buffer<N>& buf)
{
    for (auto x: buf)
	out << x;

    return out;
}



template <size_t N>
constexpr uint8_t Stream_buffer<N>::push_back(const char* str)
{
    uint8_t i = 0;
    
    while (*str and size() != max_size()){
	push_back_char(*str);
	++str;
	++i;
    }

    return i;
}


template <size_t N>
uint8_t Stream_buffer<N>::read(int& x)
{

    uint8_t n = 0;
    
    x = 0;

    while (p_ != end()){
	int d = static_cast<int>(*p_); // digit
	if (!isdigit(d))
	    return n;

	x = 10*x + d - '0';

	++p_;
    }

    return n;
}




template <size_t N>
uint8_t Stream_buffer<N>::read(double& x)
{	
    int a; 
    uint8_t n = read(a);


    if (p_ == end()){
	x = a;
	return n;
    }

    if (*p_ != decimal_point)
	return n;
    ++p_;

    n += read(a);
    x += convert_to_decimal(a);

    return n;
}


template <size_t N>
char Stream_buffer<N>::getchar()
{
    if (p_ != end()){
// return *p_++:
	char res = *p_;
	++p_;
	return res;

    }

    return 0;
}

template <size_t N>
inline void Stream_buffer<N>::unget_last_char()
{
    if (p_ != m0_)
	--p_;
}



#endif


