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
#include <dev_25LC256_hwd.h>
#include "../../../dev_EEPROM_lineal.h"

#include <atd_time.h>

#include <mega.h>
#include <atd_cast.h>
#include <atd_cstddef.h>
#include <std_iomanip.h>

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

constexpr uint8_t period_in_us = 16;

constexpr uint8_t num_pin_chip_select = 16;
constexpr uint8_t buffer_size = 2;
using EEPROM = dev::EEPROM_25LC256<num_pin_chip_select>;
using EEPROM_lineal =
    dev::EEPROM_lineal<dev::EEPROM_25LC256<num_pin_chip_select>>;

using EEPROM_ostream = dev::EEPROM_ostream<EEPROM_lineal, buffer_size>;
using EEPROM_istream = dev::EEPROM_istream<EEPROM_lineal, buffer_size>;

using SPI_master = EEPROM::SPI_master;

constexpr int sz = 80;

// necesito ahorrar memoria
constexpr const char ERROR[] = "ERROR: ";

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




// Inicializa a 0 los bytes [address, address + n)
void reset_eeprom(EEPROM_lineal& eeprom, uint16_t address, uint16_t n)
{
    UART_iostream uart;

    uint8_t buffer[sz];
    n = std::min<uint16_t>(n, sz);

    std::fill_n(buffer, n, uint8_t{0});
    
    eeprom.write(address, buffer, n);
    if (!eeprom.good())
	uart << "EEPROM en mal estado\n"
	     << ERROR << "al inicializarla a 0\n\n";
}

bool check(EEPROM_lineal& eeprom,
           uint16_t addr0,
           char* res, uint8_t n,
           const char* msg)
{
    UART_iostream uart;
    uart << msg << " ... ";

    uint8_t buf[sz];
    auto leidos = eeprom.read(addr0, buf, n);
    if (leidos != n)
	uart << ERROR << "no se han podido leer " << n<< " bytes\n";

    for (uint8_t i = 0; i < n; ++i){
	if (buf[i] != uint8_t{static_cast<uint8_t>(res[i])}){
            uart << ERROR << "i = " << i << "; buf("
                 << static_cast<uint16_t>(buf[i]) << ") != res("
                 << static_cast<int16_t>(res[i]) << ")\n";

	    return false;
	}
    }

    uart << "OK\n";

    return true;
}


bool test_only_tm()
{
    UART_iostream uart;

    constexpr uint8_t APP_ID = 0x1F;
    tm t0;
    t0.tm_year = 2013 - 1900;
    t0.tm_mon = 4 - 1;
    t0.tm_mday = 10;
    t0.tm_hour = 2;
    t0.tm_min = 30;
    t0.tm_sec = 38;

    {
	EEPROM_ostream out{0};
	if (!out){
	    uart << "Error al abrir EEPROM_ostream(0)\n";
	    return false;
	}
	
	out << APP_ID;
	out << atd::only_date(t0) << ' ';
	out << atd::only_time(t0);
    }

    {
	EEPROM_istream in{0};
	if (!in){
	    uart << "Error al abrir EEPROM_istream(0)\n";
	    return false;
	}
	uint8_t id{};
	in >> id;
	if (!in){
	    uart << "Error al leer el id\n";
	    return false;
	}
	if (id != APP_ID){
	    uart << "id erroneo\n";
	    return false;
	}
	tm t;
	in >> atd::only_date(t);
	if (!in){
	    uart << "error al leer date\n";
	    return false;
	}

	uart << "date: " << atd::only_date(t) << '\n';

	char sep;
	in >> sep;

	in >> atd::only_time(t);
	if (!in){
	    uart << "error al leer time\n";
	    return false;
	}

	uart << "time: " << atd::only_time(t) << '\n';
    }

    return true;
}

bool test(uint16_t addr0)
{
    EEPROM_ostream out;
    UART_iostream uart;

    reset_eeprom(out.eeprom(), addr0, 10);
    uart << "Pruebas básicas de escritura. Dirección: " << addr0 << "\n";
    if (!out.open(addr0))
    {
	uart << ERROR << "al abrir la memoria\n";
	// return;
    }
    
    char buf[10] = {1,2,3,4,5,6,7,8,9,10};
    char res[10] = {0,0,0,0,0,0,0,0,0,0};

    out.write(buf, 2);
    if (!check(out.eeprom(), addr0, res, 10, "write(2)"))
	return false;

    out.flush();
    res[0] = buf[0];
    res[1] = buf[1];
    if (!check(out.eeprom(), addr0, res, 10, "flush()"))
	return false;

    out.write(&buf[2], 6);
    for (int i = 0; i < 6; ++i)
	res[i] = buf[i];

    if (!check(out.eeprom(), addr0, res, 10, "write(6)"))
	return false;

    out.close();

    for (int i = 0; i < 8; ++i)
	res[i] = buf[i];

    if (!check(out.eeprom(), addr0, res, 10, "close()"))
	return false;

    return true;
}


bool test_ostream_automatico()
{
    EEPROM_ostream out;
    UART_iostream uart;

    uart << "\n\nEEPROM ostream (automático)\n"
	 << "---------------------------\n";

    if (!test(0))
	return false;

    if (!test(60))	// en el límite del paginado
	return false;
//    {
//	reset_eeprom(out.eeprom(), 0, 10);
//	uart << "Pruebas básicas de escritura\n";
//	if (!out.open(0))
//	{
//	    uart << ERROR << "al abrir la memoria\n";
//	    // return;
//	}
//	
//	char buf[10] = {1,2,3,4,5,6,7,8,9,10};
//	char res[10] = {0,0,0,0,0,0,0,0,0,0};
//
//	out.write(buf, 2);
//	if (!check(out.eeprom(), 0, res, 10, "write(2)"))
//	    return false;
//
//	out.flush();
//	res[0] = buf[0];
//	res[1] = buf[1];
//	if (!check(out.eeprom(), 0, res, 10, "flush()"))
//	    return false;
//
//	out.write(&buf[2], 6);
//	for (int i = 0; i < 6; ++i)
//	    res[i] = buf[i];
//
//	if (!check(out.eeprom(), 0, res, 10, "write(6)"))
//	    return false;
//
//	out.close();
//
//	for (int i = 0; i < 8; ++i)
//	    res[i] = buf[i];
//	if (!check(out.eeprom(), 0, res, 10, "close()"))
//	    return false;
//    }

    {
	uart << "\nEscribiendo al final de la EEPROM\n";
	uint16_t addr = 0x7FFFul + 1ul - 4ul;
	reset_eeprom(out.eeprom(), addr, 4);

	if (!out.open(addr))
	{
	    uart << ERROR << "al abrir la memoria\n";
	    return false;
	}
	
	char buf[10] = {1,2,3,4,5,6,7,8,9,10};
	char res[10] = {0,0,0,0,0,0,0,0,0,0};

	out.write(buf, 2);
	if (!out){
	    uart << ERROR << "ha fallado el write(2)\n";
	    return false;
	}

	if (!check(out.eeprom(), addr, res, 4, "write(2)"))
	    return false;

	out.flush();
	if (!out){
	    uart << ERROR << "ha fallado el flush()\n";
	    return false;
	}

	res[0] = buf[0];
	res[1] = buf[1];
	if (!check(out.eeprom(), addr, res, 4, "flush()"))
	    return false;
	if (out.eeprom().end_memory()){
	    uart << ERROR << "end_memory = true!!!\n";
	    return false;
	}

	if (!out.eeprom().good()){
	    uart << ERROR << "eeprom en mal estado, debería de estar en buen "
		    "estado\n";
	    return false;
	}

	out.write(&buf[2], 6);
	if (out.good()){
	    uart << ERROR << "no ha fallado el write()\n";
	    return false;
	}

	if (!out.eeprom().end_memory()){
	    uart << "end_memory = ERROR!!!\n";
	    return false;
	}

	if (out.eeprom().good()){
	    uart << ERROR << "eeprom en buen estado, debería de estar en mal "
		    "estado\n";
	    return false;
	}


	for (int i = 0; i < 4; ++i)
	    res[i] = buf[i];

	out.eeprom().clear();
	if (!check(out.eeprom(), addr, res, 4, "write(6)"))
	    return false;

	out.close();
    }


    {
    uint16_t address = out.eeprom().max_address() + 10;
    uart << "\nopen(max_address) ... ";
    if(!out.open(address))
	uart << "OK\n";
    else {
	uart << "FAIL\n";
	return false;
    }
    }

    uart << "test_only_tm() ... ";
    if (test_only_tm())
	uart << "OK\n";
    else{
	uart << "FAIL\n";
	return false;
    }

    return true;
}

bool test_istream_automatico()
{
    UART_iostream uart;
    uart << "EEPROM istream (automático)\n";
    const char msg[] =
	"En un lugar de la mancha,\nde cuyo nombre no quiero "
	"acordarme...\n";

    {

        EEPROM_ostream out;
	out.open(0);
	out << msg;
	if (!out){
	    uart << ERROR << "al escribir el msg\n";
	    return false;
	}
	out.close();
    }

    {
	EEPROM_istream in;
	in.open(0);
	uart << "get() ... ";
	for (uint8_t i = 0; i < strlen(msg); ++i){
	    char c;
	    if (in.get(c)){
		if (c != msg[i]){
		    uart << ERROR << "\n";
		    return false;
		}
	    }
	    else{
		uart << ERROR << "in en mal estado\n";
		return false;
	    }
	}
	uart << "OK\n";

	in.close();
	in.open(0);

	uart << "getline ... ";
	char buffer[sz];
	if (in.getline(buffer, sz)){
	    for (uint8_t i = 0; i < strlen(msg) and msg[i] != '\n'; ++i){
		if (buffer[i] != msg[i]){
		    uart << ERROR << "\n";
		    return false;
		}
	    }

	}
	else{
	    uart << ERROR << "al leer getline\n";
	    return false;
	}

	uart << "OK\n";
    }
    
    
    return true;
}



void test_eeprom_automatico()
{
    UART_iostream uart;
    SPI_master::SCK_period_in_us<period_in_us>();
    SPI_master::turn_on();

    if (test_ostream_automatico())
	test_istream_automatico();
}

int main()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

    while(1){
	init_buffer();
	test_eeprom_automatico();

	uart << "\nPulsa una tecla para continuar\n";
	char c;
	uart >> c;

    }
}
