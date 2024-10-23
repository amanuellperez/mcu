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

#ifndef __MEGA_SPI_GENERIC_H__
#define __MEGA_SPI_GENERIC_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Dispositivo genérico SPI
 *
 *  HISTORIA
 *    Manuel Perez
 *	12/02/2023 Separando traductor de dispositivo genérico.
 *	01/03/2023 clock_frequency_in_hz da error en tiempo de compilación
 *		   para frecuencias no soportadas.
 *
 ****************************************************************************/
#include "mega_SPI_basic.h"

namespace mega_{


class SPI_base : public SPI_basic{
public:
    SPI_base() = delete;

    /// Configuramos la velocidad del reloj del SPI en microsegundos.
    //
    // Observar que esta función lo único que hace es traducir la frecuencia o
    // periodo correspondiente en el divisor de frecuencia adecuado llamando a
    // la función `clock_frequency_divide_by_xxx` correspondiente.
    // No enciende el SPI, para ello hay que llamar a turn_on.
    //
    // La función clock_period_in_us traduce la forma de hablar del cliente (en
    // microsegundos) en la forma de hablar del avr (en divisor de frecuencia)
    // CUIDADO: parece ser que si se quieren conectar 2 avrs la frecuencia del
    // master tiene que ser 4 veces más lenta que la del slave:
    // Datasheet, 23.5.2. SPI Status register: 
    // When the SPI is configured as Slave, the SPI is only guaranteed 
    // to work at fosc/4 or lower
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = clock_frequency_in_hz>
    static void clock_period_in_us();

    template<uint32_t frequency
	    , uint32_t clock_frequency_in_hz = clock_frequency_in_hz>
    static void clock_frequency_in_hz();

    // Las transmisiones del SPI nunca van a fallar: al enviar un byte el SPI
    // arranca el reloj, genera 8 pulsos y marca como enviado el byte,
    // independientemente de si lo ha enviado o no, ya que el avr no tiene
    // forma de saberlo.
    /// Espera hasta que el SPI ha enviado/recibido el último byte.
    // En la practica el SPI es tan rapido que el avr no tiene que esperar.
    // TODO: esta función no FUNCIONA!!! No debo de tener claro cuándo
    // funciona is_transmission_complete. REVISARLO
    static void wait_transmission_complete()
    {
	while (!is_transmission_complete())
	{; }
    }


    /// Enviamos el byte x y esperamos hasta que lo haya enviado.
    /// Devuelve el valor recibido.
    static uint8_t write(uint8_t x)
    { return transfer(x); }


    /// Lee un byte. Espera hasta que haya leido el byte.
    static uint8_t read()
    { return transfer(transfer_value_); }


    // En SPI no existen funciones de read/write sino que realmente siempre se
    // realiza un transfer: enviamos un byte y recibimos otro. 
    // Sin embargo, en código quiero que se muestre la intención:
    // `SPI::read()` muestra claramente que queremos leer. ¿Qué valor por
    // defecto enviar?
    //
    // ¿Qué ocurre si el dispositivo con el que hablamos no actualiza su
    // byte SPI? Al leer leeremos el último byte que escribimos. Si escribo
    // 0x00 por defecto y este es el valor que escribe el dispositivo cuando
    // todo va bien no voy a poder detectar el error. Si escribo 0xFF y este
    // es un valor que NUNCA va a responder el dispositivo, en la segunda
    // lectura SPI::read() obtendré un 0xFF sabiendo que el dispositivo no
    // está respondiendo realmente. ¿Servirá para algo esto? Que el uso nos lo
    // diga.
    static void default_transfer_value(uint8_t x)
    { transfer_value_ = x;}

private:
// Cfg
    inline static uint8_t transfer_value_ = 0;

// Functions
    // Enviamos el byte x y esperamos hasta que lo haya enviado.
    // Devuelve el valor recibido.
    static uint8_t transfer(uint8_t x)
    {
	data_register(x);	// escribimos x y lo enviamos
	wait_transmission_complete();

        return data_register();// valor recibido
    }


};


class SPI_master : public SPI_base {
public:
    SPI_master() = delete;

    /// Configuramos los pines para que el SPI funciones como master.
    /// Configuramos todos: SCK, MOSI y SS como de salida, y MISO como de
    /// entrada.
    static void cfg_pins();

    /// Enciende el SPI como Master, configurándolo los pins adecuadamente
    static void turn_on()
    {
	cfg_pins();
	enable_as_a_master();
    }

    static void turn_off()
    { disable(); }
    
};


class SPI_slave : public SPI_base {
public:
    SPI_slave() = delete;

    /// Enciende el SPI como slave.
    static void turn_on()
    {
	init();
	enable_as_a_slave();
    }
    
    static void turn_off()
    { disable(); }

private:
    static void init();
};

// En caso de que se pase una frecuencia no soportada, el compilador
// tiene que decirlo.
template<uint32_t frequency
	    , uint32_t clock_frequency_in_hz>
inline void SPI_base::clock_frequency_in_hz()
{static_assert(atd::always_false_v<int>, "Frequency not supported");}


// reloj del SPI a 1MHz
// --------------------
// a 500 kHz = 2 us
template<>
inline void SPI_base::clock_period_in_us<2u, 1000000UL>() 
{clock_frequency_divide_by_2();}

// a 4 us
template<>
inline void SPI_base::clock_period_in_us<4u, 1000000UL>() 
{clock_frequency_divide_by_4();}

// a 8 us
template<>
inline void SPI_base::clock_period_in_us<8u, 1000000UL>() 
{clock_frequency_divide_by_8();}

// a 16 us
template<>
inline void SPI_base::clock_period_in_us<16u, 1000000UL>() 
{clock_frequency_divide_by_16();}

// a 32 us
template<>
inline void SPI_base::clock_period_in_us<32u, 1000000UL>() 
{clock_frequency_divide_by_32();}

// a 64 us
template<>
inline void SPI_base::clock_period_in_us<64u, 1000000UL>() 
{clock_frequency_divide_by_64();}

// a 128 us
template<>
inline void SPI_base::clock_period_in_us<128u, 1000000UL>() 
{clock_frequency_divide_by_128();}


// a 500 kHz
template<>
inline void SPI_base::clock_frequency_in_hz<500'000UL, 1'000'000UL>()
{ clock_frequency_divide_by_2(); }

// a 250 kHz
template<>
inline void SPI_base::clock_frequency_in_hz<250'000UL, 1'000'000UL>()
{ clock_frequency_divide_by_4(); }

// a 125 kHz
template<>
inline void SPI_base::clock_frequency_in_hz<125'000UL, 1'000'000UL>()
{ clock_frequency_divide_by_8(); }

// a 62.5 kHz
template<>
inline void SPI_base::clock_frequency_in_hz<62'500UL, 1'000'000UL>()
{ clock_frequency_divide_by_16(); }

// a 31.25 kHz
template<>
inline void SPI_base::clock_frequency_in_hz<31'250UL, 1'000'000UL>()
{ clock_frequency_divide_by_32(); }

// a 15.625 kHz
template<>
inline void SPI_base::clock_frequency_in_hz<15'625UL, 1'000'000UL>()
{ clock_frequency_divide_by_64(); }

// a 7812 Hz
template<>
inline void SPI_base::clock_frequency_in_hz<7'812UL, 1'000'000UL>()
{ clock_frequency_divide_by_128(); }


// reloj del SPI a 8MHz
// --------------------
// a 250 ns
//template<>
//inline void SPI_base::clock_frequency_en_ns<250u, 8000000UL>() 
//{clock_frequency_divide_by_2();}

// a 500 ns
//template<>
//inline void SPI_base::clock_frequency_en_ns<500u, 8000000UL>() 
//{clock_frequency_divide_by_4();}

// a 1 us
template<>
inline void SPI_base::clock_period_in_us<1u, 8000000UL>() 
{clock_frequency_divide_by_8();}

// a 2 us
template<>
inline void SPI_base::clock_period_in_us<2u, 8000000UL>() 
{clock_frequency_divide_by_16();}

// a 4 us
template<>
inline void SPI_base::clock_period_in_us<4u, 8000000UL>() 
{clock_frequency_divide_by_32();}

// a 8 us
template<>
inline void SPI_base::clock_period_in_us<8u, 8000000UL>() 
{clock_frequency_divide_by_64();}

// a 16 us
template<>
inline void SPI_base::clock_period_in_us<16u, 8000000UL>() 
{clock_frequency_divide_by_128();}

}// namespace avr


#endif 

