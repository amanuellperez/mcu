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

#include "main.h"


void Main::window_main()
{
    switch(state_){
	case State::stop    : window_state_stop(); break;
	case State::running : window_state_running(); break;
	case State::alarm   : window_state_alarm(); break;
    }

}


void Main::window_state_stop()
{
    while (keyboard_.key<OK_KEY>().is_not_pressed()){
	if (keyboard_.key<UP_KEY>().is_pressed()){
	    Chronometer::add(std::chrono::seconds{1});
	    print_time();
	}

	else if (keyboard_.key<DOWN_KEY>().is_pressed()){
	    Chronometer::substract(std::chrono::seconds{1});
	    print_time();
	}

	wait_release_key();
    }

    Chronometer::start();
    state_ = State::running;

    wait_release_key();
}


void Main::window_state_running()
{
    if (keyboard_.key<OK_KEY>().is_pressed()){
	Chronometer::stop();
	state_ = State::stop;
    }

    else if (std::chrono::seconds{Chronometer::now().time_since_epoch()} ==
             std::chrono::seconds{0}) {
	Chronometer::stop();
	state_ = State::alarm;
    }

    print_time();

}

void Main::window_state_alarm()
{
    lcd_.clear();
    lcd_ << "ALARM!!!";
}


void Main::print_time()
{
    lcd_.cursor_pos(0,0);
    std::chrono::seconds sec{Chronometer::now().time_since_epoch()};
    lcd_ << sec.count();
}

