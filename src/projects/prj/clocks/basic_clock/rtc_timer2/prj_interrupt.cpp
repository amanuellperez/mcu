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

#include "prj_main.h"

// El Timer2 va a generar la interrupción cada segundo, luego no es necesario
// mirar si hay un nuevo segundo o no. Esto simplifica el código pero lo hace
// menos genérico. Pero esta clase la escribe el hardwador. Si se cambia algo
// de hardware, el hardwador tiene que revisar todo este código.
ISR_CLOCK
{
    Clock::tick();
//    if (Clock::is_new_second()){
//	new_second_ = true;
//    }
}


ISR_BUTTON{
    Main::show_menu_ = true;
}
