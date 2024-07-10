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

#ifndef __MCU_TIME_H__
#define __MCU_TIME_H__

// Nombre?
//	if (no_response_after(300)_ms_while(f)) abort;
template <typename Wait_1_ms, typename Predicate>
bool no_response_after_ms_while(uint8_t time_ms, Predicate f, Wait_1_ms wait_1_ms)
{
    if (time_ms == 0)
	return true;

    while (f()){
	wait_1_ms();
	--time_ms;
	if (time_ms == 0)
	    return true;
    }

    return false;
}

struct no_response_after{
    no_response_after(uint16_t wait_time_in_ms) 
	    : wait_time_in_ms_ {wait_time_in_ms} {}

    template <typename Predicate, typename Wait_1_ms>
    bool ms_while(Predicate f, Wait_1_ms wait_1_ms)
    { return no_response_after_ms_while(wait_time_in_ms_, f, wait_1_ms);}
	    
    uint16_t wait_time_in_ms_;
};

#endif


