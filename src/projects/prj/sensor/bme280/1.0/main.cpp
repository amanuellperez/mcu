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


Main::Main()
{
// init_hardware():
    init_TWI();
    init_lcd();
    init_keyboard();
    init_sensor();
    init_rtc_clock();
}


void Main::init_TWI()
{
    TWI::turn_on<TWI_frecuency>();
}


void Main::init_lcd()
{
//    lcd_.fill('0');
}



int main()
{
    Main app;
    app.run();
}


