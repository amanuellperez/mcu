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
 *  - DESCRIPCION: Prueba de driver de teclado 8x8
 *
 *    Para empezar empecemos haciendo un driver muy sencillo: envíamos 1
 *    caracter como máximo en cada conexión.
 *
 *    El problema que tiene SPI es el sincronizar el slave con el master. Es 
 *    el master el que controla toda la transmisión. ¿Cómo sincronizar 
 *    garantizando no perder datos?
 *   
 *    Dos opciones básicas:
 *    1. El slave solo escribe cuando se desconecta del master (SS pin va a
 *       1). Usamos este momento para escribir. La única posibilidad de que
 *       se pierdan datos es que el master nada más desconectar SS reconecte y
 *       lea antes de que el slave haya escrito. Por ello, para que funcione
 *       el manual de este slave debería decir algo del tipo:
 *          "el slave escribe al desconectar el master. El master tendrá que
 *          esperar x us antes de volver a reconectar, para evitar data race".
 *
 *    2. Usar comandos. El master envía el comando "getchar", el slave escribe
 *       un caracter, y el master lo lee. Para que funcione es necesario que
 *       la frecuencia del SPI del master sea menor que ??? para darle tiempo
 *       al slave a que escriba. 
 *
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    09/12/2021 Primera prueba.
 *
 ****************************************************************************/
// Conectar un teclado de 8 x 8
// El teclado es una matriz de pulsadores.
// Sacamos la salida por UART
#include <avr_atmega.h>

#include "../dev.h"

// Microcontroller
// ---------------
namespace myu = atmega;
using UART = myu::UART_8bits;
using UART_iostream = mcu::UART_iostream<UART>;

using SPI = myu::SPI_slave;

volatile uint8_t data = 0;

void test_keyboard()
{
    UART_iostream uart;
    
    uart << "\n-------------------\n";
    uart <<   "Driver keyboard " << (int)Keypad::nrows() << "x"
         << (int)Keypad::ncols() << "\n";
    uart << "-------------------\n\n";

    Keyboard keyboard{key_code};
    SPI::data_register(uint8_t{0});

    while(1){
	data = keyboard.getchar();

	switch(data){
	    break; case Code::del: uart << "DEL"; 
	    break; case Code::AC: uart << "AC";
	    break; case Code::left: uart << "left";
	    break; case Code::right: uart << "right"; 
	    break; case Code::ANS: uart << "ANS"; 
	    break; default: uart << data; 
	}

	myu::Micro::wait_ms(100); // debouncing
    }
}



int main()
{
// init_UART();
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
 
// init_SPI()
    SPI::turn_on();
    SPI::spi_mode(0,0);
    SPI::data_order_LSB();
    SPI::interrupt_enable();
    //myu::Interrupt::enable_pin<SPI_SS_pin>();
    myu::Pin<SPI_SS_pin>::enable_change_level_interrupt();
    myu::Micro::enable_interrupts();


    test_keyboard();

}


// ----------
// Interrupts
// ----------
ISR_SPI_STC{
// De momento no hago nada. Sin embargo, se puede ampliar y
// permitir que el master envíe comandos: getchar, read(n), ungetc
// En ese caso leeríamos aquí el comando y escribiríamos la respuesta.
//    cmd = SPI::data_register();
}


ISR_PCINT_PIN_SS{
    if (myu::Pin<SPI_SS_pin>::is_one()){
	SPI::data_register(uint8_t{data});
	data = 0; // escrito
    }
}


