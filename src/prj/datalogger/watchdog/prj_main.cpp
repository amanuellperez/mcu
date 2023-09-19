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


void Main::init_uart()
{
    UART uart;
    mcu::basic_cfg(uart);
    uart.turn_on();
}

void Main::init_sensor()
{
    UART uart;
    uart << "Init sensor ... ";

    Search search;
    Sensor::Device dev;
    if(search(dev)){
	sensor_.bind(dev);
	uart << "OK\n";
    }

    else
	uart << "ERROR. No device found\n";
}


Main::Main()
{
    init_uart();
    init_sensor();
}


int main()
{
    Main app;
    app.run();
}



