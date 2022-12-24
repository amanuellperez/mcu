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

#ifndef __ATD_ITERATOR_H__
#define __ATD_ITERATOR_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Diferentes iteradores.
 *
 * HISTORIA
 *    A.Manuel L.Perez
 *    24/12/2022 Progmem_iterator, versión mínima
 *
 ****************************************************************************/

namespace atd{

// DESCRIPCION
//	
//	Algunos arrays, como los Progmem_array, solo suministran el operator[]
//  para iterar no disponiendo de iteradores nativos (no tenemos un puntero al
//  primer elemento ya que para acceder a PROGMEM hay que usar las macros de
//  avr-gcc).
//
//	Esta clase permite convertir el operator[] en el iterador
//  correspondiente.
//
// DUDA: ¿Qué nombre ponerle?
//	 De momento lo llamo Progmem_iterator para indicar que se basa en las
//  características de un array PROGMEM:
//	(1) suministra operator[]
//	(2) No suministra punteros. No puedo tener un puntero a una región de
//	    memoria PROGMEM (salvo usando las macros de avr-gcc).
//
template <typename Array>
class Progmem_iterator{
public:
// Types
    using value_type = typename Array::value_type;
    using reference  = value_type; // por ser PROGMEM es const!!!
    using size_type  = typename Array::size_type;

// Constructors
    Progmem_iterator() : data{nullptr}, i{0}  { }
    Progmem_iterator(const Array& data0, size_type i0) : data{&data0}, i{i0} { }

// Functions
    reference operator*() const { return (*data)[i]; }
    void operator++() { ++i; }
    void operator--() { --i; }

    friend
    bool operator==(const Progmem_iterator& a, const Progmem_iterator& b)
    { return a.data == b.data and a.i == b.i; }

    friend
    bool operator!=(const Progmem_iterator& a, const Progmem_iterator& b)
    { return !(a == b); }

private:
// Data
    const Array* data;
    size_type i;    // elemento al que apunta este iterador
};




}// namespace

#endif


