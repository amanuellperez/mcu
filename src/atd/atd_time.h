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

#ifndef __ATD_TIME_H__
#define __ATD_TIME_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Funciones para manejo de tiempos
 *
 *  - COMENTARIOS:
 *
 *  - HISTORIA:
 *    A.Manuel López
 *	31/10/2019 Manipuladores de tiempo std::tm.
 *
 ****************************************************************************/
#include <ctime>
#include <ostream>
#include <istream>
#include <iomanip>


namespace atd{
// Construcción
// ------------
/// Inicializa la estructura std::tm a 0, salvo el campo del año que lo inicializa
/// al 2019.
void reset(std::tm& t);


// ---------------------
// Manipulador only_date
// ---------------------
struct Only_date_no_const_ {
    std::tm* t;
    char separador;
};

struct Only_date_const_ {
    const std::tm* t;
    char separador;
};

inline Only_date_no_const_ only_date(std::tm& t, char separador = '/') 
{return Only_date_no_const_{&t, separador};}

inline Only_date_const_ only_date(const std::tm& t, char separador = '/') 
{return Only_date_const_{&t, separador};}


std::ostream& print(std::ostream& out, const Only_date_const_& d);

inline std::ostream& operator<<(std::ostream& out, const Only_date_no_const_& d)
{ return print(out, Only_date_const_{d.t, d.separador}); }

inline std::ostream& operator<<(std::ostream& out, const Only_date_const_& d)
{ return print(out, d); }


std::istream& operator>>(std::istream& in, Only_date_no_const_ d);

// ---------------------
// Manipulador only_time
// ---------------------
struct Only_time_no_const_ {
    std::tm* t;
    char separador;
};

struct Only_time_const_ {
    const std::tm* t;
    char separador;
};

inline Only_time_no_const_ only_time(std::tm& t, char sep = ':') 
{return Only_time_no_const_{&t, sep};}

inline Only_time_const_ only_time(const std::tm& t, char sep = ':') 
{return Only_time_const_{&t, sep};}


std::ostream& print(std::ostream& out, const Only_time_const_& t);

inline std::ostream& operator<<(std::ostream& out, const Only_time_no_const_& d)
{return print(out, Only_time_const_{d.t, d.separador});}

inline std::ostream& operator<<(std::ostream& out, const Only_time_const_& d)
{return print(out, d);}

std::istream& operator>>(std::istream& in, Only_time_no_const_ d);


// Funciones de validación de fechas
// ---------------------------------
/// Indica si son válidos los campos tm_mday y tm_mon. No miro el año ya que
/// en principio puede valer cualquier número.
inline bool date_is_valid(const std::tm& t)
{
    return (1 <= t.tm_mday and t.tm_mday <= 31) and
           (0 <= t.tm_mon and t.tm_mon <= 11);
}

/// Indica si el tiempo tm_hour:tm_min:tm_sec esta en el dominio de tiempos.
inline bool time_is_valid(const std::tm& t)
{
    return (0 <= t.tm_hour and t.tm_hour <= 23) and
           (0 <= t.tm_min and t.tm_min <= 59)	and
           (0 <= t.tm_sec and t.tm_sec <= 59);
}

}// namespace



#endif
