// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
#include "../../../avr_USART_iostream.h"
#include "../../../avr_time.h"

#include <atd_istream.h>
#include <atd_ostream.h>

#include <avr/power.h>
using namespace avr;



using int_type = std::char_traits<char>::int_type;
using char_type = std::char_traits<char>::char_type;
using traits = std::char_traits<char>;

void test_streambuf()
{ 
    UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

    uart << "\n---------------\n";
    uart << "Probando streambuf!\n";
    uart << "-------------------\n\n";

    uart << "Pulsa una tecla: \n\n";

    while (1){
	int_type d = uart.rdbuf()->sgetc();

	uart << "\nHas escrito [";
	uart.rdbuf()->sputc(traits::to_char_type(d));
	uart << "]\n";
	uart << "Pulsa otra tecla: ";
    }
}

template <typename Int>
void test_int(UART_iostream& uart, const char* tipo)
{
    uart << "\n\nLectura de un " << tipo << "\n";
    uart << "----------------------\n";
    while(uart){
	uart << "Escribe un número menor de ";
	if (std::numeric_limits<Int>::min() == 0)
	    uart << '0';
	else
	    uart << std::numeric_limits<Int>::min() + Int{1};

	uart << " a " << std::numeric_limits<Int>::max() << " (o enter para fin): ";

	Int r32;
	uart >> r32;

	if (uart){
	    // CUIDADO con el orden: en el momento en que escribimos en
	    // uart perdemos el caracter almacenado en la get area (que es
	    // de 1 caracter). Luego si se quiere ver el último caracter
	    // hay que leerlo antes que escribir.
	    char c;
	    uart.get(c);
	    uart << "\nHas escrito el número: [" << r32 << "]\n";
	    uart << "\nUltimo caracter escrito [" << c << "]\n";
	}

    }
    
    // sale del bucle cuando falle algo en la lectura de un uint16_t
    uart.clear();
}


void test_iostream()
{
    UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

    uart << "\n---------------\n";
    uart << "Probando UART!\n";
    uart << "---------------\n";

    while (1) {
	char c;
	uart << "\n\nPulsa una tecla para continuar\n\n";

	uart << "Un retorno de carro";
	uart << '\n';

	uart.get(c);
	uart << "\nHas escrito [" << c << "]\n";

	uart << "\nProbando operator<<\n";
	uart << "-------------------\n";
	uart << "123u        =? ";
	uint8_t x8 = 123;
	uart << atd::write_as_uint8_t(x8);

        uart << "\n+123        =? ";
	int8_t s8 = 123;
        uart << atd::write_as_int8_t(s8);

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


	uart << "\n\nProbando funciones receive\n";
	
	uart << "\nLectura de un uint8_t\n";
	uart << "\n---------------------\n";
	while(uart){
	    uart << "Escribe un número menor de 255 (o enter para fin): ";

	    uint8_t x = 0;  // = 0 para evitar warning compilador
	    uart >> atd::read_as_uint8_t(x);
            uart << "\nHas escrito el número: [" << atd::write_as_uint8_t(x)
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

	uart << "\nLectura de un línea\n";
	uart << "-------------------\n";
	uart << "Escribe una línea de texto (fin = '\r') (máximo de 10 caracteres)\n";

	char str[11];
	// CUIDADO: con screen necesito '' en lugar de '\n' como fin de
	// linea.
	if (uart.getline(str, 11, '\r'))
	    uart << "Has escrito: [" << str << "]\n";
	else {
	    uart.clear();
	    uart << "Error al leer la linea\n";
	}

	uart << "\n\nProbando que lo que escribes se transimte bien\n";
	uart << "----------------------------------------------\n";
	uart << "Escribe lo que quieras, tienes que verlo en pantalla:\n";

	while(1){
	    char c;
	    if (uart.get(c)){
		if (c == '\r')
		    uart << '\n';

		uart << c;
	    }
	    else
		break;
	}

	uart.clear();
    }

}


int main()
{
//    test_streambuf();
    test_iostream();
}

