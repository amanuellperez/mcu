// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *    A.Manuel L.Perez
 *	08/12/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_ratio.h"
#include "std_type_traits.h"
#include "std_limits.h"
#include "std_ctime.h"


namespace STD{

namespace chrono {
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


/*!
 *  \brief  duration
 *
 *  The tick period is the amount of time which occurs from one
 *  tick to the next, in units of seconds. 
 *  Tenemos que: num_segundos = count() * period;
 */
template <typename Rep, typename Period>
struct duration{
    // preconditions. TODO: con concepts esto se puede simplificar.
    static_assert(__is_ratio<Period>::value,
                  "period must be a specialization of ratio");
    static_assert(Period::num > 0, "period must be positive");

public:
    using rep    = Rep;
    using period = typename Period::type;

    // construction
    // ------------
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

    // Ma to C API
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

}// namespace chrono


}// namespace STD

#endif




