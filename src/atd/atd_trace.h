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
 *	De momento suministro dos formas de trazas:
 *
 *	1. Log_type, pensada para trazar clases de biblioteca.
 *	   Cuando depuro una nueva clase me encuentro con que necesito poder
 *	   trazar el interior de las funciones de esa clase, cosa que no puedo
 *	   sin modificar el código. Log_type sirve para eso.
 *	   Amen que se pueden dejar trazas importantes que si se pasa Cfg::log
 *	   permitirá trazarlas.
 *
 *	2. Para trazar un programa grande son útiles las trazas con niveles:
 *		ctrace(9) << "se traza si TRACE_LEVEL <= 9";
 *
 *	   Como en micros estoy obsesionado con no generar más código del
 *	   necesario en lugar de la anterior usaré
 *		ctrace<9>() << "se traza si TRACE_LEVEL <= 9";
 *		
 *	    para garantizar que el compilador elimine esta llamada si
 *	    TRACE_LEVEL < 9 o si no está definida TRACE_LEVEL.
 *
 * HISTORIA
 *    Manuel Perez
 *    11/01/2025 Empty_log, Log_type
 *    02/02/2025 ctrace
 *    16/02/2025 trace_level
 *
 ****************************************************************************/
#include <ostream>
#include <stdint.h>

namespace atd{

/***************************************************************************
 *				Log_type
 ***************************************************************************/
// La idea es poder trazar de forma condicional las clases pasándole como
// parámetro o no el flujo Cfg::log. Si se pasa Cfg::log se trazaran las
// funciones, en caso contrario no se incluirá nada de código de trazado.
//
// Ejemplo:
//	template<typename Cfg>
//	struct A{
//	    using log = atd::Log_type<Cfg>;
//	};
//
//	Y para usarla:
//	struct Cfg{
//	    using log = Log<UART_iostream>;
//	};
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



/***************************************************************************
 *				ctrace
 ***************************************************************************/
struct Null_ostream : std::ostream{
// (DUDA) puedo construirlo con un nullptr?? Debiera ya que no voy a usar
// ninguna función de ostream.
    Null_ostream() : std::ostream(nullptr) {}

    template <typename T>
    Null_ostream& operator<<(const T&) {return *this;}
};

extern Null_ostream null_ostream;

struct CTrace{
    inline static std::ostream* log = &null_ostream;
};

inline void ctrace_bind(std::ostream& out) 
{ CTrace::log = &out; }

template <int n>
std::ostream& ctrace()
{
#ifdef TRACE_LEVEL
    if constexpr (n <= TRACE_LEVEL)
	return *CTrace::log;

    else
	return null_ostream;
#else
    return null_ostream;
#endif

}


// trace_level_gt
// --------------
// Pensada para escribir cosas del tipo:
//
//	if constexpr (trace_level<9>()){
//	...
//	}
//
template <uint8_t level>
inline constexpr bool trace_level()
{ 
#ifdef TRACE_LEVEL
    return level <= TRACE_LEVEL; 
#else
    return false;
#endif
}

}// atd


#endif


