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
 *	27/12/2020 Generic_time
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
inline bool is_valid_date(const std::tm& t)
{
    return (1 <= t.tm_mday and t.tm_mday <= 31) and
           (0 <= t.tm_mon and t.tm_mon <= 11);
}

/// Indica si el tiempo tm_hour:tm_min:tm_sec esta en el dominio de tiempos.
inline bool is_valid_time(const std::tm& t)
{
    return (0 <= t.tm_hour and t.tm_hour <= 23) and
           (0 <= t.tm_min and t.tm_min <= 59)	and
           (0 <= t.tm_sec and t.tm_sec <= 59);
}


/*!
 *  \brief  Generic_time
 *
 *  Un RTC guarda la fecha en un formato, mientras que system_clock la guarda
 *  en otro. ¿Cómo poder usar funciones genéricas como user_get_time con
 *  diferentes tipos de relojes?
 *
 *  Posibles soluciones:
 *	1.- Convertir siempre todas las fechas a time_t y que user_get_time
 *	    devuelva también time_t. 
 *	    Problema: muy ineficiente, ya que el RTC DS1307 da la fecha "rota"
 *	    (en dia/mes/año ...). Para llamar a user_get_time hay que
 *	    convertirla en time_t, y a su vez user_get_time "rompe" de nuevo
 *	    la fecha para presentarsela al usuario, que la rellena; a
 *	    continuación user_get_time convierte esa fecha en time_t y el
 *	    programa vuelve a "romper" ese time_t para poder grabar la nueva
 *	    fecha en el RTC. Estamos continuamente pasando de fecha rota a
 *	    time_t perdiendo el tiempo. Introducimos ineficiencias gratuitas.
 *
 *	2.- Suministrar un user_get_time que admita directamente la fecha
 *	    rota, en lugar de time_t. Así el cliente elige si llamar a esta
 *	    versión a la que usa time_t. Problema: que cada RTC tendrá una
 *	    struct diferente para almacenar la fecha (esta struct es la que
 *	    escribimos en el traductor y dependerá de la datasheet). Luego
 *	    tenemos que crear una función diferente para cada struct.
 *
 *	    ¿Por qué no crear una única función user_get_time como template
 *	    parametrizada por esa struct? Correcto, pero ahora el problema
 *	    está en que std::tm y DS1307::Clock tienen nombres diferentes: el
 *	    año en tm es tm_year, mientras que en Clock es year. Necesito
 *	    "fusionar" estas diferentes formas de hablar. Esta struct
 *	    Generic_time es lo que hace.
 */
template <typename T>
class Generic_time;


// El formato es español. Habría que definir locales para generalizarlo.
template <typename T>
std::ostream& print_time(std::ostream& out, const Generic_time<T>& t, char sep = ':')
{ 
    char f = out.fill('0');
    out << std::setw(2) << t.hours() << sep
	<< std::setw(2) << t.minutes() << sep
	<< std::setw(2) << t.seconds();

    out.fill(f);
    
    return out;
}

template <typename T>
std::ostream& print_date(std::ostream& out, const Generic_time<T>& t, char sep = '/')
{
    char f = out.fill('0');

    out << std::setw(2) << t.day() << sep
	<< std::setw(2) << t.month() << sep
	<< std::setw(4) << t.year();

    out.fill(f);

    return out;
}


// Especialización para std::tm
// ----------------------------
template<>
class Generic_time<std::tm>{
public:
// constructor
    Generic_time(std::tm& t):t_{t} {}

// members
    int seconds() const { return t_.tm_sec; }
    void seconds(int s) { t_.tm_sec = s; }

    int minutes() const { return t_.tm_min; }
    void minutes(int m) { t_.tm_min = m; }

    int hours() const { return t_.tm_hour; }
    void hours(int h) { t_.tm_hour = h; }

    int day() const { return t_.tm_mday; }
    void day(int d) { t_.tm_mday = d; }

    // tm_mon = (0-11)
    int month() const { return t_.tm_mon + 1; }
    void month(int m) {t_.tm_mon = m - 1;}

    /* tm_year = Year - 1900 */
    int year() const {return t_.tm_year + 1900;}
    void year(int y) {t_.tm_year = y - 1900;}

private:
    std::tm& t_;
};





}// namespace



#endif
