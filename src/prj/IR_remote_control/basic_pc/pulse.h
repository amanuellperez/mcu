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

#ifndef __PULSE_H__
#define __PULSE_H__

#include <atd_array.h>

struct Pulse{
    uint16_t time_low;
    uint16_t time_high;

    uint16_t period() const {return time_low + time_high;}
};

void receive_pulses(atd::CArray_view<Pulse>& pulse);
void transmit_pulses(atd::CArray_view<Pulse>& pulse);



#endif
