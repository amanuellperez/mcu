// Copyright (C) 2019-2024 Manuel Perez 
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

// Probamos el UART.
// Conectar el FTDI y abrir screen. Lo que se escriba en teclado se envia
// al microcontrolador que lo devuelve, con lo que lo vemos en pantalla.
#include "../../mcu_UART_iostream.h"
#include <avr_atmega.h> 

#include <atd_istream.h>
#include <atd_ostream.h>

#include <avr/power.h>

// Microcontroller
// ---------------
namespace myu = atmega;
using UART = myu::UART_8bits;
using UART_iostream = mcu::UART_iostream<UART>;

constexpr char end_of_line = '\n'; // para usar `myterm`
constexpr const char end_of_line_as_char[] = "\\n";
//constexpr char end_of_line = '\r'; // para usar `screen` como terminal
//constexpr const char end_of_line_as_char[] = "\\r";

// Probados a 8 MHz:
//constexpr uint32_t baud_rate = 4'800u;    // funciona
constexpr uint32_t baud_rate = 9600u;	    // funciona
//constexpr uint32_t baud_rate = 14'400u;      NO FUNCIONA!!!
//constexpr uint32_t baud_rate = 19'200u;	    // funciona
//constexpr uint32_t baud_rate = 28'800u;	    NO funciona
//constexpr uint32_t baud_rate = 38'400u;	    // funciona
//constexpr uint32_t baud_rate = 57'600u;	    // funciona
//constexpr uint32_t baud_rate = 76'800u;	    NO funciona
// Los siguientes no funcionan a 8MHz pero sí a 16MHz:
//constexpr uint32_t baud_rate = 115'200;   
//constexpr uint32_t baud_rate = 230'400;	// FUNCIONA!!!
//constexpr uint32_t baud_rate = 250000; //NO funciona (???)
//constexpr uint32_t baud_rate = 460'800: <-- no aparece en la avr306
//constexpr uint32_t baud_rate = 500'000; // FUNCIONA!!!
//constexpr uint32_t baud_rate = 921'600;// No FUNCIONA (error 9%)
//constexpr uint32_t baud_rate = 1'000'000; // FUNCIONA!!!
constexpr uint32_t max_error = 3; // Típico error +-2.5%


using int_type = std::char_traits<char>::int_type;
using char_type = std::char_traits<char>::char_type;
using traits = std::char_traits<char>;

//void test_streambuf()
//{ 
//    myu::UART_iostream uart;
//    myu::basic_cfg<baud_rate>(uart);
//    uart.turn_on();
//
//    uart << "\n---------------\n";
//    uart << "Probando streambuf!\n";
//    uart << "-------------------\n\n";
//
//    uart << "Pulsa una tecla: \n\n";
//
//    while (1){
//	int_type d = uart.rdbuf()->sgetc();
//
//	uart << "\nHas escrito [";
//	uart.rdbuf()->sputc(traits::to_char_type(d));
//	uart << "]\n";
//	uart << "Pulsa otra tecla: ";
//    }
//}

    template <typename Int>
void test_int(UART_iostream& uart, const char* type)
{
    uart << "\n\nReading of a " << type << "\n";
    uart << "------------------\n";
    while(uart){
	uart << "Write a number between ";
	if (std::numeric_limits<Int>::min() == 0)
	    uart << '0';
	else
	    uart << std::numeric_limits<Int>::min() + Int{1};

	uart << " and " << std::numeric_limits<Int>::max() 
	    << " (or a letter to cancel): ";

	Int r32;
	uart >> r32;

	if (uart){
	    // CUIDADO con el orden: en el momento en que escribimos en
	    // uart perdemos el caracter almacenado en la get area (que es
	    // de 1 caracter). Luego si se quiere ver el último caracter
	    // hay que leerlo antes que escribir.
	    char c;
	    uart.get(c);
	    uart << "\nYou have written : [" << r32 << "]\n";
	    //	    uart << "\nLast character written [" << c << "]\n";
	}

    }

    // sale del bucle cuando falle algo en la lectura de un uint16_t
    uart.clear();
}

volatile bool time_out = false;

void test_interrupt_receive()
{
    UART_iostream uart;
    time_out = false;

    myu::enable_interrupts();
    UART::enable_interrupt_unread_data();

    uart << "Testing interrupt unread_data()\n"
	"\tPress a key to throw the interrupt\n";

    while (!time_out)
    { ; }


    uart << "\tOutside interrupt\n";

    myu::disable_interrupts();
    UART::disable_interrupt_unread_data();

    char c{};
    uart >> c;
    uart << "\tYou have written [" << c << "]\n";
}

void test_iostream()
{
    UART_iostream uart;
    myu::UART_basic_cfg<baud_rate, F_CPU, max_error>();
    uart.turn_on();

    uart << "\n----------\n";
    uart << "UART test!\n";
    uart << "----------\n";

    uart << "A = [";
    uart.put('a');
    uart << "]\n";

    while (1) {
	test_interrupt_receive();

	char c;
	uart << "\n\nPress a key to continue\n\n";

	uart << "A new line with a carriage return";
	uart << '\n';

	uart.get(c);
	uart << "\nYou have written [" << c << "]\n";

	uart << "\nTesting operator<<\n";
	uart << "--------------------\n";
	uart << "123u        =? ";
	uint8_t x8 = 123;
	uart << atd::write_as_int(x8);

	uart << "\n+123        =? ";
	int8_t s8 = 123;
	uart << atd::write_as_int(s8);

	uart << "\n-123        =? ";
	s8 = -123;
	uart << s8;

	uart << "\n45321u      =? ";
	uint16_t x16 = 45321;
	uart << x16;

	uart << "\n1237654321  =? ";
	int32_t s32 = 1237654321;
	uart << s32;

	uart << "\n-1237654321 =? ";
	s32 = -1237654321;
	uart << s32;


	uart << "\n\nTesting receive functions\n";

	uart << "\nReading uint8_t\n";
	uart << "\n---------------\n";
	while(uart){
	    uart << "Write a number less than 255 (or a letter to cancel): ";

	    uint8_t x = 0;  // = 0 para evitar warning compilador
	    uart >> atd::read_as_uint8_t(x);
	    uart << "\nYou have written: [" << atd::write_as_int(x)
		<< "]\n";
	}

	test_int<uint16_t>(uart, "uint16_t");
	test_int<int16_t>(uart, "int16_t");

	test_int<uint32_t>(uart, "uint32_t");
	test_int<int32_t>(uart, "int32_t");

	// Hay un error con sprintf y los 64, asi que de momento no lo 
	// pruebo. 
	test_int<uint64_t>(uart, "uint64_t");
	test_int<int64_t>(uart, "int64_t");

	uart << "\nReading a line\n";
	uart << "-----------------\n";
	uart << "Write a text line (end = " << end_of_line_as_char 
	    << ") (maximum number of characters: 10)\n";

	char str[11];
	// CUIDADO: con screen necesito '' en lugar de '\n' como fin de
	// linea.
	// El '\r' es necesario si se usa `screen` terminal, que es el que yo
	// estoy usando. Parece ser (???) que el protocolo TTY es enviar \r en
	// lugar de \n.
	if (uart.getline(str, 11, end_of_line))
	    uart << "You have written: [" << str << "]\n";
	else {
	    uart.clear();
	    uart << "Error reading the line\n";
	}

	uart << "\n\nTesting transmission\n";
	uart << "--------------------\n";
	uart << "Write whatever you like. If your terminal echo "
	    "the output you are going to see what you write. Press 0 to end:\n";

	constexpr uint8_t N = 20;
	char msg[N + 1];
	int i = 0;

	while(1){
	    char c;
	    if (uart.get(c)){
		msg[i] = c;
		++i;

		if (i == N or c == '0'){
		    msg[i] = '\0';
		    uart << " ---> You have written: [" << msg << "]\n";
		    i = 0;
		}

		if (c == '0')
		    break;

	    }
	    else
		break;
	}

	uart.clear();

	{ // Hexadecimal test (for myterm)
	    uart << "\n\nSend hexadecimal block? (y/n) ";
	    char ans{};
	    uart >> ans;
	    if (ans == 'y' or ans == 'Y'){
		uart << "HEX\n";
		for (uint8_t i = 0; i < 255; ++i)
		    uart.put(i);
		uart.put(255);
		uart << "HEX\n";
	    }
	}

    }

}


int main()
{
    //    test_streambuf();
    test_iostream();
}


ISR_USART_RX{
    UART_iostream uart;

// Es obligatorio o bien vaciar el buffer o desactivar la interrupción para
// evitar que se relance.
// Método 1
// --------
//    char c;
//    uart >> c;
//
// Método 2
// --------
    UART::disable_interrupt_unread_data();

    uart << "\n\tInterruption: Inside ISR_USART_RX\n\n";

    time_out = true;

}

