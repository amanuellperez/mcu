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

// #include "prj_dev.h"	// TODO: borrame 
#include <atd_bit.h>
#include <atd_algorithm.h>  // atd::copy

#include "atd_fat.h"


namespace atd{
namespace FAT32{

namespace impl_of{


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
    if (data[0] == 0x00) return Type::free_nomore;
    if (data[0] == 0xE5) return Type::free_available;

    if (attribute() == Attribute::long_name) 
	return Type::name_entry;

    return Type::info_entry;
}

uint8_t Directory_entry::free_type_to_uint8_t(Type type)
{
    if (type == Type::free_nomore) return 0x00;
    if (type == Type::free_available) return 0xE5;

    return 0xE5; // en caso de error opto por devolver free
}


// (TODO) ¿no sería mejor pasar std::span<uint8_t, 11>???
uint8_t Directory_entry::read_short_name(std::span<uint8_t> str) const
{
    atd::copy(data.begin(), data.begin() + ascii_short_name_len,
			    str.begin(), str.end());
    return ascii_short_name_len;
}

// (TODO)
//  El nombre tiene que ser único. ¿Tengo que escanear todo el directorio
//  para crear una nueva short entry?
//
// No la defino inline ya que tengo que comprobar lo anterior.
void Directory_entry::write_short_name(std::span<const uint8_t> str)
{
    std::span<uint8_t, ascii_short_name_len> 
			short_name{data.data(), ascii_short_name_len};

    long_name2short_name(str, short_name);
}


// (TODO)
//  1. Eliminar caracteres del nombre según 6.1 FAT specification
//  2. El primer caracter no puede ser 0x20 (esto se puede imponer quitando
//  todos los espacios del principio del name cuando se intente crear una
//  nueva entrada).
//
// str -> res
void Directory_entry::long_name2short_name(std::span<const uint8_t> str
				, std::span<uint8_t, ascii_short_name_len> res)
{
    static constexpr 
		uint8_t fname_len = ascii_short_name_len - ascii_extension_len;

// clear: (TODO) Esto se puede optimizar y hacerlo luego, pero complica código
    std::fill_n(res.begin(), ascii_short_name_len, ' ');

// copy fname:
    uint8_t i = 0;
    for(; i < str.size() and i < fname_len and str[i] != '.'; ++i){

	if (isprint(str[i])) // creo que estoy admitiendo de más
	    res[i] = toupper(str[i]);
	else
	    res[i] = '~'; // (DUDA) ¿qué caracter poner?
		      
    }


// look for '.':
    while (i < str.size() and str[i] != '.')
	++i;

    if (i == str.size())
	return;

    if (i > fname_len)
	res[fname_len - 1] = '~'; // así es como suelen indicar que el nombre
				   // es más largo

    ++i; // excluimos el '.'

// copy extension:
    for(uint8_t j = 0; j < ascii_extension_len and i < str.size(); ++i, ++j)
	res[fname_len + j] = toupper(str[i]);

}


uint8_t Directory_entry::read_long_name(std::span<uint8_t> str)
{
    auto n = read_long_name_impl(str);

    if (n < str.size())
	str[n] = '\0';

    return n;
}


// (TODO) Los caracteres se almacenan en 2 bytes, de momento solo me quedo con
// el primer byte por tratar con caracteres de 1 byte. Para 2 bytes esto no
// funciona.
uint8_t Directory_entry::read_long_name_impl(std::span<uint8_t> str)
{
    auto str_len = str.size();

    // Se almacenan en 2 bytes, siendo el más significativo el correspondiente
    // al ASCII (ref???)
    for (uint8_t i = 0; i < 10 / 2 and str_len > 0; ++i, --str_len){
	str[i] = data[1 + 2*i];

	if (data[1 + 2*i] == 0x00) 
	    return i;
    }

    for (uint8_t i = 0; i < 12 / 2 and str_len > 0; ++i, --str_len){
	str[5 + i] = data[14 + 2*i];

	if (data[14 + 2*i] == 0x00) 
	    return 5 + i;
    }

    for (uint8_t i = 0; i < 4 / 2 and str_len > 0; ++i, --str_len){
	str[11 + i] = data[28 + 2*i];

	if (data[28 + 2*i] == 0x00) 
	    return 11 + i;
    }

    return (str.size() - str_len);
}


// (TODO) Los caracteres se almacenan en 2 bytes, de momento solo me quedo con
// el primer byte por tratar con caracteres de 1 byte. Para 2 bytes esto no
// funciona.
//
// 7.3 FAT specification: los long_name acaban en '\0' y van seguidos de 0xFF
// para detectar corrupción
void Directory_entry::write_long_name(std::span<const uint8_t> str)
{
    uint8_t j = 1;

    for (uint8_t i = 0; i < str.size() and j < data.size(); ++i){
	data[j] = str[i];

	j += 2;
	if (j == 11) j = 14;
	if (j == 26) j = 28;
    }

    if (j != data.size()){
	data[j] = '\0';
	data[j + 1] = '\0';
	j += 2;
    }

    while(j < data.size()){
	data[j] = 0xFF;		
	data[j + 1] = 0xFF;

	j += 2;
	if (j == 11) j = 14;
	if (j == 26) j = 28;
    }


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


uint8_t Directory_entry::nname_entries_for_store_long_entry(uint8_t name_len)
{ 
    auto [q, r] = std::div(name_len, ascii_long_name_len);
    
    if (r != 0u)
	++q;	// q = número de entradas para almacenar name_len

    return q;
}



// La siguiente función es copia literal del punto 7.2 de la FAT specification
uint8_t Directory_entry::
	    check_sum_of(std::span<const uint8_t, ascii_short_name_len> str)
{
    uint8_t sum = 0;

    auto p = str.data();

    for (uint8_t i = 11; i != 0; --i) {
    // NOTE: The operation is an unsigned char rotate right
        sum = ((sum & 1) ? 0x80 : 0) + (sum >> 1) + *p++;
    }

    return sum;
}



void copy(const Directory_entry& entry, Entry_info& info)
{
    info.attribute = entry.attribute();

    info.file_cluster = entry.file_cluster();
    info.file_size = entry.file_size();

    info.creation_date = entry.creation_date();
    info.creation_time= entry.creation_time();
    info.creation_time_tenth_of_seconds = entry.creation_time_tenth_of_seconds();

    info.last_access_date = entry.last_access_date();

    info.last_modification_date = entry.last_modification_date();
    info.last_modification_time = entry.last_modification_time();
}


void copy(const Entry_info& info, Directory_entry& entry)
{
    entry.attribute(info.attribute);

    entry.file_cluster(info.file_cluster);
    entry.file_size(info.file_size);

    entry.creation_date(info.creation_date);
    entry.creation_time(info.creation_time);
    entry.creation_time_tenth_of_seconds(info.creation_time_tenth_of_seconds);

    entry.last_access_date(info.last_access_date);

    entry.last_modification_date(info.last_modification_date);
    entry.last_modification_time(info.last_modification_time);
}

void info_name2short_entry(const Entry_info& info, std::span<const uint8_t> name
				   , Directory_entry& entry)
{
    copy(info, entry);
    entry.write_short_name(name);
}

void short_entry2info_name(const Directory_entry& entry,
			    Entry_info& info, std::span<uint8_t> name)
{
    copy(entry, info);
    entry.read_short_name(name);

}

void long_name2short_entry(uint8_t order, std::span<const uint8_t> name, 
						    Directory_entry& entry)
{
    entry.data.fill(0);

    entry.attribute(Directory_entry::Attribute::long_name);
    entry.extended_order_with_mask(order);
    entry.write_long_name(name);
}


} // impl_of
} // namespace FAT32
} // namespace atd

