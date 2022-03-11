// Copyright (C) 2019-2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
 *    A.Manuel L.Perez
 *	09/11/2019 write_as_int
 *	06/03/2022 print(ostream, T)
 *
 ****************************************************************************/
#include <ostream>
#include <iomanip>
#include <stddef.h>
#include "atd_names.h"

namespace atd{
struct _Write_as_uint8_t{ uint8_t value; };

/// EXAMPLE:
/// std::ostream out;
/// uint8_t x = 10;
/// out << atd::write_as_uint8_t(x);
inline _Write_as_uint8_t write_as_uint8_t(uint8_t x)
{return _Write_as_uint8_t{x};}

inline std::ostream& operator<<(std::ostream& out, const _Write_as_uint8_t& x)
{
    return out << static_cast<uint16_t>(x.value);
}


struct _Write_as_int8_t{ int8_t value; };

inline _Write_as_int8_t write_as_int8_t(int8_t x)
{return _Write_as_int8_t{x};}

inline std::ostream& operator<<(std::ostream& out, const _Write_as_int8_t& x)
{
    return out << static_cast<int16_t>(x.value);
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
template <typename Int>
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

}// namespace


#endif


