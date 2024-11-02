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

// CUIDADO: como hay tantas cadenas de texto tengo la RAM casi llena, a la
// menor se produce stack overflow. Le he añadido un check_buffer para
// garantizar que no se produce. La idea es escribir el buffer con un valor
// conocido (0x55) y comprobar que antes de usar el buffer sigue siendo este
// valor. Para evitar esto habría que meter las cadenas de texto en la EEPROM
// del avr.
#include "../../../dev_EEPROM_iostream.h"
#include <dev_25LC256_basic.h>
#include "../../../dev_EEPROM_lineal.h"

#include <iomanip>
#include <atd_time.h>
#include <atd_cast.h>
#include <atd_cstddef.h>

#include <avr_atmega.h>


// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;


constexpr uint8_t period_in_us = 16;

constexpr uint8_t num_pin_chip_select = 16;
constexpr uint8_t buffer_size = 2;
using EEPROM_lineal =
    dev::EEPROM_lineal<dev::EEPROM_25LC256<num_pin_chip_select>>;

using EEPROM_ostream = dev::EEPROM_ostream<EEPROM_lineal, buffer_size>;
using EEPROM_istream = dev::EEPROM_istream<EEPROM_lineal, buffer_size>;

using SPI = myu::SPI_master;

constexpr int sz = 80;

// necesito ahorrar memoria
constexpr const char ERROR[] = "ERROR: ";

void print_buffer(uint16_t addr0, uint8_t* buf, uint8_t n)
{
    UART_iostream uart;

    uint8_t* pe = buf + n;

    for (uint8_t i = 0; i < n; ){
	uart << std::setw(3) << std::right << addr0 + i << " | ";
	uart << std::left;
	uint8_t* p = &buf[i];
	for (uint8_t j = 0; p != pe and j < 10; ++p, ++j){
	    atd::print_in_hex(uart, std::byte{*p});    // TODO: uart << std::hex << *p;
	    //uart << std::to_integer<char>(*p) << "  ";
	    uart << ' ';
	    // uart << std::to_integer<uint16_t>(buf[i]) << " ";
	}
	uart << "  |";
	for (uint8_t j = 0; j < 10 and i < n; ++j, ++i){
	    char c = static_cast<char>(buf[i]);
	    if (c != '\n')
		uart << c;
	    else
		uart << "\\n";
	}
	
	uart << "\n";
    }

}

uint8_t buf[sz];

void init_buffer()
{
    for (uint16_t i = 0; i < sz; ++i)
	buf[i] = uint8_t{0x55};
}

// Solo hago el check la primera vez, ya que después voy a escribir en el
// buffer
void check_buffer()
{
    static bool comprobado = false;

    if (comprobado)
	return;
    comprobado = true;

    UART_iostream uart;
    for (uint16_t i = 0; i < sz; ++i){
	if (buf[i] != uint8_t{0x55})
	    uart << ">>> " << ERROR << "stack overflow!!! buffer corrupto\n";
    }
}


void read_and_print(EEPROM_lineal& eeprom, uint16_t addr0, uint8_t n = sz)
{
    check_buffer();
    UART_iostream uart;

    if (!eeprom.good()) {
	uart << ERROR << "eeprom en mal estado!\n";
	return;
    }

//    uint8_t buf[sz];
    auto leidos = eeprom.read(addr0, buf, n);
    if (leidos != n)
	uart << ERROR << "no se han podido leer " << n<< " bytes\n";

    print_buffer(addr0, buf, n);
}

constexpr const char 
MENU_INTERACTIVO[] =  "Elige:\n"
		      "[c]lear (los primeros bytes)\n"
		      "[f]lush\n"
		      "[n]úmeros secuenciales\n"
		      "[u]int16_t\n"
		      "[t]exto\n"
		      "[r]eopen\n"
		      "[e]nd\n";

void test_eeprom_interactiva()
{
    UART_iostream uart;
    SPI::clock_period_in_us<period_in_us>();
    SPI::turn_on();

    EEPROM_ostream out;
    out.open(0);

    while(1){
	uart.clear();
	uart << "\nPrueba EEPROM (interactiva)\n"
	     <<   "---------------------------\n";
	
	read_and_print(out.eeprom(), 0);
	uart << "--\n";
//	read_and_print(out.eeprom(), (0x7FFFul + 1ul) - 100);


	uart << MENU_INTERACTIVO;

	char c;
	uart.get(c);
	if (c == 'e')
	    return;
	else if (c == 'f'){
	    uart << "Haciendo flush ... ";
	    if (out.flush())
		uart << "OK\n";
	    else 
		uart << "ERROR!!!\n";

	}
	else if (c == 'c'){
	    out.close();
	    if (out.open(0)){
		char c{0};
		for (uint16_t i = 0; i < sz; ++i)
		    out.write(&c, 1);
//		    out << uint16_t{0};	// no funciona char{0} ya que 
				    // el 0 == '\0' caracter que se ignora!!!
		out.close();
	    }
	    else
		uart << "\nError al intentar hacer el clear!\n";
	    out.open(0);
	}
	else if (c == 'r'){
	    out.close();
	    uart << "Dirección en el que abrimos el fichero:";
	    uint16_t addr;
	    uart >> addr;
	    uart << "\n" << addr << "\n";
	    if (uart){
		out.open(addr);
		uart << "Abierto OK\n";
	    } else{
		out.open(0);
		uart.clear();
		uart << "Abierto en la dirección 0\n";
	    }
	}

	else if (c == 'u'){
	    uart << "\nEscribe un uint16_t:\n";
	    uint16_t n = 0;
	    uart >> n;
	    if (!uart)
		continue;
	    
	    uart << "Escribiendo: [" << n << "]\n";
	    out << n;





	}

	else if (c == 'n'){
	    uart << "\n¿A partir de qué número quieres almacenar?\n";
	    uint16_t tmp;
	    uart >> tmp;
	    if (!uart)
		continue;
	    uint8_t n0 = atd::bounded_cast<uint8_t>(tmp);
	    uart << n0 << "\n";

	    uart << "\n¿Cuántos números almacenamos?\n";
	    uart >> tmp;
	    if (!uart)
		continue;
	    uint8_t n = atd::bounded_cast<uint8_t>(tmp);
	    uart << n << "\n";

	    if (n > sz){
		uart << "Demasiado grande. Almacenamos " << sz << " números\n";
		n = sz;
	    }

	    // OJO: un flujo escribe caracteres, luego si escribimos 
	    // out << 123 ---> lo traduce en '1', '2', '3' (son 3 bytes!)
	    for (uint8_t i = 0; i < n; ++i){
		char c = n0 + i;
		out.write(&c, 1);   // para escribir bytes, usar write!!!

				    // sino 123 lo escribe como '1''2''3'!!!
//		out << n0 + i;
	    }

	    if (out){
		uart << "OK\n";
		uart << "(no se verá el resultado hasta que no se haga un flush, o "
			"se llene el buffer de 8 bytes)\n";
	    }else{
		uart << ERROR << "\n";
		out.clear();// TODO: ¿y si ha llegado al final de la eeprom?
	    }

        }
	else if (c == 't'){
	    uart << "Escribe lo que quieras almacenar (fin return):\n";
	    char str[sz];
	    if (uart.getline(str, sz, '\r')){
		uart << "Escribiendo [" << str << "]\n";
		out << str;
	    }
	    else{
		uart.clear();
		uart << ERROR << "al leer la línea\n";
	    }
	}
	
    }// while
}







int main()
{
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();

    while(1){
	init_buffer();
	test_eeprom_interactiva();
    }
}
