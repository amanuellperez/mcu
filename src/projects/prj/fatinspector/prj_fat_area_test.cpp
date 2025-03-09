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


void Main::FAT32_area_menu()
{
    auto nsector = fat_volume_first_sector(1); // de momento solo leo
					       // particion 1
    if (nsector == 0){
	uart << "Error: can't read first sector of FAT volume\n";
	return;
    }


    Volume vol{nsector};

    if (vol.last_operation_fail()){
	uart << "ERROR\n";
	return;
    }

    while(1){
	uart << '\n';
	print_line(uart);
	
	//atd::print(uart, msg_fat_area_menu);
    
	uart << "FAT area menu\n"
		"\t0. Back main menu\n"
	        "\t1. Print list of clusters of a file\n"
		"\t2. New list\n"
		"\t3. Remove list\n"
		"\t4. Add cluster\n"
		"\t5. Remove next cluster\n";

	uint16_t cmd{};
	uart >> cmd;

	switch(cmd){
	    break; case 0: return;
	    break; case 1: FAT32_area_print_clusters(vol);
	    break; case 2: FAT32_area_new_list(vol);
	    break; case 3: FAT32_area_remove_list(vol);
	    break; case 4: FAT32_area_add_cluster(vol);
	    break; case 5: FAT32_area_remove_next_cluster(vol);

	}
    }

}

// Esta función prueba FAT_area
// Aunque es parecida a print_file_sectors, esa otra prueba File_sectors
void Main::FAT32_area_print_clusters(Volume& vol)
{

//    atd::ctrace<9>() << "FAT:"
//	        "\n\tfirst_sector         : " << vol.fat_area.first_sector(0) 
//	     << "\n\tnumber of sectors    : " << vol.fat_area.number_of_sectors()
//	     << "\n\tnumber of entries    : " << vol.fat_area.number_of_clusters()
//	     << "\n\tbytes per sector     : " << vol.fat_area.bytes_per_sector()
//	     << "\n\tnumber of active FATs: " << (uint16_t) vol.fat_area.number_of_active_FATs()
//	     << '\n';

    uart << "Cluster to print: ";
    uint32_t cluster{};
    uart >> cluster;
    
    using State = Volume::Cluster_state;

    uart << cluster << " -> ";

    while (1){
	uint32_t next_cluster{};
	auto state = vol.fat_area.next_cluster(cluster, next_cluster);

	if (state != State::allocated){
	    switch (state){
		break; case State::free: uart << "FREE\n";
		break; case State::allocated: uart << "allocate\n";
		break; case State::bad     : uart << "bad\n";
		break; case State::end_of_clusters: uart << "EOC\n";
		break; case State::reserved: uart << "reserved\n";
		break; case State::read_error: uart << "read error\n";
	    }

	    uart << '\n';
	    break;
	}
	uart << next_cluster << " -> ";
	cluster = next_cluster;
    }
}


void Main::FAT32_area_new_list(Volume& vol)
{
    auto cluster = vol.fat_area.new_list();

    if (cluster != 0)
	uart << "New cluster: " << cluster << '\n';
    else
	uart << "new_list FAIL!\n";

    vol.flush();
}

void Main::FAT32_area_remove_list(Volume& vol)
{
    uart << "First cluster of the list to remove: ";
    uint32_t cluster0;
    uart >> cluster0;

    if (vol.fat_area.remove_list(cluster0))
	uart << "OK\n";
    else
	uart << "remove_list FAIL!\n";

    vol.flush();
}



void Main::FAT32_area_add_cluster(Volume& vol)
{
    uart << "Cluster to add: ";
    uint32_t cluster0;
    uart >> cluster0;

    uart << "Add only to the end? y/n ";
    char yesno{};
    uart >> yesno;
    uint32_t cluster;

    if (yesno == 'y' or yesno == 'Y')
	cluster = vol.fat_area.push_back_cluster(cluster0);
    else
	cluster = vol.fat_area.add_cluster(cluster0);

    if (cluster != 0)
	uart << "New cluster: " << cluster << '\n';
    else
	uart << "add_cluster FAIL!\n";

    vol.flush();
}


void Main::FAT32_area_remove_next_cluster(Volume& vol)
{
    uart << "Cluster to remove (next): ";
    uint32_t cluster0;
    uart >> cluster0;

    auto cluster = vol.fat_area.remove_next_cluster(cluster0);

    if (cluster != 0)
	uart << "Return cluster: " << cluster << '\n';
    else
	uart << "add_cluster FAIL!\n";

    vol.flush();
}


