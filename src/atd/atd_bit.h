// Copyright (C) 2019-2020 A.Manuel L.Perez
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
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t

// avr define bit
#undef bit

namespace atd{


/***************************************************************************
 *			FUNCIONES DE ESCRITURA DE BITS
 ***************************************************************************/
/// Escribimos un 1 en las posiciones indicadas.
/// Ejemplo: write_one_bit<1,3>(x); // escribe un 1 en los bits 1 y 3 de x
template<int... pos, class Int>
inline constexpr void write_one_bit(Int& i)
{
    using expand = int[];
    expand{0,
	    ((i |= (Int{1} << pos)), 0)...};
}

/// Escribimos un 0 en las posiciones indicadas.
/// Ejemplo: write_zero_bit<1,3>(x); // escribe un 0 en los bits 1 y 3 de x
template<int... pos, class Int>
inline constexpr void write_zero_bit(Int& i)
{
    using expand = int[];
    expand{0,
	    ((i &= ~(Int{1} << pos)), 0)...};

}


/// Cambiamos los bits de las posiciones indicadas, de 0 -> 1 y de 1 -> 0.
/// Ejemplo: flip_bit<1,3>(x); // cambia los bits 1 y 3 de x.
template<int... pos, class Int>
inline constexpr void flip_bit(Int& i)
{
    using expand = int[];
    expand{0,
	    ((i ^= (Int{1} << pos)), 0)...};

//	return i;
}

// Otra forma de hacerlo: 
// ----------------------
// se llamaria set_bits_b(x, 1, 3, 5):
//
//template <typename Int, typename...Bit_ofs>
//constexpr auto set_bits_b(Int i, Bit_ofs...bits)
//{
//    using expand = int[];
//    void(expand{0,
//	((i |= (Int(1) << bits)),0)...});
//
//    return i;
//}


//    /// ¿el bit es cero?. Sinónimo: is_low.
//    template<typename T>
//    bool is_zero(T& x, uint8_t p)
//    {return !bit(x,p);}
//
//
//    /// ¿el bit es low?. Sinónimo: is_zero.
//    template<typename T>
//    bool is_low(T& x, uint8_t p)
//    {return is_zero(x, p);}

/// Concatena los bits x,y devolviendo la cadena de 16 bits: xy.
template <typename Int8bits>
// requires: sizeof(Int8bits) == 1 and sizeof(char) == 8bits.
inline uint16_t concat_bits(Int8bits x, Int8bits y)
{
    uint16_t res = static_cast<uint8_t>(x);

    res <<= 8;   
    res |= y;

    return res;
}

/// Devuelve x con todos los bits 0 menos el bit 'pos'.
template <uint8_t pos, typename Int>
constexpr inline Int bitmask0(Int x)
{ return x & (Int{1} << pos); }

/// Devuelve un Int con todo 0 salvo el bit 'pos' que será 1.
template <uint8_t pos, typename Int>
constexpr inline Int bitmask0()
{ return (Int{1} << pos); }

/// Devuelve un Int con todo 1 salvo el bit 'pos' que será 0
// REVISAR:
// Da un warning de que estoy intentando hacer una conversión de signed a
// unsigned. Como no encuentro donde cambio la implementación, confío que el
// compilador lo optimice (???) 
template <uint8_t pos, typename Int>
constexpr inline Int bitmask1()
{ 
//    return ~bitmask0<pos, Int>();
    Int x = bitmask0<pos, Int>();
    return ~x;
}



// cuando el tipo T es const, la reference tiene que ser const!!!
//template<typename T>
//using Reference_type = std::conditional_t<std::is_const_v<T>, 
//			    const T&, T&>;

template <uint8_t pos, typename Int>
class Bit_of{
public:
//    using Ref = Reference_type<Int>;

    constexpr explicit Bit_of(Int& x) : x_{x} {}

    constexpr operator bool() const 
    { return (x_ & (Int{1} << pos)) != Int{0};}

    // s = true ==> set bit
    // s = false ==> clear bit
    constexpr void operator=(bool s)
    {
	if (s) set();
	else clear();
    }

    constexpr void set()    { x_ |= bitmask0<pos, Int>(); }
    constexpr void clear()  { x_ &= bitmask1<pos, Int>(); }

private:
    Int& x_;	
};


template <uint8_t pos>
struct bit{
//    /// Nos dice si el bit que ocupa la posición pos es 0 ó 1.
//    template <typename Int>
//    constexpr static bool of(Int x)
//    { return static_cast<bool>(x & (Int{1} << pos)); }

    /// Nos dice si el bit que ocupa la posición pos es 0 ó 1.
    template <typename Int>
    constexpr static Bit_of<pos, Int> of(Int& x)
    { return Bit_of<pos, Int>{x};}

    template <typename Int>
    constexpr static Bit_of<pos, const Int> of(const Int& x)
    { return Bit_of<pos, const Int>{x};}
};





}// namespace

#endif

