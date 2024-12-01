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

#ifndef __DEV_DS1307_CLOCK_H__
#define __DEV_DS1307_CLOCK_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Time_point bases on DS1307
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    25/12/2020 v0.0
 *
 ****************************************************************************/
#include "dev_DS1307_hwd.h"

#include <chrono>
#include <atd_time.h>

namespace atd{
template<>
struct Generic_time_translator<dev::hwd::__DS1307_timekeeper>{
    
    using DS = dev::hwd::__DS1307_timekeeper;

    static int seconds(const DS& t) { return t.seconds; }
    static void seconds(DS& t, int s) { t.seconds= s; }

    static int minutes(const DS& t) { return t.minutes; }
    static void minutes(DS& t, int m) { t.minutes = m; }

    // OJO: de momento damos por supuesto que el reloj está configurado
    // en 00-23 horas
    static int hours(const DS& t) { return t.hours; }
    static void hours(DS& t, int h) { t.hours= h; }

    static int day(const DS& t) { return t.date; }
    static void day(DS& t, int d) { t.date = d; }

    static int month(const DS& t) { return t.month; }
    static void month(DS& t, int m) {t.month = m;}

    /* year = only 2 digits yy */
    static int year(const DS& t) {return t.year + 2000;}
    static void year(DS& t, int y) { t.year = static_cast<uint8_t>(y - 2000); }

    /* day = [1..7] ==> weekday = day - 1 */
    static int weekday(const DS& t) {return t.day - 1;}
    static void weekday(DS&t, int wd) { t.day = wd + 1;}
};

}// namespace atd


namespace dev{


/*!
 *  \brief  DS1307 as a standard clock.
 *
 *  Remember to define the correspondent TWI's ISR. Example:
 *
 *  ISR(TWI_vect)
 *  {
 *	TWI::handle_interrupt();
 *  }
 *
 */
template <typename TWI_master>
class DS1307_clock : public hwd::DS1307<TWI_master> {
public:
// From DS1307
    using Time_point = hwd::DS1307<TWI_master>::Time_point;

// As a clock
    using duration  = std::chrono::seconds; 
    using rep       = duration::rep;
    using period    = duration::period;

    using time_point = std::chrono::time_point<DS1307_clock, duration>;
    static constexpr bool is_steady = true;

    void turn_on(Time_point& t);

    // Be carefull: is more efficient to call read instead of 'now'.
    // now: first call read, then translate Time_point to time_point.
    time_point now() noexcept;

    // Map to C API
    static time_t to_time_t(const time_point& t) noexcept;
    static time_point from_time_t(time_t t) noexcept;

    static time_t to_time_t(const Time_point& t);
    static std::tm to_tm(const Time_point& t);

};


// Para saber si ha ocurrido algún error mirar el estado del RTC.
template <typename TWI>
inline void DS1307_clock<TWI>::turn_on(Time_point& t) 
{
    t.clock_on = true;
    this->write(t);
}



// Para ver si ha ocurrido algún error mirar el state del RTC.
template <typename TWI>
inline DS1307_clock<TWI>::time_point DS1307_clock<TWI>::now() noexcept
{
    Time_point t;
    this->read(t); // no compila sin el 'this' (error: unqualified lookup @_@)

    return time_point{duration{to_time_t(t)}};
}


template <typename TWI>
std::tm DS1307_clock<TWI>::to_tm(const Time_point& t)
{
    std::tm res;

    res.tm_sec  = t.seconds;
    res.tm_min  = t.minutes;
    res.tm_hour = t.hours;
    res.tm_mday = t.date;
    res.tm_wday = t.day - 1;
    res.tm_mon  = t.month - 1;
    res.tm_year = (2000 + t.year) - 1900;

    return res;
}



template <typename TWI>
inline time_t DS1307_clock<TWI>::to_time_t(const Time_point& t0)
{
    std::tm tm0 = to_tm(t0);
    return ::mktime(&tm0);
}



// en avr-libc, time_t está definido como uint32_t, time_point está usando
// seconds que tiene int64_t como representación. Como se ve con avr-libc no
// se pueden manejar time_t negativos (aunque sí time_points negativos).
template <typename TWI>
inline time_t DS1307_clock<TWI>::to_time_t(const time_point& t) noexcept
{
    return static_cast<time_t>(t.time_since_epoch().count());
}


template <typename TWI>
inline DS1307_clock<TWI>::time_point DS1307_clock<TWI>::from_time_t(time_t t) noexcept
{
    return time_point{duration{t}};
}

} // namespace






#endif



