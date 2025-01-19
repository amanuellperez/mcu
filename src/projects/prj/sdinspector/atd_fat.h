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

#ifndef __ATD_FAT_H__
#define __ATD_FAT_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para manejar FAT32 filesystems	
 *
 * REFERENCES
 *	http://elm-chan.org/docs/fat_e.html
 *	https://wiki.osdev.org/FAT#Long_File_Names (referencia osdev)
 *
 * HISTORIA
 *    Manuel Perez
 *    12/01/2025 Experimentando
 *
 ****************************************************************************/
#include <cstdlib>
#include <bit>	// endianness
		
#include <atd_bit.h> // concat_bytes

namespace atd{
    
/***************************************************************************
 *				MBR
 ***************************************************************************/
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


/***************************************************************************
 *				    FAT32
 ***************************************************************************/
// TODO: todos los datos de FAT son little endian.
// Que en tiempo de compilación se elija el formato adecuado.
namespace FAT32{

// Boot sector = VBR (volume boot record) = PBR (private boot record)
// Nombres: 3.1 de la FAT specification (o la página de elm-chan)
struct Boot_sector{
    static_assert(std::endian::native == std::endian::little);

// Uso los nombres crípticos de la FAT specification (son horribles :(

// Parte común a FAT12/16/32
    uint8_t jmp_boot[3];
    uint8_t OEM_name[8];
    uint16_t byte_per_sec; // 512 para máxima compatibilidad según elm-chan
    uint8_t sec_per_clus;
    uint16_t rsvd_sec_cnt;
    uint8_t num_fats;
    uint16_t root_ent_cnt;
    uint16_t tot_sec16;
    uint8_t media;
    uint16_t fat_sz16;
    uint16_t sec_per_trk;
    uint16_t num_heads;
    uint32_t hidd_sec;
    uint32_t tot_sec32;

// Parte específica de FAT32
    uint32_t fat_sz32;
    uint16_t ext_flags;
    uint16_t fs_ver;
    uint32_t root_clus;
    uint16_t fs_info;
    uint16_t bk_boot_sec;   // == 6 siempre (3.6)
    uint8_t reserved[12];
    uint8_t drv_num;
    uint8_t reserved1;
    uint8_t boot_sig;
    uint32_t vol_id;
    uint8_t vol_lab[11];
    uint8_t fil_sys_type[8]; // == "FAT32   "
    uint8_t boot_code[420];
    uint16_t sign;	    // == 0xAA55

// Chechs of integrity
    bool is_sign_valid() const {return sign == 0xAA55;}
    bool check_integrity() const
    {
	return is_sign_valid() 
	       and fat_sz16 == 0 // FAT32 tiene el size en fat_sz32
	       and num_fats >= 1
	       and root_ent_cnt == 0 // for FAT32 siempre vale 0
	       and tot_sec32 != 0 
	       and bk_boot_sec == 6
	       ;
    }

// FAT area
// --------
    // offset del primer sector de la FAT area i
    uint16_t FAT_area_offset(uint8_t i) const 
    {return hidd_sec + rsvd_sec_cnt + fat_sz32 * i;}

    uint32_t FAT_area_number_of_sectors() const {return fat_sz32 * num_fats;}

    // Me gusta más este sinónimo:
    uint32_t FAT_area_size() const {return FAT_area_number_of_sectors(); }

    uint8_t number_of_FATs() const {return num_fats;}

// Root directory (FAT12/16 solo)
// --------------
    uint32_t root_directory_area_offset() const 
    {return FAT_area_offset(0) + FAT_area_size(); }

    static constexpr uint8_t dir_entry_size = 32;

    // Como root_ent_cnt == 0, la root directory area para FAT32 siempre es 0.
    // Dejo esto por si implemento FAT12 en el futuro
    uint32_t root_directory_area_number_of_sectors() const
    {return (dir_entry_size * root_ent_cnt + byte_per_sec - 1) / byte_per_sec; }

    uint32_t root_directory_area_size() const  
    {return root_directory_area_number_of_sectors(); }


// Data area
// ---------
    uint32_t data_area_offset() const 
    {return root_directory_area_offset() + root_directory_area_size(); }

    uint32_t data_area_number_of_sectors() const 
    { return data_area_offset() + tot_sec32;}

    uint32_t data_area_size() const {return data_area_number_of_sectors(); }

    uint32_t first_sector_of_cluster(uint32_t n) const
    {return  data_area_offset() + (n-2) * sec_per_clus;}

    // Este es el primer sector del root directory
    uint32_t first_sector_of_root_directory() const
    {return  first_sector_of_cluster(root_clus);}

};


// Section 5, FAT specification
struct FS_info{
    static_assert(std::endian::native == std::endian::little);
    
    uint32_t lead_sig;
    uint8_t reserved1[480];
    uint32_t struct_sig;
    uint32_t free_count;
    uint32_t nxt_free;
    uint8_t reserved2[12];
    uint32_t trail_sig;


    // FAT32 specification: section 5
    bool check_integrity() const
    {
	// reserved1[i] == 0 and reserved2[i] == 0
	return	lead_sig == 0x41615252 and
		struct_sig == 0x61417272 and
		trail_sig == 0xAA550000;
    }

    uint32_t last_known_free_cluster_count() const {return free_count;}
    uint32_t first_available_free_cluster() const {return nxt_free;}
};


// Section 7, FAT specification
// De acuerdo a osdev, los caracteres son de 2 bytes (y coincide con el
// ejemplo de la FAT specification). Según un foro son UTF16 (ref???)
struct Long_file_name_entry{
    uint8_t ord;
    uint16_t name1[5]; 
    uint8_t attr; // == long_name
    uint8_t type; // == 0
    uint8_t chk_sum;
    uint16_t name2[6];
    uint16_t fst_clus_lo; // == 0
    uint16_t name3[2];

    bool is_last_entry() const {return 0x40 & ord; }
};

// Section 6, FAT specification
struct Directory{
    static_assert(std::endian::native == std::endian::little);

    uint8_t name[11];
    uint8_t attr;
    uint8_t nt_res;
    uint8_t crt_time_tenth; // tenths of a second
    uint16_t crt_time;
    uint16_t crt_date;
    uint16_t lst_acc_date;
    uint8_t fst_clus_hi[2];
    uint16_t wrt_time;
    uint16_t wrt_date;
    uint8_t fst_clus_lo[2];
    uint32_t file_size;
    

    // TODO: 0xE5 es un kanji y en japones es válido. Esto no funcionaria con
    // japones.
    bool is_empty() const 
    {return name[0] == 0x00 or name[0] == 0xE5;}

    enum class Attribute : uint8_t{
	read_only = 0x01,
	hidden    = 0x02,
	system    = 0x04,
	volume_id = 0x08,
	directory = 0x10,
	archive   = 0x20,
	// pag. 23: 
	long_name = read_only | hidden | system | volume_id
    };

    bool check_integrity() const
    {
	return true; 
	// no funciona para el attr == long_name; revisar.
//	return nt_res == 0 and
//	    0 <= crt_time_tenth and crt_time_tenth <= 199;
    }

    
    uint32_t first_data_cluster_number() const 
    {
	return atd::concat_bytes<uint32_t>(fst_clus_hi[1], fst_clus_hi[0],
				 fst_clus_lo[1], fst_clus_lo[0]);
    }


    Attribute attribute() const {return Attribute{attr};}

};
    


// Iteramos por las entradas de un directorio
struct Directory_iterator{
};




}// namespace FAT32

}// namespace dev
 

#endif


