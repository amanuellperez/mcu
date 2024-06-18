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

#ifndef __PRJ_CFG_H__
#define __PRJ_CFG_H__
#include "atd_locale.h"


// Elegir el locale para elegir el formato de fechas y demás
static constexpr 
    //locale::Format_time default_locale = locale::Format_time::spain;
    locale::Format_time default_locale = locale::Format_time::usa;


template <typename Date_time>
inline void print_datetime(std::ostream& out, const Date_time& t)
{ locale::print_datetime<default_locale>(out, t); }



#endif


