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

#include "main.h"
#include "cfg.h"
#include "user_time.h"

void Main::print(const System_clock::time_point& t0, uint8_t x0, uint8_t y0)
{
    std::time_t sec = System_clock::to_time_t(t0);
    std::tm t;
    ::gmtime_r(&sec, &t);

    ::print<Font>(scr_, atd::Generic_time_view{t}, x0, y0);
}




std::time_t Main::user_get_time(const std::time_t& t0, uint8_t x0, uint8_t y0)
{
    std::tm mt;
    ::gmtime_r(&t0, &mt);
    
    atd::Generic_time_view<std::tm> t{mt};

    ::user_get_time<Font>(scr_, keyboard_, t, x0, y0);

    return std::mktime(&mt);
}


System_clock::time_point Main::user_get_time(const System_clock::time_point& t0,
              uint8_t x0, uint8_t y0)
{
    time_t t = std::chrono::system_clock::to_time_t(t0);

    t = user_get_time(t, x0, y0);

    return std::chrono::system_clock::from_time_t(t);
}


