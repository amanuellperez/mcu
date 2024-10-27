// Copyright (C) 2022 Manuel Perez 
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

#ifndef __MEGA_MEMORY_H__
#define __MEGA_MEMORY_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Funciones para acceder a la memoria: RAM, flash, eeprom
 *
 *  TODO
 *	Lo ideal sería poder definir:
 *		avr::ROM_uint8_t x{4};
 *		avr::ROM_array<uint8_t, 4> a{1, 2,3,4};
 *
 *	y poder usarlas.
 *
 *	A día de hoy hay que indicar explícitamente PROGMEM. Lo anterior se
 *	escribe como
 *		avr::ROM_uint8_t PROGMEM x{4};
 *		avr::ROM_array<uint8_t, 4> PROGMEM a{1, 2,3,4};
 *
 *	¿cómo poder eliminar el PROGMEM para que el usuario de la clase no
 *	tenga que recordar ponerlo?
 *
 *	    
 *  HISTORIA
 *    Manuel Perez
 *    16/01/2022 v0.0 Experimental.
 *                    Suministro dos interfaces:
 *                    1. Uno es contenedor ("propietario" de la memoria)
 *                    2. El otro es una view.
 *
 *    19/11/2022 atd::print
 *    23/12/2022 begin/end
 *    06/12/2023 Pruebo a concebir la memoria del microcontrolador de dos
 *               tipos: RAM y ROM. De esta forma podemos definir un interfaz
 *               genérico para los microcontroladores: si se quiere acceder a
 *               la ROM, usar ROM_uint8_t, ROM_uint16_t, ... que serán los
 *               mismos nombres sea cual sea el microcontrolador (ese es el
 *               ideal).
 *
 ****************************************************************************/
#include <ostream>  // std::ostream
#include <avr/pgmspace.h>   
#include <type_traits>
#include <initializer_list>
#include <atd_type_traits.h>
#include <atd_iterator.h>
#include <atd_rom.h>

#include "mega_import_avr.h"

// (RRR) El problema que tengo con el manejo de la ROM (= progmem) es que el
// compilador necesita la macro PROGMEM en avr para meterla en ROM. ¿Cómo
// generalizar eso?
// De momento, voy a suponer que la macro PROGMEM, o una equivalente, la
// suministran todos los compiladores. Si tiene otro nombre, en el archivo
// correspondiente a este del micro definiré:
// #define PROGMEM FORMA_DE_DEFINIRLO_EN_ESE_MICRO
// y de esa forma puedo considerar que la macro PROGMEM es genérica.
// ¿Cómo poder generalizarlo? ¿Alguna idea?

namespace atd{
// pgm_read_xxx son macros, no se pueden escribir como mega_::pgm_read_byte
inline uint8_t progmem_read(const uint8_t& x) { return pgm_read_byte(&x); }
inline uint16_t progmem_read(const uint16_t& x) { return pgm_read_word(&x); }
// Si los undef no funciona el test. Modificar el test y hacer el undef.
//#undef pgm_read_byte
//#undef pgm_read_word
} // namespace

// Es fundamental definir la función progmem_read para built-in types antes de
// los concepts. Por ello este orden no se puede cambiar.
#include <atd_memory.h>


/***************************************************************************
 *				PROGMEM
 *
 *  Objetivo: que a la hora de programar no tengas que saber si tienes
 *  almacenada la memoria en RAM o Flash (o donde sea). 
 *
 *  Problemas: los arrays de strings dejan mucho que desear para conseguir ese
 *  objetivo. 
 *	+ Para definirlas hay que definir las cadenas por un lado y por otro
 *	  el array:
 *
 *		constexpr const char str1[] PROGMEM = "zero";
 *		constexpr const char str2[] PROGMEM = "one";
 *		constexpr const char str3[] PROGMEM = "two";
 *		constexpr avr::Progmem_string_array<3> parray_str PROGMEM = {
 *		       str1, str2, str3
 *	        };
 *
 *	  ¿Por qué hay que especificar PROGMEM si ya debería de venir incluido
 *	  en el tipo? 
 *
 *	  Lo ideal sería poder definirlas:
 *		constexpr avr::Progmem_string_array<3> str = {
 *		    "zero, "one", "two"
 *	        };
 *
 *	        y luego poder usarlas:
 *		    for (size_t i = 0; i < str.size(); ++i)
 *			uart << str[i] << '\n';
 *
 *	  (???) Posible implementación (opción 1)(???)
 *	  Quizás una forma de hacerlo sería almacenar en la PROGMEM todas las
 *	  cadenas seguidas: {"zero", "one", "two"} lo almacenamos realmente
 *	  como "zeroonetwo" sin los '\0' de C. Para ello necesitaría poder
 *	  extraer en tiempo de compilación el número de cadenas que hay y
 *	  los tamaños que tienen cada una de ellas guardando toda esta
 *	  información en un array constexpr (que no exista durante la
 *	  ejecución del programa y no ocupe espacio). De esta forma no
 *	  almacenamos un array sino un solo puntero, que sería el único que
 *	  hay que definir como PROGMEM. El operador `str[i]` lo que haría
 *	  sería calcular en tiempo de compilación las posiciones [begin, end)
 *	  de la cadena a la que se quiere acceder y... ¿qué hacer? ¿copiar el
 *	  contenido en un buffer[] interno? El problema con eso es que 
 *	  probablemente sería error-prone.
 *	  Esta opción no está nada clara ni cómo implementarla ni el interfaz.
 *
 *    + Toda esta memoria siempre hay que definirla global, nunca definirla
 *      localmente lo cual no tiene mucho sentido. 
 *      Al definirla globalmente el destructor nunca se va a llamar (en
 *      principio solo cuando se apague el micro y en ese caso no importa
 *      mucho) con lo que no hay problemas con el destructor.
 *      ¿Se podría imponer de alguna forma que se tiene que definir de forma
 *      global? 
 *
 ***************************************************************************/



namespace mega_{




// Strings
// -------
template <size_t N>
class Progmem_string{
public:
    using size_type = size_t;

    constexpr static size_type size() {return N;}

    const char operator[](size_type i) const
    {   return pgm_read_byte(&data[i]); }

    template<typename T2>
    Progmem_string& operator=(const T2&) = delete;

    // Al principio iba a definir data como 'private'. Sin embargo, si se
    // define como private da error al compilar (ya que no inicializa por
    // defecto el data). Definiéndolo como const evito el problema de
    // escritura. El usuario tiene que saber que no debe de leer directamente
    // 'data'.
    // El +1 ya que se trata de una cadena de C que acaba en \0
    const char data[N + 1];
};


// Arrays de cadenas
// ----------------
template <size_t N>
struct Element_progmem_string_array;

// Para manejar arrays de cadenas.
template <size_t N>
class Progmem_string_array{
public:
    using size_type = size_t;

    constexpr static size_type size() {return N;}

// No parece que tenga mucho sentido copiar un progmem. De hecho, si se hace
// en tiempo de ejecución funciona mal. Conviene marcar como 'delete' el copy
// constructor, pero si se hace se elimina el constructor por defecto
// necesario para inicializar `data`.
//    Progmem_string_array(const Progmem_string_array&) = delete;

    template<typename T2>
    Progmem_string_array& operator=(const T2&) = delete;

    Element_progmem_string_array<N> operator[](size_type i) const;

    const char* const data[N];
};


// ¿Por qué no definir Progmem_string_array::Element?
// Al principio lo intenté hacer así pero genera un problema con la deducción
// automática de tipos al llamar strcpy. Por eso lo dejo fuera.
// Como en principio es una clase "privada" le pongo un nombre largo: nadie
// debería de usarla.
template <size_t N>
struct Element_progmem_string_array
{
    Element_progmem_string_array(const Progmem_string_array<N>& arr0, size_t i0)
        : arr{arr0}, i{i0}
    { }

    const Progmem_string_array<N>& arr;
    size_t i; 
};


template <size_t N>
Element_progmem_string_array<N> Progmem_string_array<N>::operator[](size_type i) const
{return Element_progmem_string_array<N>{*this, i};}


// Manejamos el array como memoria normal y corriente
template <size_t N>
inline char* strcpy(char* dst, const Element_progmem_string_array<N>& src)
{ return strcpy_P(dst, (PGM_P) pgm_read_word(&(src.arr.data[src.i]))); }

// Es más eficiente y segura strlcpy que strncpy
template <size_t N>
inline char*
strncpy(char* dst, const Element_progmem_string_array<N>& src, size_t n)
{ return strncpy_P(dst, (PGM_P) pgm_read_word(&(src.arr.data[src.i])), n); }

template <size_t N>
inline size_t
strlcpy(char* dst, const Element_progmem_string_array<N>& src, size_t n)
{ return strlcpy_P(dst, (PGM_P) pgm_read_word(&(src.arr.data[src.i])), n); }




///***************************************************************************
// *				VIEWS
// *
// *  No tengo claro de que las anteriores implementaciones funcionen siempre.
// *  Aunque me gusta más la forma anterior voy a probar a crear views de las
// *  progmems. Las views se limitan a suministrar un interfaz más cómodo de
// *  acceso a la PROGMEM.
// *  
// *  TODO: eliminarlo. Lo uso en el LCD
// ***************************************************************************/
//// Se trata de un array en progmem. Como no es propietario de los datos sería 
//// un Progmem_array_view????
//template <typename T, size_t N>
//class Progmem_array_view{
//public:
//    using size_type = size_t;
//
//    static_assert(std::is_same_v<T, uint8_t> 
//	       or std::is_same_v<T, uint16_t>);
//
//    Progmem_array_view(const T* data):data_{data}{}
//
//    constexpr static size_type size() {return N;}
//
//    T operator[](size_type i) const
//    {
//	if constexpr (std::is_same_v<T, uint8_t>)
//	    return pgm_read_byte(&data_[i]);
//
//	else if constexpr (std::is_same_v<T, uint16_t>)
//	    return pgm_read_word(&data_[i]);
//
//	else
//	    static_assert(atd::always_false_v<int>, "error");
//    }
//
//private:
//    const T* data_;
//};
//
//
//
//template <typename T, size_t nrows, size_t ncols>
//class Progmem_biarray_view{
//public:
//    using size_type = size_t;
//
//    static_assert(std::is_same_v<T, uint8_t> 
//	       or std::is_same_v<T, uint16_t>);
//
//    Progmem_biarray_view(const T (*data)[ncols]) : data_{data} {}
//
//    constexpr static size_type rows() {return nrows;}
//    constexpr static size_type cols() {return ncols;}
//
//    Progmem_array_view<T, cols()> operator[](size_type i) const 
//    { return Progmem_array_view<T, cols()>{data_[i]}; }
//
//private:
//    const T (*data_)[ncols];
//};


// Interfaz genérico
// -----------------
// (RRR) ¿Por qué devolver T{pgm_read_byte(&x)} construyendo T{...}?
//	 Para que funcione con enums. Si T es un `enum class` el compilador
//	 genera un error al intentar convertir de forma implícita
//	 pgm_read_byte a esa enum. 
template <typename T>
inline constexpr T rom_read(const T& x)
{
    if constexpr (sizeof(T) == 1)
	return T{pgm_read_byte(&x)};

    else if constexpr (sizeof(T) == 2)
	return T{pgm_read_word(&x)};

    else
	static_assert(atd::always_false_v<int>, "Not implemented");
}

struct ROM_read{
    template <typename T>
    T operator()(const T& x) const
    { return rom_read(x); }
};

using ROM_uint8_t = atd::ROM<uint8_t, ROM_read>;
using ROM_uint16_t = atd::ROM<uint16_t, ROM_read>;


template <typename T, size_t N, typename Read = ROM_read>
using ROM_array = atd::ROM_array<T, N, Read>;

template <size_t N>
using ROM_string = Progmem_string<N>;	// equivalente en RAM a const char p[N];

template <size_t N>
using ROM_string_array = Progmem_string_array<N>; // == const char* p[N];

template <size_t N>
using Element_ROM_string_array = Element_progmem_string_array<N>;

/***************************************************************************
 *			    FUNCTIONS
 ***************************************************************************/


}// namespace

// Las funciones `print` dentro de `atd`.
namespace atd{
// (RRR) Cuando empiezo a usar menus en mis programas me encuentro con que 
//       rápidamente uso los 2kB de RAM en las cadenas de los menus, dando un
//       stackoverflow (me ha dado ya 2 veces). Para evitarlos, creo esta
//       función: observar que `print` NO carga la cadena en memoria sino solo
//       1 caracter. Es más lento que cargar toda la memoria a la vez (???)
//       pero apenas uso memoria RAM, que es el objetivo.
template <size_t N>
std::ostream& print(std::ostream& out, const mega_::Progmem_string<N>& str)
{
    for (size_t i = 0; i < str.size(); ++i)
	out << str[i];

    return out;
}
}// namespace

#endif


