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
#include <tuple>    // std::tie
#include <cstring>  // strlen
#include <cctype>   // isdigit
#include <string_view>
#include <iostream>

// Configuración de las cadenas de texto que puede introducir el usuario
// DEPENDE DEL locale!!!
constexpr const char str_space[] = " \t";
constexpr const char str_hours[] = "hours";
constexpr const char str_minutes[] = "minutes";
constexpr const char str_seconds[] = "seconds";


// Descarta los caracteres `chars` del principio de la cadena [p, pe)
const char* discard(const char* chars, const char* p, const char* pe);

inline const char* discard_spaces(const char* p, const char* pe)
{ return discard(str_space, p, pe); }


// Lee un `unsigned int`.
// Devuelve {número leído, puntero por donde seguir lectura}
// Devuelve -1 en caso de no leer ningún número válido.
std::pair<int, const char*> read_uint(const char* p, const char* pe);



enum class Time_unit{hours, minutes, seconds};

// Mira si en la cadena [p, pe) se encuentra la cadena [w, we) o parte de esa
// cadena.
// Ejemplo:
//	[w, we) = "hours", [p, pe) = "h 32min"
//	Como 'h' forma parte del principio de "hours" la función tiene éxito, 
//	devuelve el número de caracteres que tiene 'h'.
//
//	Si no tiene éxito devuelve 0.
//
// Devuelve el número de caracteres de la cadena [w, we) que ha encontrado en
// el principio de [p, pe).
size_t string_start_with(const char* w, const char* we, 
		      const char* p, const char* pe);

inline 
size_t read_unit_hours(const char* p, const char* pe)
{
    return string_start_with(str_hours, str_hours + std::strlen(str_hours),
			 p, pe);
}


inline 
size_t read_unit_minutes(const char* p, const char* pe)
{
    return string_start_with(str_minutes, str_minutes + std::strlen(str_minutes),
			 p, pe);
}



inline 
size_t read_unit_seconds(const char* p, const char* pe)
{
    return string_start_with(str_seconds, str_seconds + std::strlen(str_seconds),
			 p, pe);
}

// Devuelve la unidad de tiempo y un puntero por dónde continuar procesando la
// cadena [p, pe).
// Si no se encuentra una unidad de tiempo válida devuelve Time_unit::unknown
// y el puntero p que se pasó como parámetro (= no modifica los punteros [p, pe)
// en caso de no encontrar unidad)
std::pair<size_t, Time_unit>
read_time_unit(const char* p, const char* pe);


template <typename Tp>
void print_time(std::ostream& out, Tp t0)
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
// Observar que la implementación es flexible en el orden en que se
// pasen las horas, minutos y segundos. 

// Funciones de ayuda para implementar str2time_point
class Str2time_point{
public:

    // p = cadena de c
    template <typename Time_point>
    static bool convert(const char* p, Time_point& t0);

private:
    static int to_hours(int x);
    static int to_minutes(int x);
    static int to_seconds(int x);
    
    static bool is_valid_time(int hours, int minutes, int seconds);
};


template <typename Time_point>
bool Str2time_point::convert(const char* p, Time_point& t0)
{
    int hours   = 0;
    int minutes = 0;
    int seconds = 0;

    const char* pe = p + std::strlen(p);
    while(p != pe){
	int value{};
	p = discard_spaces(p, pe);
	std::tie(value, p) = read_uint(p, pe);
	if (value == -1)
	    return false;

	p = discard_spaces(p, pe);
	auto [n, unit] = read_time_unit(p, pe);
	if (n == 0)
	    return false;

	p += n;

	switch (unit){
	    break; case Time_unit::hours  : hours   = to_hours(value);
	    break; case Time_unit::minutes: minutes = to_minutes(value);
	    break; case Time_unit::seconds: seconds = to_seconds(value);
	}
    }
    if (!is_valid_time(hours, minutes, seconds))
	return false;

    std::chrono::hh_mm_ss t{std::chrono::hours{hours} +
			    std::chrono::minutes{minutes} +
			    std::chrono::seconds{seconds}};
    t0 = Time_point{t.to_duration()};
    return true;
}






// (RRR) ¿por qué no validar que sea menor que 24?
//       Porque no voy a usar días. Si quiero medir la temperatura cada 2 días
//       se pasaran cada 48 horas.
inline int Str2time_point::to_hours(int x)
{
    if (x < 0 /* or x > 23 */)
	return -1;

    return x;
}

inline int Str2time_point::to_minutes(int x)
{
    if (x < 0 or x > 59)
	return -1;

    return x;
}


inline int Str2time_point::to_seconds(int x)
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

    if (!Str2time_point::convert(str, t0))
	return false;

    return true;
}

#endif
