// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __DEV_LCD_HD4780_H__
#define __DEV_LCD_HD4780_H__

// Cuando un usuario quiera usar este LCD basta con que incluya este archivo
// de cabecera sin tener que recordar qué archivos necesita.
#include "dev_HD44780_basic.h"
#include "dev_HD44780_generic.h"
#include "dev_HD44780_charset.h"

// DUDA: Los dos siguientes archivos no pertenecen a `hwd` sino a `logic`
// Pero es cómodo tenerlos dentro de este archivo de cabecera.
// ¿Mover este archivo a `logic`? ¿dejarlo aquí? (<- raro)
#include "dev_LCD_screen.h"
#include "dev_LCD_ostream.h"

#endif
