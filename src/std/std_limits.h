// Copyright (C) 2019-2021 Manuel Perez 
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

#ifndef __MCU_STD_LIMITS_H__
#define __MCU_STD_LIMITS_H__

/****************************************************************************
 *
 * - DESCRIPCION: El equivalente a <limits>
 *
 * - HISTORIA:
 *  Manuel Perez 
 *	30/09/2019 Lo más básico.
 *	09/12/2019 lowest
 *	23/07/2021 radix, digits, digits10
 *
 ****************************************************************************/
#include "std_config.h"

#include <stdint.h>	// Voy a usar los tamaños y tipos definidos aqui
#include "std_climits.h"

namespace STD{

// DUDA: Los avr no operan directamente con double. Se supone que esta función
// se ejecuta en tiempo de compilación y por tanto no debería de instalar nada
// de código referente a double. Cierto??? Si no pudiera estar metiendo
// ineficiencias con ella.
// log10(2) = 643 / 2136. Esta función devuelve x * log10(2).
template <typename T>
inline constexpr auto __x_log10_of_2(const T& x)
{return (x * 643L / 2136);}


template <typename T>
struct numeric_limits;

template <typename T>
struct numeric_limits<const T> : public numeric_limits<T> {};

template<>
struct numeric_limits<int8_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = true;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

    static constexpr int digits		 = CHAR_BIT*sizeof(int8_t) - 1;
    static constexpr int digits10	 = __x_log10_of_2(digits);
    static constexpr int radix           = 2;

    static constexpr int8_t min() noexcept {return INT8_MIN;}
    static constexpr int8_t max() noexcept {return INT8_MAX;}

    static constexpr int8_t lowest() noexcept {return INT8_MIN;}
};

template<>
struct numeric_limits<uint8_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = false;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

    static constexpr int digits		 = CHAR_BIT*sizeof(uint8_t);
    static constexpr int digits10	 = __x_log10_of_2(digits);
    static constexpr int radix           = 2;

    static constexpr uint8_t min() noexcept {return 0;}
    static constexpr uint8_t max() noexcept {return UINT8_MAX;}

    static constexpr uint8_t lowest() noexcept {return 0;}
};


template<>
struct numeric_limits<int16_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = true;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

    static constexpr int digits		 = CHAR_BIT*sizeof(int16_t) - 1;
    static constexpr int digits10	 = __x_log10_of_2(digits);
    static constexpr int radix           = 2;

    static constexpr int16_t min() noexcept {return INT16_MIN;}
    static constexpr int16_t max() noexcept {return INT16_MAX;}

    static constexpr int16_t lowest() noexcept {return INT16_MIN;}
};

template<>
struct numeric_limits<uint16_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = false;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

    static constexpr int digits		 = CHAR_BIT*sizeof(uint16_t);
    static constexpr int digits10	 = __x_log10_of_2(digits);
    static constexpr int radix           = 2;

    static constexpr uint16_t min() noexcept {return 0;}
    static constexpr uint16_t max() noexcept {return UINT16_MAX;}

    static constexpr uint16_t lowest() noexcept {return 0;}
};



template<>
struct numeric_limits<int32_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = true;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

    static constexpr int digits		 = CHAR_BIT*sizeof(int32_t) - 1;
    static constexpr int digits10	 = __x_log10_of_2(digits);
    static constexpr int radix           = 2;

    static constexpr int32_t min() noexcept {return INT32_MIN;}
    static constexpr int32_t max() noexcept {return INT32_MAX;}

    static constexpr int32_t lowest() noexcept {return INT32_MIN;}
};

template<>
struct numeric_limits<uint32_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = false;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

    static constexpr int digits		 = CHAR_BIT*sizeof(uint32_t);
    static constexpr int digits10	 = __x_log10_of_2(digits);
    static constexpr int radix           = 2;

    static constexpr uint32_t min() noexcept {return 0;}
    static constexpr uint32_t max() noexcept {return UINT32_MAX;}

    static constexpr uint32_t lowest() noexcept {return 0;}
};


template<>
struct numeric_limits<int64_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = true;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

    static constexpr int digits		 = CHAR_BIT*sizeof(int64_t) - 1;
    static constexpr int digits10	 = __x_log10_of_2(digits);
    static constexpr int radix           = 2;

    static constexpr int64_t min() noexcept {return INT64_MIN;}
    static constexpr int64_t max() noexcept {return INT64_MAX;}

    static constexpr int64_t lowest() noexcept {return INT64_MIN;}
};

template<>
struct numeric_limits<uint64_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = false;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

    static constexpr int digits		 = CHAR_BIT*sizeof(uint64_t);
    static constexpr int digits10	 = __x_log10_of_2(digits);
    static constexpr int radix           = 2;

    static constexpr uint64_t min() noexcept {return 0;}
    static constexpr uint64_t max() noexcept {return UINT64_MAX;}

    static constexpr uint64_t lowest() noexcept {return 0;}
};


}// namespace

#endif


