// Copyright (C) 2021 Manuel Perez 
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
/****************************************************************************
 *
 *  - DESCRIPCION: Master para probar el driver de teclado
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    12/12/2021 v0.0
 *
 ****************************************************************************/
#include <mega.h>
#include "../dev.h"

// Microcontroller
// ---------------
namespace myu = atmega;
using UART = myu::UART_8bits;
using UART_iostream = mcu::UART_iostream<UART>;

// Observar que esta cfg no debería de estar aquí, sino que hay que 
// pasar SPI_master como parámetro
struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = not_generic::hwd::Pin<n>;

// La frecuencia del master tiene que ser 4 veces más lenta que la del slave.
// El slave lo tenemos a 1MHz, luego el periodo máximo debe ser 4 us.
// De hecho, al principio probe con 2 us y no funcionaba bien.
// Con 4 funciona bien, garanticemos 8 us.
    static constexpr uint16_t period_in_us = 8;	
};

using SPI = myu::SPI_master<SPI_master_cfg>;



void print_code(uint8_t code)
{
    UART_iostream uart;

    switch(code){
	break; case Code::del: uart << "DEL"; 
	break; case Code::AC: uart << "AC";
	break; case Code::left: uart << "left";
	break; case Code::right: uart << "right"; 
	break; case Code::ANS: uart << "ANS"; 
	break; default: uart << code; 
    }
}

void test_keyboard()
{

    myu::hwd::Output_pin<SPI_SS_pin> no_SS;

    while (1) {
	no_SS.write_zero();
	uint8_t res = SPI::write(uint8_t{0});
	no_SS.write_one();

	if (res != uint8_t{0})
	    print_code(static_cast<char>(res));

	myu::Micro::wait_ms(100);
    }
}


int main() 
{
// init_uart()
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

// init_SPI()
    SPI::mode(0,0);
    SPI::data_order_LSB();
    SPI::SCK_period_in_us<SPI_master_cfg::period_in_us>();
    SPI::turn_on();

    uart << "\n\nTest driver keyboard\n"
	        "--------------------\n";

    test_keyboard();
}



