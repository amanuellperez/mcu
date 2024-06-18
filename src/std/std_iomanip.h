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

#ifndef __MCU_STD_IOMANIP_H__
#define __MCU_STD_IOMANIP_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Fichero correspondiente a iomanip.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	30/10/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_basic_ios.h"
#include "std_char_traits.h"
#include "std_ostream.h"

namespace STD{

// setfill
// -------
struct _Setfill {char c;};

inline _Setfill setfill(char c) {return _Setfill{c};}

// TODO: operator<< setfill(c);

inline ostream& operator<<(ostream& out, _Setfill sf)
{ 
    out.fill(sf.c); 
    return out;
}


// setw
// ----
struct _Setw {int w;};

inline _Setw setw(int w) {return _Setw{w};}

inline ostream& operator<<(ostream& out, _Setw sw)
{
    out.width(sw.w);
    return out;
}




}// namespace

#endif



