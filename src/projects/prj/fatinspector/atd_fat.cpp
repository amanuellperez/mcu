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

#include "prj_dev.h"	// TODO: borrame 
#include <atd_bit.h>
#include <atd_algorithm.h>  // atd::copy

#include "atd_fat.h"


namespace atd{
namespace FAT32{

namespace impl_of{

/***************************************************************************
 *				FAT AREA
 ***************************************************************************/
//void FAT_area::init(const Boot_sector& bs)
//{
//    sector0_ = bs.FAT_area_first_sector();
//
//    if (bs.FAT_is_mirror_enabled())
//	nFATs_   = bs.number_of_FATs();
//    else
//	nFATs_   = bs.number_of_active_FATs();
//
//    number_of_sectors_ = bs.FAT_number_of_sectors();
//    number_of_entries_ = bs.data_area_number_of_clusters();
//    sector_size_ = bs.bytes_per_sector();
//}
//
//
//// Section 4. FAT specification
//FAT_area::Cluster_state FAT_area::cluster_state(uint32_t entry) const
//{
//    // Section 4.  Note: "The high 4 bits of a FAT32 FAT entry are reserved"
//    entry &= 0x0FFFFFFF;  
//
//    if (entry == 0x00000000) return Cluster_state::free;
//    if (entry == 0x0FFFFFF7) return Cluster_state::bad;
//    if (entry == 0x0FFFFFFF) return Cluster_state::end_of_file;
//    
//    // entrys 0 y 1 están reservados
//    // section 3.5
//    if (0x00000002 <= entry 
//		and entry <= number_of_entries_ + 1) 
//	return Cluster_state::allocated;
//	
//
//    // DUDA: La FAT specification indica que 
//    // "Reserved and should not be used.
//    //  May be interpreted as an allocated entry and the final
//    //  entry in the file (indicating end-of-file condition)"
//    //  ¿son reserved or end_of_file? @_@
//    if (0x0FFFFFF8 <= entry and entry <= 0x0FFFFFFE)
//	return Cluster_state::reserved;
//
//    return Cluster_state::reserved;
//}


/***************************************************************************
 *				DATA AREA
 ***************************************************************************/
void Data_area::init(const Boot_sector& bs)
{
    data_area_first_sector_ = bs.data_area_first_sector();
    data_area_sectors_per_cluster_ = bs.data_area_sectors_per_cluster();
}




/***************************************************************************
 *			    DIRECTORY ENTRY
 ***************************************************************************/
// Section 6.1, FAT specification
Directory_entry::Type Directory_entry::type() const
{
    if (data[0] == 0x00) return Type::last_entry;
    if (data[0] == 0xE5) return Type::free;

    if (attribute() == Attribute::long_name) 
	return Type::long_entry;

    return Type::short_entry;
}

// (TODO) ¿no sería mejor pasar std::span<uint8_t, 11>???
uint8_t Directory_entry::copy_short_name(std::span<uint8_t> str) const
{
    auto [p, q] = atd::copy(data.begin(), data.end(), 
			    str.begin(), str.end());
    return (p - data.begin());
}

// (TODO) Los caracteres se almacenan en 2 bytes, de momento solo me quedo con
// el primer byte por tratar con caracteres de 1 byte. Para 2 bytes esto no
// funciona.
uint8_t Directory_entry::copy_long_name(std::span<uint8_t> str)
{
    if (str.size() < 5) return 0;

    // Se almacenan en 2 bytes, siendo el más significativo el correspondiente
    // al ASCII (ref???)
    for (uint8_t i = 0; i < 10 / 2; ++i){
	str[i] = data[1 + 2*i];

	if (data[1 + 2*i] == 0x00) 
	    return i;
    }

    if (str.size() < 22 / 2) return 5;

    for (uint8_t i = 0; i < 12 / 2; ++i){
	str[5 + i] = data[14 + 2*i];

	if (data[14 + 2*i] == 0x00) 
	    return 5 + i;
    }

    if (str.size() < 26 / 2) return 12;

    for (uint8_t i = 0; i < 4 / 2; ++i){
	str[11 + i] = data[28 + 2*i];

	if (data[28 + 2*i] == 0x00) 
	    return 11 + i;
    }

    return 13; 
}


// 6.3 FAT specification
void Directory_entry::uint16_t2date(uint16_t date, 
			      uint8_t& day, uint8_t& month, uint16_t& year)
{
    //day   = date & 0x001F; // bits del 0-4
    day   = date & atd::make_range_bitmask<0, 4, uint16_t>();
    month = static_cast<uint8_t>(
		(date & atd::make_range_bitmask<5, 8, uint16_t>()) >> 5);
    year  = 1980 + ((date & atd::make_range_bitmask<9, 15, uint16_t>()) >> 9);
}


// 6.3 FAT specification
void Directory_entry::uint16_t2time(uint16_t time, 
		uint8_t& seconds, uint8_t& minutes, uint8_t& hours)
{
    seconds = 2*(time & atd::make_range_bitmask<0, 4, uint16_t>());
    minutes = static_cast<uint8_t>(
		(time & atd::make_range_bitmask<5, 10, uint16_t>()) >> 5);
    hours   = static_cast<uint8_t>(
		(time & atd::make_range_bitmask<11, 15, uint16_t>()) >> 11);
}



void copy(const Directory_entry& entry, Entry_info& info)
{
    info.attribute = entry.attribute();

    info.file_cluster = entry.file_cluster();
    info.file_size = entry.file_size();

    info.creation_date = entry.creation_date();
    info.creation_time_seconds = entry.creation_time_seconds();
    info.creation_time_tenth_of_seconds = entry.creation_time_tenth_of_seconds();

    info.last_access_date = entry.last_access_date();

    info.last_modification_date = entry.last_modification_date();
    info.last_modification_time = entry.last_modification_time();
}


} // impl_of
} // namespace FAT32
} // namespace atd

