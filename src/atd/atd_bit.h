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

#ifndef __ATD_BIT_H__
#define __ATD_BIT_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Funciones para manejar bits.
 *
 *   - COMENTARIOS: 
 *
 *  HISTORIA
 *  --------
 *  Manuel Perez
 *  26/07/2017 v0.0: Escrito
 *  31/07/2019 v0.1:
 *	    No me gusta el nombre de las funciones:
 *
 *			Bit_of::write_zero<ISC01>(EICRA);
 *
 *	    es un poco confuso. ¿Quién es el bit? ¿y el byte?
 *	    Lo cambio por:
 *
 *			write_zero_bit<ISC01>(EICRA);
 *
 *	    ahora queda más claro que el bit es ISC01 y el byte EICRA.
 *
 *	    Otra forma de hacerlo es la usada en Register:
 *				
 *		    Register{EICRA}.write_zero_bit<ISC01>();
 *
 *	    Esa queda más clara, pero hay que garantizar
 *	    que sea igual de eficiente que el código en C.
 *
 *  29/08/19: bit<7>::of(x)
 *  19/10/19: concat_bits, Bit_of
 *  25/01/20: concat_bytes, bitmask
 *  28/01/20: write_bits (más genérica que el resto, muy eficiente y 
 *			  muy expresiva).
 *	      is_one_bit/is_zero_bit
 *	      write_range_bits
 *  01/02/21: read_bits, zero::with_bits::to
 *  18/04/21: write_bit
 *  01/11/22: write_zero/write_one
 *  25/02/23: is_one_most_significant_bit_of
 *  28/02/23: nibble<n>(); byte<n>();
 *  21/08/23: reverse_bits
 *  29/08/24: bit  (versión dinámica)
 *  27/09/24: in(x).bit(4).write_byte(a);
 *  15/03/25: split_bytes
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t
#include <cstddef>
#include <type_traits>
#include "atd_static.h"	// static_array
#include "atd_type_traits.h"	// sizeof_in_bytes
#include "atd_concepts.h"
#include "atd_cmath.h"	    // div

// avr define bit
#undef bit

namespace atd{

// ------------
// concat_bytes
// ------------
namespace impl_of{
template <typename Int, typename Byte>
Int concat_bytes(Int res, Byte b)
{
    res <<= 8;
    res |= static_cast<uint8_t>(b);

    return res;
}


template <typename Int, typename Byte, typename... Tail>
Int concat_bytes(Int res, Byte b, Tail... b1_bn)
{
    res <<= 8;
    res |= static_cast<uint8_t>(b);

    return concat_bytes(res, b1_bn...);
}
}// impl_of

// Caso degenerado. ¿Alguien lo llamará?
template <typename Int, typename Byte, typename ...Tail>
constexpr inline Int concat_bytes(Byte b)
{
    return b;
}

/// Concatena los bytes x,y devolviendo la cadena de 16 bits: xy.
/// Observar que los concatena en el orden pasados: {MSB, ..., LSB}
template <typename Int, typename Byte, typename ...Tail>
constexpr inline Int concat_bytes(Byte b0, Tail... b1_bn)
{
    Int res{0};
    
    res = static_cast<uint8_t>(b0);
    return impl_of::concat_bytes(res, b1_bn...);
}


// -----------
// split_bytes
// -----------
// Descompone un entero de más de 1 byte en sus bytes correspondientes.
// uint16_t -> {MSB, LSB}
constexpr 
inline void split_bytes(const uint16_t& x, uint8_t& b1, uint8_t& b0)
{
    b1 = (0xFF00 & x) >> 8;
    b0 = 0x00FF & x;
}

// (DUDA) ¿inline or not inline?
// uint32_t -> {MSB...LSB}
constexpr 
inline void split_bytes( const uint32_t& x
			, uint8_t& b3, uint8_t& b2, uint8_t& b1, uint8_t& b0)
{
    b3 = (0xFF000000 & x) >> 24;
    b2 = (0x00FF0000 & x) >> 16;
    b1 = (0x0000FF00 & x) >>  8;
    b0 = 0x000000FF & x;
}

// -------
// bitmask
// -------
template <uint8_t i0, uint8_t i1, uint8_t k, typename Int>
constexpr inline Int __make_range_bitmask(Int mask)
{
    if constexpr (i0 <= k and k <= i1)
	mask |= (Int{1} << k);

    if constexpr (k == i1)
	return mask;
    else
	return __make_range_bitmask<i0, i1, k + 1>(mask);
}


// Devuelve una bitmask contigua con los bits indicados.
// Ejemplo: uint16_t mask = atd::make_range_bitmask<10,12, uint16_t>();
//	    Devuelve mask = 0x1C00;
template <uint8_t i0, uint8_t i1, typename Int>
constexpr inline Int make_range_bitmask()
{
    Int mask{0};

    return __make_range_bitmask<i0, i1, 0>(mask);
}


template <uint8_t i0, uint8_t i1, typename Int>
struct __Range_bitmask{
    static constexpr Int mask = make_range_bitmask<i0, i1, Int>();
    static constexpr uint8_t pos = i0; // syntactic sugar

    Int& reg;

    constexpr __Range_bitmask(Int& reg0) : reg{reg0} { }

    // get_bits: observar que colocamos el bit i0 en la posición 0
    constexpr operator Int() const 
    { return (reg & mask) >> pos;}

    // set_bits
    constexpr Int& operator=(const Int& x)
    {
	reg = ((reg & ~mask) | ((x << pos) & mask));
	return reg;
    }
};


// syntactic sugar
/// Returns true if r != 0, false if == 0.
template <uint8_t i0, uint8_t i1, typename Int>
inline bool to_bool(const __Range_bitmask<i0, i1, Int>& r)
{
    if (r == Int{0})
	return false;
    else
	return true;
}



/*!
 *  \brief Contiguous bitmask.
 *  
 *  Example:
 *	constexpr atd::Range_bitmask<2,6, uint8_t> mask; // mask bits 2-6
 *	uint8_t x = 0x35;
 *
 *	// Hacemos el shift, de tal manera que los bits del 0 al 4 de mask(x)
 *	// sean igual a los bits del 2 al 6 de x:
 *	//      x  = 0b0   0110101
 *	//                 --- - 
 *      // mask(x) = 0b000 01101  <-- observar cómo el bit 2 lo colocamos en
 *      //                            el 0
 *	uint8_t res = mask(x); // = get_bits_2_to_6_of(x);
 *
 *	// Al escribir en cambio escribimos directamente los bits del 2 al 6
 *	// con el valor pasado 0x16
 *	mask(x) = uint8_t{0x16}; // = set_bits_2_to_6_of(x)_equal_to(0x16);
 *
 */
template <uint8_t i0, uint8_t i1, typename Int>
struct Range_bitmask{
    static_assert (i0 <= i1, "i0 must be less or equal than i1");

    constexpr __Range_bitmask<i0, i1, Int> operator()(Int& reg) const
    { return __Range_bitmask<i0, i1, Int>{reg}; }

    constexpr __Range_bitmask<i0, i1, const Int> operator()(const Int& reg) const
    { return __Range_bitmask<i0, i1, const Int>{reg}; }
};








// Estos son los bits que queremos escribir. ¿Qué nombre darle? No es la
// mascara. 
// Ejemplo:
//	using Positions = static_array<1,3,7>;
//	using Bits      = static_array<0,1,1>;
//	uint8_t want_bits = __make_want_bits<Positions, Bits, uint8_t>();
//	// want_bits == 0b00001010
//		          ^   ^ ^   <-- escribe ese valor, deja a ceros los
//					demás.
template <typename Positions, typename Bits, typename Int>
constexpr Int __make_want_bits()
{
    Int bitmask {0};

    for (size_t i = 0; i < Positions::size; ++i){
	if (Bits::data[i])
	    bitmask |= (Int{1} << Positions::data[i]);
    }

    return bitmask;
}

// Devuelve la bitmask correspondiente a las posiciones indicadas.
// requires: std::is_same_v<Positions, atd::static_array<int...>>
//
// Ejemplo:
//	using Positions = static_array<1,3,7>;
//	uint8_t mask = make_bitmask<Positions, uint8_t>();
//	// mask == 0b10001010
template <typename Positions, typename Int>
constexpr Int make_bitmask()
{
    Int mask {0};

    for (size_t i = 0; i < Positions::size; ++i){
	mask |= (Int{1} << Positions::data[i]);
    }

    return mask;
}


/*!
 *  \brief  Manipulación de bits
 *
 *  int x;
 *  atd::write_bits<1,3,4,7>::to<1,0,1,1>::in(x);
 *
 *  Lo que hace es:
 *	    x = 0bxxxxxxxx --> 0b1xx01x1x
 *
 * (RRR) Al principio lo llamaba:
 *       atd::write_bits<1,3,4,7>::to<1,0,1,1>::in_register(x);
 *       El nombre un poco largo, por eso lo acorté. 
 *       Después de usarlo un tiempo, queda bien.
 *
 *  TODO: Tal como lo estoy implementando estoy escribiendo todo 'x' de nuevo.
 *  El problema es que hay ciertos bits como TWINT del TWI que si están a 1
 *  y se vuelven a escribir a 1 eso genera un clear, poniendose TWINT a 0. No
 *  sirve, en general, copiar el valor que tenía x. Solo hay que modificar los
 *  bits que se quieren modificar!!! Cambiarlo!!!
 */
namespace __atd{
// Todo esto se traduce en asm en un simple and y or!!!
template <int... bitpos>
struct write_bits{
    using positions = static_array<int, bitpos...>;
    
    template <int... value_bits>
    struct to{
	using positions = write_bits::positions;
	using bits = static_array<int, value_bits...>;
	static_assert(positions::size == bits::size);
	
	template <typename Int2>
	static constexpr void in(Int2& x)
	{
	    using Int = std::remove_cv_t<Int2>;

	    constexpr Int res = __make_want_bits<positions, bits, Int>();
	    constexpr Int mask = make_bitmask<positions, Int>();

	    x = (x & ~mask) | res; 
	}
    };
};
}// namespace


// Si queremos escribir varios bits, en plural
template <int... bitpos>
using write_bits = __atd::write_bits<bitpos...>;

//// Si solo queremos escribir 1 bit en singular (syntactic sugar)
//template <int... bitpos>
//using write_bit = __atd::write_bits<bitpos...>;

/*!
 *  \brief  write_bit
 *
 *  Escribiendo un programa da la impresión de que al usar write_bits en lugar
 *  de la instrucción equivalente se genera más código. 
 *  
 *  En principio es verdad que write_bits ejecuta `x = (x & ~mask) | res`
 *  que es 1 operación a más que un 'and' y 2 operaciones más que un 'or'.
 *
 *  Como esta es una función de bajo nivel que puede usarse bastante la
 *  intento optimizar.
 *
 *  Al comparar el código en ensamblador que genera:
 *	(1) x &= ~mask;
 *	(2) write_bit<3>::to<0>::in(x);
 *	(3) write_bits<3>::to<0>::in(x);
 *
 *  el compilador genera lo mismo. El ejemplo es un ejemplo sencillo. ¿Merece
 *  la pena escribir write_bit?
 *
 */
template <int bitpos>
struct write_bit{
    using positions = static_array<int, bitpos>;

    template <int value_bit>
    struct to
    {
	template <typename Int2>
	static constexpr void in(Int2& x)
	{
	    using Int = std::remove_cv_t<Int2>;
	    constexpr Int mask = make_bitmask<positions, Int>();

	    if constexpr (value_bit == 0){
		// x &= ~mask; <-- deprecated with volatile
		x = x & ~mask;
	    }

	    else{
		// x |= mask; <-- deprecated with volatile
		x = x | mask;
	    }
	}
    };

};


/*!
 *  \brief  Escribimos un rango de bits 
 *
 *  Ejemplo:
 *	// escribimos los bits [3,5] de x = res
 *	atd::write_range_bits<3,5>::to<res>::in(x);
 *
 *	Escribe `res` en los bits [3,5] de x.
 *
 */
template <int i_0, int i_n>
struct write_range_bits{
    template <auto res0>
    struct to{
	template <typename Int2>
	static constexpr void in(Int2& x)
	{
	    using Int = std::remove_cv_t<Int2>;
	    
	    constexpr Int mask = make_range_bitmask<i_0, i_n, Int>();
	    constexpr Int res = (Int{res0} << i_0) & mask;

	    x = (x & ~mask) | res;
	}
    };
};


/*!
 *  \brief  is_one_bit<1>::of(x)?
 *
 *  DUDA: is_one_bit<1>::of_register(x) vs is_one_bit<1>::of(x)???
 *  El primero al usarlo me resulta más dificil de leer. Es mejor lo criptico,
 *  pero la regla es evita ser criptico!!! Pruebo con of!! 
 *  Pero me gusta también of_register!!! Que el tiempo y el uso elija.
 *
 *  Elimino of_register ya que `x` NO tiene por qué ser un register!!!
 */
template <int pos>
struct is_one_bit{
    template <typename Int>
    static bool of(const Int& x){
	return static_cast<bool>(x & (Int{1} << pos));
    }

//    template <typename Int>
//    static bool of_register(const Int& x){
//	return of(x);
//    }
};


/*!
 *  \brief  is_zero_bit<1>::of_register(x)?
 *
 *  DUDA: mejor solo 'of' o 'of_register'. En código ahora me gusta más la
 *  segunda opción. Que la experiencia lo elija.
 *  Elimino of_register ya que `x` NO tiene por qué ser un register!!!
 */
template <int pos>
struct is_zero_bit{
    template <typename Int>
    static bool of(const Int& x){
	return !static_cast<bool>(x & (Int{1} << pos));
    }

//    template <typename Int>
//    static bool of_register(const Int& x){
//	return of(x);
//    }
};


/*!
 *  \brief  Lee los bits indicados.
 *
 *
 *  Ejemplo:
 *	int x = 0b1001001;
 *	y = atd::read_bits<3,5>::of(x)
 *	devuelve: y == 0b00001000;
 *
 *  (RRR) En principio hay 2 formas básicas de implementar esta función:
 *        1. Que haga cero todos los bits que no se quieran leer.
 *           Ejemplo: read_bits<3,4>(0b11111111) devuelve 00011000.
 *
 *        2. Que se haga lo mismo que en 1 y además se haga un shift de todos
 *           los bits a la derecha:
 *           Ejemplo: read_bits<3,4>(0b11111111) devuelve 00000011.
 *		
 *	  La 2ª opción es más complicada de implementar y no queda claro qué
 *	  hacer si se quiere leer bits al azar:
 *	      read_bits<1,3>(0b11111111) primero haría 0b00001010, y ahora
 *	      ¿cómo hacer el shift? ¿los colapso? Devuelvo 101 (eliminando el
 *	      0 en la posición del bit 0) o devuelvo 11 (que sería el valor de
 *	      los bits).
 *
 *   Quizás solo leer bits sin hacer el shift sea mejor llamarlo como hago
 *   ahora read_bits, y si se quiere hacer un shift llamarlo read_range_bits
 *   ya que se va a leer un rango. ¿Merece la pena hacerlo? ¿Generará un lío
 *   el shift? De momento pruebo con esta. El uso lo dirá.
 */
template <int... bitpos>
struct read_bits{
    using positions = static_array<int, bitpos...>;
    
    template <typename Int2>
    static constexpr std::remove_cv_t<Int2> of(Int2& x)
    {
	using Int = std::remove_cv_t<Int2>;

	constexpr Int mask = make_bitmask<positions, Int>();
	
	return x & mask;
    }

};


template <int bitpos>
using read_bit = read_bits<bitpos>;


/*!
 *  \brief  Cero con ciertos bits escritos.
 *
 *  (RRR) Al querer leer el divisor de la frecuencia del reloj de un timer (el
 *        1) encuentro el problema de cómo dejarlo en general. La forma de
 *        leer esto es leer una sería de bits de un registro. A partir de esos
 *        bits se calculan el divisor de frecuencia. 
 *
 *        El problema es que esos bits no tienen por qué ser los mismos en
 *        todos los avr. Si escribo:
 *           switch(atd::read_bits<CS12, CS11, CS10>::of(TCCR1B)){
 *		    case 0xA0: ...
 *		    case 0xA1: ...
 *		    ...
 *	    }
 *
 *	  y luego cambian la posición de los bits en otro avr no funcionaría
 *	  el código. Por supuesto que puedo usar un archivo donde escribir
 *	  todos esos valores y cambiarlos dependiendo del avr, pero la
 *	  implementación del timer es el traductor de la datasheet llena por
 *	  todos los sitios de datos de bajo nivel. Si se quiere usar un timer
 *	  diferente, hay que cambiar de traductor. No son necesarios tener 2
 *	  archivos: el traductor y otro con todas las constantes.
 *	  
 *	  Esta clase permite hacer esto:
 *           switch(atd::read_bits<CS12, CS11, CS10>::of(TCCR1B)){
 *              case 
 *              atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<0,1,0>(): ...
 *              atd::zero<uint8_t>::with_bits<CS12, CS11, CS10>::to<1,1,0>(): ...
 *              ...
 *	    }
 *
 */
template <typename Int>
struct zero{

    template <int... bitpos>
    struct with_bits{
    
	template <int... value_bits>
	static constexpr Int to()
	{
	    Int res{0};

	    write_bits<bitpos...>::template to<value_bits ...>::in(res);
	    return res;
	}
    };

};

/***************************************************************************
 *			    VERSIONES DINÁMICAS
 ***************************************************************************/
/// Escribe un 0 en el bit que ocupa la posición `pos` de `x`
template <typename Int>
inline void write_zero(Int& x, uint8_t pos)
{ x = x & ~(Int{1} << pos); }

/// Escribe un 1 en el bit que ocupa la posición `pos` de `x`
template <typename Int>
inline void write_one(Int& x, uint8_t pos)
{ x = x | (Int{1} << pos); }


// Bit
// ---
template <typename T>
struct Bit{
    constexpr Bit(T& x0, uint8_t pos0)
	    : x{x0}, pos{pos0} { }


    // DUDA: qué devolver? Bit&? Si en el futuro
    // no es necesario cambiarlo borrar este comentario
    template <Type::Integer Int>
    constexpr 
    void operator=(const Int& n)
	requires (!std::is_const_v<T>) 
    {
	if (n != 0)
	    write_one(x, pos);
	else
	    write_zero(x, pos);
    }

    // En esta clase voy a imponer la restricción de que un bit
    // solo puede valer 0 ó 1 
    constexpr operator uint8_t() const
    { return (x & (T{1} << pos))? 1: 0; }

// data
    T& x;
    uint8_t pos;
};

 
namespace impl_{
struct Bit_of{
    constexpr Bit_of(uint8_t pos0) : pos{pos0} { }

    template <typename T>
    constexpr Bit<T> of(T& x)
    { return Bit<T>{x, pos};}

    const uint8_t pos;
};

}// impl_


// Esta función no está pensada para llamarse sola, sino que hay que usar el
// idioma:
//	Bit b = bit(3).of(x);
//  Observar que la que devuelve `Bit` es la función  `of` no `bit`.
//  Que esta función devuelva impl_::Bit_of, el impl_ esta diciendo "NO ME
//  USES BAJO NINGÚN CONCEPTO". El usuario no puede usar clases de
//  implementación.
inline impl_::Bit_of bit(uint8_t pos) { return impl_::Bit_of{pos}; }


// is_one_most_significant_bit_of
// --------------------
// (RRR) Creo que podia usar make_bitmask pero solo serviría para complicar el
//       código. Con _Mask_is_one_most_significant_bit_of queda muy sencillo.
template <typename Int>
struct _Mask_is_one_most_significant_bit_of{ };

template <>
struct _Mask_is_one_most_significant_bit_of<uint8_t>{
    static constexpr uint8_t mask = 0x80;
};


template <>
struct _Mask_is_one_most_significant_bit_of<uint16_t>{
    static constexpr uint16_t mask = 0x8000;
};


template <>
struct _Mask_is_one_most_significant_bit_of<uint32_t>{
    static constexpr uint32_t mask = 0x80000000;
};


template <>
struct _Mask_is_one_most_significant_bit_of<uint64_t>{
    static constexpr uint64_t mask = 0x8000000000000000;
};


// (RRR) ¿el nombre es muy largo?
//	 Si lo que se busca es un nombre críptico mejor usar directamente:
//
//		if (x & 0x80) ...
//
//	La idea del nombre es que después de 1 año de haber escrito el código
//	no tenga que romperme los cuernos 1 hora averiguando qué hace.
//
//	    if (is_one_most_significant_bit_of(x)) ...
//
//	Ese if se entiende bien ya que se muestra la intención.
//
template <typename Int>
inline bool is_one_most_significant_bit_of(const Int& x)
{ return (x & _Mask_is_one_most_significant_bit_of<Int>::mask); }



// byte
// ----
// Devuelve el byte n de x
template <size_t n, typename T>
inline uint8_t byte(T x)
{
    static_assert(sizeof_in_bytes<T>() > n, "Index overflow");

    x >>= 8*n;
    return static_cast<uint8_t>(T{0xFF} & x);
}

// nibble 
// ------
// Devuelve el nibble n de x
template <size_t n, typename T>
inline uint8_t nibble(T x)
{
    static_assert(2*sizeof_in_bytes<T>() > n, "Index overflow");

    x >>= 4*n;
    return static_cast<uint8_t>(T{0x0F} & x);
}



// reverse_bits
// ------------
// No suministro funciones para int ya que estoy manipulando bits directamente
// (descarto usar `std::byte` porque luego hay que hacer un montón de
// castings)
// Devuelve el número con los bits en reverse order.
// Implementación básica: (muy sencilla de entender)
//inline uint8_t reverse_bits(uint8_t x)
//{
//    return ((x & 0x01) << 7) |
//	   ((x & 0x02) << 5) |
//	   ((x & 0x04) << 3) |
//	   ((x & 0x08) << 1) |
//	   ((x & 0x10) >> 1) |
//	   ((x & 0x20) >> 3) |
//	   ((x & 0x40) >> 5) |
//	   ((x & 0x80) >> 7);
//}

// Esta es una optimización de la versión anterior. La idea es usar divide &
// conquer. 
// Source: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
// Si se prueba a compilar la versión anterior y esta en goldbolt (con -O2) se
// ve claramente que esta es más eficiente.
inline uint8_t reverse_bits(uint8_t x)
{
    x = ((x & 0xF0) >> 4) | ((x & 0x0F) << 4);
    x = ((x & 0xCC) >> 2) | ((x & 0x33) << 2);
    x = ((x & 0xAA) >> 1) | ((x & 0x55) << 1);

    return x;
}


// (RRR) ¿por qué no usar concat_bytes?  
//       Quiero que sea eficiente y no tengo claro si concat_bytes introduce
//       ineficiencias gratuitas. No debiera pero no he medido.
inline uint16_t reverse_bits(uint16_t x)
{
//  std::span<uint8_t> u = view_as_bytes(x);
    uint8_t u0 = (x & 0xFF00) >> 8;
    uint8_t u1 = (x & 0x00FF);

    u0 = reverse_bits(u0);
    u1 = reverse_bits(u1);

// return concat_bytes(u1, u0);
    return ((u1 << 8) | u0); 

}


// View_as_bit_array
// ---------------

// Concebimos el array de bytes como un array de bits.
// Esta función escribe el byte x en la posición [i0, i0 + 8)
//
// Ejemplo:
//     i0= 2
//   data: 12345678 abcdefgh
//      x:       AB CDEFGH    
//  ------
//  data:  123456AB CDEFGHgh  <-- resultado
//
//
// Importante:
//  Es importante tener claro el orden de los bits en el que damos x:
//	
//  data: 0123456789...  <-- orden de los bits
//     x:     76543210   <-- orden de los bits
//
// Problema:
//  Hay 2 formas de almacenar los bytes
//  a) Como en Bitmatrix_col_1bit
//                      98 76543210    = i = posición del bit en el array
//	 76543210 76543210 76543210    = r = posición del bit dentro del byte
//	+--------+--------+--------+
//	|        |        |        |
//	+--------+--------+--------+
//          2        1        0        = I 
//
//      En este caso coincide el orden ascendente de i con el de r
//      ( i = 8*i + r )
//
//
//  b) Como en Bitmatrix_row_1bit
//                      98 76543210    = i
//	 01234567 01234567 01234567    = r'
//	+--------+--------+--------+
//	|        |        |        |
//	+--------+--------+--------+
//          2        1        0        = I
//
//      En este caso i y r tienen un orden contrario.
//      ( i = 8*I + r ; con r' + r = 7 ==> i = 8*I + 7 - r' = 8*(I + 1) - r')
//
//	Observar que b) es la forma normal de definir un array:
//	uint8_t x[3];
//
//       01234567 89..                 = i
//	 76543210 76543210 76543210    = r
//	+--------+--------+--------+
//	|        |        |        |
//	+--------+--------+--------+
//          0        1        2        = I
//
//	donde he dibujado b) usando el sentido habitual.
//
//
//  Como en a) coinciden el orden de i y r, llamemosla 'same_order' y a b)
//  'reverse_order'
template <size_t N, bool safe = true>
inline void in___bit___write_byte_same_order(uint8_t (&data)[N], 
							size_t i0, uint8_t x)
{
    auto [I0, r] = atd::div<size_t>(i0, 8u);

    if constexpr (safe) {
    if (I0 >= N)
	return;
    }

    if (r == 0){
	data[I0] = x;
	return;
    }
    data[I0] = (data[I0] & ~(0xFF << r)) | (x << r);
	
    ++I0;

    if (I0 >= N)
	return;

    data[I0] = (data[I0] & (0xFF << r)) | (x >> (8 - r));

}

template <size_t N, bool safe = true>
inline void in___bit___write_byte_reverse_order(uint8_t (&data)[N], 
							size_t i0, uint8_t x)
{
    auto [I0, r] = atd::div<size_t>(i0, 8u);

    if constexpr (safe) {
    if (I0 >= N)
	return;
    }

    if (r == 0){
	data[I0] = x;
	return;
    }
    data[I0] = (data[I0] & ~(0xFF >> r)) | (x >> r);
	
    ++I0;

    if (I0 >= N)
	return;

    data[I0] = (data[I0] & (0xFF << r)) | (x << (8 - r));

}


namespace impl_of{

template <size_t N>
struct View_as_bit_array{
// Constructor
    View_as_bit_array(uint8_t (&x) [N], size_t i0)
	: data{x}, i{i0} { }

// Operations
    void write_byte_same_order(uint8_t x)
    { in___bit___write_byte_same_order(data, i, x); }

    void write_byte_reverse_order(uint8_t x)
    { in___bit___write_byte_reverse_order(data, i, x); }

// Data
    uint8_t (& data)[N];
    size_t i;
};

}// namespace impl_of

template <size_t N>
struct View_as_bit_array{

    using Bit = impl_of::View_as_bit_array<N>;

// Constructor
    View_as_bit_array(uint8_t (&x) [N]) : data{x} {}

// Operations
    Bit bit(size_t i) {return Bit{data, i};}

// Data
    uint8_t (& data)[N];
};

template <size_t N>
inline 
View_as_bit_array<N> view_as_bit_array(uint8_t (&x) [N])
{ return View_as_bit_array<N>{x}; }

// Propuesta de idioma:
//  in(x) = convierte en una clase de atd el tipo básico.
//  La función atd::in quedará sobrecargada para cada tipo.
//  Ejemplos:
//	int x;
//	auto ax = atd::in(x);
//	ax.write(...);
//
// Esto está pensado para poder escribir cosas mas readables:
//	    in(x).write(y).in_bit(i);
//
template <size_t N>
inline 
View_as_bit_array<N> in(uint8_t (&x) [N])
{ return view_as_bit_array(x);}

}// namespace

#endif

