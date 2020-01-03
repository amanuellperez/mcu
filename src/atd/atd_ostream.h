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
 *
 ****************************************************************************/
#include <iostream>
#include <stddef.h>

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


}// namespace


#endif


