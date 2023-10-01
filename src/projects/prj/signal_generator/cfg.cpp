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
#include <user_choose_string.h>

void Main::window_cfg()
{
    lcd_ << "Timer period:"
            "\n     ";
    lcd_ << sw_period_;
    lcd_.cursor_pos(9, 1);
    lcd_ << " us";

    lcd_.cursor_pos(5, 1);

    wait_release_key();

    uint8_t opt = dev::user_choose_string_lineal<4>(
			 lcd_, keyboard_, period_opts)
			 .pos(5, 1)
			 .show(period2index(sw_period_));


    init_signal_generator(index2period(opt));

    wait_release_key();
}

