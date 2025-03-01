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


#include "prj_dev.h"
#include "prj_strings.h"
#include "dev_print.h"
#include "sdc_print.h"
#include "prj_main.h"
#include "atd_fat.h"

#include <atd_ostream.h>


void Main::sector_driver_fill_n_test()
{
    uart << "Sector driver fill_n test\n"
	    "sector0: ";

    uint32_t sector0;
    uart >> sector0;

    if (sector0 == 0)
	return;

    uart << "number of sectors to fill: ";
    uint32_t nsectors;
    uart >> nsectors;

    uart << "value: ";
    uint8_t value{};
    uart >> value;

    Sector_driver sd;
    auto nfill = sd.fill_n(sector0, nsectors, value);

    if (nfill == nsectors)
	uart << "OK\n";
    else
	uart << "FAIL\n";
}

