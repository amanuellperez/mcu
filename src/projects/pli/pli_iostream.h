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

#ifndef __PLI_IOSTREAM_H__
#define __PLI_IOSTREAM_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones genéricas para escribir/leer en iostreams
 *
 * HISTORIA
 *    Manuel Perez
 *    05/06/2024 press_key
 *
 ****************************************************************************/
#include <iostream>

namespace pli{

// press_key
// ---------
void press_key(std::iostream& os);
void press_key_to(const char* msg, std::iostream& os);

inline void press_key_to_continue(std::iostream& os)
{ press_key_to("continue", os); }

// ask
// ---
// Muestra el msg, que típicamente será una pregunta, y añade "(y/n)?"
// esperando que se responda y ó n. Si responde n devuelve false, en caso
// contrario devuelve true.
bool ask_yesno(std::iostream& os, const char* msg);

uint8_t ask_uint8_t(std::iostream& os, const char* msg);


}// namespace
 
#endif


