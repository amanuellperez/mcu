// Copyright (C) 2023-2025 Manuel Perez 
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

#include <ctype.h> // isprint

#include "prj_dev.h"
#include "prj_strings.h"
#include "dev_print.h"
#include "prj_print.h"
#include "sdc_print.h"
#include "prj_main.h"
#include "atd_fat.h"

#include <atd_ostream.h>


void Main::print_sector()
{
    uart << "Sector number to print: ";
    uint32_t nsector{};
    uart >> nsector;

    Sector_driver::Sector sector{};
    auto r = SDCard::read(nsector, sector);
    if (!r.ok()){
	uart << "ERROR reading sector " << nsector << '\n';
	return;
    }

    print_line(uart);

    uart << "Sector: " << nsector << '\n';
    atd::xxd_print(uart, sector);
    
}






void Main::print_MBR_boot_sector()
{
    uart << '\n';
    atd::print(uart, msg_print_sector_as_MBR);
    print_line(uart);

    using MBR = atd::MBR;
    using MBR_type = atd::MBR::Partition_type;
    
    Sector_driver driver;
    auto mbr = driver.template lock_sector_and_view_as<MBR>(0);


    print_question(uart, msg_is_MBR_valid_sig, false);
    print_bool_as_yes_no(uart, mbr->is_valid());

    uart << '\n';
    atd::print(uart, msg_partition1);

    print_question(uart, msg_is_fat32);
    print_bool_as_yes_no(uart, mbr->partition1.type == MBR_type::fat32_lba);

    uart << '\t';
    atd::print(uart, msg_lba_offset);
    uart << " = " << mbr->partition1.lba_first_sector << '\n';

    uart << "\tlba size = " << mbr->partition1.lba_size << '\n';

    print_question(uart, msg_is_bootable);
    print_bool_as_yes_no(uart, mbr->partition1.is_bootable());


}













void Main::print_file()
{
    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }

    Volume vol{nsector};

    if (vol.last_operation_fail()){
	uart << "ERROR reading FAT volume\n";
	return;
    }

    uart << "File cluster to print: ";
    uint32_t fcluster{};
    uart >> fcluster;

    uart << "File size: ";
    uint32_t fsize;
    uart >> fsize;

    File file{vol, fcluster, fsize};
    
    std::array<uint8_t, 16> data;

    uint32_t addr = 0;

    while(1){
	auto n = file.read(data);

	if (n == 0){
	    if (file.end_of_file())
		uart << "EOF\n";
	    else
		uart << "Error: n == 0 but is not EOF\n";

	    return;
	}

	atd::xxd_print(uart, {data.data(), n}, addr);
	addr += atd::Xxd::nbytes_per_line;

    }

}


