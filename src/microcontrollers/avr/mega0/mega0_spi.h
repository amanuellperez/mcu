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

#include "mega0_spi_basic.h"

namespace mega0_{

namespace private_{

template <typename Registers>
class SPI_base : public SPI_basic<Registers>{
public:
// Sugar
    using SPI = SPI_basic<Registers>;

// Constructor
    SPI_basic() = delete;

// SCK_frequency
    // La frecuencia SPI la sabemos en tiempo de compilación
    // Devuelve true si configura correctamente la frecuencia
    // y false en caso contrario
    template <uint32_t frequency_in_hz>
    static bool SCK_frequency_in_hz();

private:
// Helpers
    template <uint32_t frequency_in_hz, uint32_t clk_per_in_hz>
    bool SCK_frequency_in_hz_static();

    template <uint32_t frequency_in_hz>
    bool SCK_frequency_in_hz_static(uint32_t clk_per_in_hz);
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

// C++: ¿cuándo `switch constexpr (prescaler)` ?
template <typename C>
template <uint32_t frequency_in_hz, uint32_t clk_per_in_hz>
inline bool SPI_base<C>::SCK_frequency_in_hz_static()
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
	static_assert(true, "Frequency not supported");

    return true;
}

// DUDA: esta función y la anterior son iguales. ¿Se pueden fusionar en una
// sin perder que la anterior se calcula todo en tiempo de compilación?
template <typename C>
template <uint32_t frequency_in_hz>
inline bool SPI_base<C>::SCK_frequency_in_hz_static(uint32_t clk_per_in_hz)
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
 

// Observar que SPI_master no hace el Select del slave correspondiente.
// Eso es responsabilidad del cliente, que será el que sepa cómo se
// seleccioinaran los slaves.
template <typename Registers>
class SPI_master : public private_::SPI_base<Registers>{
public:
    using SPI = SPI_basic<Registers>;

// Constructor
    SPI_master() = delete;
    
    // Configura el SPI como master con la configuración pasada
    // No lo enciende. Hay que llamar a turn_on explicitamente.
    template <typename SPI_cfg>
    static void init();

// On/off
    static void turn_on();
    static void turn_off();

// Transfer
    // Envia el byte x. Devuelve el valor recibido del slave
    // Bloquea el micro: no devuelve el control hasta transmitir el byte.
    static uint8_t write(uint8_t x);

    // Lee un byte del slave. 
    // `x` es el byte que tiene que enviar el master al slave.
    // Bloquea el micro: no devuelve el control hasta transmitir el byte.
    static uint8_t write(uint8_t x = 0xFF);

private:
    
    static void cfg_pins();
};

// Será el usuario el que defina el selector de SPI, no usando SS. Por eso
// llamamos a disable_client_select_line()
template <typename R>
void SPI_master<R>::cfg_pins()
{
    Pin<SPI::MOSI_pin>::as_output();
    Pin<SPI::MISO_pin>::as_input_without_pullup();
    Pin<SPI::SCK_pin>::as_output();

    // 24.3.2.1.3 (datasheet)
    // If SSD in SPIn.CTRLB is ‘1’, the SPI does not use the SS pin. 
    // It can be used as a regular I/O pin or by other peripheral modules.
    SPI::disable_client_select_line();
}

// Ejemplo de SPI_cfg:
//
// struct SPI_cfg{
//	static constexpr bool data_order_LSB = false;
//	static constexpr uint8_t polarity    = 0;
//	static constexpr uint8_t phase       = 1;
//	static constexpr frequency_in_hz     = xxxx
// };
//
// Esta configuración la define el harwador en prj_dev.h para cada dispositivo
// SPI que se conecta.
template <typename R>
    template <typename SPI_cfg>
void SPI_master<R>::init()
{
    cfg_pins();
    
// as master
    SPI::host_mode();

// data order
    if constexpr (SPI_cfg::data_order_LSB)
	SPI::data_order_LSB();
    else
	SPI::data_order_MSB();


// mode
    if constexpr (SPI_cfg::polarity == 0){
	if constexpr (SPI_cfg::phase == 0) SPI::mode_0();
	else                               SPI::mode_1();
    } else { // polarity == 1
	if constexpr (SPI_cfg::phase == 0) SPI::mode_2();
	else                               SPI::mode_3();
    }

// frequency
    SCK_frequency_in_hz<SPI_cfg::frequency_in_hz>();

// TODO: poder definir buffer mode y demás.
// Usar `requires(SPI_cfg::buffer_mode)` para en general no sea obligatorio
// tener que definir esta variable.
}



}// mega0_


#endif


