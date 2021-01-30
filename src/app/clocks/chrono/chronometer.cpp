// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "chronometer.h"


namespace dev{

__Chronometer_sexagesimal_ms::__Chronometer_sexagesimal_ms(int32_t t)
{
    int32_t tmp;
    std::tie(tmp, milliseconds) = atd::div<int32_t>(t, 1000);
    std::tie(tmp, seconds) = atd::div<int32_t>(tmp, 60);
    std::tie(hours, minutes) = atd::div<int32_t>(tmp, 60);
}


int32_t __Chronometer_sexagesimal_ms::to_milliseconds() const
{
    int32_t res = milliseconds;
    res += seconds*1000;
    res += minutes*60*1000;
    res += hours*60*60*1000;

    return res;
}

}// namespace
