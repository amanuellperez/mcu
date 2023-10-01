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

#pragma once

#ifndef __MAIN_H__
#define __MAIN_H__

#include "dev.h"
#include <array>


class Main {
public:
    Main();
    void run();

private:
// Hardware
    LCD lcd_;
    Keyboard keyboard_;


    // almacenamos aquí la entrada del usuario
    Interface::Buffer buffer_;


// init: hardware
    void init_lcd();
    void init_keyboard() { }

// main
    void getline();

};



#endif

