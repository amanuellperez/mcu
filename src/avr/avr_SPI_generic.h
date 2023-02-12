// Copyright (C) 2023 Manuel Perez 
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

#ifndef __AVR_SPI_GENERIC_H__
#define __AVR_SPI_GENERIC_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Dispositivo genérico SPI
 *
 *  HISTORIA
 *    Manuel Perez
 *	12/02/23 Separando traductor de dispositivo genérico.
 *
 ****************************************************************************/
#include "avr_SPI_basic.h"

namespace avr_{

class SPI_base_g : public SPI_basic{
public:
    SPI_base_g() = delete;

    // Las transmisiones del SPI nunca van a fallar: al enviar un byte el SPI
    // arranca el reloj, genera 8 pulsos y marca como enviado el byte,
    // independientemente de si lo ha enviado o no, ya que el avr no tiene
    // forma de saberlo.
    /// Espera hasta que el SPI ha enviado/recibido el último byte.
    // En la practica el SPI es tan rapido que el avr no tiene que esperar.
    static void wait_transmission_complete()
    {
	while (!is_transmission_complete())
	{; }
    }


    /// Enviamos el byte x y esperamos hasta que lo haya enviado.
    /// Devuelve el valor recibido.
    static uint8_t write(uint8_t x)
    { return trade_and_wait(x); }


    /// Lee un byte. Espera hasta que haya leido el byte.
    static uint8_t read()
    { return trade_and_wait(uint8_t{0}); }


private:
    /// Enviamos el byte x y esperamos hasta que lo haya enviado.
    /// Devuelve el valor recibido.
    static uint8_t trade_and_wait(uint8_t x)
    {
	data_register(x);	// escribimos x y lo enviamos
	wait_transmission_complete();

        return data_register();// valor recibido
    }


};


class SPI_master_g : public SPI_base_g {
public:
    SPI_master_g() = delete;

    /// Enciende el SPI como Master. 
    /// La frecuencia del reloj usada será la definida por
    /// period_in_us. Falta definir la polaridad y la phase ya que cada
    /// dispositivo tendrá una polaridad y fase diferente. Esta configuración
    /// la hara cada dispositivo antes de escribir en SPI.
    template <uint16_t period_in_us>
    static void on()
    {
	init();
	clock_speed_in_us<period_in_us>();
	enable_as_a_master();
    }
    
private:
    /// Configuramos los pines para que el SPI funciones como master.
    /// Configuramos todos: SCK, MOSI y SS como de salida, y MISO como de
    /// entrada.
    static void init();
    
};


class SPI_slave_g : public SPI_base_g {
public:
    SPI_slave_g() = delete;

    /// Enciende el SPI como slave.
    static void on()
    {
	init();
	enable_as_a_slave();
    }
    
private:
    static void init();
};



}// namespace avr


#endif 

