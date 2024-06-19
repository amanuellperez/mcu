// Copyright (C) 2024 Manuel Perez 
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

#ifndef __PRJ_IOSTREAM_H__
#define __PRJ_IOSTREAM_H__

// INTERFAZ CON EL USUARIO

#include <iostream>

void press_key(std::iostream& os);
void press_key_to(const char* msg, std::iostream& os);

inline void press_key_to_continue(std::iostream& os)
{ press_key_to("continue", os); }

uint8_t ask_uint8_t(std::iostream& os, const char* msg);
bool ask_yesno(std::iostream& os, const char* msg);

#endif


