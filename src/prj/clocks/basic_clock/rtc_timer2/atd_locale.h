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

#ifndef __ATD_LOCALE_H__
#define __ATD_LOCALE_H__

#include <ostream>

#include <atd_names.h>
#include <atd_type_traits.h>

// EXPERIMENTAL: 
//	¿Cómo puedo definir el formato de la fecha?
//	Dependiendo del país el formato de la fecha sera uno u otro.
//	TODO: nombre? POSIX? c++?
namespace locale{
    enum class Format_time{ spain, usa }; // TODO: estos nombres son temporales


template <typename Date_time>
void print_datetime_spain(std::ostream& out, const Date_time& t)
{
    atd::print(out, t.day(), nm::Width{2});
    out << '/';
    atd::print(out, t.month(), nm::Width{2});
    out << '/';
    atd::print(out, t.year(), nm::Width{2});
    out << ' ';

    atd::print(out, t.hours(), nm::Width{2});
    out << ':';
    atd::print(out, t.minutes(), nm::Width{2});
    out << ':';
    atd::print(out, t.seconds(), nm::Width{2});
}


template <typename Date_time>
void print_datetime_usa(std::ostream& out, const Date_time& t)
{
    atd::print(out, t.year(), nm::Width{2});
    out << '/';
    atd::print(out, t.month(), nm::Width{2});
    out << '/';
    atd::print(out, t.day(), nm::Width{2});
    out << ' ';

    atd::print(out, t.hours(), nm::Width{2});
    out << ':';
    atd::print(out, t.minutes(), nm::Width{2});
    out << ':';
    atd::print(out, t.seconds(), nm::Width{2});
}


template <Format_time f, typename Date_time>
void print_datetime(std::ostream& out, const Date_time& t)
{
    if constexpr (f == Format_time::spain)
	print_datetime_spain(out, t);

    else if constexpr (f == Format_time::usa)
	print_datetime_usa(out, t);

    else
	static_assert(atd::always_false_v<Date_time>, "Unknown Format_time");

}



}// namespace
 

#endif


