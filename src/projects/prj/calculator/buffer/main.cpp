// Copyright (C) 2021 Manuel Perez 
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

// Probamos buffer
// Sacamos la salida por UART
#include <avr_atmega.h>
#include "../buffer.h"

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

using Buffer = Linear_array<20>;

inline std::ostream& operator<<(std::ostream& out, const Buffer& buf)
{
    print(out, buf);
    return out;
}

int main()
{
// init_UART();
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
    
    uart << "\n-----\n";
    uart << "Buffer\n";
    uart << "------\n\n";


    while(1){
	Buffer buf;
	buf.push_back('1');
	uart << "push_back =\t [" << buf << "] == 1?\n";
	buf.clear();
	push_back(buf, "123+456");
	uart << "push_back =\t [" << buf << "] == 123+456?\n";

	char d = buf.pop_front();
	uart << "pop_front =\t [" << d << "] == 1?\n";
	uart << "pop_front =\t [" << buf << "] == 23+456?\n";
	buf.push_front(d);
	uart << "push_front =\t [" << buf << "] == 123+456?\n";

// read double
	{
	buf.clear();
	uart << "clear =\t\t [" << "] == ''?\n";
	push_back(buf, "123.456+789.01");
	uart << "push_back =\t [" << buf << "] == 123.456+789.01?\n";
	double a{};
	read(buf, a);
	uart << "read =\t [";
	atd::print(uart, a);
	uart << "] == 123.456\n";
	uart << "buffer =\t [" << buf << "] == +789.01?\n";
	d = buf.pop_front();
	uart << "getchar =\t [" << d << "] == +?\n";
	uart << "buffer =\t [" << buf << "] == 789.01?\n";

	read(buf, a);
	if (a != 789.01)
	    uart << "ERROR: DIFERENTES\n";
	uart << "read =\t [";
	atd::print(uart, a);
	uart << "] == 789.01\n";
	uart << "buffer =\t [" << buf << "] == ''?\n";

	}





	char c{};
	uart >> c;

    }
}



