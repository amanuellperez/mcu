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

#ifndef __ATD_STRING_H__
#define __ATD_STRING_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Manejo de cadenas tipo C
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	16/11/2019 length, line_count, const_cstring
 *
 ****************************************************************************/

#include <cstddef>		// ptrdiff_t
#include <iostream>

namespace atd{

/// Devuelve la longitud de la cadena tipo c 'str'. 
/// Esta función está pensada para ser ejecutada en compilación.
/// Es la equivalente a strlen de C, pero en tiempo de compilación.
inline constexpr std::size_t length(const char* str) noexcept
{
    return *str ? 1 + length(str + 1) : 0;
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
class const_cstring{
public:
    // tipos
    using size_type = std::size_t;

    // constructor
    // -----------
    constexpr const_cstring(const char str[]) noexcept
	: p_{str}, len_{atd::length(str)} {}


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


private:
    // datos
    const char* p_;
    std::size_t len_;
};

inline const char* begin(const const_cstring& s) {return s.begin();}
inline const char* end(const const_cstring& s) {return s.end();}


inline std::ostream& operator<<(std::ostream& out, const const_cstring& s)
{ return out << s.data(); }



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


