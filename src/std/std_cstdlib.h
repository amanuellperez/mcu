// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __MTD_STD_CSTDLIB_H__
#define __MTD_STD_CSTDLIB_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Equivalente a cstdlib
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    09/03/2020 std::div
 *
 ****************************************************************************/
#include "std_config.h"

#include <stdlib.h> 

// Eliminamos posibles macros
#undef div
#undef ldiv

namespace STD{
    using ::div_t;
    using ::ldiv_t;
//    using ::lldiv_t;
    
    using ::ldiv;
//   using ::lldiv;

    inline ::div_t div(int x, int y) 
    { return ::div(x, y); }

    inline ::ldiv_t div(long x, long y) 
    { return ::ldiv(x, y); }

    //inline ::lldiv_t div(long long x, long long y) 
    inline void div(long long x, long long y) 
    {static_assert(true, "Not defined ::lldiv_t");}
//    {return ::lldiv(x ,y);}

}// namespace


#endif

