// Copyright (C) 2022 Manuel Perez 
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

#ifndef __AVR_ATMEGA_DEV_H__
#define __AVR_ATMEGA_DEV_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Built-in devices que suministra el atmega
 *  
 *   - HISTORIA:
 *    Manuel Perez
 *      08/12/2022 Escrito
 *
 ****************************************************************************/
#include "avr_cfg.h"	// MCU_CLOCK_FREQUENCY_IN_HZ
			
/***************************************************************************
 *			BUILT-IN DEVICES FOR ATMEGA
 ***************************************************************************/
// (RRR) ¿por qué no definir todas estas clases directamente dentro del
//	 namespace atmega?
//
//	 Porque es posible que los de avr reutilicen chips y la implementación
//	 de un Timer en el atmega sea igual que la del Timer en attiny. Como
//	 lo desconozco en lugar de cambiarlo todo (ahora todo lo tenía metido
//	 en el namespace avr) optó por cambiarlo poco a poco. 
//
//	 Si al implementar otro microcontrolador no puedo reutilizar nada de 
//	 avr_  quiere decir que puedo borrar este namespace sustituyéndolo por
//	 atmega. 
//
//	 Si en cambio resulta que el attiny::Timer0 y atmega::Timer0 solo se
//	 diferencian en la configuración podré definirlos:
//
//		namespace attiny{ using Timer0 = avr_::Timer0;}
//		namespace atmega{ using Timer0 = avr_::Timer0;}
//
//	 pudiendo reutilizar código.
//
//	 Hasta que no implemente otros micros no puedo saber si 
//	 avr_ == atmega o no.
//
//
//  Con todo definirlo de esta forma trae un efecto secundario: queda muy bien
//  documentado (en código) los dispositivos que forman el atmega. Basta con
//  leer lo que hay dentro de namespace atmega{ ... }
//

namespace atmega{

// PINS
    template <uint8_t n>
    using Pin = avr_::Pin<n>;

    template<uint8_t n>
    using Output_pin = avr_::Output_pin<n>;

    template<uint8_t n>
    using Input_pin_with_pullup = avr_::Input_pin_with_pullup<n>;

    template<uint8_t n>
    using Input_pin_without_pullup = avr_::Input_pin_without_pullup<n>;

    using ADC    = avr_::ADC;

// TIMERS
    using Timer0         = avr_::Timer0;
    using Timer0_generic = avr_::Timer0_generic;
    using Timer0_g	 = Timer0_generic;
    using Time_counter0_generic = avr_::Time_counter0_generic;
    using Time_counter0_g       = Time_counter0_generic;
    using Square_wave_generator0_g = avr_::Square_wave_generator0_g;
    using Square_wave_burst_generator0_g = avr_::Square_wave_burst_generator0_g;

    using Timer1         = avr_::Timer1;
    using Timer1_generic = avr_::Timer1_generic;
    using Timer1_g	 = Timer1_generic;
    using Time_counter1_generic = avr_::Time_counter1_generic;
    using Time_counter1_g	= Time_counter1_generic;
    using Square_wave_generator1_g = avr_::Square_wave_generator1_g;


// PROTOCOLS
    using UART_basic = avr_::UART_basic;
    using UART_iostream = avr_::UART_iostream;

    using SPI_master = avr_::SPI_master;
    using SPI_slave  = avr_::SPI_slave;

    using TWI_basic  = avr_::TWI_basic;

    // TODO: esta es genérica, no depende de avr. Sacarla de avr
    template <typename TWI, typename TWI::streamsize buffer_size0>
    using TWI_master = avr_::TWI_master<TWI, buffer_size0>;

    // TODO: esta es genérica, no depende de avr. Sacarla de avr
    template <typename TWI_master>
    using TWI_master_ioxtream = avr_::TWI_master_ioxtream<TWI_master>;

    // TODO: esta es genérica, no depende de avr. Sacarla de avr
    template <typename TWI, uint8_t buffer_size>
    using TWI_slave = avr_::TWI_slave<TWI, buffer_size>;


// MCU UTILITIES
    using Interrupt          = avr_::Interrupt;

    using Power     = avr_::Power;


// PROGMEM
    template <size_t N>
    using Progmem_string = avr_::Progmem_string<N>;

    template <size_t N>
    using Element_progmem_string_array = avr_::Element_progmem_string_array<N>;

    template <size_t N>
    using Progmem_string_array = avr_::Progmem_string_array<N>;

    template <typename T, size_t N>
    using Progmem_array_view = avr_::Progmem_array_view<T, N>;

    template <typename T, size_t nrows, size_t ncols>
    using Progmem_biarray_view = avr_::Progmem_biarray_view<T, nrows, ncols>;

    namespace literals{ using namespace avr_::literals; }

// micro
// -----
    using Micro = avr_::Micro;


/***************************************************************************
 *			FUNCIONES AUXILIARES
 ***************************************************************************/
// ¿dónde meter esto? ¿qué son?
    // El parámetro lo usamos para sobrecargar: quiero que configures el flujo
    // con la configuración básica.
    template <uint32_t baud_rate = 9600u,
	      uint32_t f_clock   = MCU_CLOCK_FREQUENCY_IN_HZ,
	      uint32_t tolerance = 2>
    inline static void basic_cfg(avr_::UART_iostream&) 
    { return avr_::UART_iostream_basic_cfg<baud_rate, f_clock, tolerance>(); }

}// namespace



#endif


