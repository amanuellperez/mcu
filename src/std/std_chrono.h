// Copyright (C) 2019-2023 Manuel Perez 
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

#ifndef __MCU_STD_CHRONO_H__
#define __MCU_STD_CHRONO_H__

/****************************************************************************
 *
 *  - DESCRIPCION: El equivalente de <chrono>
 *
 *  - TODO: De momento no implemento los requirements en cada clase. Busco
 *  aprender, y la mejor forma de aprender es fijarse en lo importante. Los
 *  requirements son para garantizar que el usuario no usa mal las clases. 
 *  (al final he añadido la mayoría, pero faltan algunos)
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	08/12/2019 v0.0
 *	21/09/2023 hh_mm_ss, treat_as_floating_point
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_ratio.h"
#include "std_type_traits.h"
#include "std_limits.h"
#include "std_ctime.h"
#include "std_cstdint.h"
#include "std_cmath.h"	// abs

namespace STD{

namespace chrono {
// Traits
// ------
template <typename Rep>
struct treat_as_floating_point : is_floating_point<Rep> { };

template <typename Rep>
inline constexpr bool treat_as_floating_point_v 
				    = treat_as_floating_point<Rep>::value;


// A duration type measures time between two points in time (time_points). A
// duration has a representation which holds a count of ticks and a tick period.
// The tick period is the amount of time which occurs from one tick to the next,
// in units of seconds. It is expressed as a rational constant using the
// template ratio.
template <typename Rep, typename Period = ratio<1>>
struct duration;

template <typename Clock, typename Duration = typename Clock::duration>
struct time_point;

// Precondiciones de duration
// --------------------------
// bool __is_ratio(typename T)
// {
//	if (is_<T>_a_specialization_of<ratio>)
//	    return true;
//	else
//	    return false;
// }
template <typename T>
struct __is_ratio : false_type { };

template <intmax_t N, intmax_t D>
struct __is_ratio<ratio<N, D>> : true_type { };


// __is_duration
// -------------
template <typename T>
struct __is_duration : public false_type { };

template <typename Rep, typename Period>
struct __is_duration<duration<Rep, Period>> : public true_type { };

template <typename T>
inline constexpr bool __is_duration_v = __is_duration<T>::value;


// __enable_if_is_duration
// -----------------------
template <typename T>
using __enable_if_is_duration = enable_if_t<__is_duration_v<T>, T>;

template <typename T>
using __disable_if_is_duration = enable_if_t<!__is_duration_v<T>, T>;



// -------------
// duration_cast
// -------------
// El número de segundos medidos por una duración viene dado por:
//
//		num_segundos = count() * period;
//
// Si queremos convertir una duración en otra tiene que cumplirse que:
//	num_segundos = count1() * period1 = count2() * period2;
//
// despejando:
//	count2() = count1() * period1 / period2;
//
//  Lo que hacemos es calcular estáticamente CF = period1 / period2 y operar
//  de acuerdo a que sean 
template <typename To_duration, typename Rep, typename Period>
constexpr inline __enable_if_is_duration<To_duration>
				duration_cast(const duration<Rep, Period>& d)
{
    constexpr ratio_divide<Period, typename To_duration::period> CF;
    using CR = common_type_t<typename To_duration::rep, Rep, intmax_t>;

    if constexpr (CF.num == 1 and CF.den == 1)	
        return To_duration(static_cast<typename To_duration::rep>(d.count()));
    
    else if (CF.num != 1 and CF.den == 1) 
	return To_duration(
	    static_cast<typename To_duration::rep>(
		    static_cast<CR>(d.count()) * static_cast<CR>(CF.num))
			  );

    else if (CF.num == 1 and CF.den != 1)
	return To_duration(static_cast<typename To_duration::rep>(
		    static_cast<CR>(d.count()) / static_cast<CR>(CF.den)));

    else
	return To_duration(static_cast<typename To_duration::rep>(
	    static_cast<CR>(
	    d.count()) * static_cast<CR>(CF.num) / static_cast<CR>(CF.den)));
}




/*!
 *  \brief  duration_values
 *
 *  Traits para que cualquiera pueda especializar para otras representaciones.
 */
template <typename Rep>
struct duration_values{
    // The value returned shall be the additive identity.
    static constexpr Rep zero() {return Rep{0};}

    // The value returned shall compare less than or equal to zero.
    static constexpr Rep min() {return numeric_limits<Rep>::lowest();}

    // The value returned shall compare greater than zero.
    static constexpr Rep max() {return numeric_limits<Rep>::max();}
};

/***************************************************************************
 *			    duration
 ***************************************************************************/
/*!
 *  \brief  duration
 *
 *  The tick period is the amount of time which occurs from one
 *  tick to the next, in units of seconds. (= the number of seconds from one
 *  tick to the next).
 *  Tenemos que: num_segundos = count() * period;
 */
template <typename Rep, typename Period0>
struct duration{
public:
    using rep    = Rep;
    using period = typename Period0::type;

    // preconditions. TODO: con concepts esto se puede simplificar.
    static_assert(__is_ratio<period>::value,
                  "period must be a specialization of ratio");
    static_assert(period::num > 0, "period must be positive");

// construction
    constexpr duration() = default;
    
    // TODO: falta requirements.
    template <typename Rep2> 
    constexpr explicit duration(const Rep2& num_ticks)
	: num_ticks_{static_cast<rep>(num_ticks)} { }

    // TODO: falta requirements.
    template <typename Rep2, typename Period2> 
    constexpr duration(const duration<Rep2, Period2>& d)
	:num_ticks_{duration_cast<duration>(d).count()} { }

    duration(const duration&) = default;
    duration& operator=(const duration&) = default;

    ~duration() = default;

// observer
    constexpr rep count() const { return num_ticks_;}

// arithmetic
    constexpr duration operator+() const;
    constexpr duration operator-() const;

    constexpr duration& operator++();
    constexpr duration operator++(int);

    constexpr duration& operator--();
    constexpr duration operator--(int);

    constexpr duration& operator+=(const duration& d);
    constexpr duration& operator-=(const duration& d);

    constexpr duration& operator*=(const rep& r);
    constexpr duration& operator/=(const rep& r);
    constexpr duration& operator%=(const rep& r);
    constexpr duration& operator%=(const duration& d);

    // special values
    static constexpr duration zero();
    static constexpr duration min();
    static constexpr duration max();

private:
    rep num_ticks_; // número de ticks que han transcurrido.
		    // el tiempo transcurrido será num_ticks_ * period 
		    // (en segundos).
};



template <typename R, typename P>
inline constexpr duration<R, P> duration<R, P>::operator+() const
{
    return *this;
}

template <typename R, typename P>
inline constexpr duration<R, P> duration<R, P>::operator-() const
{
    return duration(-num_ticks_);
}

template <typename R, typename P>
inline constexpr duration<R, P>& duration<R, P>::operator++()
{
    ++num_ticks_;
    return *this;
}

template <typename R, typename P>
inline constexpr duration<R, P> duration<R, P>::operator++(int)
{
    return duration{num_ticks_++};
}

template <typename R, typename P>
inline constexpr duration<R, P>& duration<R, P>::operator--()
{
    --num_ticks_;
    return *this;
}

template <typename R, typename P>
inline constexpr duration<R, P> duration<R, P>::operator--(int)
{
    return duration{num_ticks_--};
}

template <typename R, typename P>
inline constexpr duration<R, P>& duration<R, P>::
operator+=(const duration<R, P>& d)
{
    num_ticks_ += d.count();
    return *this;
}

template <typename R, typename P>
inline constexpr duration<R, P>& duration<R, P>::
operator-=(const duration<R, P>& d)
{
    num_ticks_ -= d.count();
    return *this;
}

template <typename R, typename P>
inline constexpr duration<R, P>& duration<R, P>::operator*=(const rep& r)
{
    num_ticks_ *= r;
    return *this;
}

template <typename R, typename P>
inline constexpr duration<R, P>& duration<R, P>::operator/=(const rep& r)
{
    num_ticks_ /= r;
    return *this;
}

template <typename R, typename P>
inline constexpr duration<R, P>& duration<R, P>::operator%=(const rep& r)
{
    num_ticks_ %= r;
    return *this;
}

template <typename R, typename P>
inline constexpr duration<R, P>& duration<R, P>::
operator%=(const duration<R, P>& d)
{
    num_ticks_ %= d.count();
    return *this;
}


// special values
template <typename R, typename P>
inline constexpr duration<R, P> duration<R, P>::zero()
{
    return duration(duration_values<rep>::zero());
}

template <typename R, typename P>
inline constexpr duration<R, P> duration<R, P>::min()
{
    return duration(duration_values<rep>::min());
}

template <typename R, typename P>
inline constexpr duration<R, P> duration<R, P>::max()
{
   return duration(duration_values<rep>::max());
}

}// namespace chrono


// -----------
// common_type
// -----------
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
struct common_type<chrono::duration<Rep1, Period1>,
                   chrono::duration<Rep2, Period2>> {
    using rep = common_type_t<Rep1, Rep2>;

    static constexpr intmax_t num = __static_gcd(Period1::num, Period2::num);
    static constexpr intmax_t den = __static_lcm(Period1::den, Period2::den);

    using period = ratio<num, den>;


    using type = chrono::duration<rep, period>;
};

namespace chrono {

// -------------------------------
// durations non-member arithmetic
// -------------------------------
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>>
operator+(const duration<Rep1, Period1>& a, const duration<Rep2, Period2>& b)
{
    using CD = common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>>;

    return CD{CD{a}.count() + CD{b}.count()};
}


template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>>
operator-(const duration<Rep1, Period1>& a, const duration<Rep2, Period2>& b)
{
    using CD = common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>>;

    return CD{CD{a}.count() - CD{b}.count()};
}

// SFINAE helper to obtain common_type<Rep1, Rep2> only if Rep2
// is implicitly convertible to it.
// if (is_convertible_v<const Rep2&, common_type_t<Rep1, Rep2>>)
//	return common_type_t<Rep1, Rep2>;
// else
//	compile_error();    
template <typename Rep1,
          typename Rep2,
          typename CRep = common_type_t<Rep1, Rep2>>
using __common_rep_t = enable_if_t<is_convertible_v<const Rep2&, CRep>, CRep>;


template <typename Rep1, typename Period, typename Rep2>
constexpr inline duration<__common_rep_t<Rep1, Rep2>, Period>
operator*(const duration<Rep1, Period>& d, const Rep2& n)
{
    using CD = duration<common_type_t<Rep1, Rep2>, Period>;

    return CD{CD{d}.count() * n};
}

template <typename Rep1, typename Rep2, typename Period>
constexpr inline duration<__common_rep_t<Rep2, Rep1>, Period>
operator*(const Rep1& n, const duration<Rep2, Period>& d)
{
    return d * n;
}

template <typename Rep1, typename Period, typename Rep2>
constexpr inline 
duration<__common_rep_t<Rep1, __disable_if_is_duration<Rep2>>,
         Period>
operator/(const duration<Rep1, Period>& d, const Rep2& n)
{
    using CD = duration<common_type_t<Rep1, Rep2>, Period>;

    return CD{CD{d}.count() / n};
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline common_type_t<Rep1, Rep2>
operator/(const duration<Rep1, Period1>& a, const duration<Rep2, Period2>& b)
{
    using D1 = duration<Rep1, Period1>;
    using D2 = duration<Rep2, Period2>;
    using CD = common_type_t<D1, D2>;

    return CD{a}.count() / CD{b}.count();
}

template <typename Rep1, typename Period, typename Rep2>
constexpr inline 
duration<__common_rep_t<Rep1, __disable_if_is_duration<Rep2>>,
         Period>
operator%(const duration<Rep1, Period>& d, const Rep2& n)
{
    using CD = duration<common_type_t<Rep1, Rep2>, Period>;

    return CD{CD{d}.count() % n};
}


template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline 
common_type_t<duration<Rep1, Period1>, 
	      duration<Rep2, Period2> >
operator%(const duration<Rep1, Period1>& a, const duration<Rep2, Period2>& b)
{
    using D1 = duration<Rep1, Period1>;
    using D2 = duration<Rep2, Period2>;
    using CD = common_type_t<D1, D2>;

    return CD{CD{a}.count() % CD{b}.count()};
}


// --------------------
// duration comparisons
// --------------------
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline 
bool operator==(const duration<Rep1, Period1>& a,
	        const duration<Rep2, Period2>& b)
{
    using D1 = duration<Rep1, Period1>;
    using D2 = duration<Rep2, Period2>;
    using CD = common_type_t<D1, D2>;

    return CD{a}.count() == CD{b}.count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline 
bool operator!=(const duration<Rep1, Period1>& a,
	        const duration<Rep2, Period2>& b)
{
    return !(a == b);
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline 
bool operator<(const duration<Rep1, Period1>& a,
	       const duration<Rep2, Period2>& b)
{
    using D1 = duration<Rep1, Period1>;
    using D2 = duration<Rep2, Period2>;
    using CD = common_type_t<D1, D2>;

    return CD{a}.count() < CD{b}.count();
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline 
bool operator<=(const duration<Rep1, Period1>& a,
	       const duration<Rep2, Period2>& b)
{
    return !(b < a);
}


template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline 
bool operator>(const duration<Rep1, Period1>& a,
	       const duration<Rep2, Period2>& b)
{
    return b < a;
}

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr inline 
bool operator>=(const duration<Rep1, Period1>& a,
	       const duration<Rep2, Period2>& b)
{
    return !(a < b);
}


/// nanoseconds
// At least 64 bits
using nanoseconds = duration<int64_t, nano>;

/// microseconds
// At least 55 bits
using microseconds = duration<int64_t, micro>;

/// milliseconds
// At least 45 bits
using milliseconds = duration<int64_t, milli>;

/// seconds
// At least 35 bits
using seconds = duration<int64_t>;

/// minutes
// At least 29 bits
using minutes = duration<int32_t, ratio< 60>>;

/// hours
// At least 23 bits
using hours = duration<int32_t, ratio<3600>>;

// syntactic sugar
template <typename From, typename To>
using __enable_if_is_convertible_t =
				enable_if_t<is_convertible_v<From, To>, From>;



/***************************************************************************
 *			    time_point
 ***************************************************************************/
/*!
 *  \brief  time_point
 *
 */
template <typename Clock, typename Duration>
class time_point {
public:
    // types
    // -----
    using clock	   = Clock;
    using duration = Duration;
    using rep      = typename duration::rep;
    using period   = typename duration::period;

    // construction
    // ------------
    constexpr time_point();
    constexpr explicit time_point(const duration& d);

    template <typename Duration2,
	 typename = __enable_if_is_convertible_t<Duration2, duration>>
    constexpr time_point(const time_point<clock, Duration2>& t)
        : d_{t.time_since_epoch()}
    { }

    // observer
    // --------
    constexpr duration time_since_epoch() const;

    // arithmetic
    // ----------
    constexpr time_point& operator+=(const duration& d);
    constexpr time_point& operator-=(const duration& d);

    // special values
    // --------------
    static constexpr time_point min();
    static constexpr time_point max();

private:
    duration d_;   
};


// construction
// ------------
template <typename C, typename D>
constexpr inline 
time_point<C, D>::time_point() 
	: d_{duration::zero()} { }

template <typename C, typename D>
constexpr inline 
time_point<C, D>::time_point(const duration& d)
	: d_{d} { }


// observer
// --------
template <typename C, typename D>
constexpr inline 
typename time_point<C,D>::duration time_point<C, D>::time_since_epoch() const
{ return d_;}


// arithmetic
// ----------
template <typename C, typename D>
constexpr inline 
time_point<C, D>& time_point<C, D>::operator+=(const duration& d)
{ 
    d_ += d;
    return *this;
}

template <typename C, typename D>
constexpr inline 
time_point<C, D>& time_point<C, D>::operator-=(const duration& d)
{ 
    d_ -= d;
    return *this;
}


// special values
// --------------
template <typename C, typename D>
constexpr inline 
time_point<C, D> time_point<C, D>::min()
{
    return time_point{duration::min()};
}


template <typename C, typename D>
constexpr inline 
time_point<C, D> time_point<C, D>::max()
{
    return time_point{duration::max()};
}


// ---------------------------------
// time_point non-memeber arithmetic
// ---------------------------------
template <typename Clock, 
	  typename Duration1, 
	  typename Rep2, typename Period2>
constexpr inline 
time_point<Clock, common_type_t<Duration1, duration<Rep2, Period2>>>
operator+(const time_point<Clock, Duration1>& t,
          const duration<Rep2, Period2>& d)
{
    using CD = common_type_t<Duration1, duration<Rep2, Period2>>;
    using TP = time_point<Clock, CD>;

    return TP{t.time_since_epoch() + d};
}

template <typename Clock, 
	  typename Duration1, 
	  typename Rep2, typename Period2>
constexpr inline 
time_point<Clock, common_type_t<Duration1, duration<Rep2, Period2>>>
operator+(const duration<Rep2, Period2>& d, 
	  const time_point<Clock, Duration1>& t)
{return t + d;}



template <typename Clock, 
	  typename Duration1, 
	  typename Rep2, typename Period2>
constexpr inline 
time_point<Clock, common_type_t<Duration1, duration<Rep2, Period2>>>
operator-(const time_point<Clock, Duration1>& t,
          const duration<Rep2, Period2>& d)
{
    using CD = common_type_t<Duration1, duration<Rep2, Period2>>;
    using TP = time_point<Clock, CD>;

    return TP{t.time_since_epoch() - d};
}

// Observar que solo podemos restar dos time_points y no sumarlos. ¿Qué
// sentido tendría sumar dos tiempos? Ninguno, por eso no se incluyó.
template <typename Clock, typename Duration1, typename Duration2>
constexpr inline 
common_type_t<Duration1, Duration2>
operator-(const time_point<Clock, Duration1>& b, // b - a
	  const time_point<Clock, Duration2>& a)
{
    return b.time_since_epoch() - a.time_since_epoch();
}


// ----------------------
// time_point comparisons
// ----------------------
template <typename Clock, typename Duration1, typename Duration2>
constexpr inline
bool operator==(const time_point<Clock, Duration1>& a, 
		 const time_point<Clock, Duration2>& b)
{
    return a.time_since_epoch() == b.time_since_epoch();
}

template <typename Clock, typename Duration1, typename Duration2>
constexpr inline
bool operator!=(const time_point<Clock, Duration1>& a, 
		 const time_point<Clock, Duration2>& b)
{
    return !(a == b);
}

template <typename Clock, typename Duration1, typename Duration2>
constexpr inline
bool operator<(const time_point<Clock, Duration1>& a, 
	       const time_point<Clock, Duration2>& b)
{
    return a.time_since_epoch() < b.time_since_epoch();
}

template <typename Clock, typename Duration1, typename Duration2>
constexpr inline
bool operator<=(const time_point<Clock, Duration1>& a, 
	       const time_point<Clock, Duration2>& b)
{
    return !(b < a);
}

template <typename Clock, typename Duration1, typename Duration2>
constexpr inline
bool operator>(const time_point<Clock, Duration1>& a, 
	       const time_point<Clock, Duration2>& b)
{
    return (b < a);
}

template <typename Clock, typename Duration1, typename Duration2>
constexpr inline
bool operator>=(const time_point<Clock, Duration1>& a, 
	       const time_point<Clock, Duration2>& b)
{
    return !(a < b);
}


// ---------------
// time_point_cast
// ---------------
template <typename To_duration, typename Clock, typename Duration>
constexpr inline 
enable_if_t<__is_duration_v<To_duration>, time_point<Clock, To_duration>>
time_point_cast(const time_point<Clock, Duration>& t)
{
    time_point<Clock, To_duration>{
			    duration_cast<To_duration>(t.time_since_epoch())};
}



/***************************************************************************
 *			    system_clock
 ***************************************************************************/
/*!
 *  \brief  system_clock
 *
 *  El reloj de pared. 
 *  Al usar la API de C (time) para medir el tiempo, mediremos en segundos.
 *
 */
// De acuerdo a cppreference time_t, aunque no está escrito en el standard de
// C, es el número de segundos. Por eso puedo garantizar que period = 1.
struct system_clock{
    using duration = chrono::seconds; // NOTA: si se cambia esto, hay que
                                      // reescribir las funciones from/to
    using rep        = duration::rep;
    using period     = duration::period;
    using time_point = chrono::time_point<system_clock, duration>;

    // Este assert procede del standard. Implica que rep tiene que ser un
    // signed type.
    static_assert(system_clock::duration::min() <
						system_clock::duration::zero(),
                  "rep must be a signed type");

    static constexpr bool is_steady = true;

    static time_point now() noexcept;

    // Map to C API
    static time_t to_time_t(const time_point& t) noexcept;
    static time_point from_time_t(time_t t) noexcept;
};


inline system_clock::time_point system_clock::now() noexcept
{
    return time_point{duration{::time(nullptr)}};
}

// en avr-libc, time_t está definido como uint32_t, time_point está usando
// seconds que tiene int64_t como representación. Como se ve con avr-libc no
// se pueden manejar time_t negativos (aunque sí time_points negativos).
inline time_t system_clock::to_time_t(const time_point& t) noexcept
{
    return static_cast<time_t>(t.time_since_epoch().count());
}


inline system_clock::time_point system_clock::from_time_t(time_t t) noexcept
{
    return time_point{duration{t}};
}


/***************************************************************************
 *			    utilities
 ***************************************************************************/
// abs
template <typename Rep, typename Period>
constexpr 
enable_if_t<numeric_limits<Rep>::is_signed, duration<Rep, Period>>
abs(duration<Rep, Period> d)
{ 
    if (d >= d.zero())
	return +d;

    else 
	return -d;
}


/***************************************************************************
 *			    hh_mm_ss
 ***************************************************************************/

// DUDA: ¿cómo llamar a las funciones de implementación?
// Las podemos meter dentro del namespace _std, o detail, o helper, o ... ???
// Necesito un convenio!!!
namespace detail{
// NOTA: La siguiente función es copia de la que tiene Howard Hinnant.
// 
// Para calcular el número de decimales que obtenemos al dividir n entre d, lo
// que hacemos es la división larga.
//
// Los de gcc creo que tienen un error en su implementación (???) Ellos para
// calcular el número de decimales lo que hacen es factorizar el denominador
// en potencias de 2 y de 5. Si no se puede factorizar completamente el número
// es irracional, siendo infinito el número de decimales que tienen. El error
// que tienen es que se han olvidado de incluir el númerador, aunque solo he
// ojeado el código.
//
// Calcula el número de decimales al dividir n entre d, máximo 19 decimales.
template <uint64_t n, uint64_t d, unsigned w = 0,
          bool should_continue = (n%d != 0) and (w < 19)>
struct ndecimals 
{
    static_assert(d > 0, "width called with zero denominator");
    static constexpr const unsigned value = 1 + ndecimals<n%d*10, d, w+1>::value;
};

template <uint64_t n, uint64_t d, unsigned w>
struct ndecimals<n, d, w, false>
{
    static constexpr const unsigned value = 0;
};


// static_pow10
// ------------
// Esta también es copia de Hinnant. 
template <unsigned exp>
struct static_pow10
{
private:
    static constexpr const uint64_t h = static_pow10<exp/2>::value;
public:
    static constexpr const uint64_t value = h * h * (exp % 2 ? 10 : 1);
};

template <>
struct static_pow10<0>
{
    static constexpr const uint64_t value = 1;
};
}// namespace
 
// ¿is_negative?
//	Supongamos que Duration está dada en segundos y que son 110 segundos.
//	Esta clase devuelve: +1h30m20s (1 hora, 30 minutos, 20 segundos).
//	Si en cambio la duración es de -110 segundos, lo que devuelve es 
//	-1h30m20s, donde el signo menos se indica con is_negative. 
template <typename Duration>
class hh_mm_ss{

    static unsigned constexpr const trial_width =
	    detail::ndecimals<Duration::period::num, 
			      Duration::period::den>::value;

public:
    static constexpr unsigned 
		fractional_width = trial_width < 19 ? trial_width : 6u;

    using precision = 
	duration<
		    common_type_t<typename Duration::rep, chrono::seconds::rep>,
		    ratio<1, detail::static_pow10<fractional_width>::value>
		>;

    constexpr hh_mm_ss() noexcept : hh_mm_ss{Duration::zero()}{}
    constexpr explicit hh_mm_ss(Duration d);

    constexpr bool is_negative() const noexcept	
    { return is_negative_;}

    constexpr chrono::hours hours() const noexcept 
    {return hours_;}

    constexpr chrono::minutes minutes() const noexcept 
    {return minutes_;}
    
    constexpr chrono::seconds seconds() const noexcept
    {return seconds_;}


    constexpr precision subseconds() const noexcept
    {return subseconds_;}

    constexpr explicit operator precision() const noexcept
    { return to_duration();}

    constexpr precision to_duration() const noexcept;

private:
    bool is_negative_;
    chrono::hours hours_;
    chrono::minutes minutes_;
    chrono::seconds seconds_;
    precision subseconds_;
};

template <typename Duration>
inline constexpr hh_mm_ss<Duration>::hh_mm_ss(Duration d)
    : is_negative_(d < Duration::zero()),
      hours_ (duration_cast<chrono::hours>(abs(d))),
      minutes_(duration_cast<chrono::minutes>(abs(d) - hours())),
      seconds_(duration_cast<chrono::seconds>(abs(d) - hours() - minutes()))
{
    if constexpr (treat_as_floating_point_v<typename precision::rep>)
	subseconds_ = abs(d) - hours() - minutes() - seconds();

    else
	subseconds_ = 
	    duration_cast<precision>(abs(d) - hours() - minutes() - seconds());

}

template <typename D>
inline constexpr hh_mm_ss<D>::precision hh_mm_ss<D>::to_duration() const noexcept
{
    if (is_negative())
	return -(hours() + minutes() + seconds() + subseconds());

    else
	return hours() + minutes() + seconds() + subseconds();
}

    

}// namespace chrono


}// namespace STD

#endif




