// Copyright (C) 2020-2022 Manuel Perez 
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
#ifndef __ATD_MEMORY_H__
#define __ATD_MEMORY_H__

/****************************************************************************
 *
 *  DESCRIPCION
 *	Cosas relacionadas con memoria.
 *
 *  HISTORIA:
 *    Manuel Perez
 *    08/04/2020 Memory_type
 *
 ****************************************************************************/

namespace atd{

// Memory_type
// -----------
enum class Memory_type{
    read = 1,
    write = 2,
    read_only = read,
    write_only = write, 
    read_and_write = read | write
};

inline constexpr bool is_readable(atd::Memory_type mem)
{
    return mem == atd::Memory_type::read or
           mem == atd::Memory_type::read_and_write;
}

inline constexpr bool is_writeable(atd::Memory_type mem)
{
    return mem == atd::Memory_type::write or
           mem == atd::Memory_type::read_and_write;
}




}// namespace

#endif


