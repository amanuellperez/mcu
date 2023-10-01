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

#include <atd_ostream.h>

#include "prj_main.h"
#include "prj_dev.h"
#include "prj_init.h"
#include "prj_strings.h"
#include "sdc_print.h"
#include "dev_print.h"


void Main::read_status()
{
    mcu::UART_iostream uart;

    auto r2 = SDCard::send_status();
    print(uart, r2);
}


void Main::init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.turn_on();
}

Main::Main()
{
    init_uart();

    mcu::UART_iostream uart;
    atd::print(uart, msg_hello);

    Chip_select::init();
    sdcard_init();
}


int main()
{
    Main app;
    app.run();
}






