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

#pragma once

#ifndef __ATD_CSTRING_H__
#define __ATD_CSTRING_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Manejo de cadenas tipo C
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	16/11/2019 length, line_count, const_cstring
 *	03/01/2020 const_nstring
 *	06/03/2022 print(out, const_nstring)
 *
 ****************************************************************************/

#include <cstddef>		// ptrdiff_t
#include <iostream>
#include "atd_ostream.h" // print(ostream)

namespace atd{

/// Devuelve la longitud de la cadena tipo c 'str'. 
/// Esta función está pensada para ser ejecutada en compilación.
/// Es la equivalente a strlen de C, pero en tiempo de compilación.
inline constexpr std::size_t strlen(const char* str) noexcept
{
    return *str ? 1 + strlen(str + 1) : 0;
}


/*!
 *  \brief Cadena de C.
 *
 *  Uno de los problemas con las cadenas de C es que no podemos usarlas con
 *  los algoritmos que admiten intervalos ya que hay que calcular end(). Si
 *  bien end() lo podemos calcular dinámicamente, buscando '\0', es mucho más
 *  eficiente hacerlo estáticamente. Esta clase es responsable de ello: de
 *  calcular estáticamente end().
 *
 *  La llamo const porque está pensada para ser usada con cadenas const char*
 *  que no cambian nunca.
 *
 *  Voy a copiar el interfaz de string_view. ¿Por qué no usar string_view? Las
 *  core guidelines comentan dejar string_view para cadenas que no acaban en
 *  '\0'. Las que acaban en '\0' son zstring (<--- no me gusta el nombre,
 *  prefiero const_cstring).
 *
 */
class const_cstring_base{
public:
// types
    using size_type = std::size_t;


// iterators
// ---------
    /// Returns an iterator to the beginning.
    constexpr const char* begin() const noexcept {return p_;}

    /// Returns an iterator to the end.
    constexpr const char* end() const noexcept {return p_ + len_;}

    /// Returns an iterator to the beginning.
    constexpr const char* cbegin() const noexcept {return p_;}

    /// Returns an iterator to the end.
    constexpr const char* cend() const noexcept {return p_ + len_;}


// element access
// --------------
    /// Returns the character at position i.
    constexpr const char operator[](size_type i) const noexcept
    {return p_[i];}

    /// Returns a pointer to the underlying character array.
    constexpr const char* data() const noexcept
    {return p_;}


// capacity
// --------
    /// Returns de number of characters.
    constexpr std::size_t length() const noexcept {return len_;}

    /// Returns de number of characters.
    constexpr std::size_t size() const noexcept {return length();}

protected:
// constructor
// -----------
    constexpr const_cstring_base(const char str[]) noexcept
	: p_{str}, len_{atd::strlen(str)} {}

    constexpr const_cstring_base(const char str[], size_type n) noexcept
	: p_{str}, len_{n} {}

private:
// data
    const char* p_;
    std::size_t len_;
};

inline const char* begin(const const_cstring_base& s) {return s.begin();}
inline const char* end(const const_cstring_base& s) {return s.end();}




/*!
 *  \brief  String tipo C
 *
 */
class const_cstring : public const_cstring_base
{
public:
    constexpr const_cstring(const char str[]) noexcept 
	: const_cstring_base(str) { }
};

inline std::ostream& operator<<(std::ostream& out, const const_cstring& s)
{ return out << s.data(); }


/*!
 *  \brief String definida por el número de elementos.
 *
 *  (RRR) Cuando quiero darle a un usuario en un LCD la opción de elegir el
 *        día de la semana, anoto todos los días en un array "DoLuMaMiJuViSa".
 *        Para imprimir el "Ju" necesito pasar o bien dos punteros (apuntando
 *        a 'J' y a la 'V') o bien un puntero y la longitud. const_nstring
 *        implementa esta segunda opción.
 *
 *  DUDA: ¿esto no sería std::span? 
 */
class const_nstring : public const_cstring_base
{
public:
    constexpr const_nstring(const char str[], size_type n) noexcept 
	: const_cstring_base(str, n) { }
};


template <typename Out>
inline Out& print(Out& out, const const_nstring& s)
{
    for (auto x: s)
	print(out, x);

    return out;
}

inline std::ostream& operator<<(std::ostream& out, const const_nstring& s)
{ 
    return print(out, s);
//    for (auto x: s)
//	out << x;
//
//    return out;
}



/*!
 *  \brief  Array de strings de longitud fija
 *
 *  Ejemplo: "DoLuMaMiJuViSa" es un array de 7 cadenas de longitud fija.
 *  En lugar de definirlo como array {"Do", "Lu", ..., "Sa"} es más práctico
 *  definirlo como una única cadena ya que esa lo podemos almacenar
 *  directamente en la EEPROM del microcontrolador.
 *
 */
class Array_const_nstrings{
public:
    using size_type = std::size_t;

// Constructor
    /// data: cadena de C con el array
    /// row_size: longitud de cada elemento.
    // Ejemplo: si data = "DoLuMaMiJuViSa", row_size == 2.
    constexpr Array_const_nstrings(const char* data,
                                   size_type row_size) noexcept
        : data_{data}, row_size_{row_size}, 
	  num_rows_{atd::strlen(data) / row_size}
    { }

    // Access
    constexpr const_nstring operator[](size_type n) noexcept
    {return const_nstring{&data_[n*row_size_], row_size_};}

    constexpr const_nstring operator[](size_type n) const noexcept
    {return const_nstring{&data_[n*row_size_], row_size_};}

    constexpr size_type size() const {return num_rows_;}

private:
// Data
    const char* data_;
    size_type row_size_;
    size_type num_rows_;
};



/***************************************************************************
 *			ALGORITMOS PARA CADENAS
 ***************************************************************************/
/// Cuenta el número de líneas que hay en str. 
/// Realmente lo que cuenta es el número de veces + 1 que aparece el caracter
/// end_line en str. 
/// Ejemplo: si str = "uno\ndos", devuelve 2. Cuidado, ya que la cadena
/// "uno\ndos\n" devuelve 3.
/// Caso extremo: La cadena vacía "" devuelve 0.
std::size_t line_count(const char* str, char end_line = '\n');


}// namespace

#endif


