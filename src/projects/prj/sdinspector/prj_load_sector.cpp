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

#include "prj_dev.h"
#include "prj_strings.h"
#include "dev_print.h"
#include "sdc_print.h"
#include "prj_main.h"

bool Main::load_sector(const SDCard::Address& add)
{
    auto r = SDCard::read(add, sector);

    if (r.ok()){
	sector.number= add;
	return true;
    }
    else{
	// TODO: poner "error!!!: DETALLES"
	UART_iostream uart;
	print(uart, r);
	return false;
    }
}

bool Main::load_sector()
{
    UART_iostream uart;

    print_question(uart, msg_sector_address, false);
    SDCard::Address add;
    uart >> add;
    uart << add << '\n';

    return load_sector(add);
}

