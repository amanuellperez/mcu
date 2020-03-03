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
 *  AManuel Lopez Perez
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
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t
#include <cstddef>
#include <type_traits>
#include "atd_type_traits.h"	// static_array

// avr define bit
#undef bit

namespace atd{

// ------------
// concat_bytes
// ------------
template <typename Int, typename Byte>
Int __concat_bytes(Int res, Byte b)
{
    res <<= 8;
    res |= static_cast<uint8_t>(b);

    return res;
}


template <typename Int, typename Byte, typename... Tail>
Int __concat_bytes(Int res, Byte b, Tail... b1_bn)
{
    res <<= 8;
    res |= static_cast<uint8_t>(b);

    return __concat_bytes(res, b1_bn...);
}


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
    return __concat_bytes(res, b1_bn...);
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

    // get_bits
    constexpr operator Int() const 
    { return (reg & mask) >> pos;}

    // set_bits
    constexpr Int& operator=(const Int& x)
    {
	reg = ((reg & ~mask) | ((x << pos) & mask));
	return reg;
    }
};


/*!
 *  \brief Contiguous bitmask.
 *  
 *  Example:
 *	constexpr atd::Range_bitmask<2,6, uint8_t> mask; // mask bits 2-6
 *	uint8_t x = 0x35;
 *	uint8_t res = mask(x); // = get_bits_2_to_6_of(x);
 *
 *	mask(x) = uint8_t{0x16}; // = set_bits_2_to_6_of(x)_equal_to(0x16);
 *
 */
template <uint8_t i0, uint8_t i1, typename Int>
struct Range_bitmask{
    static_assert (i0 <= i1, "i0 must be less or equal than i1");

    constexpr __Range_bitmask<i0, i1, Int> operator()(Int& reg) const
    { return __Range_bitmask<i0, i1, Int>{reg}; }
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
 * DUDA: al principio lo llamaba:
 *  atd::write_bits<1,3,4,7>::to<1,0,1,1>::in_register(x);
 *  El nombre un poco largo, por eso lo acorté.
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

// Si solo queremos escribir 1 bit en singular (syntactic sugar)
template <int... bitpos>
using write_bit = __atd::write_bits<bitpos...>;



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
 *  pero la regla es evisa ser criptico!!! Pruebo con of!!
 */
template <int pos>
struct is_one_bit{
    template <typename Int>
    static bool of(const Int& x){
	return static_cast<bool>(x & (Int{1} << pos));
    }
};


/*!
 *  \brief  is_zero_bit<1>::of_register(x)?
 */
template <int pos>
struct is_zero_bit{
    template <typename Int>
    static bool of(const Int& x){
	return !static_cast<bool>(x & (Int{1} << pos));
    }
};



// Ejemplo (para ver como hacer un for en metaprogramming)
//template<int... pos, typename Int>
//inline constexpr void write_one_bit(Int& i)
//{
//    using expand = int[];
//    expand{0,
//	    ((i |= (Int{1} << pos)), 0)...};
//}

}// namespace

#endif

