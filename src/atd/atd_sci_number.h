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

#ifndef __ATD_SCI_NUMBER_H__
#define __ATD_SCI_NUMBER_H__
/****************************************************************************
 *
 * DESCRIPCION
 *    Número con un número fijo de cifras significativas.
 *    Se representa internamente como x*10^n donde `x` es del tipo indicado en
 *    `Rep` (que determina, por tanto, el número de cifras significativas que
 *    se pueden tener).
 *
 * EVOLUCIÓN
 *    Esta clase pretende ser una generalización de la clase atd::Decimal y
 *    atd::ENG_Magnitude.
 *
 * HISTORIA
 *    Manuel Perez
 *    18/10/2023 Versión mínima, para probar.
 *
 ****************************************************************************/
#include <limits>
#include <algorithm>
#include <tuple>    // tie
		    
#include "atd_concepts.h"
#include "atd_math.h"

namespace atd{




/***************************************************************************
 *				Sci_number
 ***************************************************************************/
// Número con un número fijo de cifras significativas.
// ¿Cuántas cifras significativas admite? El tipo Rep nos lo indica.
// Si Rep == uint8_t los números pueden ir de 0 a 255.
// Si Rep == uint16_t pueden ir de 0 a 65535. 
//
template <Type::Integer Rep0, Type::Integer Exp0_t = int>
class Sci_number{
public:
// Types
    using Rep   = Rep0; // Rep define el número de cifras significativas
    using Exp_t = Exp0_t;

// Construction
    constexpr Sci_number() = default;

    // As an integer
    // Example: Sci_number<uint8_t> x{23'000'000};
    template <Type::Integer Int>
    explicit (!(std::is_convertible_v<Rep, Int> and
	        has_same_sign<Rep, Int>))
    constexpr Sci_number(const Int& x);

    // As a decimal number
    // Example: Sci_number<uint8_t> x{12,3}; // x == 12.3
    template <Type::Integer Int>
    constexpr Sci_number(const Int& integer_part, const Int& decimal_part);
	
    // Idioma: para crear el número 5*10^{-4} usamos la E-notation: 5E{-4}
    // En código esto lo traducimos en:
    //		auto x = Sci_number{5}.E(-4);
    constexpr Sci_number& E(Exp_t e) {exp_ = e; return *this;}

// Observers(estas dos funciones son más para depurar, ¿evitar usarlas?)
    constexpr Rep significand() const {return x_;}
    constexpr Exp_t exponent() const { return exp_;}

// Algebraic structure
    constexpr Sci_number operator-() const
	requires (Type::Signed_integer<Rep>);

    constexpr Sci_number& operator+=(const Sci_number& a);
    constexpr Sci_number& operator-=(const Sci_number& a);
    constexpr Sci_number& operator*=(const Sci_number& a);
    constexpr Sci_number& operator/=(const Sci_number& a);

// Order
    constexpr bool operator==(const Sci_number& a) const;
    constexpr bool operator<(const Sci_number& a) const;
 
// Comparison with Int
    // Esta comparación es muy cómoda de usar. Permite escribir cosas 
    // del tipo: if(sci == 230'000) ...
    template <Type::Integer Int>
    constexpr bool operator==(const Int& a) const;

private:
    // Internamente guardamos el número como: x_ * 10^exp
    Rep x_;
    Exp_t exp_;

    // ¿se puede representar usando Rep el valor de x?
    template <Type::Integer Int>
    static
    constexpr bool is_rep_value(const Int& x)
    {
	return std::numeric_limits<Rep>::min() <= x and
	       x <= std::numeric_limits<Rep>::max();
    }

    template <Type::Integer Int>
    static 
    constexpr std::pair<Rep, Exp_t> reduce_to_rep_value(Int x);

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

    constexpr void unsigned_substract(const Sci_number& a)
	requires Type::Unsigned_integer<Rep>;

    static 
    constexpr Rep divide_by_ten_to_the(Rep x, Exp_t e);

};

template <Type::Integer Rep, Type::Integer E_t>
constexpr Rep Sci_number<Rep, E_t>::rep_change_sign(const Rep& x)
{
    if constexpr (Type::Signed_integer<Rep>)
	return -x;
    else 
	return 0; // throw std::logic_error;
}

// Reduce `x` a un valor representable por Rep
// Si no se puede representar devuelve 0 (por ejemplo, -1 no se puede
// representar usando Rep == uint8_t)
template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
constexpr 
std::pair<R, E_t> Sci_number<R, E_t>::reduce_to_rep_value(Int x)
{
    Exp_t exp = 0;
    while (!is_rep_value(x)){
	x /= Int{10};
	++exp;
    }

    if (x == 0) 
	return {0, 0};

    return {static_cast<Rep>(x), exp};
}


// constructor
// -----------
template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
inline constexpr 
void Sci_number<R, E_t>::construct(const Int& integer_part)
{ std::tie(x_, exp_) = reduce_to_rep_value(integer_part); }

// Al calcular ndecimals sumo 1 digits10. Intento aprovechar al máximo el tipo
// Rep. 
// Ejemplo: Al llamar a Sci_number<uint8_t>{0,255}  quiero que 
// Sci_number == 0.255.
// Si no le sumara el 1 a digits10 obtendría Sci_number == 0.25 perdiendo la
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
void Sci_number<R, E_t>::
		construct(const Int& integer_part, const Int& decimal_part)
    // precondition (decimal_part > 0)
{
    int ndecimals = std::min(number_of_digits(decimal_part),
					std::numeric_limits<Rep>::digits10 + 1);
    using Rep2 = same_type_with_double_bits_t<Rep>;
    Rep2 x = abs(integer_part);
    exp_ = 0;

// >>> shift10_to_the_left(x)_at_most(ndecimals)
// {
    int nzeros = 0;
    for(; is_rep_value(x) and nzeros < ndecimals; ++nzeros){
	x *= 10; // shift10_to_the_left<1>(x);
	--exp_;
    }

    if (!is_rep_value(x)){
	x /= 10;
	++exp_;
    }
//} <<< shift10_to_the_left(x)_at_most(ndecimals)

    // Si x == 0, nos podemos pasar ndecimals == digits10 + 1 pudiéndo darse
    // overflow. Por eso garantizo que no se produzca el overflow.
    Rep2 y = x + most_n_significant_digits(-exp_, decimal_part);
    if (is_rep_value(y))
	x = static_cast<Rep>(y);
    else{
	++exp_;
	x += most_n_significant_digits(-exp_, decimal_part);
    }



    if (integer_part <  0)
	x_ = rep_change_sign(x); // x_ = -x_;
				 
    else 
	x_ = x;
}


template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
inline constexpr Sci_number<R, E_t>::Sci_number(const Int& x)
{ construct(x); }


// precondicion: decimal_part > 0
// Si se pasan negativas simplemente se ignoran. Seguramente sea un error de
// lógica y habría que lanzar std::logic_error
template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
constexpr 
Sci_number<R, E_t>::Sci_number(const Int& integer_part, 
			     const Int& decimal_part)
{ 
    if (!is_rep_value(integer_part) or decimal_part <= 0)
	construct(integer_part);

    else
	construct(integer_part, decimal_part);

}

// Order
// -----
// Idea: No basta con comparar signficand() y exponent() ya que no funcionaría
// al comparar los números 20*10^-4 == 2*10^-3
template <Type::Integer R, Type::Integer E_t>
inline
constexpr bool Sci_number<R, E_t>::operator==(const Sci_number<R, E_t>& a) const
{ 
    if (exp_ >= a.exp_)
	return (x_ * ten_to_the<Rep>(exp_ - a.exp_)) == a.x_;

    else
	return (a.x_ * ten_to_the<Rep>(a.exp_ - exp_)) == x_;
}

template <Type::Integer R, Type::Integer E_t>
inline
constexpr bool Sci_number<R, E_t>::operator<(const Sci_number<R, E_t>& a) const
{ 
    if (exp_ < a.exp_)
	return true;

    if (exp_ > a.exp_)
	return false;

    return x_ < a.x_;
}

template <Type::Integer R, Type::Integer E_t>
    template <Type::Integer Int>
inline constexpr bool Sci_number<R, E_t>::operator==(const Int& y) const
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
constexpr bool operator!=(const Sci_number<R, E_t>& a, const Sci_number<R, E_t>& b)
{return !(a == b);}


template <Type::Integer R, Type::Integer E_t, Type::Integer Int>
inline constexpr bool operator!=(const Sci_number<R, E_t>& a, const Int& b)
{ return !(a == b); }

template <Type::Integer R, Type::Integer E_t>
inline constexpr bool operator>(const Sci_number<R, E_t>& a, 
			         const Sci_number<R, E_t>& b)
{ return b < a; }

template <Type::Integer R, Type::Integer E_t>
inline constexpr bool operator<=(const Sci_number<R, E_t>& a, 
			         const Sci_number<R, E_t>& b)
{ return !(a > b); }

template <Type::Integer R, Type::Integer E_t>
inline constexpr bool operator>=(const Sci_number<R, E_t>& a, 
			         const Sci_number<R, E_t>& b)
{ return !(a < b); }


// Algebraic structure
// -------------------
template <Type::Integer R, Type::Integer E_t>
inline constexpr 
Sci_number<R, E_t> Sci_number<R, E_t>::operator-() const
    requires (Type::Signed_integer<R>)
{
    Sci_number res;

    res.x_ = -x_;
    res.exp_ = exp_;

    return res;
}

template <Type::Integer R, Type::Integer E_t>
constexpr void Sci_number<R, E_t>::add_equal_exponent(const Rep& a)
{
    using Int = same_type_with_double_bits_t<Rep>;

    Int res = Int{x_} + Int{a}; // esto puede generar overflow

    if (is_rep_value(res))
	x_ = static_cast<Rep>(res);

    else{
	Exp_t e{};
	std::tie(x_, e) = reduce_to_rep_value(res);
	exp_ += e;
    }
}

// Esta función es necesaria para evitar overflows al usar:
//		x2 /= ten_to_the<Rep>(e1);
template <Type::Integer Rep, Type::Integer E_t>
constexpr Rep Sci_number<Rep, E_t>::divide_by_ten_to_the(Rep x, Exp_t e)
{
    for (Exp_t i = 0; i < e; ++i){
	x /= Rep{10};
	if (x == 0)
	    return 0;
    }

    return x;
}


// precondicion: queremos sumar a x1*10^e1 el número x2
//
// Para ello hacemos:
//	    x1*10^e1 + x2 = (x1 + x2/10^e1)*10^e1
//
// TODO: Creo que se puede optimizar un poco mirando que el producto de
// x1*10^e1 sea menor que numeric_limits::digits10. 
template <Type::Integer R, Type::Integer E_t>
constexpr std::pair<R, E_t> 
    Sci_number<R, E_t>::add_number(const Rep& x1, Exp_t e1, Rep x2)
{
    if (e1 < std::numeric_limits<Rep>::digits10 + 1) 
    { // 10^e1 es representable por Rep
	using Rep2 = same_type_with_double_bits_t<Rep>;
	for (Exp_t e = e1; e > 0; --e){
	    Rep2 y = x1*ten_to_the<Rep2>(e) + x2 / ten_to_the<Rep2>(e1 - e);
	    if (is_rep_value(y))
		return {y, e1 - e};
	}
    }

    x2 = divide_by_ten_to_the(x2, e1);

    return {x1 + x2, e1};
}


template <Type::Integer R, Type::Integer E_t>
constexpr Sci_number<R, E_t>& Sci_number<R, E_t>::operator+=(const Sci_number& a)
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

    return *this;
}



// precondition: *this > a
template <Type::Integer R, Type::Integer E_t>
constexpr void Sci_number<R, E_t>::unsigned_substract(const Sci_number& a)
    requires Type::Unsigned_integer<Rep>
{
    if (exp_ == a.exp_)
	x_ -= a.x_;

    else {// Sabemos que *this > a ==> exp_ > a.exp_
	  // Operación: x1*10^e1 - x2*10^e2 = (x1 - x2/10^(e1-e2))*10^e1
	Rep x2d = divide_by_ten_to_the(a.x_, exp_ - a.exp_);

	if (x_ > x2d)
	    x_ -= x2d;
	else	// underflow (recordar que Rep es unsigned)
	    x_ = 0;

	if (x_ == 0)
	    exp_ = 0;
    }
}

template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Sci_number<R, E_t>& Sci_number<R, E_t>::operator-=(const Sci_number& a)
{
    if constexpr (Type::Signed_integer<Rep>)
	x_ += (-a);

    else{ // if (Unsigned_integer<Rep>)
	if (*this > a)
	    unsigned_substract(a);
	else	// underflow
	    x_ = 0;
    }

    return *this;
}


template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Sci_number<R, E_t>& Sci_number<R, E_t>::operator*=(const Sci_number& a)
{
    using Rep2 = same_type_with_double_bits_t<Rep>;

    Exp_t e{};
    std::tie(x_, e) = reduce_to_rep_value(Rep2{x_}*Rep2{a.x_});

    exp_ = exp_ + a.exp_ + e;

    return *this;
}


// Idea: ¿cómo dividir 1/24?
//	 Multipliquemos y dividamos por la máxima potencia de 10 representable
//	 por Rep:
//	    1/24 = 1000/24 * 10^{-3} = 41 * 10^{-3} = 0.41
//
template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Sci_number<R, E_t>& Sci_number<R, E_t>::operator/=(const Sci_number& a)
{
    if (a.x_ == 0){// TODO: devolver NaNd? otra cosa?
	x_ = 0;
	exp_ = 0;
	return *this;
    }

    using Rep2 = same_type_with_double_bits_t<Rep>;
    
    constexpr int rep_digits10 = std::numeric_limits<Rep>::digits10 + 1;

    Exp_t e{};
    Rep2 d = (Rep2{x_} * ten_to_the<Rep2>(rep_digits10)) / Rep2{a.x_};
    std::tie(x_, e) = reduce_to_rep_value(d);

    exp_ = exp_ - a.exp_ + e - rep_digits10;

    return *this;
}


template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Sci_number<R, E_t> operator+(Sci_number<R, E_t> a, const Sci_number<R, E_t>& b)
{ 
    a += b;
    return a;
}

template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Sci_number<R, E_t> operator-(Sci_number<R, E_t> a, const Sci_number<R, E_t>& b)
{ 
    a -= b;
    return a;
}

template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Sci_number<R, E_t> operator*(Sci_number<R, E_t> a, const Sci_number<R, E_t>& b)
{ 
    a *= b;
    return a;
}

template <Type::Integer R, Type::Integer E_t>
inline constexpr 
    Sci_number<R, E_t> operator/(Sci_number<R, E_t> a, const Sci_number<R, E_t>& b)
{ 
    a /= b;
    return a;
}

// print
// -----
template <typename Out, Type::Integer R, Type::Integer E_t>
void print(Out& out, const Sci_number<R, E_t>& x)
{ 
    using Rep = Sci_number<R, E_t>::Rep;
    if constexpr (sizeof(Rep) == 1) // caso uint8_t/int8_t
	out << (int) x.significand();
    else
	out << x.significand();

    if (x.exponent() != 0)
	out << " x 10^{" << x.exponent() << '}';

}

template <typename Out, Type::Integer R, Type::Integer E_t>
inline 
Out& operator<<(Out& out, const Sci_number<R, E_t>& x)
{
    print(out, x); 
    return out;
}

// is_decimal
// ----------
template <Type::Integer Rep, Type::Integer E_t>
struct is_decimal<Sci_number<Rep, E_t>> : std::true_type {};

}// namespace atd

#endif


