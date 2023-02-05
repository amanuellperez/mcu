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

#include "atd_bcd.h"

namespace atd{

Counter_BCD2::Counter_BCD2(uint8_t d1_max, uint8_t d0_max) 
{ 
    max(d1_max, d0_max); 
    reset();
}

void Counter_BCD2::max(uint8_t d1_max, uint8_t d0_max)
{
    nticks_.d0_max = d0_max;
    nticks_.d1_max = d1_max;
}

void Counter_BCD2::reset()
{
    nticks_.d0 = 0;
    nticks_.d1 = 0;
}
void Counter_BCD2::tick()
{
    if (nticks_.d0 == nticks_.d0_max){
	nticks_.d0 = 0;
	
	if (nticks_.d1 == nticks_.d1_max)
	    nticks_.d1 = 0;
	else
	    ++nticks_.d1;
    }

    else
	++nticks_.d0;
}
}// namespace

