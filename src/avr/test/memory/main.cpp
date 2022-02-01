// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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


#include "../../avr_time.h"
#include "../../avr_UART_iostream.h"
#include "../../avr_memory.h"

/***************************************************************************
 *			    TEST BÁSICO 
 ***************************************************************************/
// tipos básicos
constexpr uint8_t u8 PROGMEM = 14;
constexpr uint16_t u16 PROGMEM = 27;

// arrays de cadenas
constexpr const char str1[] PROGMEM = "zero";
constexpr const char str2[] PROGMEM = "one";
constexpr const char str3[] PROGMEM = "three";
constexpr const char* const str_array[] PROGMEM = {
    str1, str2, str3
};


// arrays en general
constexpr uint8_t array_u8[] PROGMEM = {10,20,30,40};


void test_basic()
{
    avr::UART_iostream uart;

    while(1){
    // Tipos básicos
    {
	uint8_t x8 = pgm_read_byte(&u8);
	uart << "u8 = [" << (int) x8 << "]\n";

	uint16_t x16 = pgm_read_word(&u16);
	uart << "u16 = [" << x16 << "]\n";
    }

    // Arrays de cadenas
    {
	char buffer[10];
	for (uint8_t i = 0; i < 3; ++i){
	    strcpy_P(buffer, (PGM_P) pgm_read_word(&(str_array[i])));
	    uart << "str[" << (int) i << "] = " << buffer << '\n';
	}
    }

    // Arrays
    {
	for (uint8_t i = 0; i < 4; ++i){
	    uint8_t x8 = pgm_read_byte(&array_u8[i]);
	    uart << "u8 = [" << (int) x8 << "]\n";
	}
    }


	wait_ms(1000);
    }

}



constexpr avr::Progmem<uint8_t> pu8 PROGMEM = 12;
constexpr avr::Progmem<uint16_t> pu16 PROGMEM = 200;
// Este no tiene que compilar:
// constexpr Progmem<uint32_t> pu32 PROGMEM = 200;

constexpr avr::Progmem_array<uint8_t, 4> parray_u8 PROGMEM = {10,20,30,40};
constexpr avr::Progmem_string_array<3> parray_str PROGMEM = {
    str1, str2, str3
    };

void test_progmem()
{
    avr::UART_iostream uart;

    while(1){
	uart << "\n\n";
	uint8_t x8 = pu8;
	uart << "u8 = [" << (int) x8 << "]\n";
	
//	pu8 = 7;  // no compila

    {// Arrays
	uint16_t x16 = pu16;
	uart << "u16 = [" << (int) x16 << "]\n";

	for (size_t i = 0; i < parray_u8.size(); ++i)
	    uart << "u8[" << i << "] = " << (int) parray_u8[i] << '\n';
    }
	
    uart << "Escribiendo array de " << parray_str.size() << " cadenas:\n";
    // Arrays de cadenas
    {
	char buffer[10];
	for (size_t i = 0; i < parray_str.size(); ++i){
	    avr::strcpy(buffer, parray_str[i]);
	    uart << "str[" << (int) i << "] = " << buffer << '\n';
	}
    }

    uart << "Escribiendo solo 3 caracters del array de " << parray_str.size()
         << " cadenas (test strncpy):\n";
    // Arrays de cadenas
    {
	char buffer[4];
	for (size_t i = 0; i < parray_str.size(); ++i){
	    avr::strncpy(buffer, parray_str[i], 4);
	    buffer[3] = '\0'; // garantizamos que acaba bien
	    uart << "str[" << (int) i << "] = " << buffer << '\n';
	}
    }

    uart << "Escribiendo solo 4 caracters del array de " << parray_str.size()
         << " cadenas (test strlcpy):\n";
    // Arrays de cadenas
    {
	char buffer[4];
	for (size_t i = 0; i < parray_str.size(); ++i){
	    size_t n = avr::strlcpy(buffer, parray_str[i], 4);
	    uart << "cadena original tiene [" << n << "] --> [" << (int) i << "] = " << buffer << '\n';
	}
    }


	wait_ms(1000);
    }

}


int main()
{
// init_UART();
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

    uart << "\nProgmem test\n"
	      "------------\n";

   //test_basic();
   test_progmem();
}

