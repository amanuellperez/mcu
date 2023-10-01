// Copyright (C) 2022 Manuel Perez 
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

#include "main.h"
#include "cfg.h"
#include "user_time.h"

void Main::print(const Date_time& t0, uint8_t x0, uint8_t y0)
{
    ::print<Font>(scr_, t0, x0, y0);
}




//std::time_t Main::user_get_time(const std::time_t& t0, uint8_t x0, uint8_t y0)
void Main::user_get_time(Date_time& t, uint8_t x0, uint8_t y0)
{
    ::user_get_time<Font>(scr_, keyboard_, t, x0, y0);
}


