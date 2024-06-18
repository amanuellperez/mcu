// Copyright (C) 2024 Manuel Perez 
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

#ifndef __ATD_PERCENTAGE_H__
#define __ATD_PERCENTAGE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Manejo de tantos por cien.
 *
 *	Por defecto no manejamos decimales en el atmega, amén de que la gente
 *	no matemática concibe los tantos por cien como si fueran números
 *	naturales. Esta clase facilita eso.
 *
 * HISTORIA
 *    Manuel Perez
 *    04/06/2024 Escrito
 *    15/06/2024 Parts_per<1024>
 *
 ****************************************************************************/
#include <cstdint>
#include <ostream>

#include "atd_type_traits.h"

namespace atd{

/***************************************************************************
 *				PERCENTAGE
 ***************************************************************************/
// Es un Percentage, esto es, una parte de cada 100.
// Lo voy a concebir con este significado lo cual quiere decir que no son
// válidos tantos por cien del tipo 200% o cosas raras de ese tipo. 
// Lo siento, pero si tienes 100 caramelos no puedes coger 200 de cada 100.
// ¡Es absurdo!
//
// ¿Qué operaciones se pueden hacer con los tantos por cien?
// En general no hay definida un álgebra para los tantos por cien: si te hacen
// un descuento primero del 20% y luego uno del 10% a más, no te están
// haciendo un descuento de 20 + 10 = 30%. Eso es erróneo. Por ello, no defino
// ningún tipo de álgebra.
//
class Percentage{
public:
    // precondition (0 <= x <= 100)
    constexpr Percentage(uint8_t x);


    // Calcula el tanto por cien del número x
    template <std::integral Int> // TODO: demasiado restringido, esto
				 // vale para todo número (tb floats)
				 // Pero ¿cuál es el concept de número
				 // estandar? :?
    constexpr Int of(const Int& x) const;

    // Acceso de bajo nivel. ¿cómo evitar esta función?
    constexpr uint8_t as_uint() const { return value_;};

    // print
    friend std::ostream& operator<<(std::ostream& out, const Percentage p);

private:
    uint8_t value_;

};


inline
constexpr Percentage::Percentage(uint8_t x)
    : value_{x}
{
    if (value_ > 100)
	value_ = 0; // qué error?
}


// (RRR) ¿por qué complico la implementación?
//	 La primera implementación fue:
//		{ return (x * static_cast<Int>(value_)) / Int{100}; }
//
//	¿dará overflow?
//	Al probar:
//	    uint16_t t = 50'000;
//	    atd::Percentage p{50};
//	    std::cout << p.of(t) << '\n';
//	en el ordenador funciona correctamente, sin embargo, en el micro
//	devuelve p.of(t) == 96!!!
//
//	Por ello me veo obligado a complicarla.
template <std::integral Int>
inline 
constexpr Int Percentage::of(const Int& x) const
{
    using Int2 = same_type_with_double_bits_t<Int>;
    Int2 res = (Int2{x} * Int2{value_}) / Int2{100};
    return static_cast<Int>(res);
}


inline
constexpr bool operator==(const Percentage& x, const Percentage& y)
{ return x.as_uint() == y.as_uint();}

inline
constexpr bool operator!=(const Percentage& x, const Percentage& y)
{ return !(x == y);}


inline std::ostream& operator<<(std::ostream& out, const Percentage p)
{
    return out << static_cast<int>(p.value_) << " %";
}


/***************************************************************************
 *			    PARTS_PER
 ***************************************************************************/
// (RRR) ¿Para qué necesito esta clase?
//	 Al escribir el driver del ADC lo que realmente devolvemos es un tanto
//	 por cien. El problema es que el micro devuelve el tanto por 1024, en
//	 vez de por 100.  
//	 Por una parte no quiero perder precisión: la clase anterior
//	 Percentage no admite tantos por 1024; por otra quiero que sea muy
//	 eficiente almacenando lo mínimo posible.
//
//	 Podría implementar la clase Percentage<atd::Ratio> donde Ratio sería
//	 una clase de números racionales, pero esa clase almacenaría 2 números
//	 en vez de uno solo como hace ahora Percentage. Si defino
//	 Parts_per<1024> el denominador no tengo que almacenarlo
//	 (dinámicamente). Espero (?) que esta implementación sea más eficaz
//	 que Percentage<atd::Ratio> aunque a lo mejor no merece la pena.
//
//	 Por ello, probemos a implementarla, usémosla y el uso dira si es util
//	 o no.
template <uint32_t denominator0>
class Parts_per{
public:
// Types
    using Rep = least_uint_t_to_represent_t<denominator0>;

    // precondition (0 <= x <= denominator0)
    constexpr Parts_per(const Rep& x);


    // Calcula el tanto por cien del número x
    template <std::integral Int> // TODO: demasiado restringido, esto
				 // vale para todo número (tb floats)
				 // Pero ¿cuál es el concept de número
				 // estandar? :?
    constexpr Int of(const Int& x) const;

// Acceso de bajo nivel. ¿cómo evitar esta función?
    constexpr Rep as_uint() const { return num();};
    constexpr Rep num() const {return num_;}
    constexpr Rep den() const {return den_;}

private:
    Rep num_;
    static constexpr Rep den_ = denominator0;
};

template <uint32_t D>
inline
constexpr Parts_per<D>::Parts_per(const Rep& x)
    : num_{x}
{
    if (num_ > den_)
	num_ = 0; // qué error?
}


// (RRR) ¿por qué complico la implementación?
//	 La primera implementación fue:
//		{ return (x * static_cast<Int>(value_)) / Int{100}; }
//
//	¿dará overflow?
//	Al probar:
//	    uint16_t t = 50'000;
//	    atd::Parts_per p{50};
//	    std::cout << p.of(t) << '\n';
//	en el ordenador funciona correctamente, sin embargo, en el micro
//	devuelve p.of(t) == 96!!!
//
//	Por ello me veo obligado a complicarla.
template <uint32_t D>
template <std::integral Int>
inline 
constexpr Int Parts_per<D>::of(const Int& x) const
{
    using Int2 = same_type_with_double_bits_t<Int>;
    Int2 res = (Int2{x} * Int2{num_}) / Int2{den_};
    return static_cast<Int>(res);
}


template <uint32_t D>
inline
constexpr bool operator==(const Parts_per<D>& x, const Parts_per<D>& y)
{ return x.as_uint() == y.as_uint();}

template <uint32_t D>
inline
constexpr bool operator!=(const Parts_per<D>& x, const Parts_per<D>& y)
{ return !(x == y);}


template <uint32_t D>
inline std::ostream& operator<<(std::ostream& out, const Parts_per<D> p)
{
    return out << static_cast<int>(p.num()) << " out of " << p.den();
}

}// namespace



#endif


