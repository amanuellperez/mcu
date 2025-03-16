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
#include "atd_mbr.h"

uint32_t Main::fat_volume_first_sector(const atd::MBR_partition& p, uint8_t n)
{
    if (p.type != atd::MBR::Partition_type::fat32_lba){
	atd::ctrace<4>() << "Error: partition" << n << " is not FAT32 lba!!!\n";
	return 0;
    }

    return p.lba_first_sector;
}


uint32_t Main::fat_volume_first_sector(uint8_t npartition)
{
    Sector_driver driver;
    auto mbr = driver.template lock_sector_and_view_as<atd::MBR>(0);

    if (mbr.is_null() or !mbr->is_valid()){
	atd::ctrace<4>() << "Error: MBR invalid\n";
	return 0;
    }

    switch (npartition){
	break; case 1: return fat_volume_first_sector(mbr->partition1, 1);
	break; case 2: return fat_volume_first_sector(mbr->partition2, 2);
	break; case 3: return fat_volume_first_sector(mbr->partition3, 3);
	break; case 4: return fat_volume_first_sector(mbr->partition4, 4);
    }

    return 0;
}

