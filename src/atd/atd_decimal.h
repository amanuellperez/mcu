// Copyright (C) 2020-2023 Manuel Perez 
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
#ifndef __ATD_DECIMAL_H__
#define __ATD_DECIMAL_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Clase para manejar números decimales con un número fijo de
 *	cifras.
 *
 *  - SEE: ver std::chrono, ver atd::magnitude, tienen la misma estructura.
 *
 * TODO: Sustituir esta clase a favor de Minifloat(???)
 *       No lo tengo muy claro. De momento con la primera implementación
 *       usar Minifloat aumenta el datalogger 1kB. Es más eficiente Decimal.
 *       ¿Dejar los 2?
 *       CUIDADO: ¿la diferencia estará en la función print? Revisar. Medir.
 *
 *  - TODO:
 *    ¿Cómo construir el Decimal<3> x con valor 3.001?
 *    Si escribo Decimal<3> x{3,1}, este es "3.1", pero para escribir "3.001"
 *    tendría que poder escribir Decimal<3> x{3, 001} lo cual significa
 *    "3.(octal 01)" diferente en general de "3.001". 
 *    Como esto es algo que no creo que use, y siempre se puede definir
 *    Decimal<3>::significand(3001); de momento me olvido de este
 *    problema. 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    04/03/2020 v0.0: Versión mínima (muy limitada, para probarla).
 *    05/02/2021       to_integer.
 *    08/02/2021       Había definido explicit el constructor. Sin embargo,
 *                     un 'int' es un 'Decimal<int>' con parte fraccionaria
 *                     igual a 0. Sí tiene sentido que se pueda pasar de int a
 *                     Decimal implícitamente. El recíproco no ya que perdería
 *                     decimales.
 *                     De esta forma puedo hacer: Decimal<int> + int.
 *                     Bueno, esa era la idea inicial. En las pruebas falla 
 *		       la deducción automática (¿culpa del otro constructor de
 *		       tipo template?) así que defino operator+(int, Decimal) 
 *		       (con todo no debería de ser necesario. Revisar!!!)
 *    06/03/2021       operator++/--, numeric_limits
 *    14/08/2021       se permite operar con escalares que sean convertibles
 *                     a Rep.
 *    06/03/2022       print(out, decimal)
 *    12/10/2023       is_decimal_v<Decimal> == true
 *
 ****************************************************************************/
#include <utility>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <limits>

#include "atd_math.h"
#include "atd_type_traits.h"
#include "atd_names.h" // nm::Width
#include "atd_ostream.h"

namespace atd{

template <Type::Integer Rep, int ndecimals>
class Decimal;

namespace  private_{
// is_class_decimal
// ----------------
template <typename T>
struct is_class_decimal: public std::false_type { };

template <Type::Integer Rep, int N>
struct is_class_decimal<Decimal<Rep, N>> : public std::true_type { };

template <typename T>
inline constexpr bool is_class_decimal_v = is_class_decimal<T>::value;

}// private_
 
// Un número decimal lo estoy almacenando como x*10^(-n).
// Queremos escribir x1*10^(-n1) como x2*10^(-n2):
//
//	    x2*10^(-n2) = x1*10^(-n1) ==> x2 = x1*10^(n2-n1)
//
// Fórmula que nos dice cómo calcular x2 a partir de x1.
//
// Esta función hace el cast de forma explícita, lo cual implica que en
// general, se perderá resolución. Es responsabilidad del usuario garantizar
// que esa pérdida no existe o no le importa.
// Ejemplo:
//	Decimal<uint8_t,2> x8{2,43};
//
//	Decimal<uint16_t, 2> x16 = x8; // OK, implítico
//	Decimal<uint8_t,1> x2 = x8;    // ERROR: se quiere convertir
//				       // 2.43 ---> 2.4!!! Se pierde el 0.03
//
//	Decimal<uint8_t,1> x3 = decimal_cast<Decimal<uint8_t,1>>(x8);// OK
//				      // el usuario sabe que pierde el 0.03
//
// La regla es: el único cambio implítico permitido es cuando se aumenta el
// número de bits del tipo Rep (de 8 a 16, o 32, ó 64). Cualquier otro cambio
// tiene que ser explítico usando decimal_cast.
template <typename To, Type::Integer Rep1, int n1>
	requires (private_::is_class_decimal_v<To>)
constexpr inline To decimal_cast(const Decimal<Rep1, n1>& d)
{
    using From = Decimal<Rep1, n1>;
    constexpr int n2 = To::ndecimals;

    if constexpr (std::is_same_v<From, To>)
	return d;

    if constexpr (n2 == n1)
	return To::significand(d.significand());

    else if constexpr (n2 > n1)
        return To::significand(d.significand() *
	    ten_to_the<typename To::Rep>(n2 - n1));

    else
        return To::significand(d.significand() /
			    ten_to_the<typename To::Rep>(n1 - n2));
}



/*!
 *  \brief  Número decimal con ndecimals cifras decimales.
 *
 *  Lo que vamos a hacer es operar con números con un número fijo de
 *  decimales. Esto es, si multiplicamos/dividimos números con 2 cifras
 *  decimales el resultado tiene también 2 cifras decimales!!! (= FIX)
 *
 *  Internamente estamos escribiendo el número como potencia de 10:
 *
 *		    x*10^(-n)
 *
 *  siendo x = el valor que almacenamos, y 'n' el número de cifras decimales.
 *
 *  Ejemplo:
 *	2.78 = 278*10^(-2)  ==> almacenamos 278, usamos n = 2.
 *
 */
template <Type::Integer Rep0, int ndecimals0>
class Decimal{
public:
// Types
    using Rep    = Rep0; // representación. Copio la notación de chrono.

    static constexpr int ndecimals = ndecimals0;

// Construction
    constexpr Decimal() = default;

    /// Definimos el número con la parte fraccionaria (si la tiene) igual a
    /// cero.
    // No es explicit ya que sí tiene sentido concebir que un 'int' se
    // convierta en Decimal<int>, ya que no hay pérdida de resolución. 
    constexpr Decimal(Rep integer_part)
	:x_{construct(integer_part, 0)}
    { }


    /// Definimos el número "integer_part'fractional_part". 
    constexpr Decimal(Rep integer_part, Rep fractional_part);

    template <Type::Integer Rep2, int n2>
    constexpr Decimal(const Decimal<Rep2, n2>& d);


    /// Construimos un número decimal usando su representación interna.
    /// (RRR) Podríamos usar el constructor: 
    ///
    ///		    Decimal<int, 2> x{314};
    ///
    ///	      pero por defecto yo entiendo que estoy definiendo x == 314'00.
    ///	      Para evitar ello defino significand(314); Queda clara
    ///	      la intención:
    /// 
    ///		    auto x = Decimal<int, 2>::significand(314);
    ///
    ///	      definimos x == 3'14.
    ///
    constexpr static Decimal significand(Rep x);


// Info
    
    /// Devuelve el número como {integer_part, fractional_part}
    constexpr std::pair<Rep, Rep> value() const;

// Observer
    constexpr Rep significand() const {return x_;}


// Estructura algebraica
    constexpr Decimal& operator++();
    constexpr Decimal operator++(int);

    constexpr Decimal& operator--();
    constexpr Decimal operator--(int);

    // Operaciones con decimales
    constexpr Decimal& operator+=(const Decimal& a);
    constexpr Decimal& operator-=(const Decimal& a);
    constexpr Decimal& operator*=(const Decimal& a);
    constexpr Decimal& operator/=(const Decimal& a);

    // Operaciones con escalares
    constexpr Decimal& operator+=(const Rep& a);
    constexpr Decimal& operator-=(const Rep& a);
    constexpr Decimal& operator*=(const Rep& s);
    constexpr Decimal& operator/=(const Rep& s);

// traits: valores mínimo y máximo que puede tener el Decimal
    constexpr static Decimal min();
    constexpr static Decimal max();

private:
    // Ejemplo: si num_cifras == 2 y value_ = 314, entonces el número decimal
    // vale 3.14
    Rep x_;

    static constexpr Rep ten_to_the_n = ten_to_the<Rep>(ndecimals);


// construcción
    constexpr Rep construct(Rep integer_part, Rep fractional_part);

};


template <Type::Integer I, int n>
inline constexpr 
Decimal<I,n>::Decimal(Rep integer_part, Rep fractional_part)
    : x_{construct(integer_part, fractional_part)}
{ }



// (RRR) Se podría permitir el casting de Decimal<uint16_t, 2> x1{2,34}
//       a Decimal<uint16_t, 3> x2{x1}; ya que 2.34 entra en este tipo
//       como 2.340. Sin embargo, habría casos en los que se perdería
//       resolución. Por ejemplo, si x1 = 600.00 --> x2 = 600.000
//       (overflow, ya que 600000 no entra en un uint16_t). Por eso obligo
//       a que se haga de forma explicita el casting en este caso llamando
//       a decimal_cast, para que el usuario sea consciente de si produce
//       overflow o no.
template <Type::Integer Rep, int n>
template <Type::Integer Rep2, int n2>
constexpr inline Decimal<Rep, n>::Decimal(const Decimal<Rep2, n2>& d)
    :x_{decimal_cast<Decimal>(d).significand()} 
{
    static_assert(sizeof(Rep2) <= sizeof(Rep),
		  "Rep2 has to be convertible to Rep");
    static_assert(has_same_sign<Rep2, Rep>(),
		  "Different signs of Rep2 and To_decimal::Rep!!!");
    static_assert(n2 == ndecimals,
	      "If you change the number of decimals, you loose precision."
	      "Make explicit the cast: call 'decimal_cast'");
		
}


template <Type::Integer R, int n>
inline constexpr 
Decimal<R,n> Decimal<R,n>::significand(Rep x)
{
    Decimal dec{};  // Tengo que escribirlo como dec{} para evitar un warning
    dec.x_ = x;
    return dec;
}



template <Type::Integer I, int n>
constexpr typename Decimal<I,n>::Rep 
	    Decimal<I,n>::construct(Rep integer_part, Rep fractional_part)
{
    if constexpr (n != 0){
	int ndigits = number_of_digits(fractional_part);

	if (ndigits >= n) 
	    fractional_part = most_significant_digits<Rep, n>(fractional_part);
	else{
	    fractional_part *= ten_to_the<Rep>(n - ndigits);
	}
    }
    else
	fractional_part = 0;


    x_ = integer_part * ten_to_the_n;

    if (x_ >= 0)
	x_ += fractional_part;

    else
	x_ -= fractional_part;

    return x_;
}

// (???) Esta función falla si x_ == Rep::min(), ya que en este caso no existe
//       abs(min()). Internamente todo es correcto, pero el valor que mostrará
//       será raro. 
//       ¿Modificar el código para que funcione bien ese caso?
//       Parece un caso raro, muy raro de hecho. No quiero tener que penalizar 
//       el caso habitual por culpa de gestionar un caso completamente 
//       excepcional. Usémoslo y si se presenta ese caso, entonces
//       incluyámoslo. Si no, olvidémoslo.
template <Type::Integer Rep, int n>
constexpr inline std::pair<Rep, Rep> Decimal<Rep, n>::value() const
{
    auto [q, r] = atd::div(atd::abs(x_), ten_to_the_n);

    if (x_ < 0) 
	q = -q;

    return std::pair<Rep, Rep>{q, r};
}


// operaciones con Decimals
// ------------------------
template <Type::Integer I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator++()
{
    x_ += ten_to_the_n;
    return *this;
}

template <Type::Integer I, int n>
inline constexpr Decimal<I,n> Decimal<I,n>::operator++(int)
{
    Decimal<I,n> old{*this};
    ++(*this);
    return old;
}

template <Type::Integer I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator--()
{
    x_ -= ten_to_the_n;
    return *this;
}

template <Type::Integer I, int n>
constexpr Decimal<I,n> Decimal<I,n>::operator--(int)
{
    Decimal<I,n> old{*this};
    --(*this);
    return old;
}

template <Type::Integer I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator+=(const Decimal& a)
{
    x_ += a.x_;

    return *this;
}


template <Type::Integer I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator-=(const Decimal& a)
{
    x_ -= a.x_;

    return *this;
}


template <Type::Integer R, int n>
constexpr inline Decimal<R, n>& Decimal<R,n>::operator*=(const Decimal<R,n>& a)
{
    using Int = same_type_with_double_bits_t<R>; 
    
    Int x = x_;
    Int y = a.x_;
    Int res = (x*y)/ten_to_the_n;

    x_ = res;

    return *this;
}


template <Type::Integer R, int n>
constexpr inline Decimal<R, n>& Decimal<R,n>::operator/=(const Decimal<R,n>& a)
{
    using Int = same_type_with_double_bits_t<R>; 
    
    Int x = x_;
    Int y = a.x_;
    Int res = (x * ten_to_the_n) / y;

    x_ = res;

    return *this;
}


// operaciones con Rep
// -------------------
template <Type::Integer I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator+=(const Rep& a)
{
    x_ += (a * ten_to_the_n);

    return *this;
}


template <Type::Integer I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator-=(const Rep& a)
{
    x_ -= (a * ten_to_the_n);

    return *this;
}

template <Type::Integer I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator*=(const Rep& a)
{
    x_ *= a;

    return *this;
}

template <Type::Integer I, int n>
inline constexpr Decimal<I,n>& Decimal<I,n>::operator/=(const Rep& a)
{
    x_ /= a;

    return *this;
}



}// namespace atd


// El common_type es el número con mayor cifras decimales.
// Como suele ocurrir con este tipo de funciones, hay un problema con el
// overflow. De momento voy a dar por supuesto que Rep es suficientemente
// grande para que no haya overflow (que es lo que suponemos en la práctica).
template <Type::Integer Rep1, int n1, Type::Integer Rep2, int n2>
struct std::common_type<atd::Decimal<Rep1, n1>, atd::Decimal<Rep2, n2>>{

    using Rep = std::common_type_t<Rep1, Rep2>;
    static constexpr int n = std::max(n1, n2);

    using type = atd::Decimal<Rep, n>;
};



namespace atd{
// Traits

template <Type::Integer R, int n>
inline constexpr atd::Decimal<R,n> atd::Decimal<R,n>::min()
{ 
    if constexpr (std::is_signed_v<R>){
	constexpr R i = std::numeric_limits<R>::min() / ten_to_the_n;
	constexpr R f = -std::numeric_limits<R>::min() + i*ten_to_the_n;
	return atd::Decimal<R,n>{i, f};
    }
    else
	return atd::Decimal<R,n>{0};

}


template <Type::Integer R, int n>
inline constexpr atd::Decimal<R,n> atd::Decimal<R,n>::max()
{ 
    constexpr R i = std::numeric_limits<R>::max() / ten_to_the_n;
    constexpr R f = std::numeric_limits<R>::max() - i*ten_to_the_n;

    return atd::Decimal<R,n>{i, f};
}



// Estructura algebráica de espacio vectorial
// ------------------------------------------
template <Type::Integer R1, int n1, Type::Integer R2, int n2>
constexpr inline std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>
    operator+(const Decimal<R1,n1>& a, const Decimal<R2,n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    CT x = decimal_cast<CT>(a);
    CT y = decimal_cast<CT>(b);

    x += y;

    return x;
}


template <Type::Integer R1, int n1, Type::Integer R2, int n2>
constexpr inline std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>
    operator-(const Decimal<R1,n1>& a, const Decimal<R2,n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    CT x = decimal_cast<CT>(a);
    CT y = decimal_cast<CT>(b);

    x -= y;

    return x;
}



// Como usar números decimals es realmente usar "FIX aritmetic", devolvemos
// un decimal con el número máximo de cifras con las que operamos.
template <typename R1, int n1, typename R2, int n2>
constexpr inline std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>
    operator*(const Decimal<R1,n1>& a, const Decimal<R2,n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    CT x = decimal_cast<CT>(a);
    CT y = decimal_cast<CT>(b);

    x *= y;

    return x;
}



// Como usar números decimals es realmente usar "FIX aritmetic", devolvemos
// un decimal con el número máximo de cifras con las que operamos.
template <typename R1, int n1, typename R2, int n2>
constexpr inline std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>
    operator/(const Decimal<R1,n1>& a, const Decimal<R2,n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    CT x = decimal_cast<CT>(a);
    CT y = decimal_cast<CT>(b);

    x /= y;

    return x;
}


// operaciones con escalares
// -------------------------
template <typename R1, typename R2, int n>
constexpr inline Decimal<common_type_if_convertible_t<R1, R2>, n>
operator+(Decimal<R1,n> a, const R2& b)
{ return a += b; }

template <typename R1, typename R2, int n>
constexpr inline Decimal<common_type_if_convertible_t<R1, R2>, n>
operator+(const R2& a, const Decimal<R1,n>& b)
{ return b + a;}

template <typename R1, typename R2, int n>
constexpr inline Decimal<common_type_if_convertible_t<R1, R2>, n>
operator-(Decimal<R1,n> a, const R2& b)
{ return a -= b; }

template <typename R1, typename R2, int n>
constexpr inline Decimal<common_type_if_convertible_t<R1, R2>, n>
operator-(const R2& a, const Decimal<R1,n>& b)
{ return (Decimal<R1,n>{a} - b);}

template <typename R1, typename R2, int n>
constexpr inline Decimal<common_type_if_convertible_t<R1, R2>, n>
operator*(const R2& a, Decimal<R1, n> v)
{
    v *= a;
    return v;
}

template <typename R1, typename R2, int n>
constexpr inline Decimal<common_type_if_convertible_t<R1, R2>, n>
operator*(Decimal<R1, n> v, const R2& a)
{
    return a*v;
}

// solo si R2 es convertible a R1.
template <typename R1, typename R2, int n>
constexpr inline Decimal<common_type_if_convertible_t<R1, R2>, n>
operator/(Decimal<R1, n> v, const R2& a)
{
    v /= a;
    return v;
}


template <typename R1, typename R2, int n>
constexpr inline Decimal<common_type_if_convertible_t<R1, R2>, n>
operator/(const R2& a, const Decimal<R1,n>& b)
{ return Decimal<R1,n>::significand(a * ten_to_the<R1>(n)) / b; }





// Comparisons
// -----------
template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator==(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    CT x = decimal_cast<CT>(a);
    CT y = decimal_cast<CT>(b);

    return x.significand() == y.significand();
}

template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator!=(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{ return !(a == b); }


template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator<(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{
    using CT = std::common_type_t<Decimal<R1, n1>, Decimal<R2, n2>>;

    CT x = decimal_cast<CT>(a);
    CT y = decimal_cast<CT>(b);

    return x.significand() < y.significand();
}

template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator<=(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{ return !(b < a); }


template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator>(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{ return (b < a); }


template <typename R1, int n1, typename R2, int n2>
constexpr inline 
bool operator>=(const Decimal<R1, n1>& a, const Decimal<R2, n2>& b)
{ return !(a < b); }


// Serialización
// -------------
namespace impl_of{
template <Type::Ostream Out, Type::Integer Rep, int ndecimals, typename Int>
Out& print(Out& out, const atd::Decimal<Rep, ndecimals>& d)
{
    auto [n0, f0] = d.value();
    Int n = n0;

    atd::print(out, n);

    if constexpr (ndecimals > 0){
	Int f = f0;
	atd::print(out, '.');
	atd::print(out, f, nm::Width{ndecimals});
    }

    return out;
}
}// impl_of


// (RRR) Los int8_t/uint8_t C++ los considera char, con lo que no los imprime
//	 como números. Por ello esta función gestion aparte estos tipos.
template <Type::Ostream Out, Type::Integer Rep, int ndecimals>
Out& print(Out& out, const atd::Decimal<Rep, ndecimals>& d)
{
    // para poder imprimir uint8_t como int
    if constexpr (std::is_same_v<Rep, uint8_t>)
	return impl_of::print<Out, Rep, ndecimals, unsigned int>(out, d);

    else if constexpr (std::is_same_v<Rep, int8_t>)
	return impl_of::print<Out, Rep, ndecimals, int>(out, d);

    else 
	return impl_of::print<Out, Rep, ndecimals, Rep>(out, d);
}

// TODO: falta implementarla. De momento hago un print.
template <Type::Ostream Out, Type::Integer Rep, int ndecimals>
Out& print(Out& out,
           const atd::Decimal<Rep, ndecimals>& d,
           const nm::Width<int>& w)
{
    return print(out, d);
}

template <typename Out, Type::Integer Rep, int ndecimals>
Out& operator<<(Out& out, const atd::Decimal<Rep, ndecimals>& d)
{
    return print(out, d);
}





// traits
// ------
// same_type_with_double_bits
template<Type::Integer Rep, int N>
struct same_type_with_double_bits<Decimal<Rep, N>>{
    using type = Decimal<same_type_with_double_bits_t<Rep>, N>;
};


// is_decimal
// ----------
template <Type::Integer Rep, int N>
struct is_decimal<Decimal<Rep, N>> : std::true_type {};



// multiply_decimal_type_by_ten_to_the
// -----------------------------------
// (RRR) Si queremos pasar de metros a centímetros, por ejemplo, pasar 2.14 m
//       214 cm, necesitamos garantizar que el número multiplicado por 100
//       entre en el Decimal.
//       Si por ejemplo, tenemos `Decimal<uint8_t, 2> m = 2.14` podemos
//       escribirlo como `Decimal<uint8_t, 0> cm = 214`, pero no podemos
//       escribirlo en milímetros `Decimal<uint8_t, 0> mm = 2140` ya que 2140
//       no entra en un uint8_t. En este caso tenemos que meterlo en 
//       `Decimal<uint16_t, 0> mm = 2140`.
//       Esta función se encarga de calcular el Decimal necesario.
// type multiply_decimal_type_by_ten_to_the<T, n>
// {
//	if (T == Decimal<Rep, ndec>){
//	    if (n >= 0){
//		if (ndec <= n)
//		    return Decimal<Rep, ndec - n>;
//		else 
//		    return Decimal<doble_bits<Rep>, 0>;
//	    }
//
//	    else{
//		return Decimal<Rep, ndec + n>;
//	    }
//	}
//
//	else
//	    return T;
// }
template <typename T, int n>
struct multiply_decimal_type_by_ten_to_the
{ using type = T; };

template <Type::Integer Rep, int ndec, int n>
    requires (n >= 0 and ndec >= n)
struct multiply_decimal_type_by_ten_to_the<Decimal<Rep, ndec>, n>
{ 
    using type = Decimal<Rep, ndec - n>;
};

template <Type::Integer Rep, int ndec, int n>
    requires (n >= 0 and ndec < n)
struct multiply_decimal_type_by_ten_to_the<Decimal<Rep, ndec>, n>
{ 
    using Rep2 = same_type_with_double_bits_t<Rep>;
    using type = Decimal<Rep2, 0>; // DUDA: ¿cifras decimales?
};


// Al dividir entre 10^n movemos la coma a la izda, no cambiando el número
// interno que almacenamos
template <Type::Integer Rep, int ndec, int n>
    requires (n < 0)
struct multiply_decimal_type_by_ten_to_the<Decimal<Rep, ndec>, n>
{ 
    using type = Decimal<Rep, ndec + n>;
};

template <typename T, int n>
using multiply_decimal_type_by_ten_to_the_t 
		    = typename multiply_decimal_type_by_ten_to_the<T,n>::type;

}// namespace atd



// numeric_limits
// --------------
template <Type::Integer Rep, int N>
struct std::numeric_limits<atd::Decimal<Rep,N>>{
// Constants
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = std::is_signed_v<Rep>;
    static constexpr bool is_integer     = (N == 0);
    static constexpr bool is_exact       = false;
    static constexpr bool has_infinity	 = false;
    static constexpr bool is_bounded	 = true;
    
//    static constexpr int digits10	 = std::numeric_limits<Rep>::digits10;
//    static constexpr int min_exponent10	 = -N;
//    static constexpr int max_exponent10	 = 
//				atd::numeric_limits<Rep>::max_exponent10 - N;

// Functions
    static constexpr atd::Decimal<Rep, N> min() noexcept 
    {return atd::Decimal<Rep,N>::min();}

    static constexpr atd::Decimal<Rep, N> max() noexcept 
    {return atd::Decimal<Rep,N>::max();}
};

template <Type::Integer Rep, int N>
struct std::is_signed<atd::Decimal<Rep, N>>
    : public std::bool_constant<std::is_signed_v<Rep>> {
};



#endif

