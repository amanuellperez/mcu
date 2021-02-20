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
#include <user_choose_string.h>

void Main::run()
{
    while(1){
//	if (errno_)
//	    error();
//	else 
	    window_main();

	wait_ms(100);
    }
}


void Main::window_main()
{
    uint8_t opt = dev::user_choose_string_lineal<16, 2>(
			 lcd_.screen(), keyboard_, window_main_menu)
			 .pos(0, 0)
			 .show(0);
    switch(opt){
	case sw_generator_opt: window_sw_generator(); break;
	case sweep_opt       : window_sweep_menu(); break;
	case cfg_opt         : window_cfg(); break;
    }
}



