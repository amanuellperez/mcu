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

// ¿Qué tamaños tienen los distintos tipos básicos en avr-gcc?
#include <mega.h>
#include <bit>

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

void show_sizes_types()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

    while(1){
	uart << "Type sizes\n";
	uart << "----------\n";
	uart << "sizeof(char) = " << sizeof(char) << "\n";
	uart << "sizeof(short) = " << sizeof(short) << "\n";
	uart << "sizeof(int) = " << sizeof(int) << "\n";
	uart << "sizeof(long) = " << sizeof(long) << "\n";
	uart << "sizeof(long long) = " << sizeof(long long) << "\n";
	uart << "\n";

	uart << "sizeof(unsigned short) = " << sizeof(unsigned short) << "\n";
	uart << "sizeof(unsigned int) = " << sizeof(unsigned int) << "\n";
	uart << "sizeof(unsigned long) = " << sizeof(unsigned long) << "\n";
	uart << "sizeof(unsigned long long) = " << sizeof(unsigned long long) << "\n";
	uart << "\n";

	uart << "sizeof(int8_t) = " << sizeof(int8_t) << "\n";
	uart << "sizeof(int16_t) = " << sizeof(int16_t) << "\n";
	uart << "sizeof(int32_t) = " << sizeof(int32_t) << "\n";
	uart << "sizeof(int64_t) = " << sizeof(int64_t) << "\n";
	uart << "\n";

	uart << "sizeof(uint8_t) = " << sizeof(uint8_t) << "\n";
	uart << "sizeof(uint16_t) = " << sizeof(uint16_t) << "\n";
	uart << "sizeof(uint32_t) = " << sizeof(uint32_t) << "\n";
	uart << "sizeof(uint64_t) = " << sizeof(uint64_t) << "\n";
	uart << "\n";

	uart << "sizeof(char*) = " << sizeof(char*) << "\n";
	uart << "sizeof(short*) = " << sizeof(short*) << "\n";
	uart << "sizeof(int*) = " << sizeof(int*) << "\n";
	uart << "sizeof(long*) = " << sizeof(long*) << "\n";
	uart << "sizeof(long long*) = " << sizeof(long long*) << "\n";
	uart << "\n";

	if (std::endian::native == std::endian::little)
	    uart << "is little endian\n";
	else if (std::endian::native == std::endian::big)
	    uart << "big endian\n";
	else 
	    uart << "unknown endianness!!!\n";


	char c = '\0';
	uart >> c;
    }
}

int main()
{
    show_sizes_types();
}

