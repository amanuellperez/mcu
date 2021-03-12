// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ATD_ENG_NOTATION_H__
#define __ATD_ENG_NOTATION_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Engineering notation for Magnitude
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    04/02/2021 v0.0 Implementación mínima.
 *    11/03/2021      to_eng_notation 
 *
 ****************************************************************************/
#include "atd_magnitude.h"
#include "atd_cast.h"	    // to_integer

namespace atd{

/*!
 *  \brief  Engineering notation for Magnitude
 * 
 *  Observar que se trata de una clase independiente de atd::Magnitude.
 *  Como Magnitude y Magnitude_ENG_notation representan lo mismo puedo pasar
 *  de una forma a otra (por eso suministro el constructor). Se puede operar
 *  completamente en Magnitude_ENG_notation olvidándose de Magnitude.
 *
 *  (RRR) ¿Cómo podemos representar un número?
 *	  (1) x*10^n , con x cualquier número (como potencia de 10). 
 *			Esta es Magnitude.
 *
 *	  (2) x*10^(3n), con x <= 999 (notación ENG). 
 *			Esta es Magnitude_ENG_notation
 *
 *	  (3) x*10^n, con x <= 9 (notación científica)
 *			Esta no la tengo implementada de momento.
 *
 *  A simple vista esta clase es más ineficiente que Magnitude, ya que
 *  almacena en memoria el exponente, mientras que Magnitude no (lo gestiona
 *  en tiempo de compilación). Además al suministrar funciones de impresión
 *  almacena en memoria caracteres que Magnitude no tiene. Por otra parte es
 *  muy cómoda para mostrar magnitudes en un LCD (esta es su finalidad
 *  inicial).
 *
 */
template <typename Unit0, typename Rep0>
class Magnitude_ENG_notation{
public:
    using Unit = Unit0;
    using Rep  = Rep0;
    using Exponent = int;

// constructor
    constexpr Magnitude_ENG_notation() = default;

    constexpr Magnitude_ENG_notation(const Rep& x, Exponent exp)
	: x_{x}, exp_{exp} {}

    template <typename Multiplier, typename D>
    constexpr Magnitude_ENG_notation(
        const Magnitude<Unit, Rep, Multiplier, D>& m);

    template <typename Multiplier, typename D>
    constexpr Magnitude_ENG_notation& operator=(
        const Magnitude<Unit, Rep, Multiplier, D>& m);

// algebra
    Magnitude_ENG_notation& operator++();
    Magnitude_ENG_notation operator++(int);

    Magnitude_ENG_notation& operator--();
    Magnitude_ENG_notation operator--(int);

    Magnitude_ENG_notation& operator+=(Magnitude_ENG_notation b);
    Magnitude_ENG_notation& operator-=(Magnitude_ENG_notation b);

// order
    bool operator<(const Magnitude_ENG_notation& b) const;

// observers
    Rep value() const {return x_;}
    Exponent exponent() const {return exp_;}

// print
    void print(std::ostream& out) const;

// traits
    static constexpr Magnitude_ENG_notation min();
    static constexpr Magnitude_ENG_notation max();

private:
    Rep x_;	// valor númerico
    Exponent exp_;	// exponente

// Helpers
    template <typename Multiplier, typename D>
    constexpr void init(const Magnitude<Unit, Rep, Multiplier, D>& m);

    void update_representation();

    static void common_representation(Magnitude_ENG_notation& a,
                                      Magnitude_ENG_notation& b);

    static void print_exponent(std::ostream& out, int exp);

    template <typename Int>
    void __print(std::ostream& out) const;
};

template <typename U, typename Rep>
template <typename Multiplier, typename D>
constexpr void Magnitude_ENG_notation<U, Rep>::init(
    const Magnitude<U, Rep, Multiplier, D>& m)
{
    static_assert(ratio_is_power_of_ten<Multiplier>);

    x_ = m.value();

    exp_ = ratio_exponent_of_power_of_ten<Multiplier>;

    update_representation();
}

template <typename U, typename Rep>
template <typename Multiplier, typename D>
inline constexpr Magnitude_ENG_notation<U, Rep>::Magnitude_ENG_notation(
    const Magnitude<U, Rep, Multiplier, D>& m)
{
    init(m);
}

template <typename U, typename Rep>
template <typename Multiplier, typename D>
inline constexpr Magnitude_ENG_notation<U, Rep>& Magnitude_ENG_notation<U, Rep>::
operator=(const Magnitude<U, Rep, Multiplier, D>& m)
{
    init();
}


// n = exponente
template <typename U, typename Rep>
void Magnitude_ENG_notation<U, Rep>::update_representation()
{
    while (x_ >= Rep{1000}){
	x_ /= Rep{10};
	++exp_;
    }

    int r = exp_ % 3;
    if (r == 1){
	x_ /= Rep{100};
	exp_ += 2;
    }
    else if (r == 2){
	x_ /= Rep{10};
	exp_ += 1;
    }
}


template <typename U, typename Rep>
inline Magnitude_ENG_notation<U, Rep>& Magnitude_ENG_notation<U, Rep>::operator++()
{
    ++x_;
    update_representation();
    return *this;
}

template <typename U, typename Rep>
inline Magnitude_ENG_notation<U, Rep> Magnitude_ENG_notation<U, Rep>::operator++(int)
{
    Magnitude_ENG_notation<U, Rep> old{*this};
    ++(*this);
    return old;
}


template <typename U, typename Rep>
Magnitude_ENG_notation<U, Rep>& Magnitude_ENG_notation<U, Rep>::operator--()
{
    if (x_ < Rep{2}){
	x_ *= Rep{1000};
	exp_ -= 3;
	--x_;
    }
    else {
	--x_;
	update_representation();
    }

    return *this;
}

template <typename U, typename Rep>
inline Magnitude_ENG_notation<U, Rep> Magnitude_ENG_notation<U, Rep>::operator--(int)
{
    Magnitude_ENG_notation<U, Rep> old{*this};
    --(*this);
    return old;
}

template <typename U, typename Rep>
Magnitude_ENG_notation<U, Rep>& Magnitude_ENG_notation<U, Rep>::
operator+=(Magnitude_ENG_notation b)
{
    common_representation(*this, b);
    x_ += b.x_;
    update_representation();

    return *this;
}

template <typename U, typename Rep>
Magnitude_ENG_notation<U, Rep>& Magnitude_ENG_notation<U, Rep>::
operator-=(Magnitude_ENG_notation b)
{
    common_representation(*this, b);
    x_ -= b.x_;
    update_representation();

    return *this;
}

// order
// -----
template <typename U, typename Rep>
bool Magnitude_ENG_notation<U, Rep>::operator<(const Magnitude_ENG_notation& b) const
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
inline bool operator>(const Magnitude_ENG_notation<U, R>& a, 
	       const Magnitude_ENG_notation<U, R>& b)
{ return b < a; }


template <typename U, typename R>
inline bool operator<=(const Magnitude_ENG_notation<U, R>& a, 
	       const Magnitude_ENG_notation<U, R>& b)
{ 
    return !(b < a);
}

template <typename U, typename R>
inline bool operator>=(const Magnitude_ENG_notation<U, R>& a, 
	       const Magnitude_ENG_notation<U, R>& b)
{ 
    return !(a < b);
}

// varios
// ------
template <typename U, typename Rep>
void Magnitude_ENG_notation<U, Rep>::common_representation(
    Magnitude_ENG_notation& a, Magnitude_ENG_notation& b)
{
    if (a.exp_ > b.exp_){
	a.x_ *= ten_to_the<Rep>(a.exp_ - b.exp_);
    }
    else if (b.exp_ > a.exp_){
	b.x_ *= ten_to_the<Rep>(b.exp_ - a.exp_);
    }

    a.exp_ = b.exp_;
}


template <typename U, typename Rep>
void Magnitude_ENG_notation<U, Rep>::print(std::ostream& out) const
{
    // para poder imprimir uint8_t como int
    if constexpr (std::is_same_v<Rep, uint8_t>)
	__print<unsigned int>(out);

    else if constexpr (std::is_same_v<Rep, int8_t>)
	__print<int>(out);

    else 
	__print<Rep>(out);

}

template <typename U, typename R>
template <typename Int>
void Magnitude_ENG_notation<U, R>::__print(std::ostream& out) const
{
    out << static_cast<Int>(x_) << ' ';
    print_exponent(out, exp_);
    out << Unit_symbol<Unit>;
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
void Magnitude_ENG_notation<U, R>::print_exponent(std::ostream& out, int exp)
{
    switch (exp){
	case 0:	out << ' '; break;
	case 3: out << 'k'; break;
	case 6: out << 'M'; break;
	case 9: out << 'G'; break;
	case 12: out << 'T'; break;
	case -3: out << 'm'; break;
	case -6: out << 'u'; break;
	case -9: out << 'n'; break;
	case -12: out << 'p'; break;
	default: out << '?'; break;
    }

}

template <typename U, typename R>
inline constexpr Magnitude_ENG_notation<U, R>
Magnitude_ENG_notation<U, R>::min()
{
    if constexpr (std::is_signed_v<Rep>)
    return Magnitude_ENG_notation<U, R>{std::numeric_limits<Rep>::min(),
                                        std::numeric_limits<Exponent>::max()};
    else
    return Magnitude_ENG_notation<U, R>{0,0};
}

template <typename U, typename R>
inline constexpr Magnitude_ENG_notation<U, R>
Magnitude_ENG_notation<U, R>::max()
{
    return Magnitude_ENG_notation<U, R>{std::numeric_limits<Rep>::max(),
                                        std::numeric_limits<Exponent>::max()};
}

template <typename U, typename R>
inline std::ostream& operator<<(std::ostream& out, const Magnitude_ENG_notation<U, R>& m)
{
    m.print(out);
    return out;
}

// castings
// --------
template <typename Rep1,
          typename Unit,
          typename Rep0,
	  typename Multiplier,
          typename D>
inline Magnitude_ENG_notation<Unit, Rep1>
to_eng_notation(const Magnitude<Unit, Rep0, Multiplier, D>& m)
{
    Magnitude_ENG_notation<Unit, Rep0> tmp{m};

    return Magnitude_ENG_notation<Unit, Rep1>{to_integer<Rep1>(tmp.value()),
                                              tmp.exponent()};
}

// añado este porque parece sencillo de usar. El tiempo lo dira.
template <typename Unit,
          typename Rep,
          typename Multiplier,
          typename D>
inline Magnitude_ENG_notation<Unit, Rep>
to_eng_notation(const Magnitude<Unit, Rep, Multiplier, D>& m)
{
    return Magnitude_ENG_notation<Unit, Rep>{m};
}


}// namespace



// numeric_limits
// --------------
template <typename U, typename Rep>
struct std::numeric_limits<atd::Magnitude_ENG_notation<U,Rep>>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = std::numeric_limits<Rep>::is_signed;
    static constexpr bool is_integer     = std::numeric_limits<Rep>::is_integer;
    static constexpr bool is_exact       = false; // exponente puede ser -n

    static constexpr atd::Magnitude_ENG_notation<U, Rep> min() noexcept 
    {return atd::Magnitude_ENG_notation<U, Rep>::min();}

    static constexpr atd::Magnitude_ENG_notation<U, Rep> max() noexcept
    { return atd::Magnitude_ENG_notation<U, Rep>::max(); }
};

#endif


