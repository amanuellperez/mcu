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

#ifndef __ATD_TIME_H__
#define __ATD_TIME_H__

/****************************************************************************
 *
 *  DESCRIPCION
 *	Funciones para manejo de tiempos
 *
 *  TODO
 *	Si se actualiza std::chrono a C++20 seguramente se pueda eliminar todo
 *	este fichero. (???)
 *
 *  HISTORIA
 *    Manuel Perez
 *	31/10/2019 Manipuladores de tiempo std::tm.
 *	27/12/2020 Generic_time_view
 *	08/03/2022 Generic_time_view (temporal hasta reescribir Generic_time_view)
 *	05/01/2023 Simplifico implementación Generic_time_view con requires
 *		   y renombro a Date_time_view
 *	07/01/2023 Date_time/Time_ms
 *
 *
 ****************************************************************************/
#include <ctime>
#include <ostream>
#include <istream>
#include <iomanip>
#include <chrono>

#include "atd_cstring.h"    // const_nstring
#include "atd_ostream.h"

namespace atd{

// TIME
//  A la hora de hablar hablamos de fechas (date) y qué hora es (time).
//  Cuando preguntamos la hora solemos responder (horas, minutos).
//
//	Ejemplo: 
//	    ¿qué hora es?  Las 12:45
//
//  La resolución con la que medimos el tiempo es con minutos. Sin embargo, la
//  resolución de un reloj típica es de segundos. Esto sugiere que podemos
//  medir el tiempo con diferentes resoluciones:
//
//	Time	: mide hasta segundos.
//	Time_ms	: mide milisegundos.
//	Time_us	: mide microsegundos.
//
//  Estas clases representan la hora en sexagesimal. Para representarla en
//  decimal tenemos std::chrono::seconds/milliseconds/microseconds.
//
//  Los programas funcionarán con la clase de chrono, pero a la hora de
//  presentársela al usuario final lo normal es presentarla en sexagesimal
//  (std::chrono en C++20 creo que suministra funciones para hacer esto,
//  pero... hay que leerlo, implementarlo... Si se hace se podrá eliminar este
//  fichero)
//

/***************************************************************************
 *			    Time_ms
 ***************************************************************************/
// Como no tengo que tener en cuenta años bisiestos ni la epoch inicial en
// Time_ms puedo implementarlo como un vulgar interfaz de conversión de
// Time_ms a milliseconds y viceversa.
/// Time_ms representa la hora medida con una resolución de hasta el
/// milisegundo.
struct Time_ms{
    Time_ms() {} // inicializa a 0

    explicit Time_ms(const std::chrono::milliseconds&);
    std::chrono::milliseconds as_milliseconds() const;

    uint8_t hours = 0;		// [0, 255)	<-- observar que no se limita a 1 dia
    uint8_t minutes = 0;	// [0, 60)
    uint8_t seconds = 0;	// [0, 60)
    uint16_t milliseconds = 0;  // [0, 1000)
};


inline std::chrono::milliseconds Time_ms::as_milliseconds() const
{
    std::chrono::milliseconds::rep t = 
	((hours * 60 + minutes) * 60 + seconds) * 1000 + milliseconds;

    return std::chrono::milliseconds{t};
}



/***************************************************************************
 *			    Date_time
 ***************************************************************************/
// Construcción
// ------------
/// Inicializa la estructura std::tm a 0, salvo el campo del año que lo inicializa
/// al 2019.
//TODO: mover a Date_time o eliminar
void reset(std::tm& t);



// Funciones de validación de fechas
// ---------------------------------
/// Indica si son válidos los campos tm_mday y tm_mon. No miro el año ya que
/// en principio puede valer cualquier número.
//TODO: mover a Date_time o eliminar (nombre: ok()?)
inline bool is_valid_date(const std::tm& t)
{
    return (1 <= t.tm_mday and t.tm_mday <= 31) and
           (0 <= t.tm_mon and t.tm_mon <= 11);
}

/// Indica si el tiempo tm_hour:tm_min:tm_sec esta en el dominio de tiempos.
//TODO: mover a Date_time o eliminar (nombre: ok()?)
inline bool is_valid_time(const std::tm& t)
{
    return (0 <= t.tm_hour and t.tm_hour <= 23) and
           (0 <= t.tm_min and t.tm_min <= 59)	and
           (0 <= t.tm_sec and t.tm_sec <= 59);
}

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
    static constexpr int weekday_min = 0; // Sunday
    static constexpr int weekday_max = 6;
};

// Unifico el interfaz de std::tm a mi interfaz genérico.
// (RRR) Quiero poder poner en hora los relojes Clock_s, ... y sinceramente
//	 me resulta un lío recordar el interfaz de funciones de C para manejar
//	 std::tm. Por eso creo este interfaz: porque me resulta muy sencillo
//	 de manejar.
class Date_time{
public:
    Date_time() {tm_.tm_isdst = -1; } // -1 = desconozco su valor
				     //	     Que lo calcule mktime cuando se
				     //	     llame.
// Machine form
    // CUIDADO: la inversa de mktime es localtime_r!!!
    void from_time_t(const std::time_t& t) { ::localtime_r(&t, &tm_); }
    // (RRR) Esta función es de consulta, tiene que ser const. El problema es
    //	     que mktime modifica tm_. Aunque al principio intenté dejarla como
    //	     no const, eso se propaga por el resto del código generando código
    //	     muy raro. Tiene que ser const!!!
    std::time_t to_time_t() const { return ::mktime(&((tm&)tm_));}

// Human form
    int seconds() const { return tm_.tm_sec; }
    void seconds(int s) { tm_.tm_sec = s; }

    int minutes() const { return tm_.tm_min; }
    void minutes(int m) { tm_.tm_min = m; }

    int hours() const { return tm_.tm_hour; }
    void hours(int h) { tm_.tm_hour = h; }

    int day() const { return tm_.tm_mday; }
    void day(int d) { tm_.tm_mday = d; }

    // Devuelve el mes de [1, 12]
    int month() const { return tm_.tm_mon + 1; }
    void month(int m) {tm_.tm_mon = m - 1;}

    // Devuelve el año con los 4 dígitos (el año).
    int year() const {return tm_.tm_year + 1900;}
    void year(int y) {tm_.tm_year = y - 1900;}

    /* tm_wday = weekday */
    int weekday() const {return tm_.tm_wday;}
    void weekday(int wd) {tm_.tm_wday = wd;}
    
private:
    std::tm tm_;   
};





/***************************************************************************
 *			    Date_time_view
 ***************************************************************************/
/*!
 *  \brief  Date_time_view
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
 *	    Date_time_view es lo que hace.
 */



// DUDA: ¿cómo llamarlo?
// Generic_time_translator es el interfaz que suministra cada reloj indicando
// cómo traducir de su idioma al idioma de Date_time_view. Ver
// Generic_time_translator<std::tm> para ver ejemplo de implementación.
// (RRR) El problema está en definir dos versiones de Date_time_view: const 
//	 y no const. Si se deja hacer esto a quien implementa la clase de reloj 
//	 le toca implementar las dos versiones. El Generic_time_translator
//       es una forma de implementar Date_time_view y const_Time_view en
//       general y que solo sea necesario implementar Generic_time_translator
//       una sola vez.
template <typename T>
struct Generic_time_translator;


// Observar que al internamente solo almacenar una referncia se puede pasar
// por valor.
// versión no const
template <typename T>
class Date_time_view{
public:
    using Time = std::remove_cv_t<T>;
    using GT = Generic_time_translator<Time>;

// constructor
    Date_time_view(T& t):t_{t} {}

// Conversión implícita: no const a const (es lo de esperar)
    operator Date_time_view<const Time>() const 
	requires (!std::is_const_v<T>)
	{ return Date_time_view<const Time>{t_};}

// members
    int seconds() const { return GT::seconds(t_); }
    void seconds(int s)
	requires (!std::is_const_v<T>)
	{ GT::seconds(t_, s); }

    int minutes() const { return GT::minutes(t_); }
    void minutes(int m) 
	requires (!std::is_const_v<T>)
	{ GT::minutes(t_, m); }

    int hours() const { return GT::hours(t_); }
    void hours(int h) 
	requires (!std::is_const_v<T>)
	{ GT::hours(t_, h); }

    int day() const { return GT::day(t_); }
    void day(int d) 
	requires (!std::is_const_v<T>)
	{ GT::day(t_, d); }

    int month() const { return GT::month(t_); }
    void month(int m) 
	requires (!std::is_const_v<T>)
	{ GT::month(t_, m);}

    int year() const { return GT::year(t_);}
    void year(int y) 
	requires (!std::is_const_v<T>)
	{ GT::year(t_, y);}

    int weekday() const {return GT::weekday(t_);}
    void weekday(int wd) 
	requires (!std::is_const_v<T>)
	{GT::weekday(t_, wd);}

private:
    T& t_;
};


template <typename T>
using const_Time_view = Date_time_view<T>;


// Funciones de impresión
// ----------------------
// El formato es español. Habría que definir locales para generalizarlo.
// (???) Se podía meter lo que dependa en español en un fichero _es.tcc y
// compilarlo condicionalmente.

template <typename Out, typename T>
Out&
print_time(Out& out, const const_Time_view<T>& t, char sep = ':')
{ 
//    char f = out.fill('0');
    print(out, t.hours(), nm::Width{2});
    print(out, sep);
    print(out, t.minutes(), nm::Width{2});
    print(out, sep);
    print(out, t.seconds(), nm::Width{2});
 //   out.fill(f);
    
    return out;
}

template <typename T>
inline std::ostream&
print_time(std::ostream& out, const const_Time_view<T>& t, char sep = ':')
{ return print_time<std::ostream, T>(out, t, sep); }

template <typename Out, typename T>
Out& print_date(Out& out, const const_Time_view<T>& t, char sep = '/')
{
//    char f = out.fill('0');

    print(out, t.day(), nm::Width{2});
    print(out, sep);
    print(out, t.month(), nm::Width{2});
    print(out, sep);
    print(out, t.year(), nm::Width{2});

//    out.fill(f);

    return out;
}

template <typename T>
inline std::ostream&
print_date(std::ostream& out, const const_Time_view<T>& t, char sep = '/')
{ return print_date<std::ostream, T>(out, t, sep); }

// Paso Out como segundo parámetro para que funcione la deducción automática
// al llamar a print_weekday<weekdays_length>
template <size_t weekdays_length, typename Out, typename Date_time_t>
Out& print_weekday(Out& out, const Date_time_t& t,
                             const char* weekdays)
{
    atd::Array_const_nstrings day{weekdays, weekdays_length};
    print(out, day[t.weekday()]);

    return out;
}

template <size_t weekdays_length, typename Date_time_t>
std::ostream& print_weekday(std::ostream& out,
                            const Date_time_t& t,
                            const char* weekdays)
{ return print_weekday<weekdays_length, std::ostream, Date_time_t>(out, t, weekdays); }



// Manipuladores de impresión
// --------------------------
template <typename GenT>
struct _Only_time{
    _Only_time(GenT t0, char sep0)
	: t{t0}, sep{sep0} {}

    GenT t;
    char sep;
};

// Bastaría con la versión const_Time_view, pero no convierte de forma
// automática Date_time_view en const_Time_view (???)
template <typename T>
inline _Only_time<Date_time_view<T>> only_time(Date_time_view<T> t, char sep = ':')
{ return _Only_time<Date_time_view<T>>{t, sep}; }


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
inline _Only_date<Date_time_view<T>> only_date(Date_time_view<T> t, char sep = '/')
{ return _Only_date<Date_time_view<T>>{t, sep}; }


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

// TODO: eliminar este a favor del manipulador de Date_time_view.
//       Para poder eliminarlo faltaría crear los operadores >> en
//       Date_time_view. ¿Merece la pena? ¿Los he usado alguna vez?
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
