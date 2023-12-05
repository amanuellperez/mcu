// Copyright (C) 2020-2022 Manuel Perez 
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
#ifndef __ATD_MEMORY_H__
#define __ATD_MEMORY_H__

/****************************************************************************
 *
 *  DESCRIPCION
 *	Cosas relacionadas con memoria.
 *
 *  HISTORIA:
 *    Manuel Perez
 *    08/04/2020 Memory_type
 *    25/12/2022 Progmem and Progmem_array
 *    05/12/2023 Opto por pasar como parámetro la función Read
 *               (antes se basaba en la función progmem_read que tenía que
 *               estar definida globalmente)
 *               Cambio nombre de Progmem a ROM (es más general y refleja el
 *               hecho de que se trata de memoria de solo lectura).
 *
 ****************************************************************************/
#include "atd_iterator.h"
#include <cstddef>

namespace atd{

// Memory_type
// -----------
enum class Memory_type{
    read = 1,
    write = 2,
    read_only = read,
    write_only = write, 
    read_and_write = read | write
};

inline constexpr bool is_readable(atd::Memory_type mem)
{
    return mem == atd::Memory_type::read or
           mem == atd::Memory_type::read_and_write;
}

inline constexpr bool is_writeable(atd::Memory_type mem)
{
    return mem == atd::Memory_type::write or
           mem == atd::Memory_type::read_and_write;
}


/***************************************************************************
 *				PROGMEM
 *  
 *  La Harvard architecture tiene dos zonas de memoria: la de datos y la de
 *  programa. Como los de avr llaman a esta progmem llámemosla así.
 *
 *  Al implementar estas clases en avr me encontré varios problemas:
 *	(1) El concept no funciona, ya que busca progmem_read(a) en el
 *	    namespace avr_ y no en el namespace que esté usando el cliente.
 *
 *	(2) El operator[] de ROM_array tampoco funciona ya que busca la
 *	    función avr_::progmem_read!!! 
 *
 *  Como se ve, todo es un problema con los namespaces. 
 *
 *  La forma que propongo ahora de resolver esto es generalizarlo. Al revisar
 *  el código resulta que la única función externa que uso es progmem_read
 *  siendo esa la única función que tiene que definir el cliente.
 *
 *  Dos formas de pasarla a la clase ROM_array:
 *	(1) Parametrizándola con un Driver, que tendría funciones read/write
 *	    en memoria. 
 *
 *	(2) Meter las funciones que tiene que definir el cliente en el
 *	    namespace atd.
 *
 *  Entre las dos puede que quede mejor la (2): ROM<Micro>. El problema e
 *  que para implementarla ahora hay que incluir el Micro en todos los devices
 *  implementados que usen ROM. Aunque a partir de ahora voy a ir
 *  añadiendo Device<Micro> en todos los devices hasta que no lo tenga
 *  reescrito no tengo acceso al Micro. Por ello, opto por implementar (1) y
 *  en el futuro, si se ve que queda mejor (2) que (1), migrarlo a (2).
 *
 ***************************************************************************/
// One element
// -----------
template <typename T, typename Read>
class ROM{
public:
    constexpr ROM(const T& x): x_{x} { }
    operator T() const { 
	Read read;
	return read(x_); 
    }

    template<typename T2>
    ROM& operator=(const T2&) = delete;

private:
    const T x_;
};


// Arrays
// ------
template <typename T, size_t N, typename Read>
class ROM_array{
public:
// Types
    using value_type = T;
    using size_type  = size_t;
    using iterator   = ROM_iterator<ROM_array>;

// Constructos
    template<typename T2>
    ROM_array& operator=(const T2&) = delete;


// Observers
// DUDA: este es un array clásico, siempre está lleno. Sería un Full_array
//	 o Array_full. No necesito capacity(), ni empty() ni full(). ¿Dejarlas
//	 o quitarlas? A favor de dejarlas: mismo interfaz para todos los
//	 arrays full o no. En contra: interfaz más sencillo si se eliminan.
    constexpr static size_type size() {return N;}
    constexpr static size_type capacity() {return size();}

    constexpr static bool empty() {return size() == 0;}
    constexpr static bool full () {return size() == capacity();}


// Element access
    const T operator[](size_type i) const {
	Read read;
	return read(data[i]); 
    }

// Iterators
    iterator begin() const {return iterator{*this, 0};}
    iterator end() const {return iterator{*this, size()};}
    

// Data
    // Al principio iba a definir data como 'private'. Sin embargo, si se
    // define como private da error al compilar (ya que no inicializa por
    // defecto el data). Definiéndolo como const evito el problema de
    // escritura. El usuario tiene que saber que no debe de leer directamente
    // 'data'.
    const T data[N];
};


}// namespace

#endif


