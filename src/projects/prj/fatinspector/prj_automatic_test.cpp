// Copyright (C) 2025 Manuel Perez 
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

void Main::automatic_test()
{
    using Entry = Directory::Entry;

    uint8_t hours = 10;
    uint8_t minutes = 59;
    uint8_t seconds = 44;

    uint8_t day = 13;
    uint8_t month = 2;
    uint16_t year = 2025;

    uint16_t time = Entry::to_time(hours, minutes, seconds);
    uint16_t date = Entry::to_date(day, month, year);

    uint8_t h, m, s;
    uint8_t d, month2;
    uint16_t y;

    Entry::time_as_brokendown(time, h, m, s);
    Entry::date_as_brokendown(date, d, month2, y);

    if (h != hours or m != minutes or s != seconds)
	uart << "Entry::to_time error\n";

    if (d != day or month2 != month or y != year)
	uart << "Entry::to_date error\n";
}

