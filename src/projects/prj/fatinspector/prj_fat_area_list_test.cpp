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


void Main::FAT32_area_list_menu()
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

    while(1){
	uart << '\n';
	print_line(uart);
	
	uart << "FAT area list menu\n"
		"\t0. Back main menu\n"
	        "\t1. Print list of clusters of a file\n"
		"\t2. Push back new cluster\n";

	uint16_t cmd{};
	uart >> cmd;

	switch(cmd){
	    break; case 0: return;
	    break; case 1: FAT32_area_list_print_ls(vol);
	    break; case 2: FAT32_area_push_back_cluster(vol);

	}
    }

}

void Main::FAT32_area_list_print_ls(Volume& vol)
{
    using FAT_area_list   = atd::FAT32::FAT_area_list<Sector_driver>;


    uart << "First cluster of the file (>= 2): ";
    uint32_t cluster0{};
    uart >> cluster0;
    
    if (cluster0 < 2){
	uart << "Error, cluster has to be >= 2\n";
	return;
    }

    FAT_area_list cluster_list(vol, cluster0);   
   
    if(cluster_list.last_operation_fail()){
	uart << "Error creating list\n";
	return;
    }

    while(1){
	uart << cluster_list.ncluster()
	     << '(' << cluster_list.sd_sector_number() << ") -> ";

	if(!cluster_list.next_cluster()){
	    if (cluster_list.end_of_sectors())
		uart << "EOC\n";
	    else
		uart << "ERROR cluster no allocated\n";

	    return;
	}
    }

}


void Main::FAT32_area_push_back_cluster(Volume& vol)
{
    using FAT_area_list   = atd::FAT32::FAT_area_list<Sector_driver>;

    uart << "Last cluster of a list of clusters: ";
    uint32_t cluster0{};
    uart >> cluster0;
    
    if (cluster0 < 2){
	uart << "Error, cluster has to be >= 2\n";
	return;
    }

    FAT_area_list cluster_list(vol, cluster0);    // file.begin(cluster0);
   
    if(cluster_list.last_operation_fail()){
	uart << "Error creating list\n";
	return;
    }

    uart << "Character to fill cluster: ";
    uint8_t c{};
    uart >> c;

    uint32_t next_cluster = cluster_list.push_back_cluster_fill_with(c);;
    
    if (next_cluster == 0)
	uart << "FAIL\n";
    else
	uart << "OK\n";





}

