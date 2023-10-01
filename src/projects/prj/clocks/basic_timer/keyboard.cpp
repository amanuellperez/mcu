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

#include "keyboard.h"

int16_t Keyboard_time_to_wait::time_up()
{
    if (n < 0)
	n = 0;

    if (n < 10){
	++n;
	return 1;
    }
    else if (n < 22){
	++n;
	return 10;
    }
    else if (n < 50){
	++n;
	return 60;
    }

    else
	return 600;

}

int16_t Keyboard_time_to_wait::time_down()
{
    if (n > 0)
	n = 0;

    if (n > -10){
	--n;
	return 1;
    }
    else if (n > -22){
	--n;
	return 10;
    }
    else if (n > -50){
	--n;
	return 60;
    }
    else
	return 600;

}
