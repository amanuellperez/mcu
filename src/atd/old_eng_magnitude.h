// TODO: BORRAR ESTA CLASE
//	 DADA DE BAJA EL 26/10/2023. Si no se necesita en un par de años
//	 borrarla.
//	 Usar Magnitude.
//
// Copyright (C) 2021 Manuel Perez 
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

#ifndef __ATD_ENG_MAGNITUDE_H__
#define __ATD_ENG_MAGNITUDE_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Engineering notation for Magnitude
 *
 *  TODO: BORRARLA!!! 
 *	  Mezcle ideas al crear esta clase.
 *	  Por una parte está `atd::Magnitude` y por otra parte cómo almacenar
 *	  los números grandes/pequeños en un microcontrolador si queremos
 *	  evitar el uso de float.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    04/02/2021 v0.0 Implementación mínima.
 *    11/03/2021      to_eng_magnitude
 *    15/08/2021      millivalue
 *    06/03/2022      print
 *
 ****************************************************************************/
#include "atd_ostream.h"    // atd::print(std::ostream, T)
#include "atd_magnitude.h"
#include "atd_cast.h"	    // to_integer
#include "atd_math.h"
#include "atd_type_traits.h"	// same_type_at_least32_t
#include "atd_names.h" // nm::Width

namespace atd{

// Convierte el número x*10^exp en x'*10^exp' con x' < 1000 y exp' múltiplo de
// 3.
// [x, exp] = son parametros de entrada y salida.
template <typename Int, typename Exponent>
constexpr void write_as_eng(Int& x, Exponent& exp)
{
    while (x >= Int{1000}){
	x /= Int{10};
	++exp;
    }

// update_exponent():
    int r = exp % 3;
    switch (r) {
        case 1:
	case -2:
	    if (x >= Int{100}){
		x /= Int{100};
		exp += 2;
	    }
	    else {
		x *= Int{10};
		exp -= 1;
	    }
            break;

        case 2:
	case -1:
	    if (x >= Int{10}){
		x /= Int{10};
		exp += 1;
	    }
	    else{
		x *= Int{100};
		exp -= 2;
	    }
            break;
    }
}

namespace __eng_magnitude{
//
// scalar(Int):
//
//  if (exist(Int::Scalar))
//	return Int::Scalar;
//  else
//	return Int;
//
template <typename Int, typename = std::void_t<>>
struct scalar_{
    using type = Int;
};

template <typename Int>
struct scalar_<Int, std::void_t<typename Int::Scalar>>{
    using type = typename Int::Scalar;
};

template <typename Int>
using scalar = typename scalar_<Int>::type;


}// namespace



/*!
 *  \brief  Engineering notation for Magnitude
 * 
 *  Observar que se trata de una clase independiente de atd::Magnitude.
 *  Como Magnitude y ENG_Magnitude representan lo mismo puedo pasar
 *  de una forma a otra (por eso suministro to_magnitude).
 *
 *  (RRR) ¿Cómo podemos representar un número?
 *	  (1) x * 10^n , con x cualquier número (como potencia de 10). 
 *			Esta es Magnitude.
 *
 *	  (2) x * 10^(3n), con x <= 999 (notación ENG). 
 *			Esta es ENG_Magnitude
 *
 *	  (3) x * 10^n, con x <= 9 (notación científica)
 *			Esta no la tengo implementada de momento.
 *
 *  A simple vista esta clase es más ineficiente que Magnitude, ya que
 *  almacena en memoria el exponente, mientras que Magnitude no (lo gestiona
 *  en tiempo de compilación) <--- Resulta que esto es falso. Al medir el
 *  tamaño de Hertz y ENG_Frequency para los avrs obtengo 4 bytes a ambos.
 *  ¿Por qué? Para Hertz uso como Rep uint32_t, mientras que ENG_Frequency usa
 *  uint16_t para Rep y int (que debería de ser 1 byte en avr) para el
 *  exponente. En la práctica ocupan lo mismo!!! (<--- modifico: cambio
 *  Exponent a int8_t de tal forma que sea más pequeño. En principio lo había
 *  dejado como int ya que se supone que es el tipo por defecto a usar, el más
 *  eficiente, pero esta clase la quiero usar en microcontroladores y el
 *  espacio es importante. Además son de 8 bits, no de 16).
 *
 *  Además al suministrar funciones de impresión
 *  almacena en memoria caracteres que Magnitude no tiene. Por otra parte es
 *  muy cómoda para mostrar magnitudes en un LCD (esta es su finalidad
 *  inicial).
 *
 *  Esta clase permite definir Frequency, Time, Pressure y demás, cosa que no
 *  es posible con Magnitude (hay que definir Hertz, Kilohertz...). Parece
 *  mucho más cómoda ENG_Magnitude que Magnitude. Probemos.
 *
 *  Los escalares son `int` ya que eso es lo natural: 1 / freq; 2*freq ...
 *
 */
// TODO: NO usar esta clase, usar `atd::Magnitude`. Si se quiere una
// ENG_Magnitude crearla como
//	template <typename Unit>
//	using eng_Magnitude = Magnitude<Unit, eng_Number>;
//
//	template <typename Unit>
//	using sci_Magnitude = Magnitude<Unit, sci_Number>;
//
template <typename Unit0, typename Rep0>
class ENG_Magnitude{
public:
    using Unit	   = Unit0;
    using Rep      = Rep0;
    using Exponent = int8_t;
    // using Scalar   = make_type<int>::same_sign_as<Rep>;
    using Scalar   = __eng_magnitude::scalar<Rep>;
    
    // Rep almacena de -999 a 999 (signed) o de 0 a 999.
    static_assert (sizeof(Rep) > 1, 
	    "sizeof(Rep) has to be at least 2 bytes");

    // constructor
    constexpr ENG_Magnitude() = default;

//    constexpr ENG_Magnitude(const Rep& x, Exponent exp)
//	: x_{x}, exp_{exp} {write_as_eng(x_, exp_);}

    // Solo se pueda llamar a esta función cuando Int sea
    // convertible a Rep
    template <typename Int, typename CRep = std::common_type_t<Int, Rep>>
    explicit constexpr ENG_Magnitude(Int x, Exponent exp = 0);

    // algebra
    ENG_Magnitude& operator++();
    ENG_Magnitude operator++(int);

    ENG_Magnitude& operator--();
    ENG_Magnitude operator--(int);

    ENG_Magnitude& operator+=(ENG_Magnitude b);
    ENG_Magnitude& operator-=(ENG_Magnitude b);

    template <typename Int>
    ENG_Magnitude& operator*=(const Int& a);

    template <typename Int>
    ENG_Magnitude& operator/=(const Int& a);


// order
    constexpr bool operator<(const ENG_Magnitude& b) const;

// observers
    // Un ENG es un número de la forma value() * 10^exponent();
    constexpr Rep internal_value() const {return x_;}
    constexpr Exponent exponent() const {return exp_;}

    /// Devuelve el ENG en unidades de mili.
    /// Ejemplo: si la magnitud es Voltio devuelve milivoltios;
    ///          si la unidad es Seconds devuelve miliseconds...
    /// El comportamiento está undefined si el valor del ENG en milivalues 
    /// genera overflow en Rep.
    // TODO: si resulta ser práctica esta función añadir 
    //		kilovalue(), Megavalue(), Gigavalue(), Teravalue(),
    //		microvalue(), nanovalue() ...
    //		value() = value en las unidades básicas (por ejemplo
    //			  en segundos, o en voltios...).
    constexpr Rep millivalue() const;

// print
    // imprime solo la unidad.
//    // EJ: si freq = 23.452 kHz, print_unit(std::cout) imprime 'kHz'
    template <typename Out, typename U, typename Rep>
    friend void print_unit(Out& out, const ENG_Magnitude<U, Rep>& m);

// traits
    static constexpr ENG_Magnitude min();
    static constexpr ENG_Magnitude max();

private:
// Data
    Rep x_;	// valor númerico
    Exponent exp_;	// exponente: invariante: siempre múltiplo de 3

// Helpers
    static void common_exponent(ENG_Magnitude& a, ENG_Magnitude& b);

    template <typename Out>
    static void print_exponent(Out& out, int exp);

    // (RRR) Cuando Scalar == Rep no se puede definir multiply(Rep) y
    //	     multiply(Scalar) ya que estaría duplicado. Por ello se necesitan
    //	     estas dos funciones.
    void multiply_rep(const Rep& a);
    void divide_rep(const Rep& a);

    void multiply_scalar(const Scalar& a);
    void divide_scalar(const Scalar& a);

    static Rep to_rep(Scalar x) {return to_integer<Rep>(x);}
};

// constructor
// -----------
// Observar que es obligatorio inicializar x_, exp_ sino no compila al ser
// constexpr. ¿Por qué? ???
template <typename U, typename Rep>
template <typename Int, typename CRep>
constexpr ENG_Magnitude<U, Rep>::ENG_Magnitude(Int x, Exponent exp)
    : x_{}, exp_{}
{
    x_ = to_integer<Rep>(x);
    exp_ = exp;
    write_as_eng(x_, exp_);
}


// observers
// ---------
template <typename U, typename R>
inline constexpr ENG_Magnitude<U,R>::Rep ENG_Magnitude<U,R>::millivalue() const
{
    return x_ * ten_to_the<Rep>(exp_ + 3);
}


// algebra
// -------
template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep>& ENG_Magnitude<U, Rep>::operator++()
{
    ++x_;
    write_as_eng(x_, exp_);
    return *this;
}

template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep> ENG_Magnitude<U, Rep>::operator++(int)
{
    ENG_Magnitude<U, Rep> old{*this};
    ++(*this);
    return old;
}


template <typename U, typename Rep>
ENG_Magnitude<U, Rep>& ENG_Magnitude<U, Rep>::operator--()
{
    if (x_ < Rep{2}){
	x_ *= Rep{1000};
	exp_ -= 3;
	--x_;
    }
    else {
	--x_;
	write_as_eng(x_, exp_);
    }

    return *this;
}

template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep> ENG_Magnitude<U, Rep>::operator--(int)
{
    ENG_Magnitude<U, Rep> old{*this};
    --(*this);
    return old;
}

template <typename U, typename Rep>
ENG_Magnitude<U, Rep>& ENG_Magnitude<U, Rep>::
operator+=(ENG_Magnitude b)
{
    common_exponent(*this, b);
    x_ += b.x_;
    write_as_eng(x_, exp_);

    return *this;
}

template <typename U, typename Rep>
ENG_Magnitude<U, Rep>& ENG_Magnitude<U, Rep>::
operator-=(ENG_Magnitude b)
{
    common_exponent(*this, b);
    x_ -= b.x_;
    write_as_eng(x_, exp_);

    return *this;
}


template <typename U, typename Rep>
template <typename Int>
ENG_Magnitude<U, Rep>& ENG_Magnitude<U, Rep>::operator*=(const Int& a)
{
    if constexpr (std::is_same_v<Int, Rep>)
	multiply_rep(a);

    else if constexpr (std::is_same_v<Int, Scalar>)
	multiply_scalar(a);

    else 
	static_assert(always_false_v<Int>, "Can't multiply by this type");

    return *this;
}

template <typename U, typename Rep>
template <typename Int>
ENG_Magnitude<U, Rep>& ENG_Magnitude<U, Rep>::operator/=(const Int& a)
{
    if constexpr (std::is_same_v<Int, Rep>)
	divide_rep(a);

    else if constexpr (std::is_same_v<Int, Scalar>)
	divide_scalar(a);

    else 
	static_assert(always_false_v<Int>, "Can't divide by this type");

    return *this;
}

template <typename U, typename Rep>
void ENG_Magnitude<U, Rep>::multiply_rep(const Rep& a)
{
    x_ *= a;
    write_as_eng(x_, exp_);
}


template <typename U, typename Rep>
void ENG_Magnitude<U, Rep>::divide_rep(const Rep& a)
{

    if (x_ > a){
	x_ /= a;
	write_as_eng(x_, exp_);
    }
    else {
	using Int = same_type_with_double_bits_t<Rep>;

	int n = number_of_digits(a);
        Int x    = (Int{x_} * ten_to_the<Int>(n)) / Int{a};
        exp_ -= n;
	write_as_eng(x, exp_);
	x_ = to_integer<Rep>(x);
    }
}

template <typename U, typename Rep>
inline void ENG_Magnitude<U, Rep>::multiply_scalar(const Scalar& a)
{ multiply_rep(to_rep(a)); }

template <typename U, typename Rep>
inline void ENG_Magnitude<U, Rep>::divide_scalar(const Scalar& a)
{
    divide_rep(to_rep(a));
}


template <typename U, typename Rep1, typename Rep2>
ENG_Magnitude<U, std::common_type_t<Rep1, Rep2>> operator+
(const ENG_Magnitude<U, Rep1>& a, const ENG_Magnitude<U, Rep2>& b)
{
    using Rep = std::common_type_t<Rep1, Rep2>;
    
    ENG_Magnitude<U, Rep> x{a.internal_value(), a.exponent()};
    ENG_Magnitude<U, Rep> y{b.internal_value(), b.exponent()};

    x += y;

    return x;
}


template <typename U, typename Rep1, typename Rep2>
ENG_Magnitude<U, std::common_type_t<Rep1, Rep2>> operator-
(const ENG_Magnitude<U, Rep1>& a, const ENG_Magnitude<U, Rep2>& b)
{
    using Rep = std::common_type_t<Rep1, Rep2>;
    
    ENG_Magnitude<U, Rep> x{a.internal_value(), a.exponent()};
    ENG_Magnitude<U, Rep> y{b.internal_value(), b.exponent()};

    x -= y;

    return x;
}

template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep> operator*(const Rep& a, ENG_Magnitude<U, Rep> x)
{
    x *= a;

    return x;
}

template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep> operator*(const ENG_Magnitude<U, Rep>& x,
                                       const Rep& a)
{ return a*x; }

template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep> operator*(typename ENG_Magnitude<U, Rep>::Scalar a,
                                       ENG_Magnitude<U, Rep> x)
{ 
    x *= a;
    return x; 
}

template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep> operator*(const ENG_Magnitude<U, Rep>& x, typename ENG_Magnitude<U,Rep>::Scalar a)
{ return a*x; }

template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep>
operator/(ENG_Magnitude<U, Rep> x, const Rep& a)
{
    x /= a;

    return x;
}


template <typename U, typename Rep>
inline ENG_Magnitude<U, Rep> operator/(ENG_Magnitude<U, Rep> x, typename ENG_Magnitude<U,Rep>::Scalar a)
{ 
    x /= a;
    return x; 
}




// El value() va de -999 a 999. Al multiplicar dos números los valores mínimo
// y máximo que podemos obtener es -998001, +998001 que no entran en un
// int16_t. Para evitar overflow hacemos las operaciones en un int32_t y luego
// creamos el ENG_Magnitude. De esa forma evitamos el overflow.
template <typename Unit1, typename Rep1, typename Unit2, typename Rep2>
ENG_Magnitude<Unit_multiply<Unit1, Unit2>, std::common_type_t<Rep1, Rep2>>
operator*(const ENG_Magnitude<Unit1, Rep1>& a,
          const ENG_Magnitude<Unit2, Rep2>& b)
{
    using Unit = Unit_multiply<Unit1, Unit2>;
    using Rep = std::common_type_t<Rep1, Rep2>;
    using Int = same_type_at_least32_t<Rep>;

    using ENG = ENG_Magnitude<Unit, Rep>;
    using Exponent = ENG::Exponent;

    Int y = Int{a.internal_value()} * Int{b.internal_value()};
    Exponent exp = a.exponent() + b.exponent();

    write_as_eng(y, exp);

    return ENG{static_cast<Rep>(y), exp};
}

// Rep es mínimo un entero donde podemos escribir de 0 a 999. ¿cómo dividir
// 1/999? Multipliquemos por 1000 el numerador (y restemos 3 al exponente)
template <typename Unit1, typename Rep1, typename Unit2, typename Rep2>
ENG_Magnitude<Unit_divide<Unit1, Unit2>, std::common_type_t<Rep1, Rep2>>
operator/(const ENG_Magnitude<Unit1, Rep1>& a,
          const ENG_Magnitude<Unit2, Rep2>& b)
{
    using Unit = Unit_divide<Unit1, Unit2>;
    using Rep = std::common_type_t<Rep1, Rep2>;
    using Int = same_type_at_least32_t<Rep>;

    using ENG = ENG_Magnitude<Unit, Rep>;
    using Exponent = ENG::Exponent;

    Int y = (Int{a.internal_value()} * Int{1000}) /Int{b.internal_value()};
    Exponent exp = a.exponent() - Exponent{3} - b.exponent();

    write_as_eng(y, exp);

    return ENG{static_cast<Rep>(y), exp};
}

// División entre mismo tipo = resultado adimensional.
template <typename Unit, typename Rep>
Rep operator/(const ENG_Magnitude<Unit, Rep>& a,
              const ENG_Magnitude<Unit, Rep>& b)
{
    using Int = same_type_at_least32_t<Rep>;
    using Exponent = ENG_Magnitude<Unit, Rep>::Exponent;

    Int y = (Int{a.internal_value()} * Int{1000}) /Int{b.internal_value()};
    Exponent exp = a.exponent() - Exponent{3} - b.exponent();

    write_as_eng(y, exp);

    Int res = y * ten_to_the<Int>(exp);

    return to_integer<Rep>(res);
}



// Rep es mínimo un entero donde podemos escribir de 0 a 999. ¿cómo dividir
// 1/999? Multipliquemos por 1000 el numerador (y restemos 3 al exponente)
template <typename Unit1, typename Rep>
ENG_Magnitude<Unit_inverse<Unit1>, Rep>
operator/(const Rep& a, const ENG_Magnitude<Unit1, Rep>& x)
{
    using Unit = Unit_inverse<Unit1>;
    using Int = same_type_at_least32_t<Rep>;

    using ENG = ENG_Magnitude<Unit, Rep>;
    using Exponent = ENG::Exponent;

    Int y = (Int{a} * Int{1000}) /Int{x.internal_value()};
    Exponent exp = - Exponent{3} - x.exponent();

    write_as_eng(y, exp);

    return ENG{static_cast<Rep>(y), exp};
}

template <typename Unit, typename Rep>
ENG_Magnitude<Unit_inverse<Unit>, Rep>
operator/(typename ENG_Magnitude<Unit, Rep>::Scalar a,
          const ENG_Magnitude<Unit, Rep>& x)
{
    return to_integer<Rep>(a) / x;

}


// order
// -----
template <typename U, typename Rep>
constexpr inline bool operator==(const ENG_Magnitude<U, Rep>& a,
			         const ENG_Magnitude<U, Rep>& b)
{ return a.internal_value() == b.internal_value() and a.exponent() == b.exponent(); }

template <typename U, typename Rep>
constexpr inline bool operator!=(const ENG_Magnitude<U, Rep>& a,
			         const ENG_Magnitude<U, Rep>& b)
{ return !(a == b); }



template <typename U, typename Rep>
constexpr bool ENG_Magnitude<U, Rep>::operator<(const ENG_Magnitude& b) const
{
    if (exp_ < b.exp_)
	return true;

    if (exp_ > b.exp_)
	return false;

    // exp_ == b.exp_
    if (x_ < b.x_)
	return true;

    else
	return false;
}


template <typename U, typename R>
constexpr inline bool operator>(const ENG_Magnitude<U, R>& a, 
	       const ENG_Magnitude<U, R>& b)
{ return b < a; }


template <typename U, typename R>
constexpr inline bool operator<=(const ENG_Magnitude<U, R>& a, 
	       const ENG_Magnitude<U, R>& b)
{ 
    return !(b < a);
}

template <typename U, typename R>
constexpr inline bool operator>=(const ENG_Magnitude<U, R>& a, 
	       const ENG_Magnitude<U, R>& b)
{ 
    return !(a < b);
}

// varios
// ------
template <typename U, typename Rep>
void ENG_Magnitude<U, Rep>::common_exponent(
    ENG_Magnitude& a, ENG_Magnitude& b)
{
    if (a.exp_ > b.exp_){
	a.x_ *= ten_to_the<Rep>(a.exp_ - b.exp_);
	a.exp_ = b.exp_;
    }
    else if (b.exp_ > a.exp_){
	b.x_ *= ten_to_the<Rep>(b.exp_ - a.exp_);
	b.exp_ = a.exp_;
    }

}

template <typename Out, typename U, typename Rep>
inline void print(Out& out, const ENG_Magnitude<U, Rep>& m)
{
    print(out, m.internal_value());
    print(out, ' ');
    print_unit(out, m);
}

// TODO: falta implementar esta función. De momento hace un print vulgar y
// corriente.
template <typename Out, typename U, typename Rep>
inline void
print(Out& out, const ENG_Magnitude<U, Rep>& m, const nm::Width<int>& w)
{
    print(out, m.internal_value());
    print(out, ' ');
    print_unit(out, m);
}


template <typename Out, typename U, typename Rep>
void print_unit(Out& out, const ENG_Magnitude<U, Rep>& m)
{
    using Unit = ENG_Magnitude<U, Rep>::Unit;

    ENG_Magnitude<U, Rep>::print_exponent(out, m.exponent());
    print(out, Unit_symbol<Unit>);
}

// (RRR) ¿Por qué escribir un espacio en caso de exponente 0?
//       Es una cuestión estética.
//       Sin espacio, al escribir en el LCD sin hacer clear saldría:
//		999 Hz  
//		  1 kHz
//		999 Hzz  <--- no borra la z de arriba!!!
//	  
//	  Al escribir el espacio:
//	        999  Hz
//	          1 kHz
//	        999  Hz  <--- correcto
//
//	 Esto es un formato rígido:
//	        ddd pS
//	 d = digit  (999)
//	 p = prefix (k)
//	 S = symbol (Hz)
template <typename U, typename R>
template <typename Out>
void ENG_Magnitude<U, R>::print_exponent(Out& out, int exp)
{
    switch (exp){
	case 0:	print(out, ' '); break;
	case 3: print(out, 'k'); break;
	case 6: print(out, 'M'); break;
	case 9: print(out, 'G'); break;
	case 12: print(out, 'T'); break;
	case -3: print(out, 'm'); break;
	case -6: print(out, 'u'); break;
	case -9: print(out, 'n'); break;
	case -12: print(out, 'p'); break;
	default: print(out, '?'); break;
    }

}




template <typename U, typename R>
inline constexpr ENG_Magnitude<U, R>
ENG_Magnitude<U, R>::min()
{
    if constexpr (std::is_signed_v<Rep>)
    return ENG_Magnitude<U, R>{std::numeric_limits<Rep>::min(),
                                        std::numeric_limits<Exponent>::max()};
    else
    return ENG_Magnitude<U, R>{0,0};
}

template <typename U, typename R>
inline constexpr ENG_Magnitude<U, R>
ENG_Magnitude<U, R>::max()
{
    return ENG_Magnitude<U, R>{std::numeric_limits<Rep>::max(),
                                        std::numeric_limits<Exponent>::max()};
}

template <typename Out, typename U, typename R>
inline Out& operator<<(Out& out, const ENG_Magnitude<U, R>& m)
{
    print(out, m);
    return out;
}


// castings
// --------
// (RRR) ¿Por qué no hacer ENG_magnitude{Magnitude}?
// Cuando se pierda resolución, esto es cuando no se pueda hacer una
// conversión implícita de la representación usada por Magnitude a la de
// ENG_magnitude el usuario tiene que ser consciente del casting.
// Ejemplo: pasar de Magnitude{440.23} Hz  a ENG_magnitude{440} Hz, pierde
// resolución.
template <typename Rep1,
          typename Unit,
          typename Rep0,
	  typename Multiplier,
          typename D>
inline ENG_Magnitude<Unit, Rep1>
to_eng_magnitude(const Magnitude<Unit, Rep0, Multiplier, D>& m)
{
    return ENG_Magnitude<Unit, Rep1>{to_integer<Rep1>(m.value()),
                                  ratio_exponent_of_power_of_ten<Multiplier>};

}

// añado este porque parece sencillo de usar. El tiempo lo dira.
template <typename Unit,
          typename Rep,
          typename Multiplier,
          typename D>
inline ENG_Magnitude<Unit, Rep>
to_eng_magnitude(const Magnitude<Unit, Rep, Multiplier, D>& m)
{
    return ENG_Magnitude<Unit, Rep>{m.value(),
                                  ratio_exponent_of_power_of_ten<Multiplier>};
}


template <typename Rep, typename Unit, typename ENG_Rep>
inline Magnitude<Unit, Rep, std::ratio<1>>
to_magnitude(const ENG_Magnitude<Unit, ENG_Rep>& m)
{
    ENG_Rep x = m.internal_value() * ten_to_the<ENG_Rep>(m.exponent());
    Rep res = static_cast<Rep>(x);
    return Magnitude<Unit, Rep, std::ratio<1>>{res};
}


// ENG_magnitude por defecto
// -------------------------
// Frequency
template <typename Int>
using ENG_frequency = ENG_Magnitude<Units_frequency, Int>;

template <typename Int>
constexpr inline ENG_frequency<Int> frequency_in_Hz(const Int& x)
{ return ENG_frequency<Int>{x, 0}; }

template <typename Int>
constexpr inline ENG_frequency<Int> frequency_in_kHz(const Int& x)
{ return ENG_frequency<Int>{x, 3}; }

template <typename Int>
constexpr inline ENG_frequency<Int> frequency_in_MHz(const Int& x)
{ return ENG_frequency<Int>{x, 6}; }

// Time
template <typename Int>
using ENG_time= ENG_Magnitude<Units_time, Int>;

template <typename Int>
constexpr inline ENG_time<Int> time_in_s(const Int& x)
{ return ENG_time<Int>{x, 0}; }

template <typename Int>
constexpr inline ENG_time<Int> time_in_ms(const Int& x)
{ return ENG_time<Int>{x, -3}; }

template <typename Int>
constexpr inline ENG_time<Int> time_in_us(const Int& x)
{ return ENG_time<Int>{x, -6}; }

// Length
template <typename Int>
using ENG_length= ENG_Magnitude<Units_length, Int>;



template <typename Int>
using ENG_temperature = ENG_Magnitude<Units_temperature, Int>;

template <typename Int>
using ENG_pressure = ENG_Magnitude<Units_pressure, Int>;

template <typename Int>
using ENG_velocity = ENG_Magnitude<Units_velocity, Int>;


// Volt
template <typename Int>
using ENG_electric_potential = ENG_Magnitude<Units_electric_potential, Int>;


}// namespace



// numeric_limits
// --------------
template <typename U, typename Rep>
struct std::numeric_limits<atd::ENG_Magnitude<U,Rep>>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = std::numeric_limits<Rep>::is_signed;
    static constexpr bool is_integer     = std::numeric_limits<Rep>::is_integer;
    static constexpr bool is_exact       = false; // exponente puede ser -n

    static constexpr atd::ENG_Magnitude<U, Rep> min() noexcept 
    {return atd::ENG_Magnitude<U, Rep>::min();}

    static constexpr atd::ENG_Magnitude<U, Rep> max() noexcept
    { return atd::ENG_Magnitude<U, Rep>::max(); }
};

#endif


