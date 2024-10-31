// Copyright (C) 2022-2024 Manuel Perez 
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

#ifndef __AVR_MICRO_H__
#define __AVR_MICRO_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones globales para todos los micros del tipo avr_
 *
 * HISTORIA
 *    Manuel Perez
 *    12/12/2022 Escrito
 *    27/10/2024 Generalizado para que valga para atmega328p y atmega4809
 *
 ****************************************************************************/
#include "avr_interrupt.h"
#include "avr_memory.h"
#include "avr_time.h"

namespace avr_{

/***************************************************************************
 *			    MICRO
 ***************************************************************************/
namespace protected_{
// (RRR) ¿por qué meter todas estas funciones en una clase?
//       Para poder parametrizar los devices con LCD<Micro, ...>. De esta
//       forma el código será genérico.
struct Micro{
    // Static interface
    Micro() = delete;


// Time
// ----
    template <std::integral Int>
    inline static constexpr void wait_us(Int t) {avr_::wait_us(t);}

    template <std::integral Int>
    inline static constexpr void wait_ms(Int t) {avr_::wait_ms(t);}


// Interrupts
// ----------
    /// Habilita el uso de interrupciones. Además, recordar habilitar
    /// cada interrupción por separado.
    inline static void enable_interrupts() {avr_::enable_interrupts();}

    /// Deshabilita el uso de interrupciones
    inline static void disable_interrupts() {avr_::disable_interrupts();}

    using Enable_interrupts  = avr_::Enable_interrupts;
    using Disable_interrupts = avr_::Disable_interrupts;


// ROM
// ---
    template <typename T>
    inline static
    constexpr T rom_read(const T& x) { return avr_::rom_read(x);}

    using ROM_read     = avr_::ROM_read;

    using ROM_uint8_t  = avr_::ROM_uint8_t;
    using ROM_uint16_t = avr_::ROM_uint16_t;

    template <typename T, size_t N, typename Read = ROM_read>
    using ROM_array = avr_::ROM_array<T, N, Read>;

    template <size_t N>
    using ROM_string = avr_::ROM_string<N>;

    template <size_t N>
    using Element_ROM_string_array = avr_::Element_ROM_string_array<N>;

    template <size_t N>
    using ROM_string_array = avr_::ROM_string_array<N>;

    // TODO: ¿cómo importar directamente estas funciones?
    // using ... ???
    // Manejamos el array como memoria normal y corriente
    template <size_t N>
    inline static char* strcpy(char* dst, const Element_progmem_string_array<N>& src)
    { return avr_::strcpy(dst, src); }


    // Es más eficiente y segura strlcpy que strncpy
    template <size_t N>
    inline static char*
    strncpy(char* dst, const Element_progmem_string_array<N>& src, size_t n)
    { return avr_::strncpy(dst, src, n); }

    template <size_t N>
    inline static size_t
    strlcpy(char* dst, const Element_progmem_string_array<N>& src, size_t n)
    { return avr_::strlcpy(dst, src, n); }


// RAM
// ---
    inline static int bytes_of_free_ram()  {return avr_::bytes_of_free_ram(); }
};// Micro
}// protected_ 


} // namespace

#endif
