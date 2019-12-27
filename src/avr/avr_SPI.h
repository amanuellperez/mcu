// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#ifndef __AVR_SPI_H__
#define __AVR_SPI_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Driver del SPI del avr.
 *
 *  - COMENTARIOS:
 *	¿Cómo gestionar los errores en el SPI?
 *	En principio da la impresión de que (por lo menos por software) no
 *	podemos. Si el SPI funciona como master, SS, SCK y MOSI son pins de
 *	salida. El SPI escribe ahí. El MISO es el único pin de entrada: nos
 *	limitamos a leerlo, nada más. El potencial que haya en ese pin es el
 *	que consideramos que es el byte entrante. ¡Imposible detectar que el
 *	dispositivo no está conectado!
 *
 *	La gestión de errores hay que hacerla en el driver del dispositivo.
 *	El dispositivo tendrá que tener algún tipo de protocolo con el avr: te
 *	pido tal cosa, tu me respondes con otra. Si podemos comprobar que la
 *	respuesta es o no la adecuada sabremos si está conectado o no.
 *  
 *  - TEST: Para probar el SPI mirar el programa de prueba del potenciómetro
 *	digital MCP4231. Es un dispositivo muy sencillo de programar y se 
 *	pueden poner varios para probar el SPI.
 *
 *	Ejemplo de driver: ver el potenciómetro MCP4231.
 *
 *  - HISTORIA:
 *    A.Manuel Lopez
 *	10/04/2018 v0.0
 *	10/07/2019 v0.1: Reescrito. Comienzo con traductor.
 *	19/10/2019 v0.2: Lo dejo como un traductor puro.
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t
#include <cstddef>    // byte
#include "avr_cfg.h"
#include "avr_pin.h"
#include <atd_register.h>

// Functions
namespace avr{

class SPI{
public:
    // No permitimos que se construya este objeto
    SPI() = delete;

    // ---------
    // Traductor
    // ---------
    /// Causes the SPI interrupt to be executed if a serial transmission is
    /// completed and interrupts are enable.
    static void interrupt_enable() 
    { atd::Register{SPCR}.write_one_bit<SPIE>(); }

    /// Disable interrupt.
    static void interrupt_disable() 
    { atd::Register{SPCR}.write_zero_bit<SPIE>(); }

    /// Enable SPI as master. This must be set to enable SPI operations.
    static void enable_as_a_master() 
    { atd::Register{SPCR}.write_one_bit<MSTR, SPE>(); }


    /// Enable SPI as a slave. This must be set to enable SPI operations.
    static void enable_as_a_slave() 
    { 
	atd::Register{SPCR}.write_zero_bit<MSTR>();
	atd::Register{SPCR}.write_one_bit<SPE>();
    }

    /// Disable SPI.
    static void disable() 
    { atd::Register{SPCR}.write_zero_bit<SPE>(); }
 

    /// El SPI envia primero el LSB (least significant bit = unidades)
    static void data_order_LSB() 
    { return atd::Register{SPCR}.write_one_bit<DORD>(); }


    /// El SPI envia primero el MSB (most significant bit)
    static void data_order_MSB() 
    { return atd::Register{SPCR}.write_zero_bit<DORD>(); }


    /// Configuramos el modo de operación: la polaridad cpol y la fase cpha
    static void spi_mode(bool cpol, bool cpha);


    // Selección de la velocidad del reloj
    static void clock_speed_entre_2();
    static void clock_speed_entre_4();
    static void clock_speed_entre_8();
    static void clock_speed_entre_16();
    static void clock_speed_entre_32();
    static void clock_speed_entre_64();
    static void clock_speed_entre_128();


    /// Configuramos la velocidad del reloj del SPI en microsegundos.
    // La función clock_speed_en_us traduce la forma de hablar del cliente (en
    // microsegundos) en la forma de hablar del avr (en divisor de frecuencia)
    template<uint16_t periodo
	    , uint16_t clock_frecuencia_en_hz = AVR_CLOCK_FRECUENCIA_EN_HZ>
    static void clock_speed_en_us();


    /// Is a serial transfer complete?
    static bool is_transmission_complete()
    { return atd::Register{SPSR}.is_one_bit<SPIF>(); }

    /// The SPI data register was written during data transfer?
    // To clear this bit first read WCOL, then read SPI data register.
    static bool is_a_write_collision()
    { return atd::Register{SPSR}.is_one_bit<WCOL>(); }


    /// Write x in the data register and initiates data transmissioin (si SS
    /// está seleccionado).
    static void data_register(std::byte x) {SPDR = std::to_integer<uint8_t>(x);}  

    /// Devuelve el último uint8_t recibido en el último trade.
    static std::byte data_register() {return std::byte{SPDR};}

    /// Configuramos los pines para que el SPI funciones como master.
    /// Configuramos todos: SCK, MOSI y SS como de salida, y MISO como de
    /// entrada.
    static void cfg_pines_as_master();


    // --------------------------------
    // Funciones de alto nivel (driver)
    // --------------------------------
    /// Enciende el SPI como Master. 
    /// La frecuencia del reloj usada será la definida por
    /// periodo_en_us. Falta definir la polaridad y la phase ya que cada
    /// dispositivo tendrá una polaridad y fase diferente. Esta configuración
    /// la hara cada dispositivo antes de escribir en SPI.
    template <uint16_t periodo_en_us>
    static void on_as_a_master()
    {
	cfg_pines_as_master();
	clock_speed_en_us<periodo_en_us>();
	enable_as_a_master();
    }



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
    static std::byte trade_and_wait(std::byte x)
    {
	data_register(x);	// escribimos x y lo enviamos
	wait_transmission_complete();

        return data_register();// valor recibido
    }

    /// Enviamos el byte x y esperamos hasta que lo haya enviado.
    /// Devuelve el valor recibido.
    static std::byte write(std::byte x)
    { return trade_and_wait(x); }


    /// Lee un byte. Espera hasta que haya leido el byte.
    static std::byte read()
    { return trade_and_wait(std::byte{0}); }

};



// reloj del SPI a 1MHz
// --------------------
// a 500 kHz = 2 us
template<>
inline void SPI::clock_speed_en_us<2u, 1000000UL>() 
{clock_speed_entre_2();}

// a 4 us
template<>
inline void SPI::clock_speed_en_us<4u, 1000000UL>() 
{clock_speed_entre_4();}

// a 8 us
template<>
inline void SPI::clock_speed_en_us<8u, 1000000UL>() 
{clock_speed_entre_8();}

// a 16 us
template<>
inline void SPI::clock_speed_en_us<16u, 1000000UL>() 
{clock_speed_entre_16();}

// a 32 us
template<>
inline void SPI::clock_speed_en_us<32u, 1000000UL>() 
{clock_speed_entre_32();}

// a 64 us
template<>
inline void SPI::clock_speed_en_us<64u, 1000000UL>() 
{clock_speed_entre_64();}

// a 128 us
template<>
inline void SPI::clock_speed_en_us<128u, 1000000UL>() 
{clock_speed_entre_128();}



// reloj del SPI a 8MHz
// --------------------
// a 250 ns
//template<>
//inline void SPI::clock_speed_en_ns<250u, 8000000UL>() 
//{clock_speed_entre_2();}

// a 500 ns
//template<>
//inline void SPI::clock_speed_en_ns<500u, 8000000UL>() 
//{clock_speed_entre_4();}

// a 1 us
template<>
inline void SPI::clock_speed_en_us<1u, 8000000UL>() 
{clock_speed_entre_8();}

// a 2 us
template<>
inline void SPI::clock_speed_en_us<2u, 8000000UL>() 
{clock_speed_entre_16();}

// a 4 us
template<>
inline void SPI::clock_speed_en_us<4u, 8000000UL>() 
{clock_speed_entre_32();}

// a 8 us
template<>
inline void SPI::clock_speed_en_us<8u, 8000000UL>() 
{clock_speed_entre_64();}

// a 16 us
template<>
inline void SPI::clock_speed_en_us<16u, 8000000UL>() 
{clock_speed_entre_128();}

}// namespace avr


#endif 

