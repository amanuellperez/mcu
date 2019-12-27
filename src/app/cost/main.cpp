// Copyright (C) 2019-2020 A.Manuel L.Perez
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
#include <avr_USART_iostream.h>


void show_sizes_types()
{
    avr::UART_iostream uart;
    avr::cfg_basica(uart);
    uart.on();

    while(1){
	uart << "Tamaños de los tipos del avr\n\r";
	uart << "----------------------------\n\r";
	uart << "sizeof(char) = " << sizeof(char) << "\n\r";
	uart << "sizeof(short) = " << sizeof(short) << "\n\r";
	uart << "sizeof(int) = " << sizeof(int) << "\n\r";
	uart << "sizeof(long) = " << sizeof(long) << "\n\r";
	uart << "sizeof(long long) = " << sizeof(long long) << "\n\r";
	uart << "\n\r";

	uart << "sizeof(unsigned short) = " << sizeof(unsigned short) << "\n\r";
	uart << "sizeof(unsigned int) = " << sizeof(unsigned int) << "\n\r";
	uart << "sizeof(unsigned long) = " << sizeof(unsigned long) << "\n\r";
	uart << "sizeof(unsigned long long) = " << sizeof(unsigned long long) << "\n\r";
	uart << "\n\r";

	uart << "sizeof(int8_t) = " << sizeof(int8_t) << "\n\r";
	uart << "sizeof(int16_t) = " << sizeof(int16_t) << "\n\r";
	uart << "sizeof(int32_t) = " << sizeof(int32_t) << "\n\r";
	uart << "sizeof(int64_t) = " << sizeof(int64_t) << "\n\r";
	uart << "\n\r";

	uart << "sizeof(uint8_t) = " << sizeof(uint8_t) << "\n\r";
	uart << "sizeof(uint16_t) = " << sizeof(uint16_t) << "\n\r";
	uart << "sizeof(uint32_t) = " << sizeof(uint32_t) << "\n\r";
	uart << "sizeof(uint64_t) = " << sizeof(uint64_t) << "\n\r";
	uart << "\n\r";

	uart << "sizeof(char*) = " << sizeof(char*) << "\n\r";
	uart << "sizeof(short*) = " << sizeof(short*) << "\n\r";
	uart << "sizeof(int*) = " << sizeof(int*) << "\n\r";
	uart << "sizeof(long*) = " << sizeof(long*) << "\n\r";
	uart << "sizeof(long long*) = " << sizeof(long long*) << "\n\r";
	uart << "\n\r";

	char c = '\0';
	uart >> c;
    }
}

int main()
{
    show_sizes_types();
}

