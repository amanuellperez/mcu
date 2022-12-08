// Copyright (C) 2022 A.Manuel L.Perez 
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

#ifndef __AVR_ATMEGA_DEV_H__
#define __AVR_ATMEGA_DEV_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Built-in devices que suministra el atmega
 *  
 *   - HISTORIA:
 *    A.Manuel L.Perez
 *      08/12/2022 Escrito
 *
 ****************************************************************************/
/***************************************************************************
 *		    BUILT-IN DEVICES FOR ATMEGA
 ***************************************************************************/
//namespace avr{ // TODO: change to atmega
//using namespace avr_;
//
//}// namespace

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
    using Timer0_generic_counter = avr_::Timer0_generic_counter;

    using Timer1         = avr_::Timer1;
    using Timer1_generic = avr_::Timer1_generic;
    using Timer1_generic_counter = avr_::Timer1_generic_counter;


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
    using Interrupt = avr_::Interrupt;
    using Power     = avr_::Power;


// PROGMEM
    template <typename Int>
    using Progmem = avr_::Progmem<Int>;

    template <typename T, size_t N>
    using Progmem_array = avr_::Progmem_array<T, N>;

    template <size_t N>
    using Progmem_string = avr_::Progmem_string<N>;

    template <size_t N>
    using Element_progmem_string_array = avr_::Element_progmem_string_array<N>;

    template <size_t N>
    using Progmem_string_array = avr_::Progmem_string_array<N>;

    template <typename T, size_t N>
    using Progmem_array_view = avr_::Progmem_array_view<T,N>;


    template <typename T, size_t nrows, size_t ncols>
    using Progmem_biarray_view = avr_::Progmem_biarray_view<T, nrows, ncols>;

    namespace literals{ using namespace avr_::literals; }

/***************************************************************************
 *			    FUNCTIONS
 ***************************************************************************/
    // El parámetro lo usamos para sobrecargar: quiero que configures el flujo
    // con la configuración básica.
    template <uint32_t baud_rate = 9600u,
	      uint32_t f_clock   = MCU_CLOCK_FREQUENCY_IN_HZ,
	      uint32_t tolerance = 2>
    inline void basic_cfg(avr_::UART_iostream&) 
    { return avr_::UART_iostream_basic_cfg<baud_rate, f_clock, tolerance>(); }


    /// Habilita el uso de interrupciones. Además, recordar habilitar
    /// cada interrupción por separado.
    inline void enable_interrupts() {sei();}

    /// Deshabilita el uso de interrupciones
    inline void disable_interrupts() {cli();}


    // TODO: ¿cómo importar directamente estas funciones?
    // using ... ???
    // Manejamos el array como memoria normal y corriente
    template <size_t N>
    inline char* strcpy(char* dst, const Element_progmem_string_array<N>& src)
    { return avr_::strcpy(dst, src); }

    // Es más eficiente y segura strlcpy que strncpy
    template <size_t N>
    inline char*
    strncpy(char* dst, const Element_progmem_string_array<N>& src, size_t n)
    { return avr_::strncpy(dst, src, n); }

    template <size_t N>
    inline size_t
    strlcpy(char* dst, const Element_progmem_string_array<N>& src, size_t n)
    { return avr_::strlcpy(dst, src, n); }

    inline int bytes_of_free_ram()  {return avr_::bytes_of_free_ram(); }

}// namespace



#endif


