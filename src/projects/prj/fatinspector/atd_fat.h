//// Copyright (C) 2025 Manuel Perez 
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
 *	FAT specification
 *	http://elm-chan.org/docs/fat_e.html
 *	https://wiki.osdev.org/FAT (referencia osdev)
 *
 * COMENTARIOS
 *	Duda de diseño:
 *	¿Consumir o no consumir los elementos leídos?
 *
 *	(1) Iteradores:
 *	    Los iteradores no consumen cuando leen:
 *	    p = begin();
 *	    std::cout << *p << *p;  // imprime dos veces el mismo valor
 *	    ++p;		    // consume el elemento pasando al siguiente
 *	    std::cout << *p;	    // imprime el siguiente elemento
 *
 *	(2) Flujos:
 *	    read(x); // lee x y consume ese elemento pasando al siguiente!!!
 *
 *	    read(x) es el equivalente a *p con la diferencia de que read
 *	    consume mientras que *p no consume el elemento actual.
 *
 *	Como no tenía claro esto cuando empecé a escribir esta biblioteca está
 *	todo mezclado. 
 *
 *	Resultado:
 *	    Al final uso las funciones read/write que tienen que funcionar
 *	    como las clásicas de C ==> consumen los caracteres que
 *	    leen/escriben.
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
#include <atd_contracts.h>  // precondition
			    
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
//template <typename Sector_driver>
//inline bool read(MBR& mbr)
//{ 
//    using Sector_span = typename Sector_driver::Sector_span;
//    return Sector_driver::read(0, 
//	    Sector_span{reinterpret_cast<uint8_t*>(&mbr), sizeof(MBR)});
//}
//

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

    uint8_t FAT_number_of_clusters_per_sector() const
    { return bytes_per_sector() / FAT_bytes_per_entry(); }

    uint32_t FAT_number_of_clusters() const
    { return FAT_number_of_clusters_per_sector() * FAT_number_of_sectors();}

    // El número de entradas en una FAT es:
    //	    2 + data_area_number_of_clusters()
    // donde los 2 proceden de que FAT[0] y FAT[1] son reserved.
    // En la práctica parece ser que se reserva más tamaño del necesario. Esta
    // función devuelve el exceso de sectores.
    // Cada entry de una FAT corresponde co un cluster del data area.
    // 
    // (HACK) ¿Se podría usar esta zona para guardar documentos secretos?
    uint32_t FAT_unused_entries() const
    { return FAT_number_of_clusters() 
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

    uint32_t root_directory_first_cluster() const
    {return  root_clus;}

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


// FAT_area hace referencia a Volume
template <typename S>
class Volume;

namespace impl_of{
/***************************************************************************
 *				FAT area
 *
 *  La FAT area es:
 *	(1) Un conjunto de sectores contiguos,
 *	(2) usados para almacenar las listas enlazadas que forman los clusters
 *	    de los diferentes ficheros del volumen.
 *	(3) Es redundante, esto es, la FAT area está duplicada.
 *
 * La concebimos como un driver de listas enlazadas que puede:
 *	(1) Crear/borrar una lista enlazada completa.
 *	(2) Modificar una lista enlazada añadiendo/borrando nodos.
 *
 *  Esta clase oculta también el número de copias de FATs que hay. El cliente
 *  no tiene que preocuparse de saber nada de eso, concibiendo la FAT como una
 *  única FAT.
 *
 ***************************************************************************/
// Internamente la FAT puede:
//  1. Ser una única FAT, cuando Boot_sector::is_FAT_mirror_enabled() == true
//  2. Estar duplicada en diferentes FATs, cuando is_FAT_mirror_enabled()== false
//     Entiendo (???) que en este caso todas las FATs serán contiguas, siendo
//     sus sectores [sector[n], sector[n+1] - 1], siendo 
//     sector[n] = sector[0] + n * Boot_sector::FAT_area_size()
//
// (old) En principio, como voy a dar por supuesto que tengo poca memoria RAM (el
// atmega328p tiene solo 2K y un sector ocupa 512 bytes), no voy a guardar en
// memoria la FAT. Cada vez que queramos acceder a ella, leeré el sector
// correspondiente, leeré el dato buscado, y eliminaré de memoria el sector.
//
// (new) Voy a dejar que sea el cliente el que decida lo que guarde en memoria
// o no via Sector_driver. Esa clase será la que gestionará el número de
// sectores guardados en memoria, pudiendo tener 0 sectores o todo el disco
// duro en RAM (casos extremos). 
//
namespace impl_of{

enum class FAT_Cluster_state : uint8_t
{ free = 0, allocated, bad, end_of_file, reserved, 
    read_error // este no es un estado del cluster sino un intento fallido de
	  // leer un sector de la FAT (realmente no debería ir aqui) (???)
};


}// impl_of
 
template <typename Sector_driver>
class FAT_area{
public:
// Types
    using Volume           = atd::FAT32::Volume<Sector_driver>;
    using Cluster_state    = impl_of::FAT_Cluster_state;
    using size_type        = uint16_t; // de 512 habitualmente < 2^16
				       
// Constructors
    FAT_area(Volume* vol) : volume_{vol} {}
    void init(const Boot_sector& bs);

    FAT_area(const Boot_sector& bs) {init(bs);}

// Creación/modificación de listas
    // Crea una nueva lista de 1 cluster, devolviendo el primer cluster.
    // Devuelve 0 en caso de no poder crear la nueva lista.
    uint32_t new_list();    

    // Borra toda la lista a partir de cluster0
    // remove_list(cluster0);

// Acceso a una lista enlazada
// Cuando pueda voy a usar la notación de std::list.
    // Lee el siguiente cluster a cluster0. Solo cuando state == allocated
    // tendrá sentido el valor de next_cluster.
    Cluster_state next_cluster(const uint32_t& cluster0, uint32_t& next_cluster);

    // Añade el cluster en cluster0. (me gusta más `instert_cluster` pero
    // insert en std::list inserta antes y no después como hago aquí)
    //
    // Ejemplo:
    //	   si la lista es c0 -> c1 -> c2 -> c3 -> c4
    //
    // llamar a add_cluster(c2, d), modifica la lista
    //
    //			 c0 -> c1 -> c2 -> d -> c3 -> c4
    //
    // add_cluster(cluster0, cluster)
    
    // Borra el cluster0.
    // Ejemplo:
    //	   si la lista es c0 -> c1 -> c2 -> c3 -> c4
    //
    // al llamar a remove_cluster(c2) la lista pasa a ser
    //			  c0 -> c1 -> c3 -> c4
    //
    // remove_cluster(cluster0); 



// State
    // Indica si ha habido algún error en la llamada de `read`
    bool read_error() const{ return !volume_->ok();}


// FAT_area Info
// -------------
// Como está clase es de bajo nivel voy a dejar visibles todas estas clases
// para poder depurar
    // Sector inicial donde empieza FAT_area[n]
    uint32_t first_sector(uint8_t n) const 
    {return sector0_ + n * number_of_sectors_;}

    uint32_t number_of_sectors() const {return number_of_sectors_;}
    uint32_t number_of_clusters() const {return number_of_clusters_;}
    uint32_t bytes_per_sector() const {return sector_size_;}

    uint8_t number_of_active_FATs() const {return nFATs_;}


private:
    Volume* volume_;

// FAT_area info
    uint32_t sector0_;	// sector donde empieza la primera FAT_area
    uint8_t nFATs_;	// número de FATs activas
    uint32_t number_of_sectors_; // número total de sectores de 1 FAT_area
    uint32_t number_of_clusters_; // número de clusters de la FAT_area
				 // Es el número de clusters de la data area
    uint32_t sector_size_; // número de bytes por sector

// Cfg de FAT_area


// Tipos de entradas, valores
    static constexpr uint32_t free_entry = 0x00000000;
    static constexpr uint32_t bad_entry = 0x0FFFFFF7;
    static constexpr uint32_t end_of_file_entry = 0x0FFFFFFF;

// Helpers
    // Observar la forma de hablar: la FAT_area es un conjunto de entradas,
    // donde la posición de cada entrada hace referencia al cluster de esa
    // posición.
    // Así, la entrada 10 de la FAT_area contiene:
    //	1. El estado de ese cluster: free, allocated, bad, ...
    //	2. En el caso de que sea allocated indica el valor del siguiente
    //	   cluster.
    Cluster_state cluster_state(uint32_t entry) const;
    std::pair<uint32_t, uint32_t> cluster2sector_pos(uint32_t entry) const;
    uint32_t sector0_area(uint8_t i) const;

    // Devuelve el primer cluster libre después de cluster0,
    // ó 0 en caso de no encontrar ninguno.
    // El valor por defecto es 2 ya que el primer cluster es 2, no 0.
    uint32_t find_first_free_cluster(uint32_t cluster0 = 2) const;


// Funciones de acceso a los clusters
// ----------------------------------
// Estas dos funciones permiten concebir la FAT area como un array de
// clusters. Son las equivalentes a:
//	v = FAT_area[cluster]; <---> v = read(cluster);
//	FAT_area[cluster] = v; <---> write(cluster, v);
//
// Además, write es la función que oculta que hay múltiples FAT areas.
// Observar que estoy usando cluster y entry como sinónimos.

    // Lee el valor del cluster indicado
    uint32_t read(const uint32_t& cluster) const
    { 
	auto [nsector, pos] = cluster2sector_pos(cluster);
	return volume_->template read_global<uint32_t>(sector0_ + nsector, pos);
    }

    // Escribe value en el cluster indicado
    // (TODO) Gestionar los errores. ¿Qué devolver o hacer si write_in_area
    // falla?
    bool write(const uint32_t& cluster, const uint32_t& value) const
    { 
	for (uint8_t i = 0; i < nFATs_; ++i)
	    write_in_area(i, cluster, value);

	return true; // (TODO) Gestionar
    }

    uint8_t write_in_area(uint8_t i
		      , const uint32_t& cluster, const uint32_t& value) const
    {
	auto [nsector, pos] = cluster2sector_pos(cluster);
	return volume_->template 
	write_global<uint32_t>(sector0_area(i) + nsector, pos, value);
    }
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
std::pair<uint32_t, uint32_t> 
	FAT_area<S>::cluster2sector_pos(uint32_t entry) const
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
    number_of_clusters_ = bs.data_area_number_of_clusters();
    sector_size_ = bs.bytes_per_sector();
}

template <typename S>
inline uint32_t FAT_area<S>::sector0_area(uint8_t i) const
{
atd::ctrace<9>() << "sector0_area(" << (uint16_t) i << " < " <<
    (int) nFATs_ << ")\n";
    atd::precondition<9>(i < nFATs_, "Error: FAT area number too big");
    
    return sector0_ + i * number_of_sectors_;
}


// Section 4. FAT specification
// (RRR) Aunque podía usar free_entry, ... opto por dejar los valores
// numéricos porque así puedo ver que no los estoy duplicando y comparar con
// la tabla de la section 4.
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
		and entry <= number_of_clusters_ + 1) 
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

// (DUDA) Se puede optimizar esta función iterando en lugar por el número de
// clusters por (sector, pos) con 
//	sector = [sector0_ ... sector0_ + number_of_sectors_)
//	pos = [0 .. 512)
// Y llamar directamente a volume->read_global. De esa forma evitamos el estar
// llamando continuamente a la función cluster2sector_pos dentro de read.
// Pero como estoy aprendiendo FAT32 opto por una implementación sencilla que
// entienda y no tenga errores, que una más eficiente que pueda tener errores.
template <typename S>
uint32_t FAT_area<S>::find_first_free_cluster(uint32_t cluster) const
{
    for (; cluster < number_of_clusters_; ++cluster){
	auto v = read(cluster);
	if (v == free_entry)
	    return cluster;
    }

    atd::ctrace<2>() << "FAT area full\n";
    return 0;
}


// (DUDA) ¿Añadir state para indicar que ha habido un error de escritura?
template <typename S>
uint32_t FAT_area<S>::new_list()
{
    uint32_t cluster = find_first_free_cluster();
    
    if (cluster == 0) // si no hay más clusters libres
	return 0;

    if (write(cluster, end_of_file_entry) == 0)
	return 0;

    return cluster;
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
    // Primer sector del volumen
    uint32_t first_sector() const {return sector0_;}

    uint32_t bytes_per_sector() const {return sector_size_;}
    uint32_t root_directory_first_cluster();

// Lectura/escritura de sectores
    // Lee el Int (byte) que está en la posición `pos` del sector nsector.
    // Siendo nsector coordenada local, el número de sector referido 
    // al principio del volumen.
    template <Type::Integer Int = uint8_t>
    Int read(const uint32_t& nsector, uint16_t pos)
    { return driver_.template read<Int>(sector0_ + nsector, pos); }
    
    // nsector es coordenada global, 
    // es el número de sector referido al  principio del disco
    template <Type::Integer Int = uint8_t>
    Int read_global(const uint32_t& nsector, uint16_t pos)
    { return driver_.template read<Int>(nsector, pos); }

    template <Type::Integer Int = uint8_t>
    uint8_t write_global( const uint32_t& nsector, uint16_t pos
		     , const uint32_t& value)
    { return driver_.template write<Int>(nsector, pos, value); }

    // Devuelve true si hace el flush correctamente, false en caso de error
    bool flush() { return driver_.flush();}

private:
// Reserved area
    uint32_t sector0_; // volume first sector
    uint32_t sector_size_; // número de bytes por sector
	
// Sectores cargados en memoria
    Sector_driver driver_;

// Helpers
    // Devuelve fs_info
    uint16_t init();

//    template <typename T>
//    T* read_global_as_pointer(const uint32_t& nsector)
//    { return driver_.template lock_sector<T>(nsector);}
};

// precondition: sector0_ valido
template <typename SD>
uint16_t Volume<SD>::init()
{
//    Boot_sector* bs = read_global_as_pointer<Boot_sector>(sector0_);
//    bs = driver_.template lock_sector<T>(nsector);}
//    Boot_sector* bs = 
//	reinterpret_cast<Boot_sector*>(driver_.template lock_sector(nsector)); 

    auto bs = driver_.template lock_sector_and_view_as<Boot_sector>(sector0_);

    if (bs.is_null()){
	atd::ctrace<3>() << "ERROR: can't initialize Volume::init\n";

	sector_size_ = 0; // error
	return 0;
    }

// Reserved area
    sector_size_ = bs->bytes_per_sector();

    fat_area.init(*bs);
    data_area.init(*bs);


    return bs->fs_info;
}


template <typename SD>
Volume<SD>::Volume(const uint32_t& sector0)
    : fat_area{this}, sector0_{sector0}
{
    init();
}

template <typename SD>
Volume<SD>::Volume(const uint32_t& sector0, Boot_sector_min& bs_min)
    : fat_area{this}, sector0_{sector0}
{
//    Boot_sector* bs = init();
//
//    if (bs == nullptr)
//	return;
//
//    bs_min.fs_info = bs->fs_info;
    bs_min.fs_info = init();
}


// De momento opto por leerlo de disco en lugar de memorizarlo. ¿se usará
// mucho?
template <typename SD>
uint32_t Volume<SD>::root_directory_first_cluster() 
{
//    Lock lock{driver_};
//    Boot_sector* bs = read_global_as_pointer<Boot_sector>(sector0_);

    auto bs = driver_.template lock_sector_and_view_as<Boot_sector>(sector0_);
    //if (bs == nullptr){
    if (bs.is_null()){
	atd::ctrace<3>() << "ERROR: can't read boot sector\n";

	return 0; // error
    }
    
    return bs->root_directory_first_cluster();
    
}



/***************************************************************************
 *			  FAT AREA IMPLEMENTATION
 ***************************************************************************/
namespace impl_of{
// Como esto está pensado para micros con poca memoria, basta con ir
// cargando 1 cluster de uno en uno (cada cluster tiene 32 sectores que hay
// que leer, que ocupan 32 x 512 bytes = 16 kB. Los micros que uso tienen
// máximo 6kB, no entrando en memoria)
template <typename Sector_driver>
FAT_area<Sector_driver>::Cluster_state
	FAT_area<Sector_driver>::next_cluster(const uint32_t& cluster,
							uint32_t& next_cluster)
{
    uint32_t entry = read(cluster);

    if (read_error()){
	ctrace<3>() << "Volume read error\n";
	return Cluster_state::read_error;
    }


    auto state = cluster_state(entry);
    if (state != Cluster_state::allocated)
	return state;

    next_cluster = entry;

    return Cluster_state::allocated;
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
enum class File_sectors_state : uint8_t {
    ok
    , error
    , end_of_sectors // no es end of file, ya que, en general, un fichero no
		     // ocupa todos los sectores de un cluster quedando
		     // sectores vacíos. Pero estos sectores pertenecen al
		     // cluster.
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
    using State         = impl_of::File_sectors_state;

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

// State
    bool ok() const {return state_ == State::ok;}
    bool error() const {return state_ == State::error;}
    bool end_of_sectors() const {return state_ == State::end_of_sectors;}

// Para depurar
    // Pasa a apuntar al primer sector del siguiente cluster, en caso de que
    // exista. Si existe devuelve true, en caso contrario false.
    bool next_cluster();    

private:
    uint32_t cluster_; // cluster actual
    uint8_t sector_;  // sector al que apuntamos dentro de cluster_

    State state_;

// Helpers
    
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
    uint32_t next_cluster;
    auto state = volume.fat_area.next_cluster(cluster_, next_cluster);


    if (state == Volume::Cluster_state::end_of_file){
	state_ = State::end_of_sectors;
	return false;
    }

    if (state != Volume::Cluster_state::allocated){
	state_ = State::error;
	return false;
    }

    state_ = State::ok;

    cluster_ = next_cluster;
    sector_  = 0;

    return true;
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
//
//template <typename Sector_driver0> 
//struct const_File_iterator;
//
//struct const_iterator_end{};
//

enum class File_state : uint8_t{
    ok, end_of_file
};
}// impl_of
 

// Se trata de una vista de File_sectors que permite concebirlo como un
// conjunto continuo de bytes.
//
// Hay 2 tipos de ficheros:
//	(1) Los directorios: en ellos la longitud del directorio se determina
//	    por un centinela: al encontrar 0x00 se sabe que se ha llegado al
//	    final del directorio.
//	(2) Los ficheros: su tamaño viene almacenado en la entry
//	    correspondiente del directorio.
//
//  ¿Cómo mezclar los dos tipos? Podía usar safe_Uniniatialized<uint32_t> size;
//  pero como quiero poder hacer un reset necesito recordar el tamaño del
//  fichero. Por eso uso dos sizes:
//	    Uniniatialized size_: tamaño del fichero (si está inicializado)
//	    uint32_t remainder_bytes_; numero de bytes que quedan del fichero
//	                               por leer.
//  Esto, supongo (?) que, lo tendré que modificar cuando quiera escribir.
//  Pero estoy experimentando.
//
template <typename Sector_driver0> 
class File{
public:
// Types
    using File_sectors = atd::FAT32::File_sectors<Sector_driver0>;
    using Volume = typename File_sectors::Volume;
    using State  = impl_of::File_state;

//    using const_iterator     = impl_of::const_File_iterator<Sector_driver0>;
//    using const_iterator_end = impl_of::const_iterator_end;
//
//    friend const_iterator;

// Constructors
    File(Volume& volume, const uint32_t& cluster0);
    File(Volume& volume, const uint32_t& cluster0, const uint32_t& size_in_bytes);

    
// Iteramos usando esta clase
    // Reinicia el fichero, haciendo que apunte al principio de nuevo
    void reset();

    // Reinicia el fichero, enlazándolo a un nuevo cluster0
    void reset(uint32_t cluster0);

    // Intenta leer buf.size() caracteres, almacenándolos en buf.
    // Devuelve el número de caracteres leídos.
    uint8_t read(std::span<uint8_t> buf);

// Acceso aleatorio
// uint8_t operator[](size_type i); // muy costoso en tiempo

// Iteradores
//    iterator begin();
//    iterator end();

//    const_iterator begin();
//    const_iterator_end end() const;

// State
    bool ok() const {return sector_.ok(); }
    bool error() const {return sector_.error(); }
    bool end_of_file() const {return state_ == State::end_of_file;}

// Info
    Volume& volume() {return sector_.volume;}


private:
    // (TODO) Sospecho que sobra un Uninitialized!!! El de remainder_bytes_ lo
    // quiero para no tener que pensar si está inicializado o no.
    uint32_t cluster0_; // primer cluster del fichero
    atd::Uninitialized<uint32_t> size_; // si está inicializado es el número
					 // de bytes del fichero, en caso
					 // contrario quiere decir que es
					 // directorio

    File_sectors sector_; // lista enlazada con los sectores del fichero
    uint16_t i; // byte actual al que apuntamos dentro del sector
    atd::safe_Uninitialized<uint32_t> remainder_bytes_; // numero de bytes que quedan por leer

    State state_;

// Funciones de implementacion 
// next_byte la necesito en const_iterator, pero... quiero implementarlo así
// de verdad? Mejor como un wrapper sobre esta clase.
//    static uint16_t next_byte(File_sectors& sector, uint16_t i);

    uint8_t read_byte(uint32_t i) const
    { return sector_.volume.template read_global<uint8_t>
				    (sector_.global_sector_number(), i); }
};


template <typename SD> 
inline File<SD>::File(Volume& volume, const uint32_t& cluster0)
    : cluster0_{cluster0}, sector_(volume, cluster0)
    , i{0}
{ }

template <typename SD> 
inline File<SD>::File(Volume& volume
		    , const uint32_t& cluster0
		    , const uint32_t& size_in_bytes)
    : cluster0_{cluster0}, size_{size_in_bytes}
    , sector_(volume, cluster0), i{0}, remainder_bytes_{size_in_bytes}
    
{ 
    state_ = State::ok;
}

template <typename SD> 
inline void File<SD>::reset()
{
    sector_.first_sector(cluster0_);
    i = 0;
    if (size_.is_initialized())
	remainder_bytes_ = size_;

    state_ = State::ok;
}

template <typename SD> 
inline void File<SD>::reset(uint32_t cluster0)
{
    cluster0_ = cluster0;
    reset();
}


// (RRR) ¿por qué leo de 1 byte en 1 byte?
//       Porque los bytes de un fichero están dispersos por diferentes
//       sectores. En principio el Sector_driver carga en memoria un sector,
//       con lo que no debería de ser ineficiente leer 1 byte detrás de otro.
template <typename SD> 
inline uint8_t File<SD>::read(std::span<uint8_t> buf)
{
    using size = std::span<uint8_t>::size_type;

    if (sector_.end_of_sectors() or sector_.error()
	or remainder_bytes_.value() == 0)
	return 0;

    size n = 0; 
    for (; n < buf.size(); ++n){

	buf[n] = read_byte(i); 

	--remainder_bytes_; 
	if (remainder_bytes_.value() == 0){
	    state_ = State::end_of_file;
	    return n + 1u;
	}

	++i;

	if (i >= sector_.volume.bytes_per_sector()){
	    sector_.next_sector(); 

	    if (sector_.end_of_sectors() or sector_.error())
		return n + 1u;

	    i = 0;
	}
    }

    return n;
}




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
    last_entry, // == 0x00, no hay más entries a partir de esta
    short_entry,    
    long_entry
};


struct Directory_entry{
// Types
    using Attribute = Entry_attribute_type;
    using Type = Entry_type;

// Cfg
    static constexpr uint8_t size = 32;

    // número de caracteres ASCII que admite un long_name
    static constexpr uint8_t ascii_short_name_len= 11;
    static constexpr uint8_t ascii_long_name_len = 13;

// Data
    std::array<uint8_t, size> data;

// Tipo de entrada
    Type type() const;

// short entry (section 6, FAT specification)
    // devuelve el número de caracteres copiados
    uint8_t copy_short_name(std::span<uint8_t> str) const;

    Attribute attribute() const { return Attribute{data[11]}; }
    // data[12] == 0
    
    uint8_t creation_time_tenth_of_seconds() const {return data[12];}
    uint16_t creation_time() const 
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
    // Devuelve el orden de la entrada con la mask 0x40
    uint8_t extended_order_with_mask() const {return data[0];}

    // El order de la última entrada de un long name se masquea con 0x40
    // (jejeje, yo masqueo, tu masqueas, él masquea xD. Masquear `a` es `a|0x40`)
    // Mejor: el bit número 6 order de la última entrada de un long name es 1.
    // Devuelve el orden de la entrada
    uint8_t extended_order() const {return data[0] & ~0x40; }
    bool is_last_member_of_long_name() const {return data[0] & 0x40; }

    // data[1..11) = long name
    // data[11] = attribute. En este caso ATTR_LONG_NAME
    // data[12] == 0
    uint8_t check_sum() const { return data[13]; }
    // data[14..26) = long name
    // data[26] == 0 and data[27] == 0
    // data[28..32) = long name
    // devuelve el número de caracteres copiados
    uint8_t copy_long_name(std::span<uint8_t> str);


// Funciones de conversión de formato
// (TODO) devolver time_t...
    // Extrae day/month/year de `date`, que será lo que devuelva
    // creation_date, last_modification_date ...
    static void uint16_t2date(uint16_t date, 
			      uint8_t& day, uint8_t& month, uint16_t& year);

    static void uint16_t2time(uint16_t time, 
			      uint8_t& seconds, uint8_t& minutes, uint8_t& hours);

// Funciones internas
    // Como data[12] tiene que ser 0, usemos ese valor para indicar que la
    // entry es invalida
    void null_entry() { data[12] = 1; }
    bool is_null_entry() const {return data[12] == 1;}

private:
    uint8_t copy_long_name_impl(std::span<uint8_t> str);

};


// Info básica que necesita un usuario de una entrada.
// Solo le falta el nombre que al ser de longitud variable dejo que sea el
// cliente quien decida cuánta memoria reservar para él.
struct Entry_info{
    using Attribute = Entry_attribute_type;

    Attribute attribute;

    uint32_t file_cluster;
    uint32_t file_size;

    uint16_t creation_date;
    uint16_t creation_time;
    uint8_t  creation_time_tenth_of_seconds;

    uint16_t last_access_date;

    uint16_t last_modification_date;
    uint16_t last_modification_time;

};

// Copia entry en info: entry -> info;
void copy(const Directory_entry& entry, Entry_info& info);


template <typename Sector_driver0>
struct const_Entry_iterator;

enum class Directory_state{
    ok, read_error, last_entry, long_entry_corrupted
};


}// impl_of
 
template <typename Sector_driver0> 
class Directory{
public:
// Types
    using Volume    = atd::FAT32::Volume<Sector_driver0>;
    using File	    = atd::FAT32::File<Sector_driver0>;
    using Entry	    = impl_of::Directory_entry;
    using Type      = Entry::Type;
    using Entry_info= impl_of::Entry_info;
    using Attribute = impl_of::Directory_entry::Attribute;
    using State     = impl_of::Directory_state;

// Constructors
    // Enlaza con el directorio que se encuentra en el cluster0
    Directory(Volume& volume, uint32_t cluster0);

// ls (listado del directorio)
    // Reinicia la lectura. La siguiente llamada a `read_xxx` devolverá la
    // primera entrada que encuentre.
    void first_entry();
//    void next_entry(); // read consume la entry, no se necesita next.
    
    // Lee la entry actual almacenándola en entry
    // Consume esa entry, esto es, pasa a apuntar a la siguiente entry.
    // Devuelve true si lee la siguiente entry, false si no puede hacerlo.
    // En caso de error mirar state.
    bool read(Entry& entry);

    // Lee la siguiente entrada que haya devolviendo la información básica en
    // info, y el nombre del fichero en long_name. Si el nombre del fichero no
    // entra en long_name lo trunca.
    // Devuelve true si lee una entrada, false en caso de no poder hacerlo.
    // Devuelve false si:
    //	    1. Llega a la última entrada, no pudiendo leer más.
    //	    2. Algún error.
    // Por ello mirar el state después de llamar a read_long_entry.
    bool read_long_entry(Entry_info& info, 
			 std::span<uint8_t> long_name);

    // Devuelve la siguiente entrada correspondiente al attribute att.
    // De esta forma se puede hacer un ls de solo los ficheros, o solo los
    // directorios.
    bool read_long_entry(Entry_info& info, 
			 std::span<uint8_t> long_name, 
			 Attribute att);

// cd (cambio de directorio)
    // 2 posibles formas de hacer cd:
    //	    cd("directory_name") vs cd(cluster_number)
    // La primera es muy ineficiente ya que habría que recorrer todo el
    // directorio actual hasta encontrar el cluster correspondiente.
    void cd(uint32_t cluster0);


// State
// (DUDA) File tiene un state, y esta clase otro: uso un uint16_t (2 x
// uint8_t) para el estado lo cual es desperdiciar un byte. ¿Se podría
// fusionar en 1 byte los 2 states? Si la solución no es sencilla ¿merece la
// pena complicar todo el código por ahorrar 1 byte? 
// (TODO) Observar que hay varios tipos de estado diferentes:
//	(1) El estado del file interno (ok, error de lectura)
//	(2) El estado del array de entries (last_entry)
//	(3) El resultado de la última operación realizada
//	    (long_entry_corrupted es el resultado erróneo de la función
//	    read_long_entry)
//	¡Aclarar esto!
    bool ok() const {return dir_.ok() and state_ == State::ok; }
    bool read_error() const {return state_ == State::read_error; }
    bool last_entry() const {return state_ == State::last_entry; }
    bool long_entry_corrupted() const {return state_ == State::long_entry_corrupted; }
    bool error() const {return dir_.error() 
			    or state_ == State::read_error; }

private:
    File dir_; // fichero con los sectores del directorio
    State state_;
    
    static constexpr uint8_t sizeof_entry = Entry::size;

    // Este state no lo necesito fuera
    bool end_of_file() const {return dir_.end_of_file();}


// Helpers
    bool read_long_entry(Entry& entry, Entry_info& info,
						    std::span<uint8_t> name);
    // Lo llamo copy porque realmente no lee ninguna entrada, se limita a
    // copiar la información de entry en [info, name]
    void copy_short_entry(const Entry& entry, Entry_info& info, 
					std::span<uint8_t> name);

};

template <typename S> 
inline 
Directory<S>::Directory(Volume& volume, uint32_t cluster0)
    : dir_{volume, cluster0}, state_{State::ok}
{ }

template <typename S> 
inline void Directory<S>::first_entry()
{ 
    dir_.reset(); 
    state_ = State::ok;
}

// Esta función es idéntica a first_entry()
template <typename S>
void Directory<S>::cd(uint32_t cluster0)
{
    dir_.reset(cluster0);
    state_ = State::ok;
    
}


template <typename S> 
bool Directory<S>::read(Entry& entry)
{
    if (dir_.end_of_file() or dir_.error()){
	atd::ctrace<5>() << "Trying to read EOC/error file\n";
	state_ = State::read_error;
	return false;
    }

    if (dir_.read(entry.data) != entry.data.size()){
	atd::ctrace<5>() << "Can't read Entry\n";
	state_ = State::read_error;
	return false;
    }

    state_ = State::ok;
    return true;
}



// precondition: entry es la last_entry de un long_name
// entry: es de entrada/salida. Se modifica dejando la última entrada leida
//	 (in) contiene la last_entry de un long_name
//	 (output) la short_entry del long_name
template <typename S>
bool Directory<S>::read_long_entry(Entry& entry,
					Entry_info& info, 
					std::span<uint8_t> long_name)
{
    static constexpr uint8_t size = Entry::ascii_long_name_len;

// Garantizamos que la cadena acabe en '\0' en el caso de que sea el long_name
// justo múltiplo de size (de 13 caracteres)
    uint8_t nentry = entry.extended_order();

    if (nentry * size < long_name.size())
	long_name[nentry * size] = '\0';

// Copiamos el name en long_name
    for (; nentry > 0; --nentry){
	uint8_t i = nentry - 1;

	if ((entry.type() != Type::long_entry) 
	    or (entry.extended_order() != nentry)){ // ¿están ordenadas?
	    state_ = State::long_entry_corrupted;
	    return false;
	}

	if (i * size < long_name.size()){// copiamos nombre si podemos
	    uint8_t n = std::min<uint8_t>(size, 
					long_name.size() - i * size);
	    entry.copy_long_name({long_name.begin() + i * size, n});
	}

	read(entry);
	if (read_error())
	    return false;
	
    }

// Después de las long_name entries siempre viene una short_entry
    if (entry.type() != Type::short_entry){
	state_ = State::long_entry_corrupted;
	return false;
    }

    impl_of::copy(entry, info);

    return true;
}





template <typename S>
void Directory<S>::copy_short_entry(const Entry& entry,
					Entry_info& info, 
					std::span<uint8_t> name)
{
    impl_of::copy(entry, info);
    entry.copy_short_name(name);

}

template <typename S>
bool Directory<S>::read_long_entry(Entry_info& info, 
					std::span<uint8_t> long_name)
{
    using Type = Entry::Type;

    Entry entry;

// find_first_not_free_entry:
    do{
	read(entry);

	if (read_error()) // este nombre es un poco confuso @_@
	    return false; // el state lo marcar read
    }while (entry.type() == Type::free);

    if (entry.type() == Type::last_entry){
	state_ = State::last_entry;
	return false;
    }

    if (entry.type() == Type::short_entry){
	copy_short_entry(entry, info, long_name); // copia entry en info/long_name
    }

    else // == Type::long_entry
	return read_long_entry(entry, info, long_name); // entry es la last_entry
					        // entrada del long_name

    return true;
}



template <typename S>
bool Directory<S>::read_long_entry(Entry_info& info, 
					std::span<uint8_t> long_name,
					Attribute att)
{
    while (read_long_entry(info, long_name) == true){
	if (info.attribute == att)
	    return true;
    }

    return false;
}




}// namespace FAT32

}// namespace dev
 

#endif


