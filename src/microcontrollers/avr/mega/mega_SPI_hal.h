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

#ifndef __MEGA_SPI_HAL_H__
#define __MEGA_SPI_HAL_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Dispositivo genérico SPI
 *
 *  HISTORIA
 *    Manuel Perez
 *	12/02/2023 Separando traductor de dispositivo genérico.
 *	01/03/2023 SCK_frequency_in_hz da error en tiempo de compilación
 *		   para frecuencias no soportadas.
 *	02/01/2024 Modifico SPI_master adaptándolo al protocolo del mega0.
 *		   La idea para usarlo es:
 *			1. SPI_master<Cfg>::init();
 *			   Inicializamos el SPI configurándolo.
 *			   
 *			2. SPI_master::cfg<Cfg_def>();
 *			   Configuramos el SPI para conectanos a un
 *			   dispositivo determinado. Aquí configuramos la
 *			   polaridad, la fase, el data order, ...
 *
 *			3. El cliente es responsable de hacer el select del
 *			   SPI.
 *
 ****************************************************************************/
#include "mega_SPI_hwd.h"
#include "mega_clock_frequencies.h"	

namespace mega_{
namespace hal{

namespace private_{
class SPI_base : public hwd::SPI{
public:
    SPI_base() = delete;

    /// Configuramos la velocidad del reloj del SPI en microsegundos.
    //
    // Observar que esta función lo único que hace es traducir la frecuencia o
    // periodo correspondiente en el divisor de frecuencia adecuado llamando a
    // la función `clock_frequency_divide_by_xxx` correspondiente.
    // No enciende el SPI, para ello hay que llamar a turn_on.
    //
    // La función SCK_period_in_us traduce la forma de hablar del cliente (en
    // microsegundos) en la forma de hablar del avr (en divisor de frecuencia)
    // CUIDADO: parece ser que si se quieren conectar 2 avrs la frecuencia del
    // master tiene que ser 4 veces más lenta que la del slave:
    // Datasheet, 23.5.2. SPI Status register: 
    // When the SPI is configured as Slave, the SPI is only guaranteed 
    // to work at fosc/4 or lower
    template<uint16_t period
	    , uint32_t SCK_frequency_in_hz = hwd::clock_cpu()>
    static void SCK_period_in_us();

    template<uint32_t frequency
	    , uint32_t SCK_frequency_in_hz = hwd::clock_cpu()>
    static void SCK_frequency_in_hz();

    // Inicializa SCK pudiendo pasar la frecuencia o el periodo.
    // Si está definido Cfg::frequency_in_hz llama a SCK_frequency_in_hz,
    // pero si está definido Cfg::period_in_us usa SCK_period_in_us.
    // Es más genérica que las 2 anteriores. 
    template <typename Cfg, uint32_t SCK_frequency_in_hz = hwd::clock_cpu()>
    static void SCK_init();

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
    /// TODO: esto está mal para el slave.
    ///	      El slave en la interrupción:
    ///		1) Lee el byte que ha enviado el master
    ///		2) Escribe el nuevo byte que quiere enviar cuando el master 
    ///		   le envie un nuevo byte.
    ///	        3) Hace clear del flag de la interrupción
    ///
    ///	Como son diferentes las formas de leer/escribir de master/slave estas
    ///	funciones son realmente de SPI_master y SPI_slave!!!
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

}// private_

namespace private_{
template <typename Cfg, uint32_t SCK_frequency_in_hz0>
inline void SPI_base::SCK_init()
{
    if constexpr (requires {Cfg::frequency_in_hz;})
	SCK_frequency_in_hz<Cfg::frequency_in_hz, SCK_frequency_in_hz0>();

    else if constexpr (requires {Cfg::period_in_us;})
	SCK_period_in_us<Cfg::period_in_us, SCK_frequency_in_hz0>();

    else
	static_assert(true, "You forgot to define frequency_in_hz or period_in_us!");
}


// En caso de que se pase una frecuencia no soportada, el compilador
// tiene que decirlo.
template<uint32_t frequency
	    , uint32_t SCK_frequency_in_hz>
inline void SPI_base::SCK_frequency_in_hz()
{static_assert(atd::always_false_v<int>, "Frequency not supported");}


// reloj del SPI a 1MHz
// --------------------
// a 500 kHz = 2 us
template<>
inline void SPI_base::SCK_period_in_us<2u, 1000000UL>() 
{clock_frequency_divide_by_2();}

// a 4 us
template<>
inline void SPI_base::SCK_period_in_us<4u, 1000000UL>() 
{clock_frequency_divide_by_4();}

// a 8 us
template<>
inline void SPI_base::SCK_period_in_us<8u, 1000000UL>() 
{clock_frequency_divide_by_8();}

// a 16 us
template<>
inline void SPI_base::SCK_period_in_us<16u, 1000000UL>() 
{clock_frequency_divide_by_16();}

// a 32 us
template<>
inline void SPI_base::SCK_period_in_us<32u, 1000000UL>() 
{clock_frequency_divide_by_32();}

// a 64 us
template<>
inline void SPI_base::SCK_period_in_us<64u, 1000000UL>() 
{clock_frequency_divide_by_64();}

// a 128 us
template<>
inline void SPI_base::SCK_period_in_us<128u, 1000000UL>() 
{clock_frequency_divide_by_128();}


// a 500 kHz
template<>
inline void SPI_base::SCK_frequency_in_hz<500'000UL, 1'000'000UL>()
{ clock_frequency_divide_by_2(); }

// a 250 kHz
template<>
inline void SPI_base::SCK_frequency_in_hz<250'000UL, 1'000'000UL>()
{ clock_frequency_divide_by_4(); }

// a 125 kHz
template<>
inline void SPI_base::SCK_frequency_in_hz<125'000UL, 1'000'000UL>()
{ clock_frequency_divide_by_8(); }

// a 62.5 kHz
template<>
inline void SPI_base::SCK_frequency_in_hz<62'500UL, 1'000'000UL>()
{ clock_frequency_divide_by_16(); }

// a 31.25 kHz
template<>
inline void SPI_base::SCK_frequency_in_hz<31'250UL, 1'000'000UL>()
{ clock_frequency_divide_by_32(); }

// a 15.625 kHz
template<>
inline void SPI_base::SCK_frequency_in_hz<15'625UL, 1'000'000UL>()
{ clock_frequency_divide_by_64(); }

// a 7812 Hz
template<>
inline void SPI_base::SCK_frequency_in_hz<7'812UL, 1'000'000UL>()
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
inline void SPI_base::SCK_period_in_us<1u, 8000000UL>() 
{clock_frequency_divide_by_8();}

// a 2 us
template<>
inline void SPI_base::SCK_period_in_us<2u, 8000000UL>() 
{clock_frequency_divide_by_16();}

// a 4 us
template<>
inline void SPI_base::SCK_period_in_us<4u, 8000000UL>() 
{clock_frequency_divide_by_32();}

// a 8 us
template<>
inline void SPI_base::SCK_period_in_us<8u, 8000000UL>() 
{clock_frequency_divide_by_64();}

// a 16 us
template<>
inline void SPI_base::SCK_period_in_us<16u, 8000000UL>() 
{clock_frequency_divide_by_128();}
}// private_
 


/***************************************************************************
 *				SPI_master
 ***************************************************************************/
// Observar que SPI_master no hace el Select del slave correspondiente.
// Eso es responsabilidad del cliente, que será el que sepa cómo se
// seleccionaran los slaves.
//
// Ejemplo de SPI_cfg:
//
// struct SPI_cfg{
//	template <uint8_t n>
//	using Pin = myu::hwd::Pin<n, myu::cfg::pins>;
//
//	static constexpr uint32_t frequency_in_hz     = xxxx
//	or
//	static constexpr uint32_t period_in_us = xxx
// };
//
template <typename SPI_cfg>
class SPI_master : public private_::SPI_base {
public:
    using Hwd = hwd::SPI; // hardware que hay por debajo
    using SPI = Hwd;
    using Base= private_::SPI_base;

// Constructor
    SPI_master() = delete;

    // Inicializa el SPI como master. Se le pasa la configuración común a la
    // comunicación con todos los dispositivos SPI.
    // No lo enciende. Hay que llamar a turn_on explicitamente.
    static void init();

    // Configura el SPI como master con la configuración pasada.
    // Cada dispositivo tendrá una configuración diferente.
    template <typename SPI_dev_cfg>
    static void cfg();

    /// Enciende el SPI como Master, configurándolo los pins adecuadamente
    // (DUDA) llamar aquí o no a cfg_pins? 
    static void turn_on()
    {
	cfg_pins<Cfg::template Pin>();
	SPI::enable_as_a_master();
    }

    static void turn_off()
    { disable(); }
    
private:
// Types
    using Cfg = SPI_cfg;

// helpers
    template <template <uint8_t n> typename Pin>
    static void cfg_pins();
};


template <typename C>
void SPI_master<C>::init()
{
    cfg_pins<Cfg::template Pin>();
    
// as master
    SPI::enable_as_a_master();

// frequency
    Base::template SCK_init<Cfg>();
}


// Ejemplo de SPI_dev_cfg:
//
// struct SPI_dev_cfg{
//	static constexpr bool data_order_LSB = true;
//  (or static constexpr bool data_order_MSB = true; )
//	static constexpr uint8_t polarity    = 0;
//	static constexpr uint8_t phase       = 1;
// };
//
// Esta configuración la define el harwador en prj_dev.h para cada dispositivo
// SPI que se conecta.
template <typename C>
    template <typename SPI_dev_cfg>
void SPI_master<C>::cfg()
{
// data order
    if constexpr (requires {SPI_dev_cfg::data_order_LSB;})
    {
	if constexpr (SPI_dev_cfg::data_order_LSB)
	    SPI::data_order_LSB();
	else
	    SPI::data_order_MSB();

    } else if constexpr (requires {SPI_dev_cfg::data_order_MSB;}){
	if constexpr (SPI_dev_cfg::data_order_MSB)
	    SPI::data_order_MSB();
	else
	    SPI::data_order_LSB();
    }

// mode
    SPI::mode(SPI_dev_cfg::polarity, SPI_dev_cfg::phase);

}

// En la appnote "AVR151: Setup And use of the SPI", indican que:
// The MISO, MOSI, and SCK lines of all the other slaves are tristated 
// (configured as input pins of a high impedance with no pullup resistors
// enabled). 
// ==> Conclusión: no tienen pull-up resistor.
// Pero en el momento en el que haces el chip_select conectas el registro del
// SPI del master con el registro del SPI del slave formando un circuito
// cerrado. Da la impresión de no necesitar pull-up resistors (???)
template <typename Cfg>
    template <template <uint8_t n> typename Pin>
void SPI_master<Cfg>::cfg_pins()
{
    // Configuración de los pins
    Pin<SPI::SCK_pin>::as_output();
    Pin<SPI::MOSI_pin>::as_output();
//    Pin<SPI::MISO_pin>::as_input_without_pullup();
    Pin<SPI::SS_pin>::as_output(); // fundamental para que no sea slave:
				     // punto 23.3.2: leer este punto. Indica
				     // que si se define como entrada tiene
				     // que mantenerse high, si cambia a low
				     // cambia el modo del SPI a slave!!!
}





/***************************************************************************
 *				SPI_slave
 ***************************************************************************/
class SPI_slave : public private_::SPI_base {
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



}// namespace hal
}// namespace


#endif 

