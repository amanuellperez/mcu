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

#include "prj_locale.h"
#include <ctype.h>

const char* discard(const char* chars, const char* p, const char* pe)
{
    size_t i0 = std::string_view{p, pe}.find_first_not_of(chars);

    if (i0 == std::string_view::npos)
	return p;

    return p + i0;
}


// precondición: std::isdigit(*p);
static 
std::pair<int, const char*> read_uint_unguarded(const char* p, const char* pe)
{
    int n = 0;

    for (; p != pe and isdigit(*p); ++p){

	int d = *p - '0';
	n = n*10 + d;
    }


    return {n, p};
}


std::pair<int, const char*> read_uint(const char* p, const char* pe)
{
//    p = discard_spaces(p, pe);

    if (p == pe or !isdigit(*p))
	return {-1, p};

    return read_uint_unguarded(p, pe);
}



size_t string_start_with(const char* w, const char* we, 
		      const char* p, const char* pe)
{
    size_t n = 0;
    while (w != we and p != pe and *p == *w){
	++p; ++w;
	++n;
    }

    return n;
}



// Devuelve {número de caracteres de la unidad encontrada, tipo de unidad}
// Time_unit tiene validez solo si el size_t devuelto es > 0.
std::pair<size_t, Time_unit>
read_time_unit(const char* p, const char* pe)
{
    if (p == pe)
	return {0, Time_unit::hours};

    size_t n = 0;
    Time_unit unit{};
    switch (*p){
	break; case 'h': 
	    n = read_unit_hours(p, pe);
	    unit = Time_unit::hours;

	break; case 'm': 
	    n = read_unit_minutes(p, pe);
	    unit = Time_unit::minutes;

	break; case 's': 
	    n = read_unit_seconds(p, pe);
	    unit = Time_unit::seconds;
    }

    p += n;
    if (p != pe and !isspace(*p))
	return {0, Time_unit::hours};

    return {n, unit};
}


// PARTICULAR: esta función es para esta aplicación.
// Vamos a admitir todo tipo de horas ya que no vamos a contar días.
bool Str2time_point::
is_valid_time(int hours, int minutes, int seconds)
{
    if (!(0 <= hours ))//and hours < 24))
	return false;

    if (!(0 <= minutes and minutes < 60))
	return false;

    if (!(0 <= seconds and seconds < 60))
	return false;

    return true;
}


