// Copyright (C) 2019-2021 Manuel Perez 
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

#ifndef __DEV_REGISTER_74HC595_SPI_H__
#define __DEV_REGISTER_74HC595_SPI_H__

/****************************************************************************
 *
 *   - DESCRIPCION: Traductor del registro 74HC595. 
 *
 *   - HISTORIA:
 *
 *    Manuel Perez
 *    09/06/2019 Escrito v0.1
 *    10/06/2019 v0.2: conecto el 74HC595 al SPI del avr.
 *    14/03/2020       reestructuro.
 *
 ****************************************************************************/

#include <mega.h>	// parametrizar <Micro, SPI>
#include "not_generic.h"

namespace dev{

// Conexiones hwd
// --------------
template <uint8_t pin_RCLK>
struct Register_74HC595_RCLK{
    static constexpr uint8_t RCLK = pin_RCLK;
};


template <uint8_t pin_NO_SRCLR>
struct Register_74HC595_NO_SRCLR{
    static constexpr uint8_t NO_SRCLR = pin_NO_SRCLR;
};

template <typename pin_RCLK, typename pin_NO_SRCLR>
struct Register_74HC595_pins{
    static constexpr uint8_t RCLK = pin_RCLK::RCLK;
    static constexpr uint8_t NO_SRCLR = pin_NO_SRCLR::NO_SRCLR;
};


/// Fundamental:
///	1. Hay que configurar SPI como master, llamando a SPI_master::on.
///	2. Antes que escribir en el registro hay que seleccionarlo.
///	   En principio como hay diferentes formas de selecionarlo no defino
///	   cómo hacerlo aquí.
///
/// Al usar SPI volcamos directamente un byte en el register. Si quisieramos
/// hacer un shift de 1 bit puede que esto sea ineficiente (sería mejor
/// escribir el 1, enviar un pulso, escribir 0 y enviar pulsos sucesivos sin 
/// necesidad de escribir todo el Registro cada vez que movemos 1 bit).
//
// TODO: parametrizarlo por <Micro, SPI>
template <typename pin>
class Register_74HC595_SPI{
public:
// TODO: pasar esto como parámetro
// Observar que esta cfg no debería de estar aquí, sino que hay que 
// pasar SPI_master como parámetro
    struct SPI_master_cfg{
	template <uint8_t n>
	using Pin = not_generic::hwd::Pin<n>;

// Esta la define el main.cpp. 
//	static constexpr uint32_t period_in_us = 8;
    };

    using SPI_master = not_generic::SPI_master<SPI_master_cfg>;

    Register_74HC595_SPI() 
    { 
	SPI_master::data_order_LSB();
	NO_SRCLR_.write_one(); 
    }

    /// Escribimos en el registro un byte. No devuelve el control
    /// hasta no acabar de escribir.
    // Observar que esta función se puede definir como const. Sin embargo,
    // el cliente supondrá que es no const. La dejo con el prototipo esperado.
    void buffer_write(uint8_t x) { SPI_master::write(uint8_t{x}); }

    void buffer_clear() 
    { 
	// NO_SRCLR_.negative_pulse_of_1us(); 
	NO_SRCLR_.write_zero();
	//Micro::wait_us(1);  
	not_generic::wait_us(1);  
	NO_SRCLR_.write_one();
    }

    /// Vuelca el buffer en el registro de salida.
    void flush()
    { 
	// RCLK_.pulse_of_1us(); 
	RCLK_.write_one();
	//Micro::wait_us(1);  
	not_generic::wait_us(1);  
	RCLK_.write_zero();
    }


    /// Escribimos en la salida el byte x
    void write(uint8_t x)
    {
	buffer_write(x);
	flush();
    }

    /// Vacía el buffer y la salida
    void clear()
    {
	buffer_clear();
	flush();
    }

private:
    not_generic::Output_pin<pin::RCLK> RCLK_;	
    not_generic::Output_pin<pin::NO_SRCLR> NO_SRCLR_;
};




}// namespace

#endif

