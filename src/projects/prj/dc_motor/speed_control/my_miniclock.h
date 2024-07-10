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

#ifndef __PRJ_MINICLOCK_H__
#define __PRJ_MINICLOCK_H__

#include <mcu_miniclock.h>

namespace my{

template <typename Micro_t,   
	 typename Time_counter_t>
class Miniclock_ms : public mcu::Miniclock_ms<Micro_t, Time_counter_t>{
public:
    using Base = mcu::Miniclock_ms<Micro_t, Time_counter_t>;

// construction
    Miniclock_ms() = delete;

    static void init() 
    {
	Base::init();
	on_ = false;
    }

    static void reset() { init(); }
    
// operations
    static void start() 
    {
	Base::start();
	on_ = true;
    }

    static void stop() 
    {
	Base::stop();
	on_ = false;
    }

// info
    static bool is_on () { return on_;}
    static bool is_off() { return !is_on();}

private:
    inline static volatile bool on_; 
};


}// namespace
#endif


