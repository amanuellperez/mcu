// Copyright (C) 2024-2025 Manuel Perez 
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

#ifndef __ATD_BASIC_TYPES_H__
#define __ATD_BASIC_TYPES_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Veamos los builtin types como clases
 *
 * HISTORIA
 *    Manuel Perez
 *    06/10/2024 view_of(int)
 *    08/02/2025 Uninitialized
 *    23/02/2025 Uninitialized_safe
 *
 ****************************************************************************/
#include <limits>

#include "atd_cmath.h"	// atd::abs
#include "atd_concepts.h"

namespace atd{

// View_of_int
// -----------
namespace private_{
template <Type::Integer Int0>
class View_of_int_digit_iterator{
public:
    using Int = std::remove_const_t<Int0>;
    using Digit_type = Int; // TODO: devolver mejor uint16_t o int16_t ?
			    //	    (uint8_t es un char, no un didit :(
			    
    View_of_int_digit_iterator(const Int0& x) : x_{atd::abs(x)} { }
    
    Digit_type operator*();

    void next();
    void operator++() {next();}
    void operator++(int) {next();}

    template <Type::Integer Int2>
    friend bool operator==(const View_of_int_digit_iterator<Int2>&,
	                   const View_of_int_digit_iterator<Int2>&);
private:
    Int x_;   

// Helpers
    static bool is_end(const Int& x)
    {return (x == std::numeric_limits<Int>::max());}

    static void write_end(Int& x)
    {x = std::numeric_limits<Int>::max();}
};


template <Type::Integer Int>
auto View_of_int_digit_iterator<Int>::operator*() -> Digit_type
{
    if (is_end(x_))
	return 0;

    return x_ % Int{10};
}

template <Type::Integer Int>
void View_of_int_digit_iterator<Int>::next()
{
    if (x_ < Int{10})
	write_end(x_);

    else 
	x_ /= Int{10};
}


template <Type::Integer Int>
inline 
bool operator==(const View_of_int_digit_iterator<Int>& a,
	                   const View_of_int_digit_iterator<Int>& b)
{ return a.x_ == b.x_; }

template <Type::Integer Int>
inline 
bool operator!=(const View_of_int_digit_iterator<Int>& a,
	                   const View_of_int_digit_iterator<Int>& b)
{ return !(a == b);}


template <Type::Integer Int>
inline 
View_of_int_digit_iterator<Int> view_of_int_digit_iterator_begin(const Int& x) 
{return View_of_int_digit_iterator<Int>{x};}

template <Type::Integer Int>
inline 
View_of_int_digit_iterator<Int> view_of_int_digit_iterator_end() 
{return View_of_int_digit_iterator<Int>{std::numeric_limits<Int>::max()};}

}// namespace private_
 
// Creamos una view de un int
template <Type::Integer Int>
class View_of_int{
public:
// Types
    using Digit_iterator = private_::View_of_int_digit_iterator<Int>;

// Constructor
    explicit View_of_int(Int& x) : x_{x} {}


// Iteradores por las cifras del número
    Digit_iterator digit_begin() const 
    {return private_::view_of_int_digit_iterator_begin<Int>(x_);}

    Digit_iterator digit_end() const 
    {return private_::view_of_int_digit_iterator_end<Int>();}

private:
    Int& x_;
};



// Uninitialized
// -------------
// Hay veces que interesa posponer la inicialización de una variable, como por
// ejemplo, cuando se quiere cargar un sector de una SDCard en memoria. Opto
// por cargarlo la primera vez que lo necesito.
// Aunque se podía usar std::optional<Int> esa clase duplica el tamaño de Int
// y ando buscando eficiencia. Por eso creo esta clase específica para
// enteros.
// (FUTURO) Se puede dividir esta clase en dos:
//	class Uninitialized : public View_as_class<Int> { ... }
//      donde View_as_class no hace más que concebir las clases básicas
//      (uint8_t, uint16_t, ...) como clases para poder heredar de ellas.
template <Type::Integer Int>
class Uninitialized{
public:
// De momento uso max() como valor para indicar que no se ha inicializado.
// Se podía pasar como parámetro de template
    static constexpr Int uninitialized = std::numeric_limits<Int>::max();

// Constructor
    Uninitialized() : value_ {uninitialized} { }
    Uninitialized(const Int& v) : value_{v} { }

// ¿Tiene valor?
    explicit operator bool() const { return value_ != uninitialized;}
    bool operator!() const {return value_ == uninitialized;}

// Assign
    operator Int() const { return value_;}
    Uninitialized& operator=(const Int& x) { value_ = x; return *this; }

// Operators
    Uninitialized& operator++() {++value_; return *this;}
    Uninitialized operator++(int) 
    {
	Uninitialized res{value_};
	++value_; 
	return res;
    }

    Uninitialized& operator--() {--value_; return *this;}
    Uninitialized operator--(int) 
    {
	Uninitialized res{value_};
	--value_; 
	return res;
    }


    Uninitialized& operator+=(const Int& x)
    { value_ += x; return *this; }

    Uninitialized& operator-=(const Int& x)
    { value_ -= x; return *this; }

    Uninitialized& operator*=(const Int& x)
    { value_ *= x; return *this; }

    Uninitialized& operator/=(const Int& x)
    { value_ /= x; return *this; }

private:
    Int value_;

};

template <Type::Integer Int>
inline Uninitialized<Int> operator+(Uninitialized<Int> a, const Int& b)
{ return a += b; }

template <Type::Integer Int>
inline Uninitialized<Int> operator-(Uninitialized<Int> a, const Int& b)
{ return a -= b; }

template <Type::Integer Int>
inline Uninitialized<Int> operator*(Uninitialized<Int> a, const Int& b)
{ return a *= b; }

template <Type::Integer Int>
inline Uninitialized<Int> operator/(Uninitialized<Int> a, const Int& b)
{ return a /= b; }



// Uninitialized_safe
// ------------------
// Esta clase viene sugerida porque al implementar FAT mientras que los
// ficheros conozco el size en los directorios no. Quiero tener un uint32_t
// con el size para los ficheros pero que no tenga valor para los directorios.
// Podía usar Uninitialized pero el problema es que cada vez que quisiera
// operar con el size tendría que andar mirando si está inicializado o no, y
// fijo voy a olvidar hacerlo. Por ello, delego esa responsabilidad en esta
// clase: puedo operar con el size sin preouparme de si está inicializado o no
// la variable.
//
//  Dos posibles implementaciones:
//	1) En cada operación miramos que la variable esté inicializada:
//		if (value_ != uninitialized) ...
//
//	   De esa forma garantizamos que value_ apunte al valor correcto este
//	   o no inicializada la variable.
//
//	2) Almacenamos otra variable indicando si está inicializada o no. El
//	problema de esto es que usamos más memoria.
//
//  De momento pruebo con la primera.

template <Type::Integer Int>
class Uninitialized_safe{
public:
// De momento uso max() como valor para indicar que no se ha inicializado.
// Se podía pasar como parámetro de template
    static constexpr Int uninitialized = std::numeric_limits<Int>::max();

// Constructor
    Uninitialized_safe() : value_ {uninitialized} { }
    Uninitialized_safe(const Int& v) : value_{v} { }

// ¿Tiene valor?
    explicit operator bool() const { return is_initialized();} 
    bool operator!() const {return !is_initialized();}

// Assign
    // No voy a dejar convertir a Int ya que si se olvida no mirar que no está
    // inicializada sería un error
//    operator Int() const { return value_;}
    // Antes de llamar a value, garantizar que esté inicializada.
    Int value() const {return value_;}

    // Observar que así la inicializamos. Asignarle un valor la deja
    // inicializada. Esto no me permite comparar con Ints, hay que hacerlo
    // explícitamente
    Uninitialized_safe& operator=(const Int& x) { value_ = x; return *this; }

// Operators
    Uninitialized_safe& operator++() 
    {	if (is_initialized()) ++value_; return *this;}

    Uninitialized_safe operator++(int) 
    {
	Uninitialized_safe res{value_};
	if (is_initialized()) ++value_; 
	return res;
    }

    Uninitialized_safe& operator--() 
    { if(is_initialized()) --value_; return *this;}

    Uninitialized_safe operator--(int) 
    {
	Uninitialized_safe res{value_};
	if(is_initialized()) --value_; 
	return res;
    }


    Uninitialized_safe& operator+=(const Int& x)
    { if(is_initialized()) value_ += x; return *this; }

    Uninitialized_safe& operator-=(const Int& x)
    { if(is_initialized()) value_ -= x; return *this; }

    Uninitialized_safe& operator*=(const Int& x)
    { if(is_initialized()) value_ *= x; return *this; }

    Uninitialized_safe& operator/=(const Int& x)
    { if(is_initialized()) value_ /= x; return *this; }

private:
    Int value_;

    bool is_initialized() const { return value_ != uninitialized;}

};

template <Type::Integer Int>
inline Uninitialized_safe<Int> 
	    operator+(Uninitialized_safe<Int> a, const Int& b)
{ return a += b; }

template <Type::Integer Int>
inline Uninitialized_safe<Int> 
	    operator+( const Int& b, Uninitialized_safe<Int> a)
{ return a += b; }

template <Type::Integer Int>
inline Uninitialized_safe<Int> 
	    operator-(Uninitialized_safe<Int> a, const Int& b)
{ return a -= b; }


template <Type::Integer Int>
inline Uninitialized_safe<Int> 
	    operator-(const Int& b, const Uninitialized_safe<Int>& a)
{ return b - a.value(); }

template <Type::Integer Int>
inline Uninitialized_safe<Int> 
	    operator*(Uninitialized_safe<Int> a, const Int& b)
{ return a *= b; }


template <Type::Integer Int>
inline Uninitialized_safe<Int> 
	    operator*(const Int& b, Uninitialized_safe<Int> a)
{ return a *= b; }

template <Type::Integer Int>
inline Uninitialized_safe<Int> 
	    operator/(Uninitialized_safe<Int> a, const Int& b)
{ return a /= b; }

template <Type::Integer Int>
inline Uninitialized_safe<Int> 
	    operator/(const Int& b, const Uninitialized_safe<Int>& a)
{ return b / a.value(); }


}// namespace
 

#endif


