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

#pragma once

#ifndef __DEV_MATRIX_LED_MAX7219_H__
#define __DEV_MATRIX_LED_MAX7219_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Clases para manejar las matrices de led. El driver será el MAX7219.
 *
 * COMENTARIOS
 *	Todo el código se basa en el código de Joey Conenna.
 *	https://embeddedthoughts.com/2016/04/19
 *		/scrolling-text-on-the-8x8-led-matrix-with-max7219-drivers/
 *
 * HISTORIA
 *    Manuel Perez
 *    31/12/2018 Escrito
 *
 ****************************************************************************/

#include <util/delay.h>	

#include <avr_SPI.h>
#include <string.h> // memset

#include "avr_matrix_led_ascii_chars.h"


namespace dev{
namespace Matrix_led{



/*!
 *  \brief  Matriz de leds controlada por el controlador MAX7219
 *
 *  Se pueden conectar varias matrices seguidas, cada una con su controlador
 *  MAX7219. El número de dispositivos conectados lo indica el parámetro
 *  NUM_DEVICES.
 *
 */
template <uint8_t NUM_DEVICES>
class  MAX7219{
public:
    using SPI = SPI_master;

    // time = 8 veces el tiempo que se quiere esperar (darle distintos valores
    // para probar. No funciona bien la función wait_us creo que por usar un
    // uint32_t en lugar de un uint8_t)
    // I = desde 0x00 hasta 0x0F (de menos a más intensidad)
    MAX7219(uint32_t delay_time = 50000u, byte I = 0x07) 
	:delay_time_{delay_time}
    {
	SPI::init(SPI::entre_2, 0, 0);
	max7219_init(I);
	clear();
	buffer_init();
    }



    // Clears all columns on all devices
    void clear()
    {
	for(uint8_t x = 1; x < 9; ++x) // for all columns
	{   
	    SPI::select_slave();
	    for(uint8_t i = 0; i < NUM_DEVICES; ++i)
		write(x, 0x00);    // Select column x
	    SPI::deselect_slave();
	}
    }



    // msg = puntero a una cadena de C (acabada en '\0') que queremos mostrar
    // en pantalla.
    //
    // Muestra el mensaje haciendo un scroll
    void show(const char* msg)
    {
	Ascii2char ascii2char;

	auto len = strlen(msg);

	for(uint16_t i = 0; i < len; ++i)
	{
	    // c = íncide en el array characters de la letra que queremos
	    // visualizar
	    auto c = *(msg + i);

	    // Añadimos y mostramos las 5 columnas del caracter
	    for(uint8_t i = 0; i < 5; ++i)
		scroll_push_and_show(ascii2char(c,i));

	    // Añadimos y mostramos una columna en blanco
	    scroll_push_and_show(0x00);
	}
	    
    }

    void scroll_push_and_show(byte b)
    {
	buffer_scroll_and_push(b);
	display_buffer();	
	wait_us(delay_time_); 	
    }

    // ------------------------------
    // Comandos que admite el MAX7219
    // ------------------------------
    // I = desde 0x00 hasta 0x0F (de menos a más intensidad)
    void intensity(byte I)  {comando(0x0A, I);}

    // ¿Cuántos registros mostramos? De 1 a 7
    void scan_limit(uint8_t num_registros)
    {comando(0x0B, num_registros);}

    // Encendemos el display (lo opuesto a shutdown)
    void turn_on_display()
    {comando(0x0C, 0x01);}
	
    // Desactivamos el modo display test.
    void disable_display_test()
    {comando(0x0F, 0x00);}

private:
    // Buffer array of bytes to store current display data for each column 
    // in each cascaded device
    byte buffer[NUM_DEVICES*8];		

    uint32_t delay_time_;    // Delay for scrolling speed in microseconds


    // Inicializamos todos los max7219
    void max7219_init(byte I) 
    {
	intensity(I);
	scan_limit(0x07);
	turn_on_display();
	disable_display_test(); 
    }


    // El buffer inicialmente tiene que estar vacío
    void buffer_init()
    { memset(buffer, 0x00, NUM_DEVICES*8); }       


    // Ejecutamos un comando en todos los max7219 conectados 
    void comando(byte cmd, byte data)
    {
	SPI::select_slave();

	// Loop through number of cascaded devices
	for(uint8_t i = 0; i < NUM_DEVICES; ++i) 
	    write(cmd, data);

	SPI::deselect_slave();
    }

    // Send byte through SPI
    void write(byte b)
    { SPI::trade_and_wait(b); }


    // Sends word through SPI
    void write(byte address, byte data) 
    {
	write(address);	// Write first byte
	write(data);      // Write Second byte
    }


    // Displays current buffer on the cascaded devices
    void display_buffer()
    {   
	for(uint8_t i = 0; i < NUM_DEVICES; ++i) 
	{// For each cascaded device
	    for(uint8_t j = 1; j < 9; ++j)
	    { // For each column
		SPI::select_slave();

		 // Write Pre No-Op code
		for(uint8_t k = 0; k < i; ++k)
		    write(0x00, 0x00);

		// Write column data from buffer
		write(j, buffer[j + i*8 - 1]);

		// Write Post No-Op code
		for(uint8_t k = NUM_DEVICES-1; k > i; --k)
		    write(0x00, 0x00);

		SPI::deselect_slave(); // Volcamos todos los datos
	   }
       }
    }


    // OJO: como time es un uint32_t seguramente --time involucre 4 ciclos de
    // reloj y la comparación puede que otros 4, con lo que time que se espera
    // es unas 8 veces time
    void wait_us(uint32_t time)
    {
	do {
	    _delay_us(1);
	}while (--time);
    }

    // Esta función es genérica. ¿La saco fuera? Habría que pasarle más datos
    // siendo algo más ineficiente pero podemos reutilizar código.
    // Mueve todos los elementos del array un puesto a la izda (perdiendo el
    // primer elemento) haciendo hueco al final del array, donde añadimos el
    // byte b
    // x = columna que queremos añadir al buffer
    void buffer_scroll_and_push(byte b)
    {
	// movemos todo hacia la izquierda (hacemos el scroll hacia la izda)
	memmove(&buffer[0], &buffer[1], NUM_DEVICES*8 - 1);

	buffer[NUM_DEVICES*8 - 1] = b;
    }

};

}// namespace Matrix_led
}// namespace avr

#endif

