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


namespace mcu = avr_;

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

// bidimensional
constexpr static uint8_t barray_rows = 6;
constexpr static uint8_t barray_cols = 3;
constexpr uint8_t barray_u8[barray_rows][barray_cols] PROGMEM = {
    {1,2,3},
    {4,5,6},
    {7,8,9},
    {10, 11, 12},
    {13, 14, 15},
    {16, 17, 18}
};


void test_basic()
{
    mcu::UART_iostream uart;

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

}


constexpr uint8_t nu8 = 12;
constexpr uint16_t nu16 = 47;
constexpr mcu::Progmem<uint8_t> pu8 PROGMEM = nu8;
constexpr mcu::Progmem<uint16_t> pu16 PROGMEM = nu16;
// Este no tiene que compilar:
// constexpr Progmem<uint32_t> pu32 PROGMEM = 200;

static constexpr uint8_t narray_u8[4] = {10, 20, 30, 40};
constexpr mcu::Progmem_array<uint8_t, 4> parray_u8 PROGMEM = 
	        {narray_u8[0], narray_u8[1], narray_u8[2], narray_u8[3]};
constexpr mcu::Progmem_string<6> pstr PROGMEM{"hello"};

constexpr mcu::Progmem_string_array<3> parray_str PROGMEM = {
    str1, str2, str3
    };


template <size_t N>
void f(const mcu::Progmem_string_array<N>& str0)
{
    mcu::UART_iostream uart;
    mcu::Progmem_string_array<3> str = str0; // <-- esto genera error en 
					    // tiempo de ejecución

    uart << "Dentro de f\n";
    // Arrays de cadenas
    {
	char buffer[10];
	for (size_t i = 0; i < str.size(); ++i){
	    mcu::strcpy(buffer, str[i]);
	    uart << "str[" << (int) i << "] = " << buffer << '\n';
	}
    }
    uart << "-----------\n";
}



void test_progmem()
{
    mcu::UART_iostream uart;

    uart << "\n\n";
    uint8_t x8 = pu8;
    uart << "u8:\t[" << (int) x8 << "] =? [" << (int) nu8 << "]\n";
    
//	pu8 = 7;  // no compila
    uint16_t x16 = pu16;
    uart << "u16:\t[" << x16 << "] =? [" << nu16 << "]\n";

    {// Arrays
     // TODO: ¿por qué no lo imprime bien?
//	for (size_t i = 0; i < 4; ++i)
//	    uart << narray_u8[i] << ", ";
//	uart << '\n';

	uart << "Array of uint8_t: ";
	for (size_t i = 0; i < parray_u8.size(); ++i)
	    uart << (int) parray_u8[i] << ' ';
	uart << '\n';
	
	uart << "range for       : ";
	for (const auto& x: parray_u8)
	    uart << (int) x << ' ';
	uart << '\n';
    }
	
    {// Progmem_string

	uart << "pstr = [";
	for (size_t i = 0; i < pstr.size(); ++i)
	    uart << pstr[i];
	uart << "] =? [hello]\n";
    }
    // f(parray_str); <-- no debería de compilar

    uart << "Array of " << parray_str.size() << " strings:\n";
    // Arrays de cadenas
    {
	char buffer[10];
	for (size_t i = 0; i < parray_str.size(); ++i){
	    mcu::strcpy(buffer, parray_str[i]);
	    uart << "str[" << (int) i << "] = " << buffer << '\n';
	}
    }

    uart << "Writting only 3 characters of an array of " << parray_str.size()
         << " strings (test strncpy):\n";
    // Arrays de cadenas
    {
	char buffer[4];
	for (size_t i = 0; i < parray_str.size(); ++i){
	    mcu::strncpy(buffer, parray_str[i], 4);
	    buffer[3] = '\0'; // garantizamos que acaba bien
	    uart << "str[" << (int) i << "] = " << buffer << '\n';
	}
    }

    uart << "Writting only 4 characters of an array of " << parray_str.size()
         << " strings (test strlcpy):\n";
    // Arrays de cadenas
    {
	char buffer[4];
	for (size_t i = 0; i < parray_str.size(); ++i){
	    size_t n = mcu::strlcpy(buffer, parray_str[i], 4);
	    uart << "original string has [" << n << "] --> [" 
			    << (int) i << "] = " << buffer << '\n';
	}
    }

}

void test_progmem_view()
{
    mcu::UART_iostream uart;

    mcu::Progmem_biarray_view<uint8_t, barray_rows, barray_cols> a{barray_u8};

    uart << "\n\nProgram view test\n"
	    "-----------------\n";

    uart << "Progmem_biarray_view:\n";
    for (uint8_t i = 0; i < barray_rows; ++i){
	for (uint8_t j = 0; j < barray_cols; ++j){
	    uart << (int)a[i][j] << ' ';
	}
	uart << '\n';
    }

    uart << "\n\n";
}


constexpr mcu::Progmem_string<15> menu PROGMEM{"\n\nProgmem test\n"};

int main()
{
// init_UART();
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();

    atd::print(uart, menu);

    while (1){
	uart << "---------------\n"
		"1. Basic functions\n"
		"2. Progmem\n"
		"3. Progmem_view\n";

	char ans{};
	uart >> ans;

	switch (ans){
	    break; case '1': test_basic();
	    break; case '2': test_progmem();
	    break; case '3': test_progmem_view();
	}

    }

}

