// Copyright (C) 2021 Manuel Perez 
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
#ifndef __MCU_STD_CSTRING_H_
#define __MCU_STD_CSTRING_H_
/****************************************************************************
 *
 *  - DESCRIPCION: <cstring> implementation
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    04/01/2021 v0.0
 *
 ****************************************************************************/

#include "std_config.h"

#include <string.h>

#undef memcpy
#undef memmove
#undef strcpy
#undef strncpy
#undef strcat
#undef strncat
#undef memcmp
#undef strcmp
#undef strcoll
#undef strncmp
#undef strxfrm
#undef memchr
#undef strchr
#undef strcspn
#undef strpbrk
#undef strrchr
#undef strspn
#undef strstr
#undef strtok
#undef memset
#undef strerror
#undef strlen

namespace STD{

  using ::memcpy;
  using ::memmove;
  using ::strcpy;
  using ::strncpy;
  using ::strcat;
  using ::strncat;
  using ::memcmp;
  using ::strcmp;
  using ::strcoll;
  using ::strncmp;
  using ::strxfrm;
  using ::strcspn;
  using ::strspn;
  using ::strtok;
  using ::memset;
  using ::strerror;
  using ::strlen;
  using ::memchr;
  using ::strchr;
  using ::strpbrk;
  using ::strrchr;
  using ::strstr;

}// namespace


#endif

