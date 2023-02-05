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
 *
 ****************************************************************************/
#include "avr_time.h"
#include "avr_interrupt.h"
#include "avr_memory.h"

namespace avr_{

/***************************************************************************
 *			    MICRO
 ***************************************************************************/
// (RRR) ¿por qué meter todas estas funciones en una clase?
//       Para poder parametrizar los devices con LCD<Micro, ...>. De esta
//       forma el código será genérico.
struct Micro{
    // interfaz static
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
    inline static void enable_interrupts() {sei();}

    /// Deshabilita el uso de interrupciones
    inline static void disable_interrupts() {cli();}


    // El global_flag_interrupt lo necesito para gestionar
    // Enable_interrupts/Disable_interrupts. 
    using Global_flag_interrupt = unsigned char;

    inline static 
	void global_flag_interrupt(const Global_flag_interrupt& sreg)
    { SREG = sreg; }

    inline static 
	Global_flag_interrupt global_flag_interrupt()
    { return SREG; }

// Progmem
// -------
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
  
} // namespace
#endif
