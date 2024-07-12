// Copyright (C) 2023 Manuel Perez 
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

#ifndef __ATD_MINIFLOAT_H__
#define __ATD_MINIFLOAT_H__
/****************************************************************************
 *
 * DESCRIPCION
 *    Número con un número fijo de cifras significativas.
 *
 *    Se representa internamente como x*10^n donde `x` es del tipo indicado en
 *    `Rep` (que determina, por tanto, el número de cifras significativas que
 *    se pueden tener).
 *
 * OBJETIVO
 *    Pretende ser una versión light de 'float'. 
 *    Como el atmega no opera con floats en hardware, operar con floats hace
 *    que los programas aumenten de tamaño. 
 *    Por ello creo esta clase: como sustituto de float, con la idea de que
 *    sea más pequeño.
 *    Al poder elegir el tipo interno usado realmente estaría creando un
 *    montón diferentes de floats:
 *	using float8   = Minifloat<int8_t>;
 *	using ufloat8  = Minifloat<uint8_t>;
 *	using float16  = Minifloat<int16_t>
 *	using ufloat16 = Minifloat<uint16_t>
 *	...
 *    
 *    Por supuesto, que hay que medir para ver si realmente estoy consiguiendo
 *    mi objetivo. 
 *
 * EVOLUCIÓN
 *    Esta clase pretende ser una generalización de la clase atd::Decimal y
 *    atd::ENG_Magnitude.
 *
 * IDEA
 *    Toda la implementación se basa en revisar que no haya overflow. 
 *
 * ¿CÓMO MEJORARLA?
 *  1. Experimento a probar: 
 *     Si se manejan en una misma aplicación diferentes tipos de Float 
 *     ( uFloat8, Float16...) se está generando una clase diferente con todos
 *     los métodos correspondientes duplicando el código mucho.
 *     Quizás sería interesante hacer las funciones para uint32_t/int32_t y el
 *     resto que se limiten a llamar a estas funciones.
 *     Ejemplo:
 *	Implementamos print_as_decimal(out, Minifloat<uint32_t, int>);
 *	Y las funciones particulares serían algo parecido a:
 *	    print_as_decimal(out, Minifloat<Rep, E_t> f)
 *	    {
 *		if constexpr (std::is_unsigned_v<Rep>)
 *		    print_as_decimal(out, Minifloat<uint32_t, int>(f));
 *		else
 *		    print_as_decimal(out, Minifloat<int32_t, int>(f));
 *	    }
 *
 *	De esta forma evitaríamos duplicar código. ¿Disminuirá el código así?
 *	Habría que intentar que fuese eficiente el paso de Minifloat<uint8_t>
 *	a Minifloat<uint32_t> y viceversa.
 *
 *
 *  2. Revisar código y ver qué cosas se pueden sacar de la clase y no son
 *     templates.
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    18/10/2023 Versión mínima, para probar.
 *    27/06/2024 integer_part
 *
 ****************************************************************************/
#include <limits>
#include <algorithm>
#include <type_traits>	// common_type
#include <tuple>	// std::tie
			
#include "atd_concepts.h"
#include "atd_math.h"
#include "atd_ostream.h"	// write_as_int
#include "atd_type_traits.h"	// always_false
namespace atd{

// Funciones genéricas
// -------------------
// TODO: no usarlas con este nombre en otros sitios.
// Realmente estas funciones lo que garantizan es que no haya
// overflow/underflow. `is_rep_value` es un nombre muy criptico. Elegir uno
// mejor.
// ¿se puede representar usando Rep el valor de x?
template <Type::Integer Int2, Type::Integer Int>
inline constexpr bool is_rep_value(const Int& x)
{
    return !overflow<Int2>(x);
}


// TODO: esta función la estoy usando ya
template <Type::Integer Int>
constexpr 
std::pair<Int, uint8_t> shift10_n_to_the_left(uint8_t n, const Int& x0)
{
    using Int2 = same_type_with_double_bits_t<Int>;

    Int2 x = x0;
    uint8_t ndigits = 0;
    while (ndigits < n and is_rep_value<Int>(x)){
	x *= Int2{10};
	++ndigits;
    }

    if (!is_rep_value<Int>(x)){
	x /= Int2{10};
	--ndigits;
    }

    return {static_cast<Int>(x), ndigits};

}

/***************************************************************************
 *				Minifloat
 ***************************************************************************/
// Número con un número fijo de cifras significativas.
// ¿Cuántas cifras significativas admite? El tipo Rep nos lo indica.
// Si Rep == uint8_t los números pueden ir de 0 a 255.
// Si Rep == uint16_t pueden ir de 0 a 65535. 
//
template <Type::Integer Rep0, Type::Integer Exp0_t = int>
class Minifloat{
public:
// Types
    using Rep   = Rep0; // Rep define el número de cifras significativas
    using Exp_t = Exp0_t;

// Construction
    constexpr Minifloat() = default;

    // As an integer
    // Example: Minifloat<uint8_t> x{23'000'000};
    template <Type::Integer Int>
    explicit (!(std::is_convertible_v<Rep, Int> and
	        has_same_sign_v<Rep, Int>))
    constexpr Minifloat(const Int& x);

    // As a decimal number
    // Example: Minifloat<uint8_t> x{12,3}; // x == 12.3
    template <Type::Integer Int>
    constexpr Minifloat(const Int& integer_part, const Int& decimal_part);
	
    // Conversión entre minifloats
    template <Type::Integer Rep2, Type::Integer E2_t>
    constexpr Minifloat(const Minifloat<Rep2, E2_t>& f)
	requires (std::is_convertible_v<Rep2, Rep> and
		  sizeof(Rep2) <= sizeof(Rep) and
		  std::is_convertible_v<E2_t, Exp_t>);

    // Idioma: para crear el número 5*10^{-4} usamos la E-notation: 5E{-4}
    // En código esto lo traducimos en:
    //		auto x = Minifloat{5}.E(-4);
    //
    // Esta función realmente lo que hace es añadir al exponente `e`.
    // Es equivalente a: multiply_by_ten_to_the_power_of(e)
    constexpr Minifloat& E(Exp_t e) 
    {
	if (x_ != 0) exp_ += e; // si el número es 0, su exponente es 0!!!
	return *this;
    }

// Observers(estas dos funciones son más para depurar, ¿evitar usarlas?)
    constexpr Rep significand() const {return x_;}
    constexpr Exp_t exponent() const { return exp_;}

    // CUIDADO: integer_part devuelve la parte entera siempre y cuando esta
    // entre en Rep2. Es responsabilidad del cliente garantizar que esa parte
    // entra. 
    // Si el número tiene parte decimal, trunca.
    template <Type::Integer Rep2 = Rep>
    constexpr Rep2 integer_part() const;

//    template <Type::Integer Rep2>
//    constexpr Rep decimal_part() const;

// Algebraic structure
    constexpr Minifloat operator-() const
	requires (Type::Signed_integer<Rep>);

    constexpr Minifloat& operator+=(const Minifloat& a);
    constexpr Minifloat& operator-=(const Minifloat& a);
    constexpr Minifloat& operator*=(const Minifloat& a);
    constexpr Minifloat& operator/=(const Minifloat& a);

// Order
    constexpr bool operator==(const Minifloat& a) const;
    constexpr bool operator<(const Minifloat& a) const
	requires (std::is_signed_v<Rep>);

    constexpr bool operator<(const Minifloat& a) const
	requires (std::is_unsigned_v<Rep>);
 
// Comparison with Int
    // Esta comparación es muy cómoda de usar. Permite escribir cosas 
    // del tipo: if(sci == 230'000) ...
    template <Type::Integer Int>
    constexpr bool operator==(const Int& a) const;

    // Escribe el número normalizado.
    // De momento, esta forma normalizada es escribir el número sin ningún
    // cero a la derecha, todos escritos dentro del exponente.
    // Ejemplo: en vez de guardar 250*10^0, guardamos 25*10^1, de tal forma
    // que el número es lo más pequeño posible (observar que no respetamos el
    // número de cifras significativas)
    constexpr void normalize();

    // Devuelve el valor absoluto 
    constexpr Minifloat abs() const { return Minifloat{atd::abs(x_)}.E(exp_); }
    
private:
    // Internamente guardamos el número como: x_ * 10^exp
    Rep x_;
    Exp_t exp_;


    template <Type::Integer Int>
    static 
    constexpr std::pair<Rep, Exp_t> reduce_to_rep_value(Int x);

    template <Type::Integer Int>
    static 
    constexpr std::pair<Rep, Exp_t> reduce_right_zeros(Int x, Exp_t exp);

    static 
    constexpr Rep rep_change_sign(const Rep& x);


    template <Type::Integer Int>
    constexpr void construct(const Int& integer_part);

    template <Type::Integer Int>
    constexpr void construct(const Int& integer_part, const Int& decimal_part);


    constexpr void add_equal_exponent(const Rep& a);

    static
    constexpr 
    std::pair<Rep, Exp_t> add_number(const Rep& x1, Exp_t e1, Rep x2);

    constexpr void unsigned_substract(const Minifloat& a)
	requires Type::Unsigned_integer<Rep>;

    static constexpr Rep 
	truncate___with_exponent___(const Minifloat& a, Exp_t exp);

};

template <Type::Integer Rep, Type::Integer E_t>
constexpr Rep Minifloat<Rep, E_t>::rep_change_sign(const Rep& x)
{
    if constexpr (Type::Signed_integer<Rep>)
	return -x;
    else 
	return 0; // throw std::logic_error;
}

template <Type::Integer R, Type::Integer E_t>
constexpr 
void Minifloat<R, E_t>::normalize()
{ std::tie(x_, exp_) = reduce_right_zeros(x_, exp_); }

// Reduce `x` a un valor representable por Rep
// Si no se puede representar devuelve 0 (por ejemplo, -1 no se puede
// representar usando Rep == uint8_t)
template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
constexpr 
std::pair<R, E_t> Minifloat<R, E_t>::reduce_to_rep_value(Int x)
{
    Exp_t exp = 0;
    while (!is_rep_value<Rep>(x)){
	x /= Int{10};
	++exp;
    }

    std::tie(x, exp) = reduce_right_zeros(x, exp);
//    if (x == 0)
//	return {0, 0};

    return {static_cast<Rep>(x), exp};
}

template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
constexpr 
std::pair<R, E_t> Minifloat<R, E_t>::reduce_right_zeros(Int x, Exp_t exp)
{
    if (x == 0)
	return {0, 0};

    while ((x % 10) == 0){
	x /= 10;
	++exp;
    }

    return {static_cast<Rep>(x), exp};
}


// constructor
// -----------
template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
inline constexpr 
void Minifloat<R, E_t>::construct(const Int& integer_part)
{ std::tie(x_, exp_) = reduce_to_rep_value(integer_part); }

// Al calcular ndecimals sumo 1 digits10. Intento aprovechar al máximo el tipo
// Rep. 
// Ejemplo: Al llamar a Minifloat<uint8_t>{0,255}  quiero que 
// Minifloat == 0.255.
// Si no le sumara el 1 a digits10 obtendría Minifloat == 0.25 perdiendo la
// tercera cifra decimal que sí se puede representar en un uint8_t
// precondicion: is_rep_value(integer_part)
//
// Idea de la implementación: si integer_part = 123 y decimal_part = 45
// el signficand lo calculo haciendo: 123*100 + 45
// El problema es que este número tiene que entrar en Rep. Por ello, en lugar
// de multiplicar por 100 directamente voy poco a poco:
// (1) 123*10 ¿entra en Rep? sí entra x = 123*10 = 1230
// (2) si entra sigo: 1230*10 ¿entra en Rep? ...
// correspon
template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
constexpr 
void Minifloat<R, E_t>::
		construct(const Int& integer_part, const Int& decimal_part)
    // precondition (decimal_part > 0)
{
    int ndecimals = std::min(number_of_digits(decimal_part),
					std::numeric_limits<Rep>::digits10 + 1);
    using Rep2 = same_type_with_double_bits_t<Rep>;
    Rep2 x = atd::abs(integer_part);
    exp_ = 0;

// >>> shift10_to_the_left(x)_at_most(ndecimals)
// {
    int nzeros = 0;
    for(; is_rep_value<Rep>(x) and nzeros < ndecimals; ++nzeros){
	x *= 10; // shift10_to_the_left<1>(x);
	--exp_;
    }

    if (!is_rep_value<Rep>(x)){
	x /= 10;
	++exp_;
    }
//} <<< shift10_to_the_left(x)_at_most(ndecimals)

    // Si x == 0, nos podemos pasar ndecimals == digits10 + 1 pudiéndo darse
    // overflow. Por eso garantizo que no se produzca el overflow.
    Rep2 y = x + most_n_significant_digits(-exp_, decimal_part);
    if (is_rep_value<Rep>(y))
	x = static_cast<Rep>(y);
    else{
	++exp_;
	x += most_n_significant_digits(-exp_, decimal_part);
    }


    if (integer_part <  0)
	x_ = rep_change_sign(x); // x_ = -x_;
				 
    else 
	x_ = x;

    normalize();
}


template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
inline constexpr Minifloat<R, E_t>::Minifloat(const Int& x)
{ construct(x); }


// precondicion: decimal_part > 0
// Si se pasan negativas simplemente se ignoran. Seguramente sea un error de
// lógica y habría que lanzar std::logic_error
template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
constexpr 
Minifloat<R, E_t>::Minifloat(const Int& integer_part, 
			     const Int& decimal_part)
{ 
    if (!is_rep_value<Rep>(integer_part) or decimal_part <= 0)
	construct(integer_part);

    else
	construct(integer_part, decimal_part);
}


template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Rep2, Type::Integer E2_t>
inline 
constexpr Minifloat<R, E_t>::Minifloat(const Minifloat<Rep2, E2_t>& f)
	requires (std::is_convertible_v<Rep2, Rep> and
		  sizeof(Rep2) <= sizeof(Rep) and
		  std::is_convertible_v<E2_t, Exp_t>)
	: x_{f.significand()}, exp_{f.exponent()}
{ }


// Observers
// ---------
template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Rep2>
constexpr Rep2 Minifloat<R, E_t>::integer_part() const
{
    Rep2 res = static_cast<Rep>(x_);

    if (exp_ == 0) 
	return res;

    if (exp_ > 0)
	return multiply(res).by_ten_to_the_power_of(exp_);

    else // exp_ == 0
	return divide(res).by_ten_to_the_power_of(-exp_);
}



// Order
// -----
template <Type::Integer R, Type::Integer E_t>
constexpr bool Minifloat<R, E_t>::operator==(const Minifloat<R, E_t>& a) const
{ 
    if (exp_ >= a.exp_)
	return (x_ * ten_to_the<Rep>(exp_ - a.exp_)) == a.x_;

    else
	return (a.x_ * ten_to_the<Rep>(a.exp_ - exp_)) == x_;
}


template <Type::Integer R, Type::Integer E_t>
constexpr bool Minifloat<R, E_t>::operator<(const Minifloat<R, E_t>& a) const
    requires (std::is_unsigned_v<Rep>)
{ 
    if (exp_ < a.exp_)
	return true;

    if (exp_ > a.exp_)
	return false;

    return x_ < a.x_;
}

// Escribe el número a*10^exp_ = ap * 10^exp, siendo exp > exp_, devolviendo
// el valor de ap.
//
//  Ejemplo: 56.6 = 566E-1 = 5E1 = 50
//	     Observar que hemos aproximado 56.6 a 50, por ello el nombre de
//	     truncar.
//	     En este caso: a = 566E-1 y exp = 1
template <Type::Integer Rep, Type::Integer E_t>
constexpr Rep 
Minifloat<Rep, E_t>::
    truncate___with_exponent___(const Minifloat<Rep, E_t>& a, Exp_t exp)
{
    if (a.exp_ >= exp) return a.x_; // precondition: exp_ <= exp

    return divide(a.x_).by_ten_to_the_power_of(exp - a.exp_);
}

template <Type::Integer R, Type::Integer E_t>
constexpr bool Minifloat<R, E_t>::operator<(const Minifloat<R, E_t>& a) const
    requires (std::is_signed_v<Rep>)
{ 
    int sx = sign_of(x_);
    int sa = sign_of(a.x_);

    if (sx > 0 and sa <= 0) return false;
    if (sx < 0 and sa >= 0) return true;
    if (sx == 0) return a.x_ > 0;

    if (sx > 0){
	if (exp_ == a.exp_) 
		return x_ < a.x_;
    
	// exp_ != a.exp_
	if (exp_ > a.exp_){
	    auto a2 = truncate___with_exponent___(a, exp_);
	    return x_ <= a2; // si x_ == a2 ==> x_ < a ya que al truncar
			     //	hacemos que a2 sea menor que a
	}
	// else
	auto x2 = truncate___with_exponent___(*this, a.exp_);
	return x2 < a.x_; // si x2 == a.x_ ==> x_ > a.x_
    }

//    else if (sx < 0){
	// los 2 números negativos:
	if (exp_ == a.exp_) 
		return x_ < a.x_;

	// exp_ != a.exp_
	if (exp_ > a.exp_){
	    auto a2 = truncate___with_exponent___(a, exp_);
	    return x_ < a2; // si x_ == a2 ==> x_ < a ya que al truncar
			     //	hacemos que a2 sea menor que a
	}
	// else
	auto x2 = truncate___with_exponent___(*this, a.exp_);
	return x2 <= a.x_; // si x2 == a.x_ ==> x_ > a.x_

 //   }

}

template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
constexpr bool Minifloat<R, E_t>::operator==(const Int& y) const
{
    if (exp_ >= 0){
	Int res = x_ * ten_to_the<Int>(exp_);
	return y == res;
    }

    else 
	return x_ == (y * ten_to_the<Int>(-exp_));
}

template <Type::Integer R, Type::Integer E_t>
inline
constexpr bool operator!=(const Minifloat<R, E_t>& a, const Minifloat<R, E_t>& b)
{return !(a == b);}


template <Type::Integer R, Type::Integer E_t, Type::Integer Int>
inline constexpr bool operator!=(const Minifloat<R, E_t>& a, const Int& b)
{ return !(a == b); }

template <Type::Integer R, Type::Integer E_t>
inline constexpr bool operator>(const Minifloat<R, E_t>& a, 
			         const Minifloat<R, E_t>& b)
{ return b < a; }

template <Type::Integer R, Type::Integer E_t>
inline constexpr bool operator<=(const Minifloat<R, E_t>& a, 
			         const Minifloat<R, E_t>& b)
{ return !(a > b); }

template <Type::Integer R, Type::Integer E_t>
inline constexpr bool operator>=(const Minifloat<R, E_t>& a, 
			         const Minifloat<R, E_t>& b)
{ return !(a < b); }


// Algebraic structure
// -------------------
template <Type::Integer R, Type::Integer E_t>
constexpr 
Minifloat<R, E_t> Minifloat<R,E_t>::operator-() const
    requires (Type::Signed_integer<R>)
{
    Minifloat res;

    res.x_ = -x_;
    res.exp_ = exp_;

    return res;
}

template <Type::Integer R, Type::Integer E_t>
constexpr void Minifloat<R, E_t>::add_equal_exponent(const Rep& a)
{
    using Int = same_type_with_double_bits_t<Rep>;

    Int res = Int{x_} + Int{a}; // esto puede generar overflow

    if (is_rep_value<Rep>(res))
	x_ = static_cast<Rep>(res);

    else{
	Exp_t e{};
	std::tie(x_, e) = reduce_to_rep_value(res);
	exp_ += e;
    }
}



// precondicion: queremos sumar a x1*10^e1 el número x2
//
// Primera idea: 
//	Hacer x1*10^e1 + x2 = (x1 + x2/10^e1)*10^e1
//
// Problema: 2*10^2 + 3 = (2 + 3/100)*100 = 200!!! Pero tendría que ser 203!!!
//
// Para evitar este problema modificamos la primera idea:
//	si podemos multiplicar 2*10^2 y es representable en Rep, lo hacemos, y
//	luego añadimos 3.
//	si no podemos hacer la multiplicación volvemos a la primera idea.
//
// TODO: Creo que se puede optimizar un poco mirando que el producto de
// x1*10^e1 sea menor que numeric_limits::digits10. 
template <Type::Integer R, Type::Integer E_t>
constexpr std::pair<R, E_t> 
    Minifloat<R, E_t>::add_number(const Rep& x1, E_t e1, Rep x2)
{
    if (x1 == 0)
	return {x2, 0};

    if (e1 < std::numeric_limits<Rep>::digits10 + 1) 
    { // 10^e1 es representable por Rep
	using Rep2 = same_type_with_double_bits_t<Rep>;
	for (Exp_t e = e1; e > 0; --e){
	    Rep2 y = x1*ten_to_the<Rep2>(e) + x2 / ten_to_the<Rep2>(e1 - e);
	    if (is_rep_value<Rep>(y))
		return {static_cast<Rep>(y), static_cast<Exp_t>(e1 - e)};
	}
    }

    x2 = divide(x2).by_ten_to_the_power_of(e1); 

    if (is_rep_value<Rep>(x1 + x2))
	return {static_cast<Rep>(x1 + x2), e1};

    else    // DUDA: ¿se da este caso? Ej: x1 = 255, x2 = 1 ==> overflow!!!
	return {std::numeric_limits<Rep>::max(), e1};
}


template <Type::Integer R, Type::Integer E_t>
constexpr Minifloat<R, E_t>& Minifloat<R, E_t>::operator+=(const Minifloat& a)
{
    if (exp_ == a.exp_)
	add_equal_exponent(a.x_);

    else if (exp_ > a.exp_){// x1*10^e1 + x2*10^e2 = (x1*10^(e1-e2) + x2)*10^e2
	Exp_t e{};
	std::tie(x_, e) = add_number(x_, exp_ - a.exp_, a.x_);
	exp_ = e + a.exp_;
    }

    else{
	Exp_t e{};
	std::tie(x_, e) = add_number(a.x_, a.exp_ - exp_, x_);
	exp_ = e + exp_;
    }

    normalize();

    return *this;
}



// precondition: *this > a
template <Type::Integer R, Type::Integer E_t>
constexpr void Minifloat<R, E_t>::unsigned_substract(const Minifloat& a)
    requires Type::Unsigned_integer<Rep>
{

//  Garantizamos que exp_ <= a.exp_ 
    while (exp_ > a.exp_ and is_rep_value<Rep>(x_ * 10)){ 
	x_ *= Rep{10};
	--exp_;
    }

// Caso trivial
    if (exp_ == a.exp_)
	x_ -= a.x_;

// Resto casos: precondicion: exp_ <= a.exp_
    else {// Sabemos que *this > a ==> exp_ > a.exp_
	  // Operación: x1*10^e1 - x2*10^e2 = (x1 - x2/10^(e1-e2))*10^e1
//	Rep x2d = divide_by_ten_to_the(a.x_, exp_ - a.exp_);
	Rep x2d = divide(a.x_).by_ten_to_the_power_of(exp_ - a.exp_);
	if (x_ > x2d)
	    x_ -= x2d;
	else	// underflow (recordar que Rep es unsigned)
	    x_ = 0;

	if (x_ == 0)
	    exp_ = 0;
    }
}

template <Type::Integer R, Type::Integer E_t>
constexpr 
    Minifloat<R, E_t>& Minifloat<R, E_t>::operator-=(const Minifloat& a)
{
    if constexpr (Type::Signed_integer<Rep>)
	x_ += (-a);

    else{ // if (Unsigned_integer<Rep>)
	if (*this > a)
	    unsigned_substract(a);
	else	// underflow
	    x_ = 0;
    }

    normalize();

    return *this;
}


template <Type::Integer R, Type::Integer E_t>
constexpr 
    Minifloat<R, E_t>& Minifloat<R, E_t>::operator*=(const Minifloat& a)
{
    using Rep2 = same_type_with_double_bits_t<Rep>;

    Exp_t e{};
    std::tie(x_, e) = reduce_to_rep_value(Rep2{x_}*Rep2{a.x_});

    exp_ = exp_ + a.exp_ + e;

    normalize();

    return *this;
}




// Idea: ¿cómo dividir 1/24?
//	 Multipliquemos y dividamos por la máxima potencia de 10 representable
//	 por Rep:
//	    1/24 = 1000/24 * 10^{-3} = 41 * 10^{-3} = 0.41
//
template <Type::Integer R, Type::Integer E_t>
constexpr 
Minifloat<R, E_t>& Minifloat<R, E_t>::operator/=(const Minifloat& a)
{
    if (a.x_ == 0){// TODO: devolver NaNd? otra cosa?
	x_ = 0;
	exp_ = 0;
	return *this;
    }

//    if (x_ == 0) // El algoritmo implementado devuelve 0*10^{-10} si x == 0!!
//	return *this;

    using Rep2 = same_type_with_double_bits_t<Rep>;
    
    int rep_digits10 = std::numeric_limits<Rep>::digits10 + 1;


    auto [d, n] = shift10_n_to_the_left<Rep2>(rep_digits10, x_);
    d /= a.x_;

    Exp_t e{};
    std::tie(x_, e) = reduce_to_rep_value(d);

    exp_ = exp_ - a.exp_ + e - n;

    normalize();

    return *this;
}



template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Minifloat<R, E_t> operator+(Minifloat<R, E_t> a, const Minifloat<R, E_t>& b)
{ 
    a += b;
    return a;
}

template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Minifloat<R, E_t> operator-(Minifloat<R, E_t> a, const Minifloat<R, E_t>& b)
{ 
    a -= b;
    return a;
}

template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Minifloat<R, E_t> operator*(Minifloat<R, E_t> a, const Minifloat<R, E_t>& b)
{ 
    a *= b;
    return a;
}

template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Minifloat<R, E_t> operator/(Minifloat<R, E_t> a, const Minifloat<R, E_t>& b)
{ 
    a /= b;
    return a;
}


// print
// -----
// Imprime como potencia de 10: x*10^n
template <typename Out, Type::Integer R, Type::Integer E_t>
void print_as_power_of_ten(Out& out, const Minifloat<R, E_t>& x)
{ 
    out << write_as_int(x.significand());

    if (x.exponent() != 0)
	out << " x 10^{" << write_as_int(x.exponent()) << '}';

}

template <typename Out, Type::Integer Rep, Type::Integer E_t>
void print_as_decimal_positive_exponent(Out& out, const Minifloat<Rep, E_t>& f)
{
    out << write_as_int(f.significand());

    for (E_t i = 0; i < f.exponent(); ++i)
	out << '0';
}


// Idea: descomponer el número de `n` cifras en `ndigits + ndecimals`
// donde `ndigits` da el número de cifras enteras que tiene.
template <typename Out, Type::Integer Rep, Type::Integer E_t>
void print_as_decimal_negative_exponent(Out& out, const Minifloat<Rep, E_t>& f)
{
    auto x = f.significand();

    if constexpr (std::is_signed_v<Rep>){
	if (x < 0){
	    out << '-';
	    x = -x;
	}
    }

    E_t n = number_of_digits(x);

    E_t ndecimals = -f.exponent();
    E_t ndigits   = n > ndecimals? (n - ndecimals): 0;
    
    for (E_t i = 0; i < ndigits; ++i, --n){ 
	auto digit{x / ten_to_the<Rep>(n - 1)}; // digit = most_significant_digit(x);
	out << write_as_int(digit);
	x -= digit * ten_to_the<Rep>(n - 1);   // remove_most_significant_digit(x);
    }


    if (ndigits == 0)   // TODO: los americanos no escriben el 0. (???)
	out << '0';

    out << '.'; // TODO: meter esto en atd_locale.h

    n = number_of_digits(x);
    while (n < ndecimals){
	out << '0';
	--ndecimals;
    }

    for (E_t i = 0; i < ndecimals; ++i, --n){ // TODO: idéntico al bucle anterior. Meterlo en función. Pero ¿qué nombre?
	auto digit{x / ten_to_the<Rep>(n - 1)}; // digit = most_significant_digit(x);
	out << write_as_int(digit);
	x -= digit * ten_to_the<Rep>(n - 1);   // remove_most_significant_digit(x);
    }
}

// Imprime el número como decimal. 
// De momento, para probar solo lo imprime como decimal si tiene 3 cifras
// decimales como máximo o 3 ceros.
// Ejemplos: 
//	    123*10^{-2} = 1.23
//	    123*10^{3}  = 123000
template <typename Out, Type::Integer Rep, Type::Integer E_t>
void print_as_decimal(Out& out, const Minifloat<Rep, E_t>& f)
{
    if (f.exponent() < -5 or f.exponent() > 5)
	print_as_power_of_ten(out, f);

    else if (f.exponent() >= 0)
	print_as_decimal_positive_exponent(out, f);

    else 
	print_as_decimal_negative_exponent(out, f);

}


template <typename Out, Type::Integer R, Type::Integer E_t>
inline void print(Out& out, const Minifloat<R, E_t>& x)
{
    print_as_decimal(out, x);
}

template <typename Out, Type::Integer R, Type::Integer E_t>
inline 
Out& operator<<(Out& out, const Minifloat<R, E_t>& x)
{
    print(out, x); 
    return out;
}

// is_decimal
// ----------
template <Type::Integer Rep, Type::Integer E_t>
struct is_decimal<Minifloat<Rep, E_t>> : std::true_type {};


// is_minifloat
// ------------
template <typename T>
struct is_minifloat: public std::false_type { };

template <Type::Integer Rep, Type::Integer E_t>
struct is_minifloat<Minifloat<Rep, E_t>> : public std::true_type { };

template <typename T>
inline constexpr bool is_minifloat_v = is_minifloat<T>::value;


// minifloat_cast
// --------------
template <typename To, Type::Integer Rep1, Type::Integer E1_t>
    requires (is_minifloat_v<To>)
constexpr To minifloat_cast(const Minifloat<Rep1, E1_t>& f)
{ 
    using Rep2 = typename To::Rep;

    // Caso: std::is_convertible_v<Rep1, Rep2> and sizeof(Rep1) >= sizeof(Rep2)
    if constexpr (requires{To{f};})
	return To{f}; 

    // sizeof(Rep1) < sizeof(Rep2) ==> se van a truncar cifras significativas
    else if constexpr (std::is_convertible_v<Rep1, Rep2>){
	if (f.significand() == 0)
	    return To{0};

	To res = f.significand();
	res.E(f.exponent()); // res *= ten_to_the(f.exponent());
	return res;
    }

    else
	static_assert(atd::always_false_v<To>, 
		    "Can't convert Minifloat::Rep1 to Minifloat::Rep2");

}





// Alias
// -----
// Voy a usar int8_t como exponente para intentar que sean más pequeños.
using Float8   = Minifloat<int8_t, int8_t>;
using uFloat8  = Minifloat<uint8_t, int8_t>;
using Float16  = Minifloat<int16_t, int8_t>;
using uFloat16 = Minifloat<uint16_t, int8_t>;
using Float32  = Minifloat<int32_t, int8_t>;
using uFloat32 = Minifloat<uint32_t, int8_t>;


}// namespace atd


// common_type
// -----------
template <Type::Integer Rep1, Type::Integer E1_t,
	  Type::Integer Rep2, Type::Integer E2_t>
struct std::common_type<atd::Minifloat<Rep1, E1_t>, atd::Minifloat<Rep2, E2_t>>{

    using Rep = std::common_type_t<Rep1, Rep2>;
    using E_t = std::common_type_t<E1_t, E2_t>;

    using type = atd::Minifloat<Rep, E_t>;
};

#endif


