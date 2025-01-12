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

#pragma once

#ifndef __DEV_FAT_H__
#define __DEV_FAT_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para manejar FAT32 filesystems	
 *
 * REFERENCES
 *	http://elm-chan.org/docs/fat_e.html
 *
 * HISTORIA
 *    Manuel Perez
 *    12/01/2025 Experimentando
 *
 ****************************************************************************/
#include <cstdlib>
#include <bit>	// endianness
		
namespace dev{
    
// TODO: todos los datos de FAT son little endian.
// Que en tiempo de compilación se elija el formato adecuado.
namespace FAT32{

namespace private_{

// ---
// MBR
// ---

// TODO: la wikipedia (https://en.wikipedia.org/wiki/Partition_type)
// tiene un MONTÓN de tipos @_@
// De momento solo necesito la fat32_lba
// Los nombres están elegidos leyendo la página de elm-chan (me han quedado un
// poco... raros ^_^')
enum class MBR_partition_type : uint8_t{
    blank_entry = 0x00,
    fat12_less_65536_sectors = 0x01,
    fat16_less_65536_sectors = 0x04,
    extended_partition_chs_or_lba = 0x05,
    fat12_or_16_more_65536_sectors = 0x06,
    hpfs_or_ntfs_or_exFat = 0x07,
    fat32_chs_or_lba = 0x0B,
    fat32_lba = 0x0C,
    fat12_or_16_lba = 0x0E,
    extended_partition_lba = 0x0F
};


}// private_
 
struct MBR_partition{
    static_assert(std::endian::native == std::endian::little);

    using Type = private_::MBR_partition_type;

    uint8_t boot_id;
    uint8_t start_hd;
    uint16_t start_cy_sc;
    Type type;
    uint8_t end_hd;
    uint16_t end_cy_sc;
    uint32_t lba_offset;
    uint32_t lba_size;

    bool is_bootable() const {return boot_id == 0x80;}
    bool is_not_bootable() const {return boot_id == 0x00;}
};


// TODO: según la wikipedia hay varias MBR. Esta es la clásica.
// Llamarla MBR_classic? o usar solo esta? o vesionarla? MBR_v1?
struct MBR{
    static_assert(std::endian::native == std::endian::little);

    uint8_t bootcode[446];

    using Partition_type = MBR_partition::Type;

    MBR_partition partition1;
    MBR_partition partition2;
    MBR_partition partition3;
    MBR_partition partition4;

    uint16_t sig;

    bool is_valid() const {return sig == 0xAA55;}
};



}// namespace FAT32

}// namespace dev
 

#endif


