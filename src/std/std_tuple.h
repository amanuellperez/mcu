// Copyright (C) 2020 Manuel Perez 
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

#ifndef __MCU_STD_TUPLE_H__
#define __MCU_STD_TUPLE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: El equivalente a <tuple>
 *
 *  - HISTORIA:
 *    Manuel Perez 
 *	22/02/2020: ignore, implementación MINIMA de tuple para que funcione
 *		    std::tie. 
 *		    TODO: implementar tuple. De hecho conviene revisar todos
 *		    los requirements (no implemento ninguno) y todo lo demás.
 *		    De momento solo estoy interesado en que funcione std::tie.
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_iterator.h"
#include "std_utility.h"    // tuple_element

namespace STD{
// La forma de implementar tuple es usando una jerarquía de clases recursiva:
//
//                    __Tdata<1, string>        __T<2>
//                         ^                       ^
//                         |                       |
//                         +------------+----------+
//                                      |
// __Tdata<0,int>                 __T<1,string>
//      ^                               ^
//      |                               |
//      +-----------+-------------------+
//                  |
//          _T<0, int, string>
//
// Los __Tdata son los que contienen el dato.

// Dato
template <size_t i, typename T>
struct __Tuple_data{
    T value_;
};

template <size_t i, typename... T>
struct __Tuple;

// Base case
template <size_t i>
struct __Tuple<i> {};


// Construcción de la jerarquía de clases
template <size_t i, typename T0, typename... T1_n>
struct __Tuple<i, T0, T1_n...> :
    private __Tuple_data<i, T0>,
    public __Tuple<i + 1, T1_n...> 
{ 
    using Data = __Tuple_data<i, T0>;
    using Base = __Tuple<i + 1, T1_n...>;
    
// construction
    explicit 
    constexpr __Tuple(const T0& x0, const T1_n&... x1_n)
	: Data{x0}, Base{x1_n...} { }

    constexpr __Tuple(const __Tuple&) = default;
    constexpr __Tuple(__Tuple&& t): Base(move(__base(t))),
				    Data(forward<T0>(t)) { }



// conversión
    // convertimos la referencia t a la referencia de la base, para poder
    // iterar sobre la jerarquía.
    static constexpr Base& __base(__Tuple& t) noexcept {return t;}
    static constexpr const Base& __base(const __Tuple& t) noexcept {return t;}

// data
// DUDA: gcc define estas funciones como static, ¿por qué?
    constexpr T0& __value() noexcept
    { return Data::value_; }

    constexpr const T0& __value() const noexcept
    { return Data::value_; }


protected:
// operator=
    template <typename... U0_n>
    void __assign(const __Tuple<i, U0_n...>& t)
    {
	__value() = t.__value();
	Base(*this).__assign(__Tuple<i, U0_n...>::Base(t));
    }

    template <typename U0, typename... U1_n>
    void __assign(const __Tuple<i, U0, U1_n...>&& t)
    {
	__value() = forward<U0>(t.__value());
	Base(*this).__assign(move(__Tuple<i, U0, U1_n...>::Base(t)));
    }
};




// tuple
// -----
template <typename... T0_n>
class tuple : public __Tuple<0, T0_n...>
{
    using __Tuple_base = __Tuple<0, T0_n...>;

public:
    // TODO: falta poner que sea 'conditionally explicit'
    constexpr tuple(const T0_n&... args) : __Tuple_base(args...) { }

    tuple(const tuple&) = default;
    tuple(tuple&&)      = default;


    // TODO: faltan requirements, leer el standard. (hacerlo cuando tenga los
    // concepts)
    tuple& operator=(const tuple<T0_n...>& u)
    {
	__assign(u);
	return *this;
    }

    // TODO: faltan requirements, leer el standard. (hacerlo cuando tenga los
    // concepts)
    tuple& operator=(const tuple<T0_n...>&& u)
    {
	__assign(move(u));
	return *this;
    }


    // TODO: faltan requirements, leer el standard. (hacerlo cuando tenga los
    // concepts)
    template <typename... U0_n>
    tuple& operator=(const tuple<U0_n...>& u)
    {
	__assign(u);
	return *this;
    }

    // TODO: faltan requirements, leer el standard. (hacerlo cuando tenga los
    // concepts)
    template <typename... U0_n>
    tuple& operator=(const tuple<U0_n...>&& u)
    {
	__assign(move(u));
	return *this;
    }
};


// gcc incluye esta partial specialization para convertir
// pair en tuples y viceversa.
template <typename T1, typename T2>
class tuple<T1, T2> : public __Tuple<0, T1, T2>
{
    using __Tuple_base = __Tuple<0, T1, T2>;

public:
    // TODO: falta poner que sea 'conditionally explicit'
    constexpr tuple(const T1& x1, const T2& x2) : __Tuple_base(x1, x2) { }

    tuple(const tuple&) = default;
    tuple(tuple&&)      = default;

    // TODO: faltan requirements, leer el standard. (hacerlo cuando tenga los
    // concepts)
    template <typename U1, typename U2>
    tuple(const pair<U1, U2>& p) : __Tuple_base(p.first, p.second) { }

    template <typename U1, typename U2>
    tuple(pair<U1, U2>&& p)
        : __Tuple_base(forward<U1>(p.first), forward<U2>(p.second))
    { }


    // TODO: faltan requirements, leer el standard. (hacerlo cuando tenga los
    // concepts)
    template <typename U1, typename U2>
    tuple& operator=(const pair<U1, U2>& p)
    { // DUDA: sin this-> el compilador genera un error. ¿Por qué no sabe
      // a quien pertenece value()?
	this->__value() = p.first;
	this->__base(*this).__value() = p.second;

	return *this;
    }

    // TODO: faltan requirements, leer el standard. (hacerlo cuando tenga los
    // concepts)
    template <typename U1, typename U2>
    tuple& operator=(pair<U1, U2>&& p)
    {
	this->__value() = forward<U1>(p.first);
	this->__base(*this).__value() = forward<U2>(p.second);

	return *this;
    }

};


// tuple_element
// -------------

// tuple_element<i, __Tuple<j, T0, T1_n...>>:
//  
//  if (i == j) return T0;
//  else return tuple_element<i, __Tuple<j + 1, T1_n...>>;
//
//  NOTA: gcc implementa esto de forma muy diferente usando herencia.
template <size_t i, typename T>
struct __tuple_element;

template <size_t i, typename T0, typename... T1_n>
struct __tuple_element<i, __Tuple<i, T0, T1_n...>>
{
    using type = T0;
};

template <size_t i, size_t j, typename T0, typename... T1_n>
struct __tuple_element<i, __Tuple<j, T0, T1_n...>>
{
    using type = typename __tuple_element<i, __Tuple<j + 1, T1_n...>>::type;
};

// TODO: añadir gestión de errores. Si se llama a tuple con un indice
// demasiado grande genera el compilador un monton de errores ininteligibles.

template <size_t i, typename T>
struct tuple_element;

template <size_t i, typename... T0_n>
struct tuple_element<i, tuple<T0_n...>>
    : public __tuple_element<i, __Tuple<0, T0_n...>> 
{ };



// Element access
// --------------
//  DUDA: gcc implementa esto de forma muy diferente. De hecho, a día de hoy
//  no entiendo cómo funciona lo de gcc. 
//  Lo que hago es simplemente navegar por la jerarquía buscando el índice i.
template <size_t i, size_t j, typename T0, typename... T1_n>
constexpr auto&
__get(__Tuple<j, T0, T1_n...>& t) noexcept
{ 
    if constexpr (i == j) 
	return t.__value();

    else
	return __get<i>(static_cast<__Tuple<j+1, T1_n...>&>(t));
}


template <size_t i, size_t j, typename T0, typename... T1_n>
constexpr const auto&
__get(const __Tuple<j, T0, T1_n...>& t) noexcept
{ 
    if constexpr (i == j) 
	return t.__value();

    else
	return __get<i>(static_cast<const __Tuple<j+1, T1_n...>&>(t));
}


template <size_t i, typename... T0_n>
constexpr tuple_element_t<i, tuple<T0_n...>>&
get(tuple<T0_n...>& t) noexcept
{
    return __get<i, 0, T0_n...>(t);
}


template <size_t i, typename... T0_n>
constexpr const tuple_element_t<i, tuple<T0_n...>>&
get(const tuple<T0_n...>& t) noexcept
{
    return __get<i, 0, T0_n...>(t);
}



// ignore
// ------
struct __Ignore{
    template <typename T>
    constexpr const __Ignore& operator=(const T&) const
    { return *this; }
};

inline constexpr __Ignore ignore;


// tie
// ---
template <typename... T0_n>
constexpr tuple<T0_n&...> tie(T0_n&... args) noexcept
{ return tuple<T0_n&...>(args...);}



}// namespace

#endif

