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

void Main::print_time()

{
    print_datetime(uart, Clock::now_as_date_time());
    uart << '\n';
}

int get_int(const char* name)
{
    my_mcu::UART_iostream uart;

    int x{};

    uart << name;
    uart << ": ";
    uart >> x;
    uart << x << '\n'; // echo. Borrarlo
			 
    return x;
}

void Main::get_time()
{
    Date_time dt;

// TODO: esto depende del locale!!!
    dt.hours(get_int("Hour"));
    dt.minutes(get_int("Minutes"));
    dt.seconds(get_int("Seconds"));

    dt.day(get_int("Day"));
    dt.month(get_int("Month"));
    dt.year(get_int("Year"));


    Clock::set(dt);


}
