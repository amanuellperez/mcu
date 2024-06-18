// Copyright (C) 2019-2020 Manuel Perez 
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

#include "atd_time.h"
#include "atd_istream.h"

#include <type_traits>

namespace atd{

Time_ms::Time_ms(const std::chrono::milliseconds& t0)
{
    auto t = t0.count();

    milliseconds = t % 1000u;
    t /= 1000u;

    seconds = t % 60u;
    t /= 60u;

    minutes = t % 60u;
    t /= 60u;

    hours = t;
}



void reset(tm& t) 
{
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = 1;
    t.tm_mon = 0;
    t.tm_year = 2019 - 1900;
    t.tm_isdst = 0;
}


std::ostream& print(std::ostream& out, const Only_date_const_& d)
{
    char f = out.fill('0');
    out << std::setw(2) << d.t->tm_mday << d.separador
	<< std::setw(2) << d.t->tm_mon + 1 << d.separador
	<< std::setw(4) << 1900 + d.t->tm_year;

    out.fill(f);

    return out;
}


// Hay un problema con la struct tm definida en avr: no la define como el
// standard, sino como int8_t. Eso me da problemas a la hora de compilarlo con
// gcc. Para evitarlos uso la variable tmp.
// Returns: true = todo va bien, false = error.
static bool read(std::istream& in, Only_date_no_const_ d)
{
    char separador ='\0';   // para que no de el warning el compilador

    int8_t tmp;
    in >> atd::read_as_int8_t(tmp);
    d.t->tm_mday = tmp;

    in >> separador;
    if (separador != d.separador)
	return false;

    in >> atd::read_as_int8_t(tmp);
    d.t->tm_mon = tmp;
    in >> separador;
    if (separador != d.separador)
	return false;
    
    in >> d.t->tm_year;

    --d.t->tm_mon;
    d.t->tm_year -= 1900;

    return is_valid_date(*d.t);
}


std::istream& operator>>(std::istream& in, Only_date_no_const_ d)
{
    if (!read(in, d))
	in.setstate(std::ios_base::failbit);

    return in;
}


std::ostream& print(std::ostream& out, const Only_time_const_& t)
{
    char f = out.fill('0');
    out << std::setw(2) << t.t->tm_hour<< t.separador
	<< std::setw(2) << t.t->tm_min << t.separador
	<< std::setw(2) << t.t->tm_sec;

    out.fill(f);

    return out;
}

// Hay un problema con la struct tm definida en avr: no la define como el
// standard, sino como int8_t. Eso me da problemas a la hora de compilarlo con
// gcc. Para evitarlos uso la variable tmp.
static bool read(std::istream& in, Only_time_no_const_ d)
{
    char separador ='\0';   // para que no de el warning el compilador

    int8_t tmp;
    in >> atd::read_as_int8_t(tmp);
    d.t->tm_hour = tmp;
    in >> separador;
    if (separador != d.separador)
	return false;

    in >> atd::read_as_int8_t(tmp);
    d.t->tm_min = tmp;
    in >> separador;
    if (separador != d.separador)
	return false;
    
    in >> atd::read_as_int8_t(tmp);
    d.t->tm_sec = tmp;

    return is_valid_time(*d.t);
}

std::istream& operator>>(std::istream& in, Only_time_no_const_ d)
{
    if (!read(in, d))
	in.setstate(std::ios_base::failbit);

    return in;
}






}// namespace


