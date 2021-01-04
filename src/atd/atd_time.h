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

#include "atd_cstring.h"    // const_nstring


namespace atd{
// Construcción
// ------------
/// Inicializa la estructura std::tm a 0, salvo el campo del año que lo inicializa
/// al 2019.
void reset(std::tm& t);



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

struct Generic_time_traits{
    static constexpr int seconds_min = 0;
    static constexpr int seconds_max = 59;

    static constexpr int minutes_min = 0;
    static constexpr int minutes_max = 59;

    static constexpr int hours_min = 0;
    static constexpr int hours_max = 23;

    static constexpr int day_min = 1;
    static constexpr int day_max = 31;

    static constexpr int month_min = 1;
    static constexpr int month_max = 12;

    // (RRR) Empezamos en 0 para poder usar el array weekday_as_str
    static constexpr int weekday_min = 0;
    static constexpr int weekday_max = 6;
};


// DUDA: ¿cómo llamarlo?
// Generic_time_translator es el interfaz que suministra cada reloj indicando
// cómo traducir de su idioma al idioma de Generic_time. Ver
// Generic_time_translator<std::tm> para ver ejemplo de implementación.
// (RRR) El problema está en definir dos versiones de Generic_time: const y no
//       const. Si se deja hacer esto a quien implementa la clase de reloj le
//       toca implementar las dos versiones. El Generic_time_translator
//       es una forma de implementar Generic_time y const_Generic_time en
//       general y que solo sea necesario implementar Generic_time_translator
//       una sola vez.
template <typename T>
struct Generic_time_translator;


// versión const
template <typename T>
class const_Generic_time{
public:
    using GT = Generic_time_translator<T>;

// constructor
    const_Generic_time(const T& t):t_{t} {}

// members
    int seconds() const { return GT::seconds(t_); }
    int minutes() const { return GT::minutes(t_); }
    int hours() const   { return GT::hours(t_); }

    int day() const   { return GT::day(t_); }
    int month() const { return GT::month(t_); }
    int year() const  { return GT::year(t_);}

    // weekday = [0..6]
    int weekday() const {return GT::weekday(t_);}

private:
    const T& t_;
};


// Observar que al internamente solo almacenar una referncia se puede pasar
// por valor.
// versión no const
template <typename T>
class Generic_time{
public:
    using GT = Generic_time_translator<T>;

// constructor
    Generic_time(T& t):t_{t} {}

// Conversión implícita: no const a const (es lo de esperar)
    operator const_Generic_time<T>() const { return const_Generic_time{t_};}

// members
    int seconds() const { return GT::seconds(t_); }
    void seconds(int s) { GT::seconds(t_, s); }

    int minutes() const { return GT::minutes(t_); }
    void minutes(int m) { GT::minutes(t_, m); }

    int hours() const { return GT::hours(t_); }
    void hours(int h) { GT::hours(t_, h); }

    int day() const { return GT::day(t_); }
    void day(int d) { GT::day(t_, d); }

    int month() const { return GT::month(t_); }
    void month(int m) { GT::month(t_, m);}

    int year() const { return GT::year(t_);}
    void year(int y) { GT::year(t_, y);}

    int weekday() const {return GT::weekday(t_);}
    void weekday(int wd) {GT::weekday(t_, wd);}

private:
    T& t_;
};


// Funciones de impresión
// ----------------------
// El formato es español. Habría que definir locales para generalizarlo.
// (???) Se podía meter lo que dependa en español en un fichero _es.tcc y
// compilarlo condicionalmente.
template <typename T>
std::ostream&
print_time(std::ostream& out, const const_Generic_time<T>& t, char sep = ':')
{ 
    char f = out.fill('0');
    out << std::setw(2) << t.hours() << sep
	<< std::setw(2) << t.minutes() << sep
	<< std::setw(2) << t.seconds();

    out.fill(f);
    
    return out;
}

template <typename T>
std::ostream& print_date(std::ostream& out, const const_Generic_time<T>& t, char sep = '/')
{
    char f = out.fill('0');

    out << std::setw(2) << t.day() << sep
	<< std::setw(2) << t.month() << sep
	<< std::setw(4) << t.year();

    out.fill(f);

    return out;
}



template <size_t weekdays_length, typename T>
std::ostream& print_weekday(std::ostream& out,
                            const const_Generic_time<T>& t,
                            const char* weekdays)
{
    atd::Array_const_nstrings day{weekdays, weekdays_length};
    out << day[t.weekday()];

    return out;
}


// Versiones no const.
// TODO: estas versiones sobran. Deberían de poder convertirse directamente el
// Generic_time a const_Generic_time. Sin embargo, falla la deducción
// automática. ¿Por qué???
template <typename T>
inline std::ostream&
print_time(std::ostream& out, const Generic_time<T>& t, char sep = ':')
{ return print_time(out, const_Generic_time<T>{t}, sep); }

template <typename T>
inline std::ostream&
print_date(std::ostream& out, const Generic_time<T>& t, char sep = '/')
{ return print_date(out, const_Generic_time<T>{t}, sep); }

template <size_t weekdays_length, typename T>
std::ostream&
print_weekday(std::ostream& out, const Generic_time<T>& t, const char* weekdays)
{
    return 
	print_weekday<weekdays_length>(out, const_Generic_time<T>{t}, weekdays);
}


// Manipuladores de impresión
// --------------------------
template <typename GenT>
struct _Only_time{
    _Only_time(GenT t0, char sep0)
	: t{t0}, sep{sep0} {}

    GenT t;
    char sep;
};

// Bastaría con la versión const_Generic_time, pero no convierte de forma
// automática Generic_time en const_Generic_time (???)
template <typename T>
inline _Only_time<Generic_time<T>> only_time(Generic_time<T> t, char sep = ':')
{ return _Only_time<Generic_time<T>>{t, sep}; }

template <typename T>
inline _Only_time<const_Generic_time<T>> only_time(const_Generic_time<T> t,
                                                   char sep = ':')
{ return _Only_time<const_Generic_time<T>>{t, sep}; }

template <typename T>
inline std::ostream& operator<<(std::ostream& out, _Only_time<T> ot)
{
    print_time(out, ot.t, ot.sep);
    return out;
}


template <typename GenT>
struct _Only_date{
    _Only_date(GenT t0, char sep0)
	: t{t0}, sep{sep0} {}

    GenT t;
    char sep;
};

template <typename T>
inline _Only_date<Generic_time<T>> only_date(Generic_time<T> t, char sep = '/')
{ return _Only_date<Generic_time<T>>{t, sep}; }

template <typename T>
inline _Only_date<const_Generic_time<T>> only_date(const_Generic_time<T> t,
                                                   char sep = '/')
{ return _Only_date<const_Generic_time<T>>{t, sep}; }


template <typename T>
inline std::ostream& operator<<(std::ostream& out, _Only_date<T> ot)
{
    print_date(out, ot.t, ot.sep);
    return out;
}




// Especialización para std::tm
// ----------------------------
template<>
struct Generic_time_translator<std::tm>{
// members
    static int seconds(const std::tm& t) { return t.tm_sec; }
    static void seconds(std::tm& t, int s) { t.tm_sec = s; }

    static int minutes(const std::tm& t) { return t.tm_min; }
    static void minutes(std::tm& t, int m) { t.tm_min = m; }

    static int hours(const std::tm& t) { return t.tm_hour; }
    static void hours(std::tm& t, int h) { t.tm_hour = h; }

    static int day(const std::tm& t) { return t.tm_mday; }
    static void day(std::tm& t, int d) { t.tm_mday = d; }

    // tm_mon = (0-11)
    static int month(const std::tm& t) { return t.tm_mon + 1; }
    static void month(std::tm& t, int m) {t.tm_mon = m - 1;}

    /* tm_year = Year - 1900 */
    static int year(const std::tm& t) {return t.tm_year + 1900;}
    static void year(std::tm& t, int y) {t.tm_year = y - 1900;}

    /* tm_wday = weekday */
    static int weekday(const std::tm& t) {return t.tm_wday;}
    static void weekday(std::tm& t, int wd) {t.tm_wday = wd;}

};

// TODO: eliminar este a favor del manipulador de Generic_time.
//       Para poder eliminarlo faltaría crear los operadores >> en
//       Generic_time. ¿Merece la pena? ¿Los he usado alguna vez?
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

inline Only_date_no_const_ only_date(std::tm& t, char sep= '/') 
{return Only_date_no_const_{&t, sep};}

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





}// namespace



#endif
