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
#ifndef __ATD_ROM_H__
#define __ATD_ROM_H__

/****************************************************************************
 *
 *  DESCRIPCION
 *	Manejo de la ROM (progmem en el avr)
 *  
 *  TODO
 *	Los contenedores definidos aquí son similares a los definidos en alp
 *	(todo el código relacionado con alp::Matrix). La diferencia entre lo
 *	de aquí y alp es que aquí los datos están en la ROM y son constantes,
 *	mientras que en alp estan en el heap y todo es dinámico.
 *	¿Se pueden fusionar? ¿organizar? 
 *
 *  HISTORIA:
 *    Manuel Perez
 *    25/12/2022 Progmem and Progmem_array
 *    05/12/2023 Opto por pasar como parámetro la función Read
 *               (antes se basaba en la función progmem_read que tenía que
 *               estar definida globalmente)
 *               Cambio nombre de Progmem a ROM (es más general y refleja el
 *               hecho de que se trata de memoria de solo lectura).
 *    12/08/2024 ROM_biarray	
 *
 ****************************************************************************/
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
 *  Entre las dos puede que quede mejor la (2): ROM<Micro>. El problema es
 *  que para implementarla ahora hay que incluir el Micro en todos los devices
 *  implementados que usen ROM. Aunque a partir de ahora voy a ir
 *  añadiendo Device<Micro> en todos los devices hasta que no lo tenga
 *  reescrito no tengo acceso al Micro. Por ello, opto por implementar (1) y
 *  en el futuro, si se ve que queda mejor (2) que (1), migrarlo a (2).
 *
 ***************************************************************************/
#include "atd_iterator.h"
#include <cstddef>

namespace atd{
/***************************************************************************
 *				ROM views
 ***************************************************************************/
template <typename Container>
class ROM_subcontainer{
public:
    using value_type	 = typename Container::value_type;
    using size_type	 = typename Container::size_type;
    using iterator	 = ROM_iterator<ROM_subcontainer>;
    using const_iterator = iterator;


// Constructor
    ROM_subcontainer() : c_{nullptr}, i0_{0}, ie_{i0_} {}

    // pre: i0 <= ie
    ROM_subcontainer(const Container& c, size_type i0, size_type ie)
	    : c_{&c}, i0_{i0}, ie_{ie} { }


// Dimensions
    constexpr size_type size() {return ie_ - i0_;}
    constexpr size_type capacity() {return size();}

// Info
    constexpr bool empty() {return size() == 0;}
    constexpr bool full () {return size() == capacity();}


// Element access
    // pre: i0_ + i < ie_
    const value_type operator[](size_type i) const { return  (*c_)[i0_ + i];}

// Iterators
    const_iterator begin() {return const_iterator{*this, 0};}
    const_iterator end() {return const_iterator{*this, size()};}

    const_iterator begin() const {return const_iterator{*this, 0};}
    const_iterator end() const {return const_iterator{*this, size()};}

private:
// Data
    // El subcontainer es c_[i0..ie)
    const Container* c_;
    size_type i0_, ie_; 
};


/***************************************************************************
 *				    ROM
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
    using value_type	 = T;
    using size_type	 = size_t;
    using iterator	 = ROM_iterator<ROM_array>;
    using const_iterator = iterator;

// Constructor
    template<typename T2>
    ROM_array& operator=(const T2&) = delete;


// Dimensions
// DUDA: este es un array clásico, siempre está lleno. Sería un Full_array
//	 o Array_full. No necesito capacity(), ni empty() ni full(). ¿Dejarlas
//	 o quitarlas? A favor de dejarlas: mismo interfaz para todos los
//	 arrays full o no. En contra: interfaz más sencillo si se eliminan.
    constexpr static size_type size() {return N;}
    constexpr static size_type capacity() {return size();}

// Info
    constexpr static bool empty() {return size() == 0;}
    constexpr static bool full () {return size() == capacity();}


// Element access
    const T operator[](size_type i) const {
	Read read;
	return read(data[i]); 
    }

// Iterators
    const_iterator begin() const {return const_iterator{*this, 0};}
    const_iterator end() const {return const_iterator{*this, size()};}
    

// Data
    // Al principio iba a definir data como 'private'. Sin embargo, si se
    // define como private da error al compilar (ya que no inicializa por
    // defecto el data). Definiéndolo como const evito el problema de
    // escritura. El usuario tiene que saber que no debe de leer directamente
    // 'data'.
    const T data[N];
};


// ROM_biarray
// -----------
// Debería ser el equivalente a alp::Matrix con la diferencia de que
// alp::Matrix es dinámico mientras que este es static (esta en la ROM)
template <typename T, size_t nrows, size_t ncols, typename Read>
class ROM_biarray{
public:
// Types
    using value_type = T;
    using size_type  = size_t;
    using Ind        = size_t; // DUDA: pasarlo como parámetro
    using iterator   = ROM_iterator<ROM_biarray>;
    using const_iterator   = iterator;
    
    using const_Row = ROM_subcontainer<ROM_biarray>;

// Constructor
    template<typename T2>
    ROM_biarray& operator=(const T2&) = delete;


// Dimensions
    constexpr static size_type rows() { return nrows;}
    constexpr static size_type cols() { return ncols;}

    constexpr static size_type size() {return rows() * cols();}
    constexpr static size_type capacity() {return size();}

// Info
    constexpr static bool empty() {return size() == 0;}
    constexpr static bool full () {return size() == capacity();}


// Acceso como array unidimensional
    const T operator[](size_type i) const {
	Read read;
	return read(data[i]); 
    }

// Acceso como array bidimensional
    const T operator()(Ind i, Ind j) const {
	Read read;
	return read(data[index(i, j)]); 
    }

    const_Row row(Ind i) const
	{ return const_Row{*this, index(i, 0), index(i + 1, 0)};}

// Unidimensional access
    const_iterator begin() const {return const_iterator{*this, 0};}
    const_iterator end() const {return const_iterator{*this, size()};}
    
// DUDA: en Matrix tengo acceso por filas, ¿merece la pena añadirlo?
//       De momento esto es una prueba, escribamos lo mínimo.

// Data
    // Al principio iba a definir data como 'private'. Sin embargo, si se
    // define como private da error al compilar (ya que no inicializa por
    // defecto el data). Definiéndolo como const evito el problema de
    // escritura. El usuario tiene que saber que no debe de leer directamente
    // 'data'.
    const T data[size()];

//private:
    size_type index(Ind i, Ind j) const { return i * cols() + j; }
};


}// namespace

#endif


