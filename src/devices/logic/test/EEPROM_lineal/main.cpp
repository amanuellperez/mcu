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

#include "../../dev_EEPROM_lineal.h"
#include <dev_25LC256_basic.h>
#include <avr_atmega.h>
#include <atd_cast.h>
#include <atd_cstddef.h>

namespace mcu = atmega;

using SPI = atmega::SPI_master;

constexpr uint8_t period_in_us = 16;

constexpr uint8_t num_pin_chip_select = 16;
using EEPROM = dev::EEPROM_lineal<dev::EEPROM_25LC256<num_pin_chip_select>>;

constexpr uint16_t sz = 300;


void print_buffer(uint16_t addr0, uint8_t* buf, uint16_t n)
{
    atmega::UART_iostream uart;

    uint16_t i = 0;
    for (; i < n; ++i){
	if (!(i % 10))
	    uart << "\n" << addr0 + i << " | ";
	// uart << std::to_integer<uint8_t>(buf[i]) << "  ";
	uart << static_cast<uint16_t>(buf[i]) << "  ";
	// atd::print_in_hex(uart, buf[i]); uart << "  ";
    }
}

void read_and_print(EEPROM& eeprom, uint16_t addr0, uint16_t n = sz)
{
    atmega::UART_iostream uart;

    uint8_t buf[sz];

    auto leidos = eeprom.read(addr0, buf, n);


    if (leidos != n){
	uart << "ERROR: no se han podido leer " << n<< " bytes\n";
	if (eeprom.no_response()){
	    uart << "Error: la EEPROM no responde. Revisar las conexiones\n";
	    mcu::Micro::wait_ms(1000);
	    eeprom.clear();
	}
    }

    print_buffer(addr0, buf, n);
}

void check_true(bool ok, const char* msg)
{
    atmega::UART_iostream uart;
    
    uart << msg << " ... ";
    if (ok)
	uart << " OK";
    else 
	uart << " FAIL";

    uart << "\n";
}

void test_eeprom_interactiva()
{
    atmega::UART_iostream uart;
    SPI::clock_period_in_us<period_in_us>();
    SPI::on();

    EEPROM eeprom;

    while(1){
	uart << "\nPrueba EEPROM lineal\n"
	     <<      "-------------------\n";
	
	check_true(eeprom.good(), "good()");

	read_and_print(eeprom, 0, 100);
	uart << "--\n";
	read_and_print(eeprom, 0x7FFF + 1ul - sz, 100);

	uart << "\n¿En qué dirección quieres escribir?\n";
	uint16_t address;
	uart >> address;
	uart << address << "\n";

	uart << "\n¿A partir de qué número quieres almacenar?\n";
	uint16_t n0;
	uart >> n0;
	uart << n0 << "\n";

	uart << "\n¿Cuántos números almacenamos?\n";
	uint16_t n;
	uart >> n;
	uart << n << "\n";


	if (n > sz){
	    uart << "Demasiado grande. Almacenamos " << sz << " números\n";
	    n = sz;
	}

	uint8_t buf[sz];
	for (uint16_t i = 0; i < n; ++i)
	    buf[i] = uint8_t{static_cast<uint8_t>(n0 + i)};

	auto escritos = eeprom.write(address, buf, n);
	uart << "escritos = " << escritos << "\n";

	if (eeprom.good())
	    uart << "Todo OK\n";
	else if (eeprom.end_memory()){
	    uart << "Error: se ha alcanzado el final de la memoria\n";
	    eeprom.clear();
	}
	else if (eeprom.no_response()){
	    uart << "Error: la EEPROM no responde. Revisar las conexiones\n";
	    mcu::Micro::wait_ms(5000);
	    eeprom.clear();
	}

    }// while
}



// lee n bytes a partir de addr0, comparando el resultado con res[0..n)
bool check_read(EEPROM& eeprom, uint16_t addr0, uint8_t* res, uint8_t n)
{
    atmega::UART_iostream uart;

    uint8_t buf_read[sz];
    std::fill_n(buf_read, sz, uint8_t{0});

    auto leidos = eeprom.read(addr0, buf_read, n);
    if (leidos != n){
	uart << " FAIL  (leidos)\n";
	return false;
    }
    for (uint8_t i = 0; i < n; ++i){
	if (buf_read[i] != res[i]){
	    uart << " FAIL (buf_read[" << i << "] != res)\n";
	    uart << "      (" << buf_read[i]
		 << " != " << res[i] << ")\n";
	    return false;
	}
    }

    return true;
}


void test_write(EEPROM& eeprom,
                           uint16_t address,
                           uint8_t n0,
                           uint8_t n)
{
    atmega::UART_iostream uart;

    uint8_t buf_write[sz];
    for (uint8_t i = 0; i < n; ++i)
	buf_write[i] = uint8_t{static_cast<uint8_t>(n0 + i)};

    uart << "write(" << address << ", " << n << ") ... ";
    auto escritos = eeprom.write(address, buf_write, n);
    if (escritos != n)
	uart << " FAIL (escritos != n)\n";
    else{
	if (check_read(eeprom, address, buf_write, n))
	    uart << " OK\n";
    }
}




void test_fill_n(EEPROM& eeprom)
{
    atmega::UART_iostream uart;

    uint16_t addr0 = 300;
    uint8_t n = 100;

    uart << "fill_n ... ";
    auto escritos = eeprom.fill_n(addr0, n, uint8_t{4});

    if (escritos != n){
	uart << " FAIL (escritos != n)\n";
    }else{
	uint8_t res[sz];
	std::fill_n(res, n, uint8_t{4});
	if (check_read(eeprom, addr0, res, n) )
	    uart << " OK\n";
    }
    
}


void test_eeprom_automatico()
{
    atmega::UART_iostream uart;
    SPI::clock_period_in_us<period_in_us>();
    SPI::on();
    EEPROM eeprom;

    while(1){
	uart << "\nProbando EEPROM_lineal (automático)\n";
	uart << "-----------------------------------\n";
	uart << "Pulsa una tecla para empezar\n";
	char c;
	uart.get(c);
	
	test_write(eeprom, uint16_t{0}, 0, 100);
	test_write(eeprom, uint16_t{0x7FFF - 99u}, 0, 100);
	test_fill_n(eeprom);
	
	if (!eeprom.good()){
	    uart << "EEPROM en mal estado. Cambiamos su estado a good\n";
	    eeprom.clear();
	}

    }// while
}

int main()
{
    atmega::UART_iostream uart;
    atmega::basic_cfg(uart);
    uart.turn_on();

test_eeprom_interactiva();
//    test_eeprom_automatico();
}
