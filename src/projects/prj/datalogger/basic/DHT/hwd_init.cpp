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

#include "../prj_main.h"

void Main::init_hwd()
{
    init_uart();
    init_sensor();
}

void Main::init_uart()
{
    myu::UART_basic_cfg();
    uart.turn_on();
    myu::UART_basic::enable_interrupt_unread_data();
}

void Main::init_sensor()
{
    uart << "Init sensor ... ";

// Como hay un menú y se requiere la interacción del usuario para empezar
// no es necesario esperar 1 segundo a más.
//    Micro::wait_ms(sensor_start_time_ms);

//    uart << "OK\n";
    uart << " SKIPPING waiting time\n";
}

