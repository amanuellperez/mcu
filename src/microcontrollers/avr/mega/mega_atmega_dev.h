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

#ifndef __MEGA_ATMEGA_DEV_H__
#define __MEGA_ATMEGA_DEV_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Built-in devices que suministra el atmega
 *  
 *   - HISTORIA:
 *    Manuel Perez
 *      08/12/2022 Escrito
 *
 ****************************************************************************/
#include "mega_import_avr.h"    // import avr_;
#include <mcu_UART_iostream.h>  // Este lo incluyo por comodidad aquí

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
    using Micro = mega_::Micro<hwd::cfg::pins_28>;


namespace hwd{
    template <uint8_t n>
    using Pin = mega_::hwd::Pin<n>;

    template <uint8_t n> // ((TODO)) borrar Output_pin!!!
    using Output_pin = mega_::hwd::Output_pin<n>;

    template <uint8_t n>
    using Enable_change_level_interrupt = mega_::hwd::Enable_change_level_interrupt<n>;

    using ADC      = mega_::hwd::ADC;

    using Timer0   = mega_::hwd::Timer0;
    using Timer1   = mega_::hwd::Timer1;
    using Timer2   = mega_::hwd::Timer2;

    using UART	   = mega_::hwd::UART;
    using SPI      = mega_::hwd::SPI;
    using TWI      = mega_::hwd::TWI;

    using Sleep    = mega_::hwd::Sleep;
    using Watchdog = mega_::hwd::Watchdog;

} // hwd

namespace hal{
// ADC
    using ADC	    = mega_::hal::ADC;

    template <uint8_t AREF_connect_to, uint16_t adc_frequency_in_kHz>
    using ADC_pin_single_mode	  
	    = mega_::hal::ADC_pin_single_mode<AREF_connect_to, adc_frequency_in_kHz>;

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
    using Time_counter0  = mega_::hal::Time_counter0;
    // TODO: borrar Square_wave_generator0_g a favor de SWG0_pin
    using Square_wave_generator0_g = mega_::hal::Square_wave_generator0_g;
    using Square_wave_burst_generator0_g = mega_::hal::Square_wave_burst_generator0_g;
    using SWG0           = mega_::hal::Square_wave_generator0_g;

    template <uint8_t npin>
    using PWM0_pin	 = mega_::hal::PWM0_pin<npin>;

    // Timer1
    // ------
    using Time_counter1  = mega_::hal::Time_counter1;

    template <uint8_t npin>
    using PWM1_pin	 = mega_::hal::PWM1_pin<npin>;

    template <uint8_t npin>
    using SWG1_pin	 = mega_::hal::SWG1_pin<npin>;

    // TODO: borrar estos 2
    using Square_wave_generator1_g = mega_::hal::Square_wave_generator1_g;
    using SWG1           = mega_::hal::Square_wave_generator1_g;

    // Timer2
    // ------
    using Time_counter2  = mega_::hal::Time_counter2;

    template <uint16_t timeout_ms_max>
    using Time_counter2_32kHz_g= mega_::hal::Time_counter2_32kHz_g<timeout_ms_max>;
} // namespace hal

// driver = sinónimo de hal
namespace driver{
    using namespace hal;
}

// PROTOCOLS
// De momento dejo esto fuera de driver. Hoy me gusta así. (mañana...)
    using UART_8bits    = mega_::hal::UART_8bits;

    template <typename Cfg>
    using SPI_master = mega_::hal::SPI_master<Cfg>;

    using SPI_slave  = mega_::hal::SPI_slave;


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


    namespace literals{ using namespace mega_::literals; }

    
//// TIME
//    using Wait_1_us = mega_::Wait_1_us;
//    using Wait_1_ms = mega_::Wait_1_ms;


}// namespace

// Micros equivalentes a nivel de código
namespace atmega328p = atmega;

#endif


