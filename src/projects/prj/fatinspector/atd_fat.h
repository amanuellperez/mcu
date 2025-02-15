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
 *	https://wiki.osdev.org/FAT (referencia osdev)
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

// read/write
// La MBR siempre está en el sector 0
template <typename Sector_driver>
inline bool read(MBR& mbr)
{ 
    using Sector_span = typename Sector_driver::Sector_span;
    return Sector_driver::read(0, 
	    Sector_span{reinterpret_cast<uint8_t*>(&mbr), sizeof(MBR)});
}


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
    uint16_t FS_info_sector() const {return fs_info;}
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

// En el boot sector hay un montón de bytes desperdiciados que no los quiero
// para nada. Nos quedamos con la información mínima que no vamos a guardar en
// Volume para poder devolverla al leer el Boot_sector
// Lo iré rellenando según vaya necesitando cosas (quiero que sea lo más
// pequeña posible)
struct Boot_sector_min{
    uint16_t fs_info;

    uint16_t FS_info_sector() const {return fs_info;}
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
    


// FAT_area hace referencia a Volume
template <typename S>
class Volume;

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
 
template <typename Sector_driver>
class FAT_area{
public:
// Types
    using Volume           = atd::FAT32::Volume<Sector_driver>;
    using Cluster_state    = impl_of::FAT_Cluster_state;
    using Result_read_next = impl_of::FAT_Result_read_next;
    using size_type        = uint16_t; // de 512 habitualmente < 2^16
				       
// Constructors
    FAT_area(Volume* vol) : volume_{vol} {}
    void init(const Boot_sector& bs);

    FAT_area(const Boot_sector& bs) {init(bs);}


    // Lee los clusters que siguen al cluster0 y los mete en file_clusters.
    // Devuelve el número de clusters leídos y el estado del último cluster
    // leído (allocated, end_of_file, ...)
    Result_read_next read_next(const uint32_t& cluster0, 
			    std::span<uint32_t> file_clusters) const;

    // Lee el siguiente cluster a cluster0 guardándolo en next_cluster.
    Result_read_next read_next(const uint32_t& cluster0, uint32_t& next_cluster);

    // Lee el sector nsector de la FAT area.
    // nsector es relativo a la FAT area.
    template <Type::Integer Int>
    Int read(uint32_t nsector, size_type pos) const
    { return volume_->template read_global<Int>(sector0_ + nsector, pos);}

    // Indica si ha habido algún error en la llamada de `read`
    bool read_error() const{ return !volume_->ok();}

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
    Volume* volume_;

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
// tanto entry, como (nsector, pos) son coordenadas locales a la FAT area.
template <typename S>
inline 
std::pair<uint32_t, uint32_t> FAT_area<S>::cluster2sector_pos(uint32_t entry) const
{ return atd::div(entry, sector_size_ / sizeof(uint32_t)); }



template <typename S>
void FAT_area<S>::init(const Boot_sector& bs)
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
template <typename S>
FAT_area<S>::Cluster_state FAT_area<S>::cluster_state(uint32_t entry) const
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

    using FAT_area  = impl_of::FAT_area<Sector_driver>;
    using Data_area = impl_of::Data_area;
    using Cluster_state = FAT_area::Cluster_state;

// Data
    FAT_area fat_area;
    Data_area data_area;

// Constructors
    Volume(const uint32_t& sector0);

    // Esta devuelve la información del boot sector que no queda anotada por
    // defecto en Volume
    Volume(const uint32_t& sector0, Boot_sector_min& bs);

// State
    bool ok() const { return driver_.ok() == true and sector_size_ != 0;}
    bool error() const { return !ok(); }

// Reserved area
    uint32_t volume_first_sector() const {return sector0_;}
    uint32_t bytes_per_sector() const {return sector_size_;}


// Lectura/escritura de sectores
    // nsector es coordenada local, 
    // el número de sector referido al principio del volumen
    template <Type::Integer Int = uint8_t>
    Int read(const uint32_t& nsector, uint8_t pos)
    { return driver_.template read<Int>(sector0_ + nsector, pos); }
    
    // nsector es coordenada global, 
    // es el número de sector referido al  principio del disco
    template <Type::Integer Int = uint8_t>
    Int read_global(const uint32_t& nsector, uint8_t pos)
    { return driver_.template read<Int>(nsector, pos); }

// Funciones globales (nsectors son relativos a todo el disco duro)
// (TODO) ¿Qué pasa si falla la lectura? No está devolviendo error!!!
// Añadir un state para saber si el sector se ha leido adecuadamente!!!
    template <typename T>
    T* read_global_as(const uint32_t& nsector)
    { return driver_.template sector_pointer_as<T>(nsector); }

// (TODO) ¿Qué pasa si falla la lectura? No está devolviendo error!!!
    template <typename T>
    T* read_as(const uint32_t& nsector)
    { return read_global_as<T>(sector0_ + nsector);}


private:
// Reserved area
    uint32_t sector0_; // volume first sector
    uint32_t sector_size_; // número de bytes por sector
	
// Sectores cargados en memoria
    Sector_driver driver_;

// Helpers
    void init(Boot_sector& bs);



// Funciones locales (nsectors son relativos al volumen)
    static bool read_global(uint32_t nsector, Boot_sector& bs); 

    static bool read_global(uint32_t nsector, Sector_span sector) 
    { return Sector_driver::read(nsector, sector); }
//
//    // Lee el sector `nsector` del volumen. `nsector` es relativo al volumen.
//    bool read(uint32_t nsector, Sector_span sector) const
//    { return Sector_driver::read(sector0_ + nsector, sector); }
//
//    // Lee el boot sector que se encuentra en el sector `nsector` del disco.
//    bool read(uint32_t nsector, FS_info& fs) const;
//
//    bool read(Boot_sector& bs) const;

};

// precondition: sector0_ valido
template <typename SD>
void Volume<SD>::init(Boot_sector& bs)
{
    if (!read_global(sector0_, bs)){
	sector_size_ = 0;
	return;
    }

// Reserved area
    sector_size_ = bs.bytes_per_sector();

    fat_area.init(bs);
    data_area.init(bs);
}


template <typename SD>
Volume<SD>::Volume(const uint32_t& sector0)
    : fat_area{this}, sector0_{sector0}
{
    Boot_sector bs;
    init(bs);
}

template <typename SD>
Volume<SD>::Volume(const uint32_t& sector0, Boot_sector_min& bs_min)
    : fat_area{this}, sector0_{sector0}
{
    Boot_sector bs;
    init(bs);

    bs_min.fs_info = bs.fs_info;
}



template <typename SD>
inline bool Volume<SD>::read_global(uint32_t nsector, Boot_sector& bs)
{
    return read_global(nsector, 
	    Sector_span{reinterpret_cast<uint8_t*>(&bs), sizeof(Boot_sector)});
}

//template <typename SD>
//inline bool Volume<SD>::read(Boot_sector& bs) const
//{ return read_global(sector0_, bs); }
//
//
//template <typename SD>
//inline bool Volume<SD>::read(uint32_t nsector, FS_info& fs) const
//{
//    return read(nsector, 
//	    Sector_span{reinterpret_cast<uint8_t*>(&fs), sizeof(FS_info)});
//}


/***************************************************************************
 *			  FAT AREA IMPLEMENTATION
 ***************************************************************************/
namespace impl_of{
// Funciones que necesitan conocer la definición de Volume
template <typename Sector_driver>
FAT_area<Sector_driver>::Result_read_next 
	FAT_area<Sector_driver>::read_next(const uint32_t& cluster, 
				std::span<uint32_t> file_clusters) const
{
    // nsector0 es relativo al primer sector de este volumen
    auto [nsector0, pos] = cluster2sector_pos(cluster);

    for (uint8_t i = 0; i < file_clusters.size(); ++i){
	uint32_t entry = read<uint32_t>(nsector0, pos);

	if (read_error()){
	    ctrace<3>() << "Volume read error\n";
	    return {Cluster_state::read_error, 0};
	}


	auto state = cluster_state(entry);
	if (state != Cluster_state::allocated)
	    return {state, i};

	file_clusters[i] = entry;

	uint32_t nsector;
	std::tie(nsector, pos) = cluster2sector_pos(entry);

	if (nsector != nsector0) // solo leemos 1 sector cada vez
	    return {Cluster_state::allocated, static_cast<uint8_t>(i + 1)};

    }

    return {Cluster_state::allocated, 
			    static_cast<uint8_t>(file_clusters.size())};

}

// (FUTURO) Como esto está pensado para micros con poca memoria, basta con ir
// cargando 1 cluster de uno en uno (cada cluster tiene 32 sectores que hay
// que leer, que ocupan 32 x 512 bytes = 16 kB. Los micros que uso tienen
// máximo 6kB, no entrando en memoria)
template <typename Sector_driver>
FAT_area<Sector_driver>::Result_read_next 
	FAT_area<Sector_driver>::read_next(const uint32_t& cluster, uint32_t& next_cluster)
{
    uint32_t n[1];
    auto res = read_next(cluster, n);
    next_cluster = n[0];

    return res;
}

} // namespace impl_of

/***************************************************************************
 *				FILE_SECTORS
 * -------------------------------------------------------------------------
 *
 * Un FAT32::File_sectors es:
 *
 *  1) para el usuario una lista enlazada de S sectores.
 *  2) implementado internamente como una lista enlazada de C clusters.
 *
 * Lo que nos permite es iterar por los sectores de un fichero.
 *
 ***************************************************************************/
namespace impl_of{
enum class File_state : uint8_t {
    ok
    , error
    , end_of_file
};


}// impl_of
 

template <typename Sector_driver0> 
class File_sectors{
public:
// Types
    using Volume	= atd::FAT32::Volume<Sector_driver0>;
    using Sector_driver = Sector_driver0;
    using Sector	= typename Volume::Sector;
    using Sector_span   = typename Volume::Sector_span;
    using State         = impl_of::File_state;

// Data
    Volume& volume;

// Constructors
    File_sectors(Volume& volume); // no olvidar llamar a first_sector!!!
    File_sectors(Volume& volume, uint32_t cluster0);

    // Reiniciamos el File_sectors, haciendo que apunte al primer sector del
    // cluster0
    void first_sector(uint32_t cluster0);

// Operations
    // Pasa al siguiente cluster de la lista enlazada.
    // Devuelve true si lo obtiene, false si falla. 
    bool next_sector();

// read/write
    // Devuelve el número del sector global actual
    uint32_t global_sector_number() const;

    // Devuelve el byte i del sector actual al que apunta global_sector_number
    template <Type::Integer Int>
    Int read(uint32_t i) const
    { return volume.template read_global<Int>(global_sector_number(), i); }

    // Al ser una lista enlazada para conocer el size hay que iterar por todos
    // los elementos, no siendo muy eficiente. Implementarlo si se necesita.
    // cluster inicial sin iterar por todos los cluster de dicho fichero.
//    uint32_t nclusters() const; 
//    uint32_t nsectors() const {nclusters() * sectors_per_cluster();}

// State
    bool ok() const {return state_ == State::ok;}
    bool error() const {return state_ == State::error;}
    bool end_of_file() const {return state_ == State::end_of_file;}

// Para depurar
    // Pasa a apuntar al primer sector del siguiente cluster, en caso de que
    // exista. Si existe devuelve true, en caso contrario false.
    bool next_cluster();    

private:
    uint8_t cluster_; // cluster actual
    uint8_t sector_;  // sector al que apuntamos dentro de cluster_

    State state_;

// Helpers
    bool update_cache_with_next_clusters(uint32_t cluster0);
    
};

template<typename SD>
inline File_sectors<SD>::File_sectors(Volume& vol)
    : volume{vol}, cluster_{0}, sector_{0} // inicializo a 0 para detectar errores
{ state_ = State::error; } 

template<typename SD>
inline File_sectors<SD>::File_sectors(Volume& vol,  uint32_t cluster0)
    : volume{vol}
{ first_sector(cluster0); }

template<typename SD>
inline void File_sectors<SD>::first_sector(uint32_t cluster0)
{ 
    cluster_ = cluster0;
    sector_  = 0;
    state_   = State::ok;
}



// Precondition: cluster_.sector_ >= vol_.data_area.first_sector();
template<typename SD>
inline uint32_t File_sectors<SD>::global_sector_number() const
{ return volume.data_area.first_sector_of_cluster(cluster_) + sector_; }


template<typename SD>
bool File_sectors<SD>::next_cluster()
{
    sector_ = 0;

    return update_cache_with_next_clusters(cluster_);
}


template<typename SD>
bool File_sectors<SD>::next_sector()
{
    state_ = State::ok;

    ++sector_;
    if (sector_ < volume.data_area.sectors_per_cluster())
	return true;

    return next_cluster();
}


template<typename SD>
bool File_sectors<SD>::update_cache_with_next_clusters(uint32_t cluster0)
{
    uint32_t next_cluster;
    auto res = volume.fat_area.read_next(cluster0, next_cluster);

    if (res.state == Volume::Cluster_state::read_error)
	state_ = State::error;
    else
	state_ = State::ok;

    cluster_ = next_cluster;
    sector_  = 0;

    return state_ == State::ok;
}

/***************************************************************************
 *				    FILE
 * -------------------------------------------------------------------------
 *
 *  Concebimos un fichero como un conjunto secuencial de bytes, por el que
 *  podemos iterar de forma habitual.
 *
 *  Doy la opción de concebir el fichero como un conjunto de bytes o de
 *  uint32_t ya que el FAT area es un array de uint32_t. De esa forma
 *  resulta muy sencillo iterar por las entries esa zona.
 *
 ***************************************************************************/
namespace impl_of{

template <typename Sector_driver0, Type::Integer Int> 
struct const_File_iterator;

struct const_iterator_end{};

}// impl_of
 
// Se trata de una vista de File_sectors que permite concebirlo como un
// conjunto continuo de bytes.
template <typename Sector_driver0
	, Type::Integer Int0 = uint8_t // es un fichero de Ints
	> 
class File{
public:
// Types
    using Int    = Int0;
    using File_sectors = atd::FAT32::File_sectors<Sector_driver0>;
    using Volume = typename File_sectors::Volume;
    using State  = typename File_sectors::State;

    using const_iterator     = impl_of::const_File_iterator<Sector_driver0, Int>;
    using const_iterator_end = impl_of::const_iterator_end;

    friend const_iterator;

// Constructors
    File(Volume& volume, uint32_t cluster0);

    
// Iteramos usando esta clase
    // Reinicia el fichero, haciendo que apunte al principio de nuevo
    void reset();

    // Avanza nInts. Por ejemplo, si Int = uint8_t y nInts = 32, avanza 32
    // bytes. Pero si Int = uin32_t y nInts = 32, avanza 32 uint32_t.
    void next(uint8_t nInts = 1);

    uint8_t read();

// Acceso aleatorio
// uint8_t operator[](size_type i); // puede ser costoso en tiempo

// Iteradores
//    iterator begin();
//    iterator end();

    const_iterator begin();
    const_iterator_end end() const;

// State
    bool ok() const {return sectors_.ok(); }
    bool error() const {return sectors_.error(); }
    bool end_of_file() const {return sectors_.end_of_file();}

// Info
    Volume& volume() {return sectors_.volume;}


private:
    uint32_t cluster0_; // primer cluster del fichero
			
    File_sectors sectors_; 
    uint16_t i; // byte actual al que apuntamos dentro del sector
    // assert(volume.bytes_per_sector() < 2^sizeof(uint16_t));
    

// Funciones de implementacion 
    static uint16_t next_byte(File_sectors& sector, uint16_t i);
};


template <typename SD, Type::Integer I> 
inline File<SD, I>::File(Volume& volume, uint32_t cluster0)
    : cluster0_{cluster0}, sectors_(volume, cluster0)
{}

template <typename SD, Type::Integer I> 
inline void File<SD, I>::reset()
{
    sectors_.first_sector(cluster0_);
    i = 0;
}

// (DUDA) modificar next_byte directamente para que avance directamente
// sizeof(Int)? La implementación que ahora suministro es sencilla de
// entender y mantener.
template <typename SD, Type::Integer I> 
inline void File<SD, I>::next(uint8_t nInts)
{ 
    for (; nInts > 0; --nInts){
	for (uint8_t k = 0; k < sizeof(Int); ++k)
	    i = next_byte(sectors_, i); 
    }
}

template <typename SD, Type::Integer I> 
inline uint8_t File<SD, I>::read()
{ 
    // if (sector.end_of_file() or sector.error()) return 0xEA;
    return sectors_.template read<Int>(i); 
}


template <typename SD, Type::Integer I> 
uint16_t File<SD, I>::next_byte(File_sectors& sector, uint16_t i)
{
    if (sector.end_of_file() or sector.error())
	return i;

    ++i;
    if (i < sector.volume.bytes_per_sector())
	return i;

    i = 0;
    sector.next_sector(); // puede devolver error o end_of_file en state
    
    return i;
}

namespace impl_of{

template <typename Sector_driver0, Type::Integer Int> 
struct const_File_iterator{
// Data
    using File_sectors = atd::FAT32::File_sectors<Sector_driver0>;
    using File	       = atd::FAT32::File<Sector_driver0>;

    File_sectors sector;
    uint16_t i; // byte actual al que apuntamos dentro del sector

    const_File_iterator(const File_sectors& s, uint32_t cluster0) 
	: sector{s}, i{0} { sector.first_sector(cluster0); }

    const_File_iterator& operator++()
    {
	for (uint8_t k = 0; k < sizeof(Int); ++k)
	    i = File::next_byte(sector, i); 

	return *this;
    }
    
    const_File_iterator operator++(int)
    {
	const_File_iterator tmp = *this;
	++*this;
	return tmp;
    }

    // Observar que como es un const_iterator puedo devolver 
    // directamente un Int en lugar de Int&
    Int operator*()
    {
	// if (sector.end_of_file() or sector.error()) return 0xEA;
	return sector.template read<Int>(i);
    }

    //pointer operator->() { return &(**this); }

    friend
    bool operator==(const const_File_iterator& a, const const_iterator_end&)
    { return a.i == 0 and a.sector.end_of_file(); }

    friend
    bool operator!=(const const_File_iterator& a, const const_iterator_end& b)
    { return !(a == b); }
};


}// impl_of



template <typename SD, Type::Integer I> 
inline File<SD, I>::const_iterator File<SD, I>::begin()
{ return const_iterator{sectors_, cluster0_}; }

template <typename SD, Type::Integer I> 
inline File<SD, I>::const_iterator_end File<SD, I>::end() const
{ return const_iterator_end{}; }


/***************************************************************************
 *				    DIRECTORY
 ***************************************************************************/
namespace impl_of{
enum class Entry_attribute_type: uint8_t{
    read_only = 0x01,
    hidden    = 0x02,
    system    = 0x04,
    volume_id = 0x08,
    directory = 0x10,
    archive   = 0x20,
    long_name = read_only | hidden | system | volume_id
};

enum class Entry_type : uint8_t{
    free,			
    free_and_no_more_entries,
    short_entry,    
    long_entry
};


struct Directory_entry{
// Types
    using Attribute_type = Entry_attribute_type;
    using Type = Entry_type;

// Cfg
    static constexpr uint8_t size = 32;

// Data
    std::array<uint8_t, size> data;

// Tipo de entrada
    Type type() const;

// short entry (section 6, FAT specification)
    // devuelve el número de caracteres copiados
    uint8_t copy_short_name(std::span<uint8_t> str);

    Attribute_type attribute_type() const { return Attribute_type{data[11]}; }
    // data[12] == 0
    
    uint8_t creation_time_tenth_of_seconds() const {return data[12];}
    uint16_t creation_time_seconds() const 
	    {return atd::concat_bytes<uint16_t>(data[15], data[14]);}

    uint16_t creation_date() const
	    {return atd::concat_bytes<uint16_t>(data[17], data[16]);}

    uint16_t last_access_date() const
	    {return atd::concat_bytes<uint16_t>(data[19], data[18]);}

    // devuelve el cluster del file asociado con esta entrada
    uint32_t file_cluster() const
    {return atd::concat_bytes<uint32_t>(data[21], data[20], data[27], data[26]);}

    uint16_t last_modification_time() const
	    {return atd::concat_bytes<uint16_t>(data[23], data[22]);}

    uint16_t last_modification_date() const
	    {return atd::concat_bytes<uint16_t>(data[25], data[24]);}

    uint32_t file_size() const
    {return atd::concat_bytes<uint32_t>(data[31], data[30], data[29], data[28]);}


// long name entry (section 7, FAT specification)
    uint8_t extended_order() const {return data[0];}
    // data[1..11) = long name
    // data[11] = attribute. En este caso ATTR_LONG_NAME
    // data[12] == 0
    uint8_t check_sum() const { return data[13]; }
    // data[14..26) = long name
    // data[26] == 0 and data[27] == 0
    // data[28..32) = long name
    // devuelve el número de caracteres copiados
    uint8_t copy_long_name(std::span<uint8_t> str);

};




template <typename Sector_driver0>
struct const_Entry_iterator;

}// impl_of
 
template <typename Sector_driver0> 
class Directory_of_entries{
public:
// Types
    using Volume    = atd::FAT32::Volume<Sector_driver0>;
    using File	    = atd::FAT32::File<Sector_driver0>;
    using Entry	    = impl_of::Directory_entry;


// Constructors
    // Enlaza con el directorio que se encuentra en el cluster0
    Directory_of_entries(Volume& volume, uint32_t cluster0);

// Operaciones
    void first_entry();
//    void next_entry();
    
    // Lee la entry actual almacenándola en entry
    // Consume esa entry, esto es, pasa a apuntar a la siguiente entry.
    void read(Entry& entry);

// State
    bool ok() const {return dir_.ok(); }
    bool error() const {return dir_.error(); }
    bool end_of_file() const {return dir_.end_of_file();}

private:
    File dir_; // fichero con los sectores del directorio
    
    static constexpr uint8_t sizeof_entry = Entry::size;
};

template <typename S> 
Directory_of_entries<S>::Directory_of_entries(Volume& volume, uint32_t cluster0)
    : dir_{volume, cluster0}
{ }

template <typename S> 
inline void Directory_of_entries<S>::first_entry()
{ dir_.reset(); }

//template <typename S> 
//inline void Directory_of_entries<S>::next_entry()
//{ dir_.next(sizeof_entry); }
//
template <typename S> 
inline void Directory_of_entries<S>::read(Entry& entry)
{
    if (dir_.end_of_file() or dir_.error())
	return;

// (TODO) dir_.read(entry.data); // lee los 32 bytes. Es un std::span 
    for (uint8_t i = 0; i < sizeof_entry; ++i){
	entry.data[i] = dir_.read();
	dir_.next();
    }
}

}// namespace FAT32

}// namespace dev
 

#endif


