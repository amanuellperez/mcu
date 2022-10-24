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

#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Stopwatch
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    13/04/2022 v0.0
 *
 ****************************************************************************/
#include <dev_clocks.h>

namespace my{
// Solo una instancia de esta clase posible (Chronometer_ms solo admite una)
// por eso el constructor esta delete.
template <typename Timer,   // = Generic_timer
	 uint16_t timer_period_in_us> 
class Stopwatch{
public:
// types
    using Chrono         = dev::Chronometer_ms<Timer, timer_period_in_us>;
    using Rep		 = Chrono::rep;
    using Sexagesimal_ms = Chrono::Sexagesimal_ms;

// Constructors
    Stopwatch() = delete;
    static void init(); 

// Interface
    static void start();
    static void stop();

// Data
    static Rep count();
    static Sexagesimal_ms sexagesimal_count();

// Don't forget the interruption
    static void tick();

private:
    enum class State{off, on, halt};
    inline static State state_;
};


template <typename T, uint16_t t>
void Stopwatch<T,t>::init() 
{ 
    state_ = State::off;
    Chrono::init(); 
}


template <typename T, uint16_t t>
void Stopwatch<T,t>::start()
{
    switch(state_){
	break; case State::off:
	    Chrono::on();
	    state_ = State::on;

	break; case State::on:
	    Chrono::off();
	    state_ = State::halt;
	    
	break; case State::halt:
	    Chrono::on();
	    state_ = State::on;
    }
}


template <typename T, uint16_t t>
void Stopwatch<T,t>::stop()
{
    if (state_ == State::off)
	return;

    Chrono::off();
    Chrono::reset();
    state_ = State::off;

//    switch(state_){
//	break; case State::off:
//	    // nothing
//
//	break; case State::on:
//	    Chrono::off();
//	    state_ = State::off;
//		
//	break; case State::halt:
//	    Chrono::off();
//	    Chrono::reset();
//	    state_ = State::off;
//    }
}

template <typename T, uint16_t t>
inline Stopwatch<T,t>::Rep Stopwatch<T,t>::count()
{return Chrono::count();}

template <typename T, uint16_t t>
inline Stopwatch<T,t>::Sexagesimal_ms Stopwatch<T,t>::sexagesimal_count()
{return Chrono::sexagesimal_count();}

template <typename T, uint16_t t>
inline void Stopwatch<T,t>::tick() {Chrono::tick();}

}// namespace

#endif



