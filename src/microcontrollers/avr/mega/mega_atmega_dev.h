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
#include "mega_private.h"    // import avr_;
			
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
    using namespace mega_;

// MICRO
    using Micro = mega_::Micro;


// PINS
    template <uint8_t n>
    using Pin = mega_::Pin<n>;

    template <uint8_t n>
    using Enable_change_level_interrupt = mega_::Enable_change_level_interrupt<n>;

//    template<uint8_t n>
//    using Output_pin = avr_::Output_pin<n>;
//
//    template<uint8_t n>
//    using Input_pin_with_pullup = avr_::Input_pin_with_pullup<n>;
//
//    template<uint8_t n>
//    using Input_pin_without_pullup = avr_::Input_pin_without_pullup<n>;

// ADC
    using ADC_basic = mega_::ADC_basic;
    using ADC	    = mega_::ADC;

    template <uint8_t AREF_connect_to, uint16_t adc_frequency_in_kHz>
    using ADC_pin_single_mode	  
	    = mega_::ADC_pin_single_mode<AREF_connect_to, adc_frequency_in_kHz>;

// TIMERS
    // TODO: sería más genérico el pasar solamente el número de pin sin
    // indicar el Timer al que pertenece.
    //
    // Ej: PWM_pin<11> pwm1; // usaría el Timer0
    //     PWM_pin<15> pwm2; // usaría el Timer1
    // 
    // De esa forma no tendríamos que indicar en código el Timer que estamos
    // usando. ¿Más sencillo o lo complica? 
    //
    // Timer0
    // ------
    using Timer0         = mega_::Timer0;
    using Time_counter0  = mega_::Time_counter0;
    // TODO: borrar Square_wave_generator0_g a favor de SWG0_pin
    using Square_wave_generator0_g = mega_::Square_wave_generator0_g;
    using Square_wave_burst_generator0_g = mega_::Square_wave_burst_generator0_g;
    using SWG0           = mega_::Square_wave_generator0_g;

    template <uint8_t npin>
    using PWM0_pin	 = mega_::PWM0_pin<npin>;

    // Timer1
    // ------
    using Timer1         = mega_::Timer1;
    using Time_counter1  = mega_::Time_counter1;

    template <uint8_t npin>
    using PWM1_pin	 = mega_::PWM1_pin<npin>;

    template <uint8_t npin>
    using SWG1_pin	 = mega_::SWG1_pin<npin>;

    // TODO: borrar estos 2
    using Square_wave_generator1_g = mega_::Square_wave_generator1_g;
    using SWG1           = mega_::Square_wave_generator1_g;

    // Timer2
    // ------
    using Timer2         = mega_::Timer2;
    using Time_counter2  = mega_::Time_counter2;

    template <uint16_t timeout_ms_max>
    using Time_counter2_32kHz_g= mega_::Time_counter2_32kHz_g<timeout_ms_max>;

// PROTOCOLS
    using UART_basic    = mega_::UART_basic;
    using UART_iostream = mega_::UART_iostream;

    using SPI        = mega_::SPI_basic;
    using SPI_master = mega_::SPI_master;
    using SPI_slave  = mega_::SPI_slave;

    using TWI_basic  = mega_::TWI_basic;

// MCU UTILITIES
    using Sleep    = mega_::Sleep;
    using Watchdog = mega_::Watchdog;

// ROM (Generic interface of progmem)
    template <typename T>
    inline constexpr T rom_read(const T& x) { return mega_::rom_read(x);}

    using ROM_read     = mega_::ROM_read;

    using ROM_uint8_t  = mega_::ROM_uint8_t;
    using ROM_uint16_t = mega_::ROM_uint16_t;

    template <typename T, size_t N, typename Read = ROM_read>
    using ROM_array = mega_::ROM_array<T, N, Read>;

    template <size_t N>
    using ROM_string = mega_::ROM_string<N>;

    template <size_t N>
    using Element_ROM_string_array = mega_::Element_ROM_string_array<N>;

    template <size_t N>
    using ROM_string_array = mega_::ROM_string_array<N>;

    // TODO: borrar las views de progmem. DEPRECATED
    template <typename T, size_t N>
    using Progmem_array_view = mega_::Progmem_array_view<T, N>;

    template <typename T, size_t nrows, size_t ncols>
    using Progmem_biarray_view = mega_::Progmem_biarray_view<T, nrows, ncols>;


    namespace literals{ using namespace mega_::literals; }

    
//// TIME
//    using Wait_1_us = mega_::Wait_1_us;
//    using Wait_1_ms = mega_::Wait_1_ms;

/***************************************************************************
 *			FUNCIONES AUXILIARES
 ***************************************************************************/
// ¿dónde meter esto? ¿qué son?
    // El parámetro lo usamos para sobrecargar: quiero que configures el flujo
    // con la configuración básica.
    template <uint32_t baud_rate = 9600u,
	      uint32_t f_clock   = mega_::clock_frequency_in_hz,
	      uint32_t tolerance = 2>
    inline static void basic_cfg(mega_::UART_iostream&) 
    { return mega_::UART_basic_cfg<baud_rate, f_clock, tolerance>(); }

}// namespace

// Micros equivalentes a nivel de código
namespace atmega328p = atmega;

#endif


