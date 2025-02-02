// Copyright (C) 2023 Manuel Perez 
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

#ifndef __DEV_PRINT_H__
#define __DEV_PRINT_H__

#include "prj_dev.h"
#include "dev_strings.h"

// Convenio (v0.0):
//  + Funciones `print` se limitan a imprimir un mensaje.
//  + Funciones `ask` imprimen un mensaje y devuelven una respuesta (sería el
//    análogo a `scanf` de C)
template <typename String>
void print_question(std::ostream& out, const String& str, bool with_tab = true)
{
    if (with_tab)
	out << '\t';

    atd::print(out, str);
    out << "? ";
}


void print_bool_as_yes_no(std::ostream& out, bool x);
void ask_press_key_to_continue(std::iostream& out);
inline void print_line(std::ostream& out) { atd::print(out, msg_line);}


#endif


