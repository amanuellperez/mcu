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
#include <atd_array.h>	// Array_of_bytes_view
#include <atd_cmath.h>	// div
#include "atd_sector.h"

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
    uint32_t lba_first_sector;
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
    uint16_t root_ent_cnt;  // == 0
    uint16_t tot_sec16;	    // == 0
    uint8_t media;	    // ???
    uint16_t fat_sz16;	    // == 0
    uint16_t sec_per_trk;   // ???
    uint16_t num_heads;	    // ???
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
    uint8_t drv_num;	    // ???
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
	       and root_ent_cnt == 0 
	       and tot_sec16 == 0
	       and fat_sz16 == 0 // FAT32 tiene el size en fat_sz32
	       and num_fats >= 1
	       and tot_sec32 != 0 
	       and bk_boot_sec == 6
	       ;
    }


// Hidden area
// -----------
// La hidden area son todos los sectores que hay antes del primer sector de
// este volumen.
    uint32_t hidden_area_number_of_sectors() const {return hidd_sec; }

// Volume info
// -----------
    uint32_t volume_number_of_sectors() const {return tot_sec32;}
    uint16_t bytes_per_sector() const {return byte_per_sec;}

    bool is_volume_label_and_id_set() const {return boot_sig == 0x29;}
    std::span<const uint8_t, 11> volumen_label() const {return vol_lab;}
    uint32_t volume_id() const {return vol_id;}

// Reserved area
// -------------
    uint32_t reserved_area_first_sector() const {return hidden_area_number_of_sectors();}
    uint32_t reserved_area_number_of_sectors() const {return rsvd_sec_cnt;}
    uint16_t FS_Info_sector() const {return fs_info;}
    uint16_t backup_boot_sector() const {return bk_boot_sec;}

// FAT area
// --------
// El FAT area está formada por una sucesión de FATs:
//
//	FAT area = FAT[0] U FAT[1] U FAT[2] U ... FAT[n-1]
//
// siendo U el símbolo de unión.
// En lo que sigue `FAT_area_...` se referirá a la unión de las FATs, mientras
// que `FAT_...` será información de una única FAT.
    // Primer sector de la FAT area i
    uint32_t FAT_first_sector(uint8_t i) const 
    {return hidd_sec + rsvd_sec_cnt + fat_sz32 * i;}

    uint32_t FAT_area_first_sector() const 
    { return FAT_first_sector(0); }

    uint8_t number_of_FATs() const {return num_fats;}

    // número de sectores que ocupa el FAT area
    uint32_t FAT_area_number_of_sectors() const {return fat_sz32 * num_fats;}

    // número de sectores de una FAT
    uint32_t FAT_number_of_sectors() const
    { return FAT_area_number_of_sectors() / number_of_FATs(); }
    
    bool FAT_is_mirror_enabled() const
    { return (ext_flags & 0x0040) == 0; }

    // Cuando is_FAT_mirror_enabled() == false, solo hay una FAT activa:
    uint8_t number_of_active_FATs() const 
    { return ext_flags & 0x0007; }

    // en FAT32 cada entrada es un uint32_t
    static constexpr uint8_t FAT_bytes_per_entry() { return sizeof(uint32_t);}

    uint8_t FAT_number_of_entries_per_sector() const
    { return bytes_per_sector() / FAT_bytes_per_entry(); }

    uint32_t FAT_number_of_entries() const
    { return FAT_number_of_entries_per_sector() * FAT_number_of_sectors();}

    // El número de entradas en una FAT es:
    //	    2 + data_area_number_of_clusters()
    // donde los 2 proceden de que FAT[0] y FAT[1] son reserved.
    // En la práctica parece ser que se reserva más tamaño del necesario. Esta
    // función devuelve el exceso de sectores.
    // Cada entry de una FAT corresponde co un cluster del data area.
    // 
    // (HACK) ¿Se podría usar esta zona para guardar documentos secretos?
    uint32_t FAT_unused_entries() const
    { return FAT_number_of_entries() 
				    - (2 + data_area_number_of_clusters());}

    uint32_t FAT_unused_sectors() const
    { return (FAT_unused_entries() * FAT_bytes_per_entry()) / bytes_per_sector();}


// FAT32 no tiene Root area. NO BORRAR de momento.
// Lo dejo como ejemplo por si se implementa FAT12/16.
//// Root directory (FAT12/16 solo)
//// --------------
//    uint32_t root_directory_area_first_sector() const 
//    {return FAT_area_first_sector(0) + FAT_area_size(); }
//
//    static constexpr uint8_t dir_entry_size = 32;
//
//    // Como root_ent_cnt == 0, la root directory area para FAT32 siempre es 0.
//    // Dejo esto por si implemento FAT12 en el futuro
//    uint32_t root_directory_area_size() const
//    {return (dir_entry_size * root_ent_cnt + byte_per_sec - 1) / byte_per_sec; }


// Data area
// ---------
    uint32_t data_area_first_sector() const 
    {return FAT_area_first_sector() + FAT_area_number_of_sectors(); }

    uint32_t data_area_number_of_sectors() const 
    { return tot_sec32 - FAT_area_number_of_sectors() - reserved_area_number_of_sectors();}

    uint8_t data_area_sectors_per_cluster() const {return sec_per_clus;}


    uint32_t data_area_number_of_clusters() const
    { return data_area_number_of_sectors() / data_area_sectors_per_cluster(); }

    uint32_t first_sector_of_cluster(uint32_t n) const
    {return  data_area_first_sector() + (n-2) * data_area_sectors_per_cluster();}

    uint32_t root_directory_first_sector() const
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
struct Directory_entry{
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
    



/***************************************************************************
 *				FAT
 *
 *  Concebimos la FAT como un array lineal, en lugar de un conjunto de
 *  sectores. Esta clase oculta el hecho de que la FAT realmente esté formada
 *  realmente por un array de sectores.
 *
 *  Oculta también el número de copias de FATs que hay. El cliente no tiene
 *  que preocuparse de saber nada de eso, concibiendo la FAT como una única
 *  FAT.
 *
 ***************************************************************************/
// Internamente la FAT puede:
//  1. Ser una única FAT, cuando Boot_sector::is_FAT_mirror_enabled() == true
//  2. Estar duplicada en diferentes FATs, cuando is_FAT_mirror_enabled()== false
//     Entiendo (???) que en este caso todas las FATs serán contiguas, siendo
//     sus sectores [sector[n], sector[n+1] - 1], siendo 
//     sector[n] = sector[0] + n * Boot_sector::FAT_area_size()
//
// En principio, como voy a dar por supuesto que tengo poca memoria RAM (el
// atmega328p tiene solo 2K y un sector ocupa 512 bytes), no voy a guardar en
// memoria la FAT. Cada vez que queramos acceder a ella, leeré el sector
// correspondiente, leeré el dato buscado, y eliminaré de memoria el sector.
//
// El problema es que eso es muy ineficiente. Si hicieramos:
//
//	    uint32_t x = FAT[cluster]; // esto carga el sector, y lee el valor
//
//  sería muy ineficiente leer los clusters de un fichero.
//
//  Por ello, la función `read` lee no solo el valor de un cluster de un
//  fichero sino N valores. Si la lista de clusters de un fichero es:
//
//	c0 -> c1 -> c2 -> c3 -> c4 -> ... -> cn
//
//  podemos hacer:
//	
//	    uint32_t file[4];
//	    FAT.read(c3, file); 
//
//  Esta función intenta meter en file c4, c5, c6 y c7 siempre todos estos
//  clusters esten en el MISMO sector. Si no estuvieran en el mismo sector,
//  read solo devuelve los clusters de ese sector, teniendo que volver a
//  llamar a `read` para leer el resto.
//
class FAT{
public:
// Constructors
    //(???) Dos opciones: o pasamos el boot sector o lo leemos. ¿Cuál es más
    //práctica? Ni idea. Experimentemos.
    FAT(const Boot_sector& bs);

    // Lee los clusters que siguen al cluster0 y los mete en file_clusters.
    template <typename Sector_driver, uint8_t N>
    uint8_t read(const uint32_t& cluster0, std::span<uint32_t, N> file_clusters);


// Como está clase es de bajo nivel voy a dejar visibles todas estas clases
// para poder depurar
    // Sector inicial donde empieza FAT[n]
    uint32_t first_sector(uint8_t n) const 
    {return sector0_ + n * number_of_sectors_;}

// Info
    uint32_t number_of_sectors() const {return number_of_sectors_;}
    uint32_t number_of_entries() const {return number_of_entries_;}
    uint32_t bytes_per_sector() const {return sector_size_;}

    uint8_t number_of_active_FATs() const {return nFATs_;}

private:
// Data
    uint32_t sector0_;	// sector donde empieza la primera FAT
    uint8_t nFATs_;	// número de FATs activas
    uint32_t number_of_sectors_; // número total de sectores de 1 FAT
    uint32_t number_of_entries_; // número de entradas de la FAT
				 // Es el número de clusters de la data area
    uint32_t sector_size_; // número de bytes por sector

// Cfg de FAT
    enum class Cluster_state{ free, allocated, bad, end_of_file, reserved };

    // Observar la forma de hablar: la FAT es un conjunto de entradas, donde
    // la posición de cada entrada hace referencia al cluster de esa posición.
    // Así, la entrada 10 de la FAT contiene:
    //	1. El estado de ese cluster: free, allocated, bad, ...
    //	2. En el caso de que sea allocated indica el valor del siguiente
    //	   cluster.
    Cluster_state cluster_state(uint32_t entry) const;
    std::pair<uint32_t, uint32_t> cluster2sector_pos(uint32_t entry) const;

// Helpers
};

FAT::FAT(const Boot_sector& bs)
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
FAT::Cluster_state FAT::cluster_state(uint32_t entry) const
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


// FAT es un array de uint32_t implementado como sectores de 512 bytes.
// Lo concebimos de 2 formas: como un array lineal de uint32_t, o como un
// array bidimensional de 512 uint8_t cada fila.
//
// Tenemos dos sistemas de coordenadas: un índice líneal que es el número de
// entry, y un par (nsector, pos) que nos dice en qué sector está la entrada y
// en que posición dentro de ese sector. La forma de relacionarlos es la
// habitual:
//		entry = nsector * 128 + pos
//
// A partir de esta fórmula calculamos nsector y pos.
// (el 128 = 512 / 4; yo no voy a complicar la cosa como hace la FAT
// specification y calcular la posición del uint8_t y luega hacer el casting
// ya que concibo la FAT como un array lineal de uint32_t)
// 
inline 
std::pair<uint32_t, uint32_t> FAT::cluster2sector_pos(uint32_t entry) const
{ return atd::div(entry, sector_size_ / sizeof(uint32_t)); }

template <typename Sector_driver, uint8_t N>
uint8_t FAT::read(const uint32_t& cluster, 
					std::span<uint32_t, N> file_clusters)
{
    using Sector = Sector_driver::Sector;

    // nsector0 es relativo al primer sector de este volumen
    // sector0_ + nsector0 = sector real
    auto [nsector0, pos] = cluster2sector_pos(cluster);

    Sector sector_container{};
    Sector_driver::read(sector0_ + nsector0, sector_container);
    
    atd::Array_of_bytes_view<Sector, uint32_t> sector{sector_container};

    uint32_t nsector{};

    uint8_t i = 0;
    for (; i < N; ++i){
	uint32_t entry = sector[pos];

	if (cluster_state(entry) == Cluster_state::allocated){
	    file_clusters[i] = entry;

	    std::tie(nsector, pos) = cluster2sector_pos(entry);

	    if (nsector != nsector0) // solo leemos 1 sector cada vez
		return i;
	}
    }

// TODO: hay que devolver el estado del cluster ¿bad? ¿EOC? ...
    return i;

}








}// namespace FAT32

}// namespace dev
 

#endif


