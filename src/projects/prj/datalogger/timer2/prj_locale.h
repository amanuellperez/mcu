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

#ifndef __PRJ_LOCALE_H__
#define __PRJ_LOCALE_H__

#include <chrono>
#include <cctype>
#include <tuple>    // std::tie

template <typename Time_point>
void print_time(std::ostream& out, Time_point t0)
{ 
    std::chrono::hh_mm_ss t{t0.time_since_epoch()};

    out << t.hours().count() << " h " 
	<< t.minutes().count() << " min "
	<< t.seconds().count() << " s";
}

// convierte str -> t0
// El formato será: 
//	`3h 20min 43s`, donde h = horas, min = minutos, s = segundos
//
// DUDA: alguna forma standard de hacer esto?  (std::from_chars???)
//       Observar que la implementación es flexible en el orden en que se
//       pasen las horas, minutos y segundos. 

// Funciones de ayuda para implementar str2time_point
template <typename Time_point>
struct Str2time_point{
    enum class Unit{hours, minutes, seconds, unknown};

    static
    bool convert(const char* p, uint8_t str_size, Time_point& t0);

    static 
    std::pair<int8_t, const char*> read_int(const char* p, const char* pe);

    static 
    std::pair<Unit, const char*> read_unit(const char* p, const char* pe);

    static int8_t to_hours(int8_t x);
    static int8_t to_minutes(int8_t x);
    static int8_t to_seconds(int8_t x);
};


// Uso -1 para indicar error o valor no relleno.
template <typename Time_point>
bool Str2time_point<Time_point>::
		convert(const char* p, uint8_t str_size, Time_point& t0)
{
    int8_t hours   = -1;
    int8_t minutes = -1;
    int8_t seconds = -1;

    const char* pe = p + str_size;
    for (;*p != '\0' and p != pe; ++p){
	int n{};
	std::tie(n, p) = read_int(p, pe);
	if (n == -1)
	    return false;

	std::tie(unit, p) = read_unit(p, pe);
	switch (unit){
	    break; case Unit::hours  : hours   = to_hours(n);
	    break; case Unit::minutes: minutes = to_minutes(n);
	    break; case Unit::seconds: seconds = to_seconds(n);
	    break; case Unit::unknown: return false;
	}
    }

TODO: comprobar que hh_mm_ss es ok
    t0 = t.to_duration();
    return true;


}

template <typename Tp>
const char*
Str2time_point<Tp>::discard_spaces(const char* p, const char* pe)
{
    std::find_if_not(AQUIII
}

template <typename Tp>
std::pair<int8_t, const char*> 
Str2time_point<Tp>::read_int(const char* p, const char* pe)
{
    p = discard_spaces(p, pe);
}


// (RRR) ¿por qué no validar que sea menor que 24?
//       Porque no voy a usar días. Si quiero medir la temperatura cada 2 días
//       se pasaran cada 48 horas.
template <typename Tp>
inline int8_t Str2time_point<Tp>::to_hours(int8_t x)
{
    if (x < 0 /* or x > 23 */)
	return -1;

    return x;
}

template <typename Tp>
inline int8_t Str2time_point<Tp>::to_minutes(int8_t x)
{
    if (x < 0 or x > 59)
	return -1;

    return x;
}


template <typename Tp>
inline int8_t Str2time_point<Tp>::to_seconds(int8_t x)
{
    if (x < 0 or x > 59)
	return -1;

    return x;
}


// Devuelve true si consigue leer un tiempo, o false en caso contrario
// (formato incorrecto).
// El formato será: 
//	`3h 20min 43s`, donde h = horas, min = minutos, s = segundos
template <typename Time_point>
bool read_time(std::istream& in, Time_point& t0)
{
    constexpr uint8_t str_size = 20;

    char str[str_size];	    
    if (!in.getline(str, str_size, '\r'))// TODO:
	return false;		    // '\r' porque yo uso `screen` como terminal 
				    // not_generic!!!

    if (!Str2time_point::convert(str, str_size, t0))
	return false;

    return true;
}

#endif
