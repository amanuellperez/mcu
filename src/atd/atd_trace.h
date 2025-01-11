// Copyright (C) 2025 Manuel Perez 
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

#ifndef __ATD_TRACE_H__
#define __ATD_TRACE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para manejar trazas/logs ...
 *
 * HISTORIA
 *    Manuel Perez
 *    11/01/2025 Empty_log, 
 *
 ****************************************************************************/

namespace atd{

// Empty_log
// ---------
struct Empty_log{
    template<typename T>
    Empty_log(const T& x){}

    template <typename T>
    Empty_log& operator<<(const T& x){ return *this;}
    
};

// typename  Log_type<T>
// {
//	if requires (typename T::log)
//	    return T::log;
//	else
//	    return Empty_log;
// }
namespace impl_of{
template <typename T, bool with_log = requires {typename T::log;}>
struct Log_type;

template <typename T>
struct Log_type<T, true>{
    using log = T::log;
};

template <typename T>
struct Log_type<T, false>{
    using log = Empty_log;
};
}

template <typename T>
using Log_type = typename impl_of::Log_type<T>::log;

}// atd


#endif


