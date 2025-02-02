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

#include "atd_fat.h"

namespace atd{
namespace FAT32{

namespace impl_of{

/***************************************************************************
 *				FAT AREA
 ***************************************************************************/
void FAT_area::init(const Boot_sector& bs)
{
    sector0_ = bs.FAT_area_first_sector();

    if (bs.FAT_is_mirror_enabled())
	nFATs_   = bs.number_of_FATs();
    else
	nFATs_   = bs.number_of_active_FATs();

    number_of_sectors_ = bs.FAT_number_of_sectors();
    number_of_entries_ = bs.data_area_number_of_clusters();
    sector_size_ = bs.bytes_per_sector();
}


// Section 4. FAT specification
FAT_area::Cluster_state FAT_area::cluster_state(uint32_t entry) const
{
    // Section 4.  Note: "The high 4 bits of a FAT32 FAT entry are reserved"
    entry &= 0x0FFFFFFF;  

    if (entry == 0x00000000) return Cluster_state::free;
    if (entry == 0x0FFFFFF7) return Cluster_state::bad;
    if (entry == 0x0FFFFFFF) return Cluster_state::end_of_file;
    
    // entrys 0 y 1 están reservados
    // section 3.5
    if (0x00000002 <= entry 
		and entry <= number_of_entries_ + 1) 
	return Cluster_state::allocated;
	

    // DUDA: La FAT specification indica que 
    // "Reserved and should not be used.
    //  May be interpreted as an allocated entry and the final
    //  entry in the file (indicating end-of-file condition)"
    //  ¿son reserved or end_of_file? @_@
    if (0x0FFFFFF8 <= entry and entry <= 0x0FFFFFFE)
	return Cluster_state::reserved;

    return Cluster_state::reserved;
}


/***************************************************************************
 *				DATA AREA
 ***************************************************************************/
void Data_area::init(const Boot_sector& bs)
{
    data_area_first_sector_ = bs.data_area_first_sector();
    data_area_sectors_per_cluster_ = bs.data_area_sectors_per_cluster();
}

} // impl_of
} // namespace FAT32
} // namespace atd

