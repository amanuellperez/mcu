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

#include "main.h"

void Main::run()
{
    lcd_.clear();

    while(1){
	// TODO: if (errno_) error();
	if (sensor_.error()) {
	    lcd_.clear();
	    lcd_ << "Sensor error";
	} else if (rtc_.error()) {
	    lcd_.clear();
	    lcd_ << "RTC error";
	}
	else
	    window_main_.run();

	my_mcu::Micro::wait_ms(time_polling);
    }
}

