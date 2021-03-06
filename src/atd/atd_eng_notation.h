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
 *
 ****************************************************************************/
#include "atd_magnitude.h"

namespace atd{

/*!
 *  \brief  Engineering notation for Magnitude
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

// constructor
    template <typename Multiplier, typename D>
    constexpr Magnitude_ENG_notation(
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
    int exponent() const {return exp_;}

// print
    void print(std::ostream& out) const;

private:
    Rep x_;	// valor númerico
    int exp_;	// exponente

    void update_representation();

    static void common_representation(Magnitude_ENG_notation& a,
                                      Magnitude_ENG_notation& b);

    static void print_exponent(std::ostream& out, int exp);

};

template <typename U, typename Rep>
template <typename Multiplier, typename D>
constexpr Magnitude_ENG_notation<U, Rep>::Magnitude_ENG_notation(
    const Magnitude<U, Rep, Multiplier, D>& m)
{
    static_assert(ratio_is_power_of_ten<Multiplier>);

    x_ = m.value();

    exp_ = ratio_exponent_of_power_of_ten<Multiplier>;

    update_representation();
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
Magnitude_ENG_notation<U, Rep>& Magnitude_ENG_notation<U, Rep>::operator++()
{
    ++x_;
    update_representation();
    return *this;
}

template <typename U, typename Rep>
Magnitude_ENG_notation<U, Rep> Magnitude_ENG_notation<U, Rep>::operator++(int)
{
    Magnitude_ENG_notation<U, Rep> res{*this};
    ++res;
    return res;
}


template <typename U, typename Rep>
Magnitude_ENG_notation<U, Rep>& Magnitude_ENG_notation<U, Rep>::operator--()
{
    if (x_ < 2){
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
Magnitude_ENG_notation<U, Rep> Magnitude_ENG_notation<U, Rep>::operator--(int)
{
    Magnitude_ENG_notation<U, Rep> res{*this};
    --res;
    return res;
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


template <typename U, typename R>
void Magnitude_ENG_notation<U, R>::print(std::ostream& out) const
{
    out << x_ << ' ';
    print_exponent(out, exp_);
    out << Unit_symbol<Unit>;
}

template <typename U, typename R>
void Magnitude_ENG_notation<U, R>::print_exponent(std::ostream& out, int exp)
{
    switch (exp){
	case 0:		  break;
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
std::ostream& operator<<(std::ostream& out, const Magnitude_ENG_notation<U, R>& m)
{
    m.print(out);
    return out;
}

}// namespace


#endif


