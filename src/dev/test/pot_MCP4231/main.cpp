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

// Conectar dos potenciometros. De esta forma probamos SPI también.
#include "../../dev_pot_MCP4231.h"
#include <avr_UART.h>
#include <avr_time.h>
#include <atd_cast.h>

using namespace avr;
using dev::Pot_MCP4231;


constexpr uint8_t num_pin_no_CS1 = 14;
constexpr uint8_t num_pin_no_CS2 = 16;

// Funciona hasta con 10 MHz
constexpr uint8_t periodo_en_us = 2;	
//constexpr uint8_t periodo_en_us = 16;
//
template <uint8_t n>
void test_pot(Pot_MCP4231<n>& pot, const char* nombre)
{
    UART_iostream uart;

    if (pot.is_hardware_shutdown())
	uart << nombre << " tiene hardware shutdown == 1!!!"
			  " No debería de funcionar\n\r";
    uart << "Valor resistencia para " << nombre
         << " (en tantos por 128) = \r\n";

    uint16_t r;
    uart >> r;

    if (!uart){
	uart.clear();
	uart << "Error al leer el valor\r\n";
	return;
    }
    
    uart << r << " de 128 en R1\n\r";
    pot.template write_data<1>(r);

    if (r <= 128u)
	r = 128u - r;
    else r = 128u;
    uart << r << " de 128 en R0\n\r";
    pot.template write_data<0>(r);
}


template <uint8_t n>
void test_incremento(Pot_MCP4231<n>& pot, const char* nombre)
{
    UART_iostream uart;
    constexpr uint8_t num_incrementos = 8;

    uart << "¿incremento la resistencia? (s/n)\n\r";
    char c = 0; // la inicializo para evitar el warning 
    uart >> c;
    if (c == 's'){
	for (uint8_t i = 0; i < num_incrementos; ++i){
	    pot.template increment_wiper<0>();
	    pot.template increment_wiper<1>();
	    uart << "Incrementado " << i << " de " << num_incrementos << "\n\r";
	    wait_ms(1000);
	}
    }

    uart << "¿decremento la resistencia? (s/n)\n\r";
    uart >> c;
    if (c == 's'){
	for (uint8_t i = 0; i < num_incrementos; ++i){
	    pot.template decrement_wiper<0>();
	    pot.template decrement_wiper<1>();
	    uart << "Derementado " << i << " de " << num_incrementos << "\n\r";
	    wait_ms(1000);
	}
    }
}


template <uint8_t n>
void test_flags(Pot_MCP4231<n>& pot, const char* nombre)
{
    UART_iostream uart;

    uart << "resistor 0 in shutdown cfg? ";
    if (pot.resistor0_shutdown()) uart << "si";
    else uart << "no";
    uart << "\n\r";
	

    uart << "pongo resistor 0 in shutdown cfg? (s/n)";
    char c = 0;
    uart >> c;
    if (c == 's'){
	pot.resistor0_shutdown(true);
	uart << "\n\rresistor0 in shutdown\n\r";
    }else if (c == 'n'){
	pot.resistor0_shutdown(false);
	uart << "\n\rresistor0 in shutdown\n\r";
    }


}



void test() {
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

    SPI::on_as_a_master<periodo_en_us>();

    Pot_MCP4231<num_pin_no_CS1> pot1;
    Pot_MCP4231<num_pin_no_CS2> pot2;
    pot1.cfg_SPI();


    while(1){
	uart << "\r\nPrueba potenciómetro\r\n";
	uart <<     "--------------------\r\n";


	uart << "Potenciómetro pot1\n\r";
//	test_pot(pot1, "pot1");
	test_incremento(pot1, "pot1");
	test_flags(pot1, "pot1");

//	uart << "Potenciómetro pot2\n\r";
//	test_incremento(pot2, "pot2");
//	test_pot(pot2, "pot2");
//	test_flags(pot2, "pot2");

    }// while
}



int main()
{
    test();
}
