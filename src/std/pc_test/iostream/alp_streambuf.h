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

#ifndef __ALP_STREAMBUF_H__
#define __ALP_STREAMBUF_H__

#ifndef __MCU_STD_STREAMBUF_H__
#include "../../std_streambuf.h"
#endif

#include <iostream>
#include <vector>
#include <cstring>

#include <string.h> // memcpy
using namespace std;

// Flujo solo de salida
struct LCD
{
//    int print(char c) {std::cout << c; return 1;}
    int print(char c) {std::cout.put(c); return 1;}
};


// Flujo bidireccional
struct UART
{
    UART():i{0} {}
    UART(const char* msg0):msg_{msg0}, i{0} {}

    int print(char c) {std::cout << c; return 1;}
    bool read(char& c) 
    {
	if (msg_[i] == '\0')
	    return false;

	c = msg_[i];
	++i;

	return true;
    }

    void reset() { i = 0;}

    constexpr static char msg[] = "1234567890ABCDE67890abcde";
    const char* msg_ = msg;

    int i;
};


namespace mtd{
class my_streambuf:public mtd::streambuf{
public:
    // Fundamental: inicializar la put y get area.
    my_streambuf(LCD& lcd):lcd_{lcd}
    { }


    virtual ~my_streambuf() {}

protected:
    virtual int_type overflow(int_type c = traits_type::eof()) override
    { 
	lcd_.print(static_cast<char_type>(c));
	return 1;
    }

    // Esta no es necesario escribirla
    virtual streamsize xsputn(const char_type* s, streamsize n) override
    { 
	for (int i = 0; i < n; ++i)
	    lcd_.print(s[i]);

	return n;
    }

private:
    LCD& lcd_;   
};



// Se trata de un buffer que puede usarse como bidireccional.
// Voy a usar un único array como buffer tanto para la put como la get area.
// La put area estará activa cuando pptr() != 0, mientras que la get area
// estará activa cuando gptr() != 0. Aunque supongo que debería de usar
// nullptr (o no???) es más cómodo usar 0 como nullptr.
class fix_streambuf:public mtd::streambuf{
public:
    fix_streambuf(UART& uart):uart_{uart}
    {
	as_output();
    }

    void as_output()
    {
	if (gptr())
	    sync_get_area();
	
	setp(buffer_, buffer_ + buffer_size); 
	setg(buffer_, 0, buffer_ + buffer_size); 
    }

    void as_input()
    {
	if (pptr())
	    sync_put_area();
	
	setp(buffer_, buffer_ + buffer_size); 
	setg(buffer_, buffer_ + buffer_size, buffer_ + buffer_size); 
    }


    void reset() {uart_.reset();}

    // para depurar
    void print_get_area()
    {
	std::cout << "get area = [";
	for (char* p = gptr(); p != egptr(); ++p)
	    std::cout << *p;
	std::cout << "]\n";
    }

    void print_put_area()
    {
	std::cout << "put area = [";
	for (char* p = pbase(); p != pptr(); ++p)
	    std::cout << *p;
	std::cout << "]\n";
    }

    void print_buffer()
    {
	std::cout << "\nbuffer_ = ";
	for (int i = 0; i < buffer_size; ++i)
	    std::cout << buffer_[i];
	std::cout << "\n";
    }
protected:
    // Devuelve -1 en fallo.
    virtual int sync() override 
    {
	if (pptr())
	    return sync_put_area();

	if (gptr())
	    return sync_get_area();

	return -1;
    }


    virtual int_type overflow(int_type c = traits_type::eof()) override
    { 
	if (pptr() == 0)
	    return traits_type::eof();

	sync_put_area();

	*pptr() = c;
	pbump(1);

	return 1;
    }


    virtual int_type underflow() override
    {
	if (gptr() == 0)
	    return traits_type::eof();
	
	if (sync_get_area() == -1)
	    return traits_type::eof();

	return *gptr();
    }

private:
    UART& uart_;   
    constexpr static int buffer_size = 10;
    char_type buffer_[buffer_size];

    int sync_put_area()
    {
	int i = 0;
	for (auto p = pbase(); p != pptr(); ++p, ++i)
	    uart_.print(*p);

	setp(buffer_, buffer_ + buffer_size);
	return i;
    }

    int sync_get_area()
    {
	char_type data[buffer_size];
    
	int n = read_get_area(data, buffer_size);

	if (n == 0) return -1;

	char_type* gptr0 = buffer_ + buffer_size - n;
	memcpy(gptr0, data, n);
//print_buffer();

	setg(buffer_, gptr0, buffer_ + buffer_size);

//print_get_area();

	return n;
    }


    // Lee de la input sequence máximo N caracteres guardándolos en s.
    // Devuelve el número de caracteres leidos.
    int read_get_area(char_type* s, int N)
    {
	char c;
	for (int i = 0; i < N; ++i){
	    if (!uart_.read(c))
		return i;

	    s[i] = traits_type::to_char_type(c);
	}

	return N;
    }


};



} // namespace



#endif

