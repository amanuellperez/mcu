// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __AVR_MEMORY_H__
#define __AVR_MEMORY_H__

// Indica el número de bytes de RAM libres (para ver si se está usando
// demasiada memoria).
// Ver avr-libc-user-manual-2.0.0, punto 3.
// Esta función es copia de internet.
// No puedo incluirla dentro del namespace avr. Da error (???)
inline int bytes_of_free_ram() 
{
  extern int __heap_start; 
  extern int* __brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


#endif


