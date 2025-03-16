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

#ifndef __PRJ_PRINT_H__
#define __PRJ_PRINT_H__

#include <ostream>
#include <span>
#include "prj_main.h"

void print_as_char(std::ostream& out, std::span<uint8_t> str);
void print(std::ostream& out, std::span<uint8_t> str);
void print(std::ostream& out, Attribute att, bool with_tab = true);

uint8_t read_cstring(std::ostream& out, std::span<uint8_t> str);


#endif
