// Copyright (C) 2019-2023 Manuel Perez 
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
#include <avr_time.h>
#include "../../../mega_UART.h"

#include <atd_ostream.h>

// Microcontroller
// ---------------
namespace my_mcu = mega_;


// Hwd devices
// -----------
using UART = my_mcu::UART_basic;

// Cfg
// ---
static constexpr uint16_t max_timeout_ms = 5000;
static constexpr uint8_t read_byte_fail = 0x04; // EOT
						
// Functions
// ---------
void init_uart()
{
    my_mcu::UART_basic_cfg();

    UART::enable_receiver();
    UART::enable_transmitter();
}

// Devuelve el número de caracteres escritos.
uint8_t write(char c, uint16_t timeout = max_timeout_ms)
{
    while (!UART::is_data_register_empty())
    { 
	if (timeout == 0)
	    return 0;

	--timeout;
	my_mcu::wait_ms(1);
    }

    UART::data_register(c);

    return 1;
}


// Devuelve el número de caracteres escritos.
uint8_t write(const char* s, uint16_t timeout = max_timeout_ms)
{
    uint8_t nchars = 0;

    while(*s != '\0'){
	if (write(*s, timeout) == 0)
	    return nchars;

	++s;
	++nchars;
    }

    return nchars;
}

// Devuelve el caracter leído. 
// ¿Qué devolver en caso de error? Los primeros caracteres de ASCII son
// caracteres de control. Se puede usar uno de ellos. Posibilidades:
// EOT (end of transmission), ETB (end of transmission block), CAN (cancel) 
// :???
uint8_t read_byte(uint16_t timeout = max_timeout_ms)
{
    while(!UART::are_there_data_unread()) {
	if (timeout == 0)
	    return read_byte_fail;

	--timeout;
	my_mcu::wait_ms(1);
    }

    return UART::data_register();
}


uint8_t read(char* msg, uint8_t size, uint16_t timeout = max_timeout_ms)
{
    uint8_t i = 0;

    while (1) {
	uint8_t c = read_byte(timeout);
	if (c == read_byte_fail)
	    return i;

	msg[i] = c;
	++i;

	if (i == size or c == '\n')
	    return i;
    }

    return i;
}


void print_int_as_hex(uint8_t x)
{
    write("0x");

    for (size_t i = sizeof(uint8_t); i > 0; --i){
	uint8_t b = (x >> (i - 1) * 8);
	uint8_t b1 = (b >> 4) & 0x0F;
	uint8_t b0 = (b & 0x0F);

	write(atd::nibble2hex(b1));
	write(atd::nibble2hex(b0));

    }
}


void print_register(const char* name, uint8_t value)
{
    write(name);
    write(" = ");
    print_int_as_hex(value);
    write("\n\r");
}

void print_registers()
{
    write("\n\rRegisters\n\r"
	      "---------\n\r");

    print_register("UDR0  ", UDR0);

    print_register("UCSR0A", UCSR0A);
    print_register("UCSR0B", UCSR0B);
    print_register("UCSR0C", UCSR0C);

    print_register("UBRR0H", UBRR0H);
    print_register("UBRR0L", UBRR0L);
}


void main_hello()
{
    write("\r\n\nUART_basic test\n\r"
	        "---------------\n\r");
}


int main() 
{
    init_uart();

    main_hello();

    while(1){
	write("\r\nMenu\n\r"
		  "----\n\r"
		  "0. print registers\n\n\r"
		  "You can also:\n\r"
		  "+ Write any other character to test 'read_byte'\n\r"
		  "+ Don't write anything and test timeout\n\n\r");

	char opt{};
	opt = read_byte();

	switch (opt){
	    break; case '0': print_registers();
	    break; case read_byte_fail: write("Read fail.  Time out?\n\r");
	    break; default: 
			write("You have written [");
			write(opt);
			write("]\n\r");
	}



    }
}


