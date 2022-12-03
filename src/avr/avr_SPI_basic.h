// Copyright (C) 2019-2021 A.Manuel L.Perez 
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

#pragma once

#ifndef __AVR_SPI_BASIC_H__
#define __AVR_SPI_BASIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del SPI.
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
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	10/04/2018 v0.0
 *	10/07/2019 Reescrito. Comienzo con traductor.
 *	19/10/2019 Lo dejo como un traductor puro.
 *	10/04/2021 SPI_basic_basic, SPI_master, SPI_slave
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t
#include <cstddef>    // byte
#include "avr_cfg.h"
#include "avr_pin.h"
#include <atd_bit.h>
#include "avr_interrupt.h"

namespace avr{

class SPI_basic{
public:
    // No permitimos que se construya este objeto
    SPI_basic() = delete;

    // ---------
    // Traductor
    // ---------
    /// Causes the SPI interrupt to be executed if a serial transmission is
    /// completed and interrupts are enable.
    // CUIDADO: recordar llamar enable_interrupts!!! 
    static void interrupt_enable() 
    {atd::write_bit<SPIE>::to<1>::in(SPCR);}

    /// Disable interrupt.
    static void interrupt_disable() 
    {atd::write_bit<SPIE>::to<0>::in(SPCR);}

    /// Enable SPI as master. This must be set to enable SPI operations.
    static void enable_as_a_master() 
    {atd::write_bits<MSTR, SPE>::to<1,1>::in(SPCR);}


    /// Enable SPI as a slave. This must be set to enable SPI operations.
    static void enable_as_a_slave() 
    { atd::write_bits<MSTR, SPE>::to<0,1>::in(SPCR); }

    /// Disable SPI.
    static void disable() 
    {atd::write_bit<SPE>::to<0>::in(SPCR);}
 

    /// El SPI envia primero el LSB (least significant bit = unidades)
    static void data_order_LSB() 
    {atd::write_bit<DORD>::to<1>::in(SPCR);}


    /// El SPI envia primero el MSB (most significant bit)
    static void data_order_MSB() 
    {atd::write_bit<DORD>::to<0>::in(SPCR);}


    /// Configuramos el modo de operación: la polaridad cpol y la fase cpha
    static void spi_mode(bool cpol, bool cpha);


    // Selección de la velocidad del reloj
    static void clock_speed_divide_by_2();
    static void clock_speed_divide_by_4();
    static void clock_speed_divide_by_8();
    static void clock_speed_divide_by_16();
    static void clock_speed_divide_by_32();
    static void clock_speed_divide_by_64();
    static void clock_speed_divide_by_128();


    /// Configuramos la velocidad del reloj del SPI en microsegundos.
    // La función clock_speed_in_us traduce la forma de hablar del cliente (en
    // microsegundos) en la forma de hablar del avr (en divisor de frecuencia)
    // CUIDADO: parece ser que si se quieren conectar 2 avrs la frecuencia del
    // master tiene que ser 4 veces más lenta que la del slave:
    // Datasheet, 23.5.2. SPI Status register: 
    // When the SPI is configured as Slave, the SPI is only guaranteed 
    // to work at fosc/4 or lower
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void clock_speed_in_us();


    /// Is a serial transfer complete?
    static bool is_transmission_complete()
    {return atd::is_one_bit<SPIF>::of_register(SPSR);}

    /// The SPI data register was written during data transfer?
    // To clear this bit first read WCOL, then read SPI data register.
    static bool is_a_write_collision()
    {return atd::is_one_bit<WCOL>::of_register(SPSR);}


    /// Write x in the data register and initiates data transmissioin (si SS
    /// está seleccionado).
    static void data_register(std::byte x) {SPDR = std::to_integer<uint8_t>(x);}  

    /// Devuelve el último uint8_t recibido en el último trade.
    static std::byte data_register() {return std::byte{SPDR};}
};


class SPI_base : public SPI_basic{
public:
    SPI_base() = delete;

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


class SPI_master : public SPI_base {
public:
    SPI_master() = delete;

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


class SPI_slave : public SPI_base {
public:
    SPI_slave() = delete;

    /// Enciende el SPI como slave.
    static void on()
    {
	init();
	enable_as_a_slave();
    }
    
private:
    static void init();
};



// reloj del SPI a 1MHz
// --------------------
// a 500 kHz = 2 us
template<>
inline void SPI_basic::clock_speed_in_us<2u, 1000000UL>() 
{clock_speed_divide_by_2();}

// a 4 us
template<>
inline void SPI_basic::clock_speed_in_us<4u, 1000000UL>() 
{clock_speed_divide_by_4();}

// a 8 us
template<>
inline void SPI_basic::clock_speed_in_us<8u, 1000000UL>() 
{clock_speed_divide_by_8();}

// a 16 us
template<>
inline void SPI_basic::clock_speed_in_us<16u, 1000000UL>() 
{clock_speed_divide_by_16();}

// a 32 us
template<>
inline void SPI_basic::clock_speed_in_us<32u, 1000000UL>() 
{clock_speed_divide_by_32();}

// a 64 us
template<>
inline void SPI_basic::clock_speed_in_us<64u, 1000000UL>() 
{clock_speed_divide_by_64();}

// a 128 us
template<>
inline void SPI_basic::clock_speed_in_us<128u, 1000000UL>() 
{clock_speed_divide_by_128();}



// reloj del SPI a 8MHz
// --------------------
// a 250 ns
//template<>
//inline void SPI_basic::clock_speed_en_ns<250u, 8000000UL>() 
//{clock_speed_divide_by_2();}

// a 500 ns
//template<>
//inline void SPI_basic::clock_speed_en_ns<500u, 8000000UL>() 
//{clock_speed_divide_by_4();}

// a 1 us
template<>
inline void SPI_basic::clock_speed_in_us<1u, 8000000UL>() 
{clock_speed_divide_by_8();}

// a 2 us
template<>
inline void SPI_basic::clock_speed_in_us<2u, 8000000UL>() 
{clock_speed_divide_by_16();}

// a 4 us
template<>
inline void SPI_basic::clock_speed_in_us<4u, 8000000UL>() 
{clock_speed_divide_by_32();}

// a 8 us
template<>
inline void SPI_basic::clock_speed_in_us<8u, 8000000UL>() 
{clock_speed_divide_by_64();}

// a 16 us
template<>
inline void SPI_basic::clock_speed_in_us<16u, 8000000UL>() 
{clock_speed_divide_by_128();}

}// namespace avr


#endif 

