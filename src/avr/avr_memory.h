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

#ifndef __AVR_MEMORY_H__
#define __AVR_MEMORY_H__
/****************************************************************************
// *
 *  - DESCRIPCION: Funciones para acceder a la memoria: RAM, flash, eeprom
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    16/01/2022 v0.0
 *
 ****************************************************************************/
#include <avr/pgmspace.h>   

#include <type_traits>
#include <initializer_list>
#include <atd_type_traits.h>

namespace avr{

// Indica el número de bytes de RAM libres (para ver si se está usando
// demasiada memoria).
// Ver avr-libc-user-manual-2.0.0, punto 3.
// Esta función es copia de internet.
// No puedo incluirla dentro del namespace avr. Da error (???)
inline int bytes_of_free_ram() 
{
  extern int __heap_start; 
  extern int* __brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

/***************************************************************************
 *				PROGMEM
 ***************************************************************************/
template <typename Int>
class Progmem{
public:
    // TODO: implementar otros tipos (lo marca la función pgm_read_xxx)
    static_assert(std::is_same_v<Int, uint8_t> or std::is_same_v<Int, uint16_t>);

    constexpr Progmem(const Int& x): x_{x} { }
    operator Int() const
    {   
	if constexpr (std::is_same_v<Int, uint8_t>)
	    return pgm_read_byte(&x_);

	else if constexpr (std::is_same_v<Int, uint16_t>)
	    return pgm_read_word(&x_);

	else
	    static_assert(atd::always_false_v<int>, "error");

    }

    template<typename Int2>
    Progmem& operator=(const Int2&) = delete;

private:
    const Int x_;
};


template <typename T, size_t N>
class Progmem_array{
public:
    using size_type = size_t;


    // TODO: implementar otros tipos
    static_assert(std::is_same_v<T, uint8_t> or std::is_same_v<T, uint16_t>);

    constexpr size_type size() const noexcept 
    {return N;}

    T operator[](size_type i) const
    {   
	if constexpr (std::is_same_v<T, uint8_t>)
	    return pgm_read_byte(&data[i]);

	else if constexpr (std::is_same_v<T, uint16_t>)
	    return pgm_read_word(&data[i]);

	else
	    static_assert(atd::always_false_v<int>, "error");

    }

    template<typename T2>
    Progmem_array& operator=(const T2&) = delete;

    // Al principio iba a definir data como 'private'. Sin embargo, si se
    // define como private da error al compilar (ya que no inicializa por
    // defecto el data). Definiéndolo como const evito el problema de
    // escritura. El usuario tiene que saber que no debe de leer directamente
    // 'data'.
    const T data[N];
};



// Para manejar arrays de cadenas.
template <size_t N>
class Progmem_string_array{
public:
    using size_type = size_t;

    constexpr size_type size() const noexcept 
    {return N;}

    template<typename T2>
    Progmem_string_array& operator=(const T2&) = delete;

    char* strcpy(char* dst, size_type i) const
    { return strcpy_P(dst, (PGM_P) pgm_read_word(&(data[i]))); }

    const char* const data[N];

};




}// namespace
#endif


