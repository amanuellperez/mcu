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
#include <span>

#include <atd_bit.h> // concat_bytes
#include <atd_array.h>	// Array_of_bytes_view
#include <atd_cmath.h>	// div

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
    


namespace impl_of{
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
namespace impl_of{

enum class FAT_Cluster_state : uint8_t
{ free = 0, allocated, bad, end_of_file, reserved, 
    read_error // este no es un estado del cluster sino un intento fallido de
	  // leer un sector de la FAT (realmente no debería ir aqui) (???)
};

// De momento, como no quiero que ocupe demasiado, limito nread a 5 bits 
// (se pueden leer hasta 32 uint32_t, que son 32 x 4 = 128 bytes, más los
// sectores cargados en memoria... se usaría bastante memoria)
struct FAT_Result_read_next{
    FAT_Cluster_state state : 3;
    uint8_t nread           : 5; // (???) nread? otro nombre? 

    FAT_Result_read_next(FAT_Cluster_state st, uint8_t n)
	: state{st}, nread{n} {}
};

static_assert(sizeof(FAT_Result_read_next) == 1);

}// impl_of
 
class FAT_area{
public:
// Types
    using Cluster_state    = impl_of::FAT_Cluster_state;
    using Result_read_next = impl_of::FAT_Result_read_next;

// Constructors
    FAT_area() = default; // recordar llamar a init
    void init(const Boot_sector& bs);

    FAT_area(const Boot_sector& bs) {init(bs);}


    // Lee los clusters que siguen al cluster0 y los mete en file_clusters.
    //template <typename Sector_driver, uint8_t N>
    template <typename Sector_driver>
    Result_read_next read_next(const uint32_t& cluster0, 
			    //std::span<uint32_t, N> file_clusters) const;
			    std::span<uint32_t> file_clusters) const;


    template <typename Sector_driver>
    bool read(uint32_t nsector, typename Sector_driver::Sector& sector) const
    { return Sector_driver::read(sector0_ + nsector, sector); }

// FAT_area Info
// --------
// Como está clase es de bajo nivel voy a dejar visibles todas estas clases
// para poder depurar
    // Sector inicial donde empieza FAT_area[n]
    uint32_t first_sector(uint8_t n) const 
    {return sector0_ + n * number_of_sectors_;}

    uint32_t number_of_sectors() const {return number_of_sectors_;}
    uint32_t number_of_entries() const {return number_of_entries_;}
    uint32_t bytes_per_sector() const {return sector_size_;}

    uint8_t number_of_active_FATs() const {return nFATs_;}


private:
// FAT_area info
    uint32_t sector0_;	// sector donde empieza la primera FAT_area
    uint8_t nFATs_;	// número de FATs activas
    uint32_t number_of_sectors_; // número total de sectores de 1 FAT_area
    uint32_t number_of_entries_; // número de entradas de la FAT_area
				 // Es el número de clusters de la data area
    uint32_t sector_size_; // número de bytes por sector
			   

// Cfg de FAT_area

    // Observar la forma de hablar: la FAT_area es un conjunto de entradas, donde
    // la posición de cada entrada hace referencia al cluster de esa posición.
    // Así, la entrada 10 de la FAT_area contiene:
    //	1. El estado de ese cluster: free, allocated, bad, ...
    //	2. En el caso de que sea allocated indica el valor del siguiente
    //	   cluster.
    Cluster_state cluster_state(uint32_t entry) const;
    std::pair<uint32_t, uint32_t> cluster2sector_pos(uint32_t entry) const;

// Helpers
};


// FAT_area es un array de uint32_t implementado como sectores de 512 bytes.
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
// (el 128 = 512 / 4; yo no voy a complicar la cosa como hace la FAT_area
// specification y calcular la posición del uint8_t y luega hacer el casting
// ya que concibo la FAT_area como un array lineal de uint32_t)
// 
inline 
std::pair<uint32_t, uint32_t> FAT_area::cluster2sector_pos(uint32_t entry) const
{ return atd::div(entry, sector_size_ / sizeof(uint32_t)); }

//template <typename Sector_driver, uint8_t N>
template <typename Sector_driver>
FAT_area::Result_read_next FAT_area::read_next(const uint32_t& cluster, 
				//std::span<uint32_t, N> file_clusters) const
				std::span<uint32_t> file_clusters) const
{
    using Sector = Sector_driver::Sector;

    // nsector0 es relativo al primer sector de este volumen
    auto [nsector0, pos] = cluster2sector_pos(cluster);

    Sector sector_container{};
    if (!read<Sector_driver>(nsector0, sector_container))
	return {Cluster_state::read_error, 0};
    
    atd::Array_of_bytes_view<Sector, uint32_t> sector{sector_container};

    uint32_t nsector{};

    for (uint8_t i = 0; i < file_clusters.size(); ++i){
	uint32_t entry = sector[pos];

	auto state = cluster_state(entry);
	if (state != Cluster_state::allocated)
	    return {state, i};

	file_clusters[i] = entry;

	std::tie(nsector, pos) = cluster2sector_pos(entry);

	if (nsector != nsector0) // solo leemos 1 sector cada vez
	    return {Cluster_state::allocated, static_cast<uint8_t>(i + 1)};

    }

    return {Cluster_state::allocated, 
			    static_cast<uint8_t>(file_clusters.size())};

}


/***************************************************************************
 *				DATA AREA
 ***************************************************************************/
class Data_area{
public:
// Constructors
    Data_area() = default;
    void init(const Boot_sector& bs);

    Data_area(const Boot_sector& bs) {init(bs);}

// Info
// ----
    uint32_t first_sector() const
    { return data_area_first_sector_;}

    uint8_t  sectors_per_cluster() const
    { return data_area_sectors_per_cluster_;}

    // Esta es la función fundamental que vamos a usar para localizar los
    // sectores
    uint32_t first_sector_of_cluster(uint32_t n) const
    {return  first_sector() + (n-2) * sectors_per_cluster();}

    uint32_t sector_number(uint32_t c, uint8_t s) const
    { return first_sector_of_cluster(c) + s; }

private:
// Data Area info 
    uint32_t data_area_first_sector_;
    uint8_t data_area_sectors_per_cluster_;
};


} // impl_of

/***************************************************************************
 *				VOLUME
 ***************************************************************************/
// La reserved area, salvo cuando se vaya a formatear un volumen, no se va a
// tocar. Por ello, si se necesita información sobre ella leerla directamente
// del Sector_device
template <typename Sector_driver0>
class Volume{
public:
// Types 
    using Sector_driver = Sector_driver0;
    using Sector	= typename Sector_driver::Sector;
    using Sector_span   = typename Sector_driver::Sector_span;
    static constexpr auto sector_size = Sector_driver::sector_size;

    using FAT_area  = impl_of::FAT_area;
    using Data_area = impl_of::Data_area;
    using Cluster_state = impl_of::FAT_area::Cluster_state;

// Data
    FAT_area fat_area;
    Data_area data_area;

// Constructors
    Volume(const uint32_t& sector0);

// State
    bool error() const {return sector_size_ == 0;}

// Reserved area
    uint32_t volume_first_sector() const {return sector0_;}
    uint32_t bytes_per_sector() const {return sector_size_;}


// Sectors read/write
// Funciones globales (nsectors son relativos a todo el disco duro)
    // Lee el boot sector que se encuentra en el sector `nsector` del disco.
    static bool read_global(uint32_t nsector, Boot_sector& bs); 

    static bool read_global(uint32_t nsector, Sector_span sector) 
    { return Sector_driver::read(nsector, sector); }


// Funciones locales (nsectors son relativos al volumen)
    // Lee el sector `nsector` del volumen. `nsector` es relativo al volumen.
    bool read(uint32_t nsector, Sector_span sector) const
    { return Sector_driver::read(sector0_ + nsector, sector); }


private:
// Reserved area
    uint32_t sector0_; // volume first sector
    uint32_t sector_size_; // número de bytes por sector
		       
};

template <typename SD>
Volume<SD>::Volume(const uint32_t& sector0)
    :sector0_{sector0}
{
    Boot_sector bs;
    if (!read_global(sector0_, bs))
	sector_size_ = 0;

// Reserved area
    sector_size_ = bs.bytes_per_sector();

    fat_area.init(bs);
    data_area.init(bs);
}


template <typename SD>
inline bool Volume<SD>::read_global(uint32_t nsector, Boot_sector& bs)
{
    return read_global(nsector, 
	    Sector_span{reinterpret_cast<uint8_t*>(&bs), sizeof(Boot_sector)});
}



/***************************************************************************
 *				FILE
 ***************************************************************************/
// Un FAT32::File es:
//
//  1) para el usuario una lista enlazada de S sectores.
//  2) implementado internamente como una lista enlazada de C clusters.
//
// Internamente guardamos parte de la lista de clusters del fichero:
//
// File: C clusters:
//       +---+---+---+---+---+---+---+---+---+---+---+---+
//       | 0 | 1 | 2 | 3 | 4 | 5 |...|   |   |   |   |C-1|
//       +---+---+---+---+---+---+---+---+---+---+---+---+
//                    ~~~~~~~~~~~
//                    almacenados
//
// número de índices de clusters que almacenamos en memoria.
//
// (???) Se podía implementar esto como un iterador que iterase por los
// sectores del fichero. Pero al querer almacenar una pequeña cache con la
// lista de los clusters ese iterador no cumpliría con el requisito de que sea
// pequeño, no siendo realmente un iterador clásico. Por eso no lo defino como
// iterador (aunque tenga las mismas funciones con otros nombres).
namespace impl_of{
enum class File_state : uint8_t {
    ok
    , read_error
    , end_of_file
};


}// impl_of
template <typename Sector_driver0, uint8_t cluster_sublist_size> 
class File{
public:
// Types
    using Volume	= atd::FAT32::Volume<Sector_driver0>;
    using Sector_driver = Sector_driver0;
    using Sector	= typename Volume::Sector;
    using Sector_span   = typename Volume::Sector_span;
    using State         = impl_of::File_state;

    static_assert(cluster_sublist_size > 0);

// Constructors
    File(Volume& volume, uint32_t cluster0);

    // Devuelve el número del sector global actual
    uint32_t sector_number() const;

    // Lee el sector actual cargándolo en `sector`
    bool read(Sector_span& sector) const;

    // Pasa al siguiente cluster de la lista enlazada.
    // Devuelve true si lo obtiene, false si falla. 
    bool next_sector();

    // Al ser una lista enlazada para conocer el size hay que iterar por todos
    // los elementos, no siendo muy eficiente. Implementarlo si se necesita.
    // cluster inicial sin iterar por todos los cluster de dicho fichero.
//    uint32_t nclusters() const; 
//    uint32_t nsectors() const {nclusters() * sectors_per_cluster();}

// State
    bool ok() const {return state_ == State::ok;}
    bool error() const {return state_ == State::read_error;}
    bool end_of_file() const {return state_ == State::end_of_file;}

private:
// Data
    Volume& vol_;

    // Vamos a usar una pequeña cache para almacenar los clusters:
    // +---+---+---+---+---+---+---+---+
    // |   |   |   |   | / | / | / | / | x
    // +---+---+---+---+---+---+---+---+
    //  0  .. c_ ..      ^               ^
    //                   |               |
    //             cluster_size_     capacity_ (=cluster_sublist_size)
    //  Donde / significa que es un elemento del array sin usar, y xx es que
    //  no pertenece al array (es el end).
    //
    std::array<uint32_t, cluster_sublist_size> cluster_;    // cache
    uint8_t cluster_size_;  // número de elementos de la cache validos
			    // cluster_[0...cluster_size_) son los clusters
			    // válidos de cluster_
			   
    // [c, s] = sector actual. c es el número de cluster en cluster_, mientras
    // que s es el sector dentro de cluster_[c].
    uint8_t c_;
    uint8_t s_; 

    State state_;

// Helpers
    bool update_cache_with_next_clusters(uint32_t cluster0
					    , bool save_first_cluster);
    
};

template<typename SD, uint8_t N>
inline File<SD, N>::File(Volume& vol,  uint32_t cluster0)
    : vol_{vol}
{ update_cache_with_next_clusters(cluster0, true); }

// Precondition: cluster_[c_] >= vol_.data_area.first_sector();
template<typename SD, uint8_t N>
inline uint32_t File<SD,N>::sector_number() const
{ return vol_.data_area.first_sector_of_cluster(cluster_[c_]) + s_; }


template<typename SD, uint8_t N>
inline bool File<SD,N>::read(Sector_span& sector) const
{ return vol_.read_global(sector_number(), sector); }

template<typename SD, uint8_t N>
bool File<SD,N>::next_sector()
{
    state_ = State::ok;

    ++s_;
    if (s_ < vol_.data_area.sectors_per_cluster())
	return true;

    s_ = 0;
    ++c_;
    if (c_ < cluster_size_)
	return true;

    // ya no hay más clusters en el fichero:
    if (cluster_size_ < cluster_.size()) {
	c_ = 0; // no es necesario pero es por si alguien llama a read despues
		// de que esta función devuelva false.
	state_ = State::end_of_file;
	return false;
    }

    return update_cache_with_next_clusters(cluster_[c_ - 1], false);
}


template<typename SD, uint8_t N>
bool File<SD,N>::update_cache_with_next_clusters(uint32_t cluster0
					    , bool save_first_cluster)
{
    uint8_t incr = 0; // guardamos en cluster_[0..end)

    if (save_first_cluster){
	cluster_[0] = cluster0;
	incr = 1; // tenemos que guardar en cluster_[1..end)
    }


    auto res = vol_.fat_area.template
		    read_next<Sector_driver>(cluster0, {cluster_.begin() + incr, cluster_.end()});

    if (res.state == Volume::Cluster_state::read_error)
	state_ = State::read_error;
    else
	state_ = State::ok;

    cluster_size_ = res.nread;

    c_ = 0;
    s_ = 0;

    return state_ == State::ok;
}


}// namespace FAT32

}// namespace dev
 

#endif


