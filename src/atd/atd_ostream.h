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

#ifndef __ATD_OSTREAM_H__
#define __ATD_OSTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de ostream
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	09/11/2019 write_as_int
 *	06/03/2022 print(ostream, T)
 *	05/11/2022 print_as_hex
 *	23/10/2023 Generalizo write_as_uint8_t comom write_as_int
 *	05/01/2024 print_int_as_hex: prefijo \x
 *
 ****************************************************************************/
#include <ostream>
#include <iomanip>
#include <stddef.h>
#include "atd_names.h"
#include "atd_concepts.h"

namespace atd{

// write_as_int
// ------------
// (RRR) El standard de C++ no define uint8_t/int8_t como enteros sino como
//       caracteres (básicamente se miente al decir que uint8_t es un número).
//       Eso genera el problema de que al imprimirlos se imprimen como
//       caracteres. 
//       La función `write_as_int` pretende dar un interfaz genérico para
//       poder imprimir números sean del tipo que sea (sobraría si
//       uint8_t/int8_t fueran realmente lo que son: números).
//
// No se puede meter Write_as_int_t dentro del namespace impl_of, ya que la
// deducción automática de templates luego no encontraría la función
// operator<< correspondiente.
template <Type::Integer Int>
struct Write_as_int_t { Int value;};

template <Type::Integer Int>
inline Write_as_int_t<Int> write_as_int(const Int& x)
{return Write_as_int_t<Int>{x};}

template <Type::Integer Int>
inline std::ostream& operator<<(std::ostream& out, 
				    const Write_as_int_t<Int>& x)
{
    if constexpr (std::is_same_v<Int, uint8_t>)
	return out << static_cast<uint16_t>(x.value);

    if constexpr (std::is_same_v<Int, int8_t>)
	return out << static_cast<int16_t>(x.value);

    return out << x.value;
}


/***************************************************************************
 *		OSTREAM COMO DISPOSITIVO GENÉRICO DE SALIDA
 ***************************************************************************/
// (RRR) Una screen es un dispositivo de salida pero no es un flujo de
//       caracteres (mas bien es un sistema de ventanas). Si quiero integrar
//       los ostreams con las screens necesito concebirlos los dos como
//       dispositivos genéricos de salida. Estos dispositivos tendrán una
//       función 'print' que es la que suministro aquí.
//       De momento esto es experimental (06/03/2022)

// Números
// -------
inline std::ostream& print(std::ostream& out, short x) { return out << x; }
inline std::ostream& print(std::ostream& out, int x) { return out << x; }
inline std::ostream& print(std::ostream& out, long x) { return out << x; }
inline std::ostream& print(std::ostream& out, long long x) { return out << x; }

inline std::ostream& print(std::ostream& out, unsigned short x) { return out << x; }
inline std::ostream& print(std::ostream& out, unsigned int x) { return out << x; }
inline std::ostream& print(std::ostream& out, unsigned long x) { return out << x; }
inline std::ostream& print(std::ostream& out, unsigned long long x) { return out << x; }

// avr-g++ no tiene float ni double
#ifndef __AVR__ 
inline std::ostream& print(std::ostream& out, float x) { return out << x; }
inline std::ostream& print(std::ostream& out, double x) { return out << x; }
#endif

// Números fijando anchura
// -----------------------
// La impresión de números con anchura es:
// 1. Todos los números se alinean a la derecha (es como se colocan para sumar)
// 2. si fill = true, se pone el 0 (por defecto)
template <Type::Integer Int>
std::ostream&
__print_ostream(std::ostream& out, Int x, const nm::Width<int>& w, bool fill)
{
    if (fill)
	out.fill('0');

    out << std::setw(w) << x; 

    return out;
}

inline std::ostream&
print(std::ostream& out, short x, const nm::Width<int>& w, bool fill = true)
{ return __print_ostream(out, x, w, fill);}

inline std::ostream&
print(std::ostream& out, int x, const nm::Width<int>& w, bool fill = true)
{ return __print_ostream(out, x, w, fill);}

inline std::ostream&
print(std::ostream& out, long x, const nm::Width<int>& w, bool fill = true)
{ return __print_ostream(out, x, w, fill);}

inline std::ostream&
print(std::ostream& out, long long x, const nm::Width<int>& w, bool fill = true)
{ return __print_ostream(out, x, w, fill);}

inline std::ostream& print(std::ostream& out,
                           unsigned short x,
                           const nm::Width<int>& w, bool fill = true)
{ return __print_ostream(out, x, w, fill);}

inline std::ostream& print(std::ostream& out,
                           unsigned int x,
                           const nm::Width<int>& w, bool fill = true)
{ return __print_ostream(out, x, w, fill);}

inline std::ostream& print(std::ostream& out,
                           unsigned long x,
                           const nm::Width<int>& w, bool fill = true)
{ return __print_ostream(out, x, w, fill);}

inline std::ostream& print(std::ostream& out,
                           unsigned long long x,
                           const nm::Width<int>& w, bool fill = true)
{ return __print_ostream(out, x, w, fill);}



// caracteres
// ----------
inline std::ostream& print(std::ostream& out, char x) { return out << x; }
inline std::ostream& print(std::ostream& out, unsigned char x) { return out << x; }
inline std::ostream& print(std::ostream& out, signed char x) { return out << x; }
inline std::ostream& print(std::ostream& out, const char* x) { return out << x; }



/***************************************************************************
 *			    FORMATS
 ***************************************************************************/
template <Type::Integer Int>
char nibble2hex(Int x)
{
    switch(x){
	break; case 0x00: return '0';
	break; case 0x01: return '1';
	break; case 0x02: return '2';
	break; case 0x03: return '3';
	break; case 0x04: return '4';
	break; case 0x05: return '5';
	break; case 0x06: return '6';
	break; case 0x07: return '7';
	break; case 0x08: return '8';
	break; case 0x09: return '9';
	break; case 0x0A: return 'A';
	break; case 0x0B: return 'B';
	break; case 0x0C: return 'C';
	break; case 0x0D: return 'D';
	break; case 0x0E: return 'E';
	break; case 0x0F: return 'F';
    }

    return '?';
}

// No he implementado la salida en hexadecimal en std::ostream, así que
// suministro esta. Esto quedará obsoleto cuando implemente ios_base::hex
//
// prefix: indica qué prefijo ponerle al número hexadecimal. Valores:
//	    0 - no poner prefijo
//	    'x' - poner prefijo 0x
//	    '\' - poner prefijo '\'
template <Type::Integer Int>
std::ostream& print_int_as_hex(std::ostream& out, Int x, uint8_t prefix = 'x')
{
    if (prefix == 'x')
	out << "0x";

    else if (prefix == '\\')
	out << "\\x";

    for (size_t i = sizeof(Int); i > 0; --i){
	Int b = (x >> (i - 1) * 8);
	Int b1 = (b >> 4) & 0x0F;
	Int b0 = (b & 0x0F);

	out << nibble2hex(b1);
	out << nibble2hex(b0);

    }

    return out;
}



}// namespace


#endif


