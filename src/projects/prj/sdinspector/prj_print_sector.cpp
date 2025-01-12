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
#include "dev_fat.h"

void Main::print_sector()
{
    UART_iostream uart;
    print_line(uart);
    sector.print(uart);

}

bool Main::print_sector_fromto_ask(Sector::Address& from, size_t& sz)
{
    UART_iostream uart;

    uart << '\n';
    print_line(uart);

    print_question(uart, msg_print_sector_from, false);
    uart >> from;
    if (from > Sector::sector_size){
	atd::print(uart, msg_print_sector_from_to_big);
	uart << Sector::sector_size << '\n';
	return false;
    }

    uart << '\n';
    print_question(uart, msg_print_sector_size, false);
    sz = 0;
    uart >> sz; // TODO: si se da retorno de carro modifica 'sz' ????
		// De hecho hace cosas raras. Para no depurarlo le indico al
		// usuario que escriba explícitamente '0'. TODO: depurarlo!!!
		// Al rato ha empezado a funcionar bien (???)

    if (sz == 0)    // imprimimos hasta el final
	sz = Sector::sector_size - from;

    else if (from + sz > Sector::sector_size){
	atd::print(uart, msg_print_sector_size_to_big);
	uart << '\n';
	return false;
    }

    uart << '\n';

    return true;
}


void Main::print_sector_fromto()
{
    UART_iostream uart;

    Sector::Address from{};
    size_t sz{};
    if (!print_sector_fromto_ask(from, sz))
	return;

    uart << '\n';
    print_line(uart);
    sector.print(uart, from, sz);

}


void Main::print_sector_as_MBR()
{
    UART_iostream uart;
    uart << '\n';
    atd::print(uart, msg_print_sector_as_MBR);
    print_line(uart);

    using MBR = dev::FAT32::MBR;
    using MBR_type = dev::FAT32::MBR::Partition_type;

    MBR* mbr = reinterpret_cast<MBR*>(sector.data());

    print_question(uart, msg_is_MBR_valid_sig, false);
    print_bool_as_yes_no(uart, mbr->is_valid());

    uart << '\n';
    atd::print(uart, msg_partition1);

    print_question(uart, msg_is_fat32);
    print_bool_as_yes_no(uart, mbr->partition1.type == MBR_type::fat32_lba);

    uart << '\t';
    atd::print(uart, msg_lba_offset);
    uart << " = " << mbr->partition1.lba_offset << '\n';

    print_question(uart, msg_is_bootable);
    print_bool_as_yes_no(uart, mbr->partition1.is_bootable());


}

