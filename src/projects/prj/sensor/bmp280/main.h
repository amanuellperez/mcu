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

#ifndef __SENSOR_MAIN_H__
#define __SENSOR_MAIN_H__

#include "dev.h"

class Main{
public:
    Main();
    void run();

private:
// Hardware
    LCD_ostream lcd_;
    Sensor sensor_;

// init: hardware
    void init_TWI();
    void init_lcd();
    void init_sensor();

// Window: main
    void window_main();
    void show_window_main();

// sensor
    void print_sensor();
};


#endif

