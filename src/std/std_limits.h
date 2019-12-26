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

#ifndef __MCU_STD_LIMITS_H__
#define __MCU_STD_LIMITS_H__

/****************************************************************************
 *
 * - DESCRIPCION: El equivalente a <limits>
 *
 * - HISTORIA:
 *  A.Manuel Lopez 
 *	30/09/2019 Lo más básico.
 *	09/12/2019 lowest
 *
 *
 ****************************************************************************/
#include "std_config.h"

#include <stdint.h>	// Voy a usar los tamaños y tipos definidos aqui

namespace STD{
template <typename T>
struct numeric_limits;


template<>
struct numeric_limits<int8_t>{
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed      = true;
    static constexpr bool is_integer     = true;
    static constexpr bool is_exact       = true;

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

    static constexpr uint64_t min() noexcept {return 0;}
    static constexpr uint64_t max() noexcept {return UINT64_MAX;}

    static constexpr uint64_t lowest() noexcept {return 0;}
};


}// namespace

#endif


