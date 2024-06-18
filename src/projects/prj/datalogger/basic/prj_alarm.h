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

#ifndef __PRJ_ALARM_H__
#define __PRJ_ALARM_H__

#include "hwd_dev.h"
#include "prj_locale.h"

// template <typename Clock>
class Alarm{
public:
    using rep        = Clock::rep;
    using time_point = Clock::time_point;
    using duration   = Clock::duration;

    Alarm();
    
    void reset(rep t = 0);
    void reset(rep t, duration incr);

    time_point time() const {return time_next_alarm_;}
    void update(time_point t);

    void print(std::ostream& out) const;

private:
    time_point time_next_alarm_;
    duration incr_alarm_;
};

inline Alarm::Alarm()
	: time_next_alarm_{},
	  incr_alarm_ {std::chrono::seconds{1}}
{}

inline void Alarm::reset(rep t)
{ time_next_alarm_ = time_point{duration{t}}; }

inline void Alarm::reset(rep t, duration incr)
{
    reset(t);
    incr_alarm_ = incr;
}


inline void Alarm::update(time_point t)
{ time_next_alarm_ = t + incr_alarm_; }


inline void Alarm::print(std::ostream& out) const
{
    out << "Time next alarm: ";
    print_time(out, time_next_alarm_);
    out << "\nAlarm every ";
    out << incr_alarm_.count() << " seconds\n";
}


#endif


