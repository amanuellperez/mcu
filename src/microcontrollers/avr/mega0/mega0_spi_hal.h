// Copyright (C) 2024 Manuel Perez 
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

#ifndef __MEGA0_SPI_H__
#define __MEGA0_SPI_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Dispositivo genérico SPI	
 *
 *	Ampliamos el traductor SPI_basic, añadiendo funciones de más alto
 *	nivel y dando un interfaz a todos los dispositivos SPI.
 *
 * HISTORIA
 *    Manuel Perez
 *    10/11/2024 Implementación mínima
 *
 ****************************************************************************/
#include "mega0_spi_hwd.h"
#include "mega0_clock_frequencies.h"
#include "mega0_pin_hwd.h"

#include <atd_type_traits.h>	// always_false_v

namespace mega0_{

namespace hal{

namespace private_{

template <typename Registers>
class SPI_base : public hwd::SPI<Registers>{
public:
// Sugar
    using SPI = hwd::SPI<Registers>;

// Constructor
    SPI_base() = delete;

// SCK_frequency
    // La frecuencia SPI la sabemos en tiempo de compilación
    // Devuelve true si configura correctamente la frecuencia
    // y false en caso contrario
    template <uint32_t frequency_in_hz>
    static bool SCK_frequency_in_hz();

private:
// Helpers
    template <uint32_t frequency_in_hz, uint32_t clk_per_in_hz>
    static constexpr bool SCK_frequency_in_hz_static();

    template <uint32_t frequency_in_hz>
    static constexpr bool SCK_frequency_in_hz_static(uint32_t clk_per_in_hz);
};


template <typename C>
    template <uint32_t frequency_in_hz>
inline bool SPI_base<C>::SCK_frequency_in_hz()
{
    if constexpr (is_clk_per_constant())
	return SCK_frequency_in_hz_static<frequency_in_hz, clk_per()>();

    else
	return SCK_frequency_in_hz_dynamic<frequency_in_hz>(clk_per());
}

// ((C++)) ¿cuándo `switch constexpr (prescaler)` ?
//
// TODO: 
//  1. Añadir el SPI::clock_double_speed() para poder operar a frecuencias más
//     rápidas.
//  2. Garantizar que en client mode:
//	"To ensure correct sampling of this clock signal, the minimum low and
//	  high periods must each be longer than two peripheral clock cycles."
//	  (datasheet 24.3.2.2) Que genere un error en tiempo de compilación 
//	  pero ¿no sabemos que esta en client_mode? Sí, si quien llama a esta
//	  función es SPI_slave!!! (pasarle is_slave a 
//	  SCK_frequency_in_hz_static<is_slave>)
template <typename C>
template <uint32_t frequency_in_hz, uint32_t clk_per_in_hz>
inline constexpr bool SPI_base<C>::SCK_frequency_in_hz_static()
{
    constexpr uint32_t prescaler = clk_per_in_hz / frequency_in_hz;

    if constexpr (prescaler == 2)
	SPI::clock_peripheral_divide_by_2();

    else if constexpr (prescaler == 16)
	SPI::clock_peripheral_divide_by_16();

    else if constexpr (prescaler == 64)
	SPI::clock_peripheral_divide_by_64();

    else if constexpr (prescaler == 128)
	SPI::clock_peripheral_divide_by_128();

    else
	static_assert(atd::always_false_v<C>, "Frequency not supported");

    return true;
}

// ((DUDA)) esta función y la anterior son iguales. ¿Se pueden fusionar en una
// sin perder que la anterior se calcula todo en tiempo de compilación?
template <typename C>
template <uint32_t frequency_in_hz>
inline constexpr 
bool SPI_base<C>::SCK_frequency_in_hz_static(uint32_t clk_per_in_hz)
{
    uint32_t prescaler = clk_per_in_hz / frequency_in_hz;

    switch (prescaler){
	break; case   2: SPI::clock_peripheral_divide_by_2();
	break; case  16: SPI::clock_peripheral_divide_by_16();
	break; case  64: SPI::clock_peripheral_divide_by_64();
	break; case 128: SPI::clock_peripheral_divide_by_128();
	break; default : return false;
    }

    return true;
}

}// private_
 


/***************************************************************************
 *				SPI_master
 ***************************************************************************/
// Configuración de SPI_master
struct SPI_master_cfg{
// Modes
    // Como se lee mejor normal_mode que Mode::normal, optó por definirlo como
    // uint8_t en vez de enum (y total, son solo dos posibles valores)
    // enum class Mode{ normal, buffer}; 
    static constexpr uint8_t normal_mode = 0;
    static constexpr uint8_t buffer_mode = 1;

};

// Observar que SPI_master no hace el Select del slave correspondiente.
// Eso es responsabilidad del cliente, que será el que sepa cómo se
// seleccionaran los slaves.
//
// Ejemplo de SPI_cfg:
//
// struct SPI_cfg{
//	template <uint8_t n>
//	using Pin = myu::hwd::Pin<n, myu::cfg_40_pins::pins>;
//
//	static constexpr uint32_t frequency_in_hz     = xxxx
//	static constexpr auto mode = SPI_master_cfg::normal_mode;
//	
// };
//
template <typename Registers, 
	  typename SPI_cfg // configuración global del SPI
	  >
class SPI_master : public private_::SPI_base<Registers>{
public:
    using Hwd = hwd::SPI<Registers>; // hardware que hay por debajo
    using SPI = Hwd;
    using Base= private_::SPI_base<Registers>;

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

// cfg
    static void default_transfer_value(uint8_t x)
    { transfer_value_ = x;}

// On/off
    static void turn_on();
    static void turn_off();

// Write/read (polling method)
// ----------
// Las siguientes funciones bloquean el micro, no devolviendo el control hasta
// que el SPI no acabe de escribir o leer.
    // Envia el byte x. Devuelve el valor recibido del slave
    // Bloquea el micro: no devuelve el control hasta transmitir el byte.
    // CUIDADO: recordar seleccionar el chip antes de escribir.
    static uint8_t write(uint8_t x);

    // Lee un byte del slave. 
    // `x` es el byte que tiene que enviar el master al slave.
    // Bloquea el micro: no devuelve el control hasta transmitir el byte.
    static uint8_t read(uint8_t x = transfer_value_);

    static void wait_untill_transfer_is_complete();

private:
// Types
    using Cfg = SPI_cfg;

// Cfg
    inline static uint8_t transfer_value_ = 0;

// helpers
    template <template <uint8_t n> typename Pin>
    static void cfg_pins();

    static void wait_untill_write_is_complete();
    static void wait_untill_read_is_complete();

};

// Será el usuario el que defina el selector de SPI, no usando SS. Por eso
// llamamos a disable_client_select_line()
template <typename R, typename C>
    template <template <uint8_t n> typename Pin>
void SPI_master<R, C>::cfg_pins()
{
    Pin<SPI::MOSI_pin>::as_output();
    Pin<SPI::MISO_pin>::as_input_without_pullup();
    Pin<SPI::SCK_pin>::as_output();

    // 24.3.2.1.3 (datasheet)
    // If SSD in SPIn.CTRLB is ‘1’, the SPI does not use the SS pin. 
    // It can be used as a regular I/O pin or by other peripheral modules.
    SPI::disable_client_select_line();
}


template <typename R, typename C>
void SPI_master<R, C>::init()
{
    cfg_pins<Cfg::template Pin>();
    
// as master
    SPI::host_mode();

// frequency
    if constexpr (requires {Cfg::frequency_in_hz;})
	Base::template SCK_frequency_in_hz<Cfg::frequency_in_hz>();

// Mode
    if constexpr (Cfg::mode == SPI_master_cfg::normal_mode)
	SPI::buffer_mode_disable();

    else if constexpr (Cfg::mode == SPI_master_cfg::buffer_mode)
	SPI::buffer_mode_enable();

    else 
	static_assert(atd::always_false_v<R>, "Wrong mode");

}

// Ejemplo de SPI_dev_cfg:
//
// struct SPI_dev_cfg{
//	static constexpr bool data_order_LSB = false;
//	static constexpr uint8_t polarity    = 0;
//	static constexpr uint8_t phase       = 1;
// };
//
// Esta configuración la define el harwador en prj_dev.h para cada dispositivo
// SPI que se conecta.
template <typename R, typename C>
    template <typename SPI_dev_cfg>
void SPI_master<R, C>::cfg()
{
// TODO: esta función de "data order" coincide con la definida en mega, y se
// podría generalizar `if
// (is_defined<SPI_dev_cfg::data_order_LSB>::and_is_true()) ... pero no se
// cómo hacerlo.
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
    if constexpr (SPI_dev_cfg::polarity == 0){
	if constexpr (SPI_dev_cfg::phase == 0) SPI::mode_0();
	else                               SPI::mode_1();
    } else { // polarity == 1
	if constexpr (SPI_dev_cfg::phase == 0) SPI::mode_2();
	else                               SPI::mode_3();
    }

// TODO: de esta forma podemos trabajar con dispositivos que funcionen con
// frecuencias diferentes.
// if constexpr (requires (SPI_dev_cfg::frequency_in_hz))
//	Base::template SCK_frequency_in_hz<Cfg::frequency_in_hz>();
}


template <typename R, typename C>
inline void SPI_master<R, C>::turn_on()
{ SPI::enable(); }

template <typename R, typename C>
inline void SPI_master<R, C>::turn_off()
{ SPI::disable(); }


template <typename R, typename C>
inline uint8_t SPI_master<R, C>::write(uint8_t x)
{
    SPI::data(x);	// escribimos x y lo enviamos
    wait_untill_write_is_complete();
    
    return SPI::data();// valor recibido
}

template <typename R, typename C>
inline uint8_t SPI_master<R, C>::read(uint8_t x)
{ return write(x); }


// TODO: añadirle un counter. Si supera más de x que devuelva el control, asi
// evitamos que se quede colgado.
template <typename R, typename C>
inline void SPI_master<R, C>::wait_untill_write_is_complete()
{
    if constexpr (Cfg::mode == SPI_master_cfg::normal_mode){
	while (!SPI::is_interrupt_flag_set()) 
	{ ; }
    }

    else if constexpr (Cfg::mode == SPI_master_cfg::buffer_mode){
	while (!SPI::is_transfer_complete_flag_set()) { ; }

	while (!SPI::is_receive_complete_flag_set()) { ; }
    }

    else
	static_assert(atd::always_false_v<R>, 
			"Why did you call this function without init SPI?");


}


template <typename R, typename C>
inline void SPI_master<R, C>::wait_untill_read_is_complete()
{
    if constexpr (Cfg::mode == SPI_master_cfg::normal_mode){
	while (!SPI::is_interrupt_flag_set()) 
	{ ; }
    }

    else if constexpr (Cfg::mode == SPI_master_cfg::buffer_mode){
	while (!SPI::is_receive_complete_flag_set()) 
	{ ; }
    }

    else
	static_assert(atd::always_false_v<R>, "Why did you call this function without init SPI?");
}

} // namespace hal
}// mega0_


#endif


