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
#ifndef __MCU_STD_CHAR_TRAITS_H__
#define __MCU_STD_CHAR_TRAITS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Implementación de char_traits. Forma parte de <string>
 *
 * HISTORIA
 *    Manuel Perez
 *    24/09/2019 Versión mínima para que funcione el sistema de flujos
 *
 ****************************************************************************/
#include "std_config.h"

#include <stdio.h>  // EOF
#include <string.h> // memcpy

namespace STD{

// >>> (impl) ¿Cómo evitar llamadas recursivas?
// Algunas de las funciones definidas en char_traits son completamente
// genéricas: se pueden implementar en la clase genérica y luego llamarlas en
// las clases especializadas.
//
// Ejemplo:
//  template <typename CharT>
//  size_t char_traits<CharT>::length(const char_traits* s)
//  {
//	implementación genérica (la da el estandard)
//  }
//
//  ...
//  struct char_traits<char>
//  {
//	static size_t length(const char_type* s)
//	{
//	   return char_traits<char_type>::length(s); // NO FUNCIONA!!!
//	}
//  };
//
//  Al intentar llamar a la clase genérica desde la clase especializada no
//  ya que el compilador traduce la función anterior en:
//	static size_t length(const char_type* s)
//	{
//	   return char_traits<char>::length(s); // la misma función!!!
//	}
//  generando una llamada recursiva.
//
//  ¿Cómo resolver este problema?
//  1) Estilo C: creamos funciones char_traits_length(...) que son las que
//               llamamos dentro de las clases especializadas.
//
//  2) Estilo C++ (idéntico al de C): hacemos lo mismo, pero queda más claro
//		definir la clase char_traits<CharT> en el namespace impl_of.
//		De esa forma el código anterior quedará:
//  struct char_traits<char>
//  {
//	static size_t length(const char_type* s)
//	{
//	   return impl_of::char_traits<char_type>::length(s); 
//	}
//  };
//  
// (RRR) Pero no lo voy a implementar así, aunque los de gcc tienen esta
// estructura, ya que creo que no es correcta en este caso.
//
// El motivo está en el uso de las funciones eq(), lt() que dependen del
// caracter. Al definir los algoritmos find, ... dentro de
// impl_of::char_traits se usa NO los traits reales de char, char8_t, ...,
// sino los traits genéricos que se están definiendo en esta clase
// impl_of::char_traits. Con lo que se está perdiendo la idea de los traits. 
// Por ello opto por definir los algoritmos dentro de
// <<< (impl)
//namespace impl_of{
//template <typename CharT> 
//struct char_traits
//{
//    using char_type = CharT;
//
//    static constexpr bool eq(char_type x, char_type y) noexcept;
//
//    static constexpr size_t length(const char_type* s);
//
//    static constexpr 
//    const char_type* find(const char_type* s, size_t n, const char_type& c);
//};
//
//template <typename CharT> 
//inline constexpr 
//bool char_traits<CharT>::eq(char_type x, char_type y) noexcept
//{ return x == y; }
//
//template <typename CharT> 
//constexpr size_t char_traits<CharT>::length(const char_type* p)
//{
//    size_t i = 0;
//    while (!eq(p[i], char_type())){
//	++i;
//    }
//
//    return i;
//}
//
//
//template <typename CharT> 
//constexpr 
//const char_traits<CharT>::char_type* 
//char_traits<CharT>::find(const char_type* s, size_t n, const char_type& c)
//{
//    for (size_t i = 0; i < n; ++i){
//	if (eq(s[i], c))
//	    return &s[i];
//    }
//
//    return 0;
//}
//
//}// namespace impl_of

template <typename CharT> 
struct char_traits // : impl_of::char_traits<CharT>
{ };



// char_traits<char>
// -----------------
template<>
struct char_traits<char>{
    using char_type = char;
    using int_type  = int;
//    using off_type  = streamoff; <-- esto depende del hardware ¿cómo
//    definirlo aquí, para microcontroladores? 
//    using pos_type  = streampos;
//    using state_type= mbstate_t;
//    using comparison_category = strong_ordering;
    
    static constexpr void assign(char_type& x, const char_type& a) noexcept
    { x = a;}

    // De acuerdo a LWG 467
    // (https://cplusplus.github.io/LWG/lwg-defects.html#467)
    // esta comparación debe ser entre unsigned char. Los de gcc no lo tienen
    // implementado como unsigned char (???)
    static constexpr bool eq(char_type x, char_type y) noexcept
    { return static_cast<unsigned char>(x) == static_cast<unsigned char>(y);}

    static constexpr bool lt(char_type x, char_type y) noexcept
    { // LWG 467
	return static_cast<unsigned char>(x) < static_cast<unsigned char>(y); 
    }

    // (RRR) Los de gcc devuelven +1 y -1 en caso de que no sean iguales las
    //       cadenas. Pero ¿por qué perder información? El algoritmo conoce la
    //       posición 'i' en la que se produce la primera diferencia. ¿Por qué
    //       no suministrarle esa información al que llama a la función?
    static constexpr int compare(const char_type* s1, const char_type* s2
				, size_t n)
    {
	if (n == 0) 
	    return 0;

	for (size_t i = 0; i < n; ++i){
	    if (!eq(s1[i], s2[i])){
		if(lt(s1[i], s2[i]))
		    return -1; // mejor: return -i;
		else
		    return 1;  // mejor: return i;
	    }
	}

	return 0;
    }

    static constexpr size_t length(const char_type* s)
    {
	size_t i = 0;
	while (!eq(s[i], char_type())){
	    ++i;
	}

	return i;
    }


    static constexpr 
	const char_type* find(const char_type* s, size_t n, const char_type& c)
    {
	for (size_t i = 0; i < n; ++i){
	    if (eq(s[i], c))
		return &s[i];
	}

	return 0;
    }

//    static constexpr char_type* move(char_type* s1, const char_type* s2,    
//				    size_t n);

//    static constexpr char_type* move(char_type* s1, const char_type* s2, size_t n);

    static constexpr 
	char_type* copy(char_type* dest, const char_type* src, size_t n)
    {
	if (n == 0)
	    return dest;

	return static_cast<char_type*>(memcpy(dest, src, n));
    }

//    static constexpr
//	char_type* assign(char_type* s, size_t n, char_type a);

    static constexpr char_type to_char_type(const int_type& c) noexcept
    {return static_cast<char_type>(c);}

    static constexpr int_type not_eof(int_type c) noexcept
    {return (c == eof()) ? 0 : c;}

//    static constexpr char_type to_char_type(int_type c)  noexcept;
    
    // DUDA: esto es copia del char_traits del compilador gcc. ¿Por qué es
    // necesario hacer el cast a unsigned char?
    static constexpr int_type to_int_type(const char_type& c) noexcept
    {return static_cast<int_type>(static_cast<unsigned char>(c));}
//    static constexpr int_type to_int_type(char_type c) noexcept
//    {return static_cast<int_type>(c);}

    static constexpr bool eq_int_type(const int_type& c1, const int_type& c2) noexcept
    {return c1 == c2;}

    static constexpr int_type eof() noexcept
    {return static_cast<int_type>(EOF);}

};




}// namespace

#endif

