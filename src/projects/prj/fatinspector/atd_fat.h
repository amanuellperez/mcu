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
#include <atd_basic_types.h>	// Uniniatialized	


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
 *				FAT32
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



// FAT_area hace referencia a Volume
template <typename S>
class Volume;

namespace impl_of{
/***************************************************************************
 *				FAT AREA
 * -------------------------------------------------------------------------
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
{ free = 0, allocated, bad, end_of_clusters, reserved, 
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

// Creación/modificación de listas
    // Crea una nueva lista de 1 cluster, devolviendo el primer cluster.
    // Devuelve 0 en caso de no poder crear la nueva lista.
    uint32_t new_list();    

    // Borra toda la lista a partir de cluster0
    // Precondicion: cluster0 es el primer elemento de una lista.
    // Si no fuera el primer elemento dejaría inconsistente la lista.
    // Ejemplo:
    //	Si la lista es 
    //	    c0 -> c1 -> c2 -> c3 -> EOF
    //
    //	y llamamos remove_list(c2) el resultado será
    //	    c0 -> c1 -> FREE
    //
    //	Observar que c1 no apunta a EOF sino a FREE.
    //
    //	Devuelve true si todo va bien, false en caso de error.
    bool remove_list(uint32_t cluster0);

// Acceso a una lista enlazada
    // Lee el siguiente cluster a cluster0. Solo cuando state == allocated
    // tendrá sentido el valor de next_cluster.
    Cluster_state next_cluster(const uint32_t& cluster0, uint32_t& next_cluster);

    // Añade el cluster en cluster0. (me gusta más `instert_cluster` pero
    // insert en std::list inserta antes y no después como hago aquí)
    //
    // Ejemplo:
    //	   si la lista es c0 -> c1 -> c2 -> c3 -> c4
    //
    // llamar a add_cluster(c2), modifica la lista
    //
    //			 c0 -> c1 -> c2 -> d -> c3 -> c4
    //
    // Devuelve el cluster nuevo 'd' al que apunta c2.
    // En caso  de error devuelve 0.
    uint32_t add_cluster(const uint32_t& cluster0);

    // Igual que add_cluster con la única diferencia de que solo añade el
    // cluster si cluster0 es el último elemento de la lista. Si no lo es, no
    // hace nada devolviendo 0.
    uint32_t push_back_cluster(const uint32_t& cluster0);


    // Borra el cluster0.
    // Ejemplo:
    //	   si la lista es c0 -> c1 -> c2 -> c3 -> c4
    //
    // al llamar a remove_cluster(c2) la lista pasa a ser
    //			  c0 -> c1 -> c3 -> c4
    //
    // Devuelve el cluster al que apunta `cluster` (c3 en el ejemplo)
    // En caso  de error devuelve 0.
    //
    // Observar que no estamos borrando el cluster0 sino el cluster al que
    // apunta cluster0, de ahí el nombre 'next'. (borrar el cluster0 es muy
    // ineficiente ya que, al ser una lista enlazada, habría que buscar el
    // elemento anterior a cluster0 lo cual es costoso en tiempo).
    uint32_t remove_next_cluster(const uint32_t& cluster0); 




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

    // (TODO) sector_size_ está repetido aquí y en Volume. No almacenarlo 2
    // veces!!!
    uint32_t sector_size_; // número de bytes por sector

// Cfg de FAT_area


// Tipos de entradas, valores
    static constexpr uint32_t free_entry = 0x00000000;
    static constexpr uint32_t bad_entry = 0x0FFFFFF7;
    static constexpr uint32_t end_of_file_entry = 0x0FFFFFFF;

    static bool is_allocated(const uint32_t& cluster)
    { return	    cluster != free_entry 
		and cluster != bad_entry
		and cluster != end_of_file_entry; }

// Helpers
    // Observar la forma de hablar: la FAT_area es un conjunto de clusters,
    // donde la posición de cada entrada hace referencia al cluster de esa
    // posición.
    // Así, la entrada 10 de la FAT_area contiene:
    //	1. El estado de ese cluster: free, allocated, bad, ...
    //	2. En el caso de que sea allocated indica el valor del siguiente
    //	   cluster.
    Cluster_state cluster_state(uint32_t cluster) const;
    std::pair<uint32_t, uint32_t> 
			    cluster2sector_pos(const uint32_t& cluster) const;
    uint32_t sector0_area(uint8_t i) const;

    // Devuelve el primer cluster libre después de cluster0,
    // ó 0 en caso de no encontrar ninguno.
    // El valor por defecto es 2 ya que el primer cluster es 2, no 0.
    uint32_t find_first_free_cluster(uint32_t cluster0 = 2) const;

    uint32_t add_cluster(const uint32_t& cluster, bool only_at_the_end);

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

    bool read_error() const {return volume_->read_error(); }

    // Escribe value en el cluster indicado
    // (TODO) Gestionar los errores. ¿Qué devolver o hacer si write_in_area
    // falla?
    //
    // COMENTARIOS
    // Todas las funciones new_list, remove_list, add_cluster, ...
    // tienen que leer de FAT0 y luego escribir en FAT0, FAT1, leer en FAT0,
    // escribir en FAT0, FAT1, ... Si se activan las trazas a nivel 100 se ve
    // muy bien los accesos a SDCard::read/write. ¡Accede demasiado! Pero ese
    // es el precio a pagar por solo mantener un sector en memoria. Se podía
    // hacer más eficiente si Sector_driver mantuviese 2 sectores en memoria,
    // de esa forma se ahorrarían muchos SDCard::read. Pero eso necesita más
    // memoria de RAM. Lo bueno del planteamiento es que es el cliente, a
    // través de la clase Sector_driver, el que decide esto.
    // Con todo ¿se podrá implementar de otra forma más eficiente para
    // Sector_driver que solo mantentan 1 sector en memoria?
    bool write(const uint32_t& cluster, const uint32_t& value) const
    { 
	for (uint8_t i = 0; i < nFATs_; ++i){
	    if (write_in_area(i, cluster, value) == 0)
		return false;
	}

	return true; 
    }

    
    // Devuelve el número de `value` escritos: 
    // 0 si hay error, 1 si todo va bien.
    uint8_t write_in_area(uint8_t i
		      , const uint32_t& cluster, const uint32_t& value) const
    {
	auto [nsector, pos] = cluster2sector_pos(cluster);
	return 
	    volume_->template 
		write_global<uint32_t>(sector0_area(i) + nsector, pos, value);
    }
};


// FAT_area es un array de uint32_t implementado como sectores de 512 bytes.
// Lo concebimos de 2 formas: como un array lineal de uint32_t, o como un
// array bidimensional de 512 uint8_t cada fila.
//
// Tenemos dos sistemas de coordenadas: un índice líneal que es el número de
// cluster0, y un par (nsector, pos) que nos dice en qué sector está la entrada y
// en que posición dentro de ese sector. La forma de relacionarlos es la
// habitual:
//		cluster0 = nsector * 128 + pos
//
// A partir de esta fórmula calculamos nsector y pos.
// (el 128 = 512 / 4; yo no voy a complicar la cosa como hace la FAT_area
// specification y calcular la posición del uint8_t y luega hacer el casting
// ya que concibo la FAT_area como un array lineal de uint32_t)
// 
// tanto cluster0, como (nsector, pos) son coordenadas locales a la FAT area.
template <typename S>
inline 
std::pair<uint32_t, uint32_t> 
	FAT_area<S>::cluster2sector_pos(const uint32_t& cluster) const
{ return atd::div(cluster, sector_size_ / sizeof(uint32_t)); }



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
    atd::precondition<9>(i < nFATs_, "Error: FAT area number too big");
    
    return sector0_ + i * number_of_sectors_;
}


// Section 4. FAT specification
// (RRR) Aunque podía usar free_entry, ... opto por dejar los valores
// numéricos porque así puedo ver que no los estoy duplicando y comparar con
// la tabla de la section 4.
template <typename S>
FAT_area<S>::Cluster_state FAT_area<S>::cluster_state(uint32_t cluster) const
{
    // Section 4.  Note: "The high 4 bits of a FAT32 FAT cluster are reserved"
    cluster &= 0x0FFFFFFF;  

    if (cluster == 0x00000000) return Cluster_state::free;
    if (cluster == 0x0FFFFFF7) return Cluster_state::bad;
    if (cluster == 0x0FFFFFFF) return Cluster_state::end_of_clusters;
    
    // clusters 0 y 1 están reservados
    // section 3.5
    if (0x00000002 <= cluster 
		and cluster <= number_of_clusters_ + 1) 
	return Cluster_state::allocated;
	

    // DUDA: La FAT specification indica que 
    // "Reserved and should not be used.
    //  May be interpreted as an allocated cluster and the final
    //  cluster in the file (indicating end-of-file condition)"
    //  ¿son reserved or end_of_file? @_@
    if (0x0FFFFFF8 <= cluster and cluster <= 0x0FFFFFFE)
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
	
	if (read_error())
	    return 0;

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


// Borra toda la lista de clusters c0 -> ...
template <typename S>
bool FAT_area<S>::remove_list(uint32_t cluster0)
{
    while (is_allocated(cluster0)){

	auto cluster1 = read(cluster0); 
					
	if (read_error())
	    return false;
					
	if (write(cluster0, free_entry) == 0)
	    return false;

	cluster0 = cluster1;
    }

    return true;
    
}


// Convertimos:
//	c0 -> c1
//
//  en
//	c0 -> d -> c1
//
//  devolviendo d.
//
//  Si only_at_the_end == true solo añade el cluster en caso de que cluster0
//  sea el último cluster de la lista.
template <typename S>
uint32_t FAT_area<S>::add_cluster(const uint32_t& cluster0
		      , bool only_at_the_end)
{
    auto cluster1 = read(cluster0);
		
    if (read_error())
	return 0;

    if (only_at_the_end and cluster1 != end_of_file_entry)
	return 0;

    uint32_t d = find_first_free_cluster();

    if (d == 0) // si no hay más clusters libres
	return 0;

    if (write(d, cluster1) == 0){
	return 0;
    }

    if (write(cluster0, d) == 0)
	return 0;

    return d;
}

template <typename S>
inline uint32_t FAT_area<S>::add_cluster(const uint32_t& cluster0)
{ return add_cluster(cluster0, false); }

template <typename S>
inline uint32_t FAT_area<S>::push_back_cluster(const uint32_t& cluster0)
{ return add_cluster(cluster0, true); }


// Convertimos:
//	c0 -> c1 -> c2
// en
//	c0 -> c2
//
template <typename S>
uint32_t FAT_area<S>::remove_next_cluster(const uint32_t& cluster0)
{
    auto cluster1 = read(cluster0);

    if (read_error())
	return 0;

    if (!is_allocated(cluster1))
	return 0;

    auto cluster2 = read(cluster1);
				    
    if (read_error())
	return 0;

				    
    if (write(cluster1, free_entry) == 0)
	return 0;

    if (write(cluster0, cluster2) == 0)
	return 0;

    return cluster2;
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

    // Devuelve el primer sector del `cluster` indicado. 
    // El sector devuelto es global (coordenadas de SDCard)
    uint32_t first_sector_of_cluster(uint32_t cluster) const
    {return  first_sector() + (cluster - 2) * sectors_per_cluster();}

//    uint32_t sector_number(uint32_t cluster, uint8_t sector) const
//    { return first_sector_of_cluster(cluster) + sector; }



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

// Reserved area
    // Primer sector del volumen
    uint32_t first_sector() const {return sector0_;}

    uint32_t bytes_per_sector() const {return sector_size_;}
    uint32_t root_directory_first_cluster();

// Lectura/escritura de sectores 
    // (DUDA) Esto no es más que llamar a las funciones de Sector_driver.
    //        ¿Merce la pena?
    //
    // Lee el Int (byte) que está en la posición `pos` del sector nsector.
    // Siendo nsector coordenada local, el número de sector referido 
    // al principio del volumen.
//    template <Type::Integer Int = uint8_t>
//    Int read(const uint32_t& nsector, uint16_t pos)
//    { return driver_.template read<Int>(sector0_ + nsector, pos); }
    
    // nsector es coordenada global, 
    // es el número de sector referido al  principio del disco
    // Para ver si falla mirar el state correspondiente
    template <Type::Integer Int = uint8_t>
    Int read_global(const uint32_t& nsector, uint16_t pos)
    { return driver_.template read<Int>(nsector, pos); }

    uint32_t read_global(const uint32_t& nsector, const uint32_t& pos
		  , std::span<uint8_t> buf)
    {return driver_.read(nsector, pos, buf); }

    // Devuelve 1 si todo va bien, 0 si no consigue escribir.
    // Si falla mirar el state correspondiente para ver error (write_error())
    template <Type::Integer Int = uint8_t>
    uint8_t write_global( const uint32_t& nsector, uint16_t pos
		     , const uint32_t& value)
    { return driver_.template write<Int>(nsector, pos, value); }

    // Devuelve true si hace el flush correctamente, false en caso de error
    bool flush() { return driver_.flush();}

// Algorithms
    // Rellena todo el cluster con el valor indicado.
    // Devuelve true si todo va bien.
    bool fill_cluster(const uint32_t& cluster, uint8_t value);


// Errno
    bool last_operation_ok() const 
    { return driver_.last_operation_ok() == true and sector_size_ != 0;}

    bool last_operation_fail() const { return !last_operation_ok(); }

    bool read_error() const { return driver_.read_error(); }
    bool write_error() const { return driver_.write_error(); }

private:
// Reserved area
    uint32_t sector0_; // volume first sector
    uint32_t sector_size_; // número de bytes por sector
	
// Sectores cargados en memoria
    Sector_driver driver_;

// Helpers
    // Devuelve fs_info
    uint16_t init();

};

// precondition: sector0_ valido
template <typename SD>
uint16_t Volume<SD>::init()
{
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
inline 
Volume<SD>::Volume(const uint32_t& sector0)
    : fat_area{this}, sector0_{sector0}
{ init(); }

template <typename SD>
inline
Volume<SD>::Volume(const uint32_t& sector0, Boot_sector_min& bs_min)
    : fat_area{this}, sector0_{sector0}
{ bs_min.fs_info = init(); }


// De momento opto por leerlo de disco en lugar de memorizarlo. ¿se usará
// mucho?
template <typename SD>
uint32_t Volume<SD>::root_directory_first_cluster() 
{
    auto bs = driver_.template lock_sector_and_view_as<Boot_sector>(sector0_);
    if (bs.is_null()){
	atd::ctrace<3>() << "ERROR: can't read boot sector\n";

	return 0; // error
    }
    
    return bs->root_directory_first_cluster();
    
}

template <typename SD>
inline 
bool Volume<SD>::fill_cluster(const uint32_t& cluster, uint8_t value)
{
    auto sector0 = data_area.first_sector_of_cluster(cluster);

    if (driver_.fill_n(sector0, data_area.sectors_per_cluster(), value)
		!= data_area.sectors_per_cluster())
	return false;

    return true;
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
 *			FAT AREA LIST (of clusters)
 * -------------------------------------------------------------------------
 *
 * Un FAT32::FAT_area_list es una lista de clusters concreta de la FAT area.
 * La podemos ver de dos formas:
 *
 *  1) como una lista enlazada de S sectores.
 *  2) como una lista enlazada de C clusters cada uno de ellos con un número
 *     determinado de sectores.
 *
 ***************************************************************************/
namespace impl_of{
// Resultado de la última operacion
enum class FAT_area_list_errno: uint8_t {
    ok = 0  // fail == distinto de 0
    , next_cluster_no_allocated = 1
    , end_of_sectors		= 2
};

// ¿En qué estado se puede encontrar la lista enlazada de los sectores?
enum class FAT_area_list_state : uint8_t {
    ok             = 0,	// apunta a un sector válido
    uninitialized  = 1, // no se ha inicializado
    end_of_sectors = 2  // se ha llegado al final de los sectores
};


struct FAT_area_list_ertate {
    using Errno = FAT_area_list_errno;
    using State = FAT_area_list_state;

    State state : 4;
    Errno errno : 4;

    // Añado este constructor para evitar warnings del compilador @_@
    FAT_area_list_ertate() :state{}, errno{}{} 

    void operator=(State st) { state = st; }
    void operator=(Errno e)  { errno = e; }

    bool operator==(State st) const { return state == st; }
    bool operator!=(State st) const { return state != st; }
    
    bool operator==(Errno e) const { return errno == e; }
    bool operator!=(Errno e) const { return errno != e; }

};

static_assert(sizeof(FAT_area_list_state) == 1);

}// impl_of
 

template <typename Sector_driver0> 
class FAT_area_list{
public:
// Types
    using Volume	= atd::FAT32::Volume<Sector_driver0>;
    using Sector_driver = Sector_driver0;
    using Sector	= typename Volume::Sector;
    using Sector_span   = typename Volume::Sector_span;
    using State         = impl_of::FAT_area_list_ertate::State;
    using Errno         = impl_of::FAT_area_list_ertate::Errno;

// Data
    Volume& volume;

// Constructors
    FAT_area_list(Volume& volume); // no olvidar llamar a first_sector!!!
    FAT_area_list(Volume& volume, uint32_t cluster0);

    // Reiniciamos el FAT_area_list, haciendo que apunte al primer sector del
    // cluster0
    void first_sector(uint32_t cluster0);

// Operations
    // Pasa al siguiente cluster de la lista enlazada.
    // Devuelve true si lo obtiene, false si falla. 
    bool next_sector();

    // Añade un nuevo cluster solo si el cluster actual es el último de la
    // lista de clusters.
    // Devuelve el nuevo cluster añadido ó 0 en caso de error.
    uint32_t push_back_cluster();

    // Añade un nuevo cluster y llena todo el cluster con el valor value.
    // Necesario para los directorios: los cluster hay que inicializarlos a 0.
    // Devuelve el nuevo cluster añadido ó 0 en caso de error.
    uint32_t push_back_cluster_fill_with(uint8_t value);

// read/write
    // Devuelve el número del sector global actual 
    uint32_t global_sector_number() const;

// Errno = resultado de la última operación
    bool last_operation_ok() const {return errno() == Errno::ok; }
    bool last_operation_fail() const {return errno() != Errno::ok; }

    bool next_cluster_no_allocated() const 
    {return errno() == Errno::next_cluster_no_allocated;}

    Errno errno() const {return errno_state_.errno;}

// State
    State state() const {return errno_state_.state;}

    bool uninitialized() const { return state() == State::uninitialized; }
    bool end_of_sectors() const {return state() == State::end_of_sectors;}


// Para depurar
    // Pasa a apuntar al primer sector del siguiente cluster, en caso de que
    // exista. Si existe devuelve true, en caso contrario false.
    bool next_cluster();    

    // Devuelve el número de cluster actual al que apunta la lista
    uint32_t ncluster() const {return cluster_;}
    
private:
    // Un sector dentro de una lista enlazada de clusters viene definido por
    // el par (cluster, sector)
    uint32_t cluster_; // cluster actual
    uint8_t sector_;   // sector al que apuntamos dentro de cluster_

    impl_of::FAT_area_list_ertate errno_state_;

    void state(State st) {errno_state_.state = st; }

    bool ok() {errno_state_.errno = Errno::ok; return true; }
    bool errno(Errno e) {errno_state_.errno = e; return false;}
    
};


template<typename SD>
inline FAT_area_list<SD>::FAT_area_list(Volume& vol)
    : volume{vol}, cluster_{0}, sector_{0} // inicializo a 0 para detectar errores
{ state(State::uninitialized); } 

template<typename SD>
inline FAT_area_list<SD>::FAT_area_list(Volume& vol,  uint32_t cluster0)
    : volume{vol}
{ first_sector(cluster0); }

template<typename SD>
inline void FAT_area_list<SD>::first_sector(uint32_t cluster0)
{ 
    cluster_ = cluster0;
    sector_  = 0;
    state(State::ok);
}



// Precondition: cluster_.sector_ >= vol_.data_area.first_sector();
template<typename SD>
inline uint32_t FAT_area_list<SD>::global_sector_number() const
{ return volume.data_area.first_sector_of_cluster(cluster_) + sector_; }


template<typename SD>
bool FAT_area_list<SD>::next_sector()
{
    if (end_of_sectors())
	return false;

    ++sector_;

    if (sector_ < volume.data_area.sectors_per_cluster()){
	state(State::ok);
	return ok();
    }

    return next_cluster();
}


// Postcondition:
//  if(FAT_area_list.end_of_sectors()) 
//	==> sector_ > data_area.sectors_per_cluster();
template<typename SD>
bool FAT_area_list<SD>::next_cluster()
{
    uint32_t next_cluster;
    auto cluster_state = volume.fat_area.next_cluster(cluster_, next_cluster);

    if (cluster_state == Volume::Cluster_state::end_of_clusters){
	state(State::end_of_sectors);
	return errno(Errno::end_of_sectors);
    }

    if (cluster_state != Volume::Cluster_state::allocated)
	return errno(Errno::next_cluster_no_allocated);

    state(State::ok);

    cluster_ = next_cluster;
    sector_  = 0;

    return ok();
}


template<typename SD>
uint32_t FAT_area_list<SD>::push_back_cluster()
{
    uint32_t new_cluster = volume.fat_area.push_back_cluster(cluster_);

    if (new_cluster == 0)
	return 0;

    if (end_of_sectors()){
	state(State::ok);
	sector_ = volume.data_area.sectors_per_cluster(); // no necesario,
				// pero me da garantias de que funcione bien
	next_sector();
    }

    return new_cluster;
}

template<typename SD>
uint32_t FAT_area_list<SD>::push_back_cluster_fill_with(uint8_t value)
{
    auto new_cluster = push_back_cluster();

    if (new_cluster == 0)
	return 0;

atd::ctrace<9>() << "new_cluster = " << new_cluster << "(" << global_sector_number()
    << ")\n";
    if (!volume.fill_cluster(new_cluster, value)){
	atd::ctrace<3>() << "Can't initialize cluster " << new_cluster 
			 << " to " << (uint16_t) value << '\n';

    }

    return new_cluster;
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
 

// Se trata de una vista de FAT_area_list que permite concebirlo como un
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
    using FAT_area_list = atd::FAT32::FAT_area_list<Sector_driver0>;
    using Volume = typename FAT_area_list::Volume;
    using State  = impl_of::File_state;

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


// State
// (TODO) Se mezcla el errno con el state!!! Separarlo.
    bool ok() const {return sector_.last_operation_ok(); }
    bool error() const {return sector_.last_operation_fail(); }

    // ¿Hemos llegado al final del fichero?
    bool end_of_file() const {return state_ == State::end_of_file;}

    // Cuando queremos añadir bytes a un fichero, incrementamos la posición
    // del end_of_file. Pero puede ocurrir que lleguemos al final de la lista
    // enlazada de sectores, no teniendo más sectores donde escribir bytes.
    // En ese momento habrá que llamar a FAT_area::add_cluster() para añadir
    // sectores a ese fichero donde poder escribir. La siguiente función nos
    // dice cuándo hemos alcanzado el final de los sectores y no tenemos más
    // donde escribir.
    bool end_of_sectors() const {return sector_.end_of_sectors(); }

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

    FAT_area_list sector_; // lista enlazada con los sectores del fichero
    uint16_t i; // byte actual al que apuntamos dentro del sector
    atd::safe_Uninitialized<uint32_t> remainder_bytes_; // numero de bytes que quedan por leer

    State state_;

// Funciones de implementacion 
// next_byte la necesito en const_iterator, pero... quiero implementarlo así
// de verdad? Mejor como un wrapper sobre esta clase.
//    static uint16_t next_byte(FAT_area_list& sector, uint16_t i);

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
//       (TODO) Crear en sector_driver::read(span) y que el valor devuelto
//       sea el número de bytes escritos. Sería un poco más eficiente que esta
//       función.
template <typename SD> 
uint8_t File<SD>::read(std::span<uint8_t> buf)
{
    using size = std::span<uint8_t>::size_type;

    if (sector_.end_of_sectors() or sector_.last_operation_fail()
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

	    if (sector_.end_of_sectors() or sector_.last_operation_fail())
		return n + 1u;

	    i = 0;
	}
    }

    return n;
}




/***************************************************************************
 *				DIRECTORY
 * -------------------------------------------------------------------------
 *
 *  Un directory es un array de entries.
 *
 *  La forma de determinar el final del array es con un centinela: el 0x00 de
 *  DIR_name[0].
 *
 *  Tenemos, la lista de clusters (o de sectores):
 *	s0 -> s1 -> s2 -> s3 -> ... -> sn -> ... -> s31 -> x
 *	                                                   ^
 *	                                                   |
 *	                                           final  del cluster
 *
 *  Pero en sn tenemos la última entrada del directorio. (La analogía es
 *  pensar en un libro de líneas, donde en cada línea escribimos una entry.
 *  Llenamos el libro hasta sn y el resto de las páginas está vacía. El libro
 *  acaba en 'x'. Si llenamos el libro tenemos que comprar un nuevo cuaderno
 *  llamando a FAT_area::add_cluster()).
 *
 *  Es importante observar la diferencia con los ficheros, que se definen por
 *  su tamaño (file_size = número de bytes que tienen).
 *
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

enum class Directory_errno : uint8_t{
    ok = 0, // fail distinto de 0
    read_error, 
    long_entry_corrupted,
    end_of_sectors, // se ha llegado al final del último cluster
    last_entry	    // se ha alcanzado la última entrada, no hay más
};

enum class Directory_state : uint8_t{
    ok          = 0,	// apunta a una entrada válida
    last_entry  = 1	// se ha llegado a la última entrada del directorio
};


struct Directory_ertate {
    using Errno = Directory_errno;
    using State = Directory_state;

    State state : 4;
    Errno errno : 4;

    // Añado este constructor para evitar warnings del compilador @_@
    Directory_ertate() :state{State::ok}, errno{Errno::ok}{} 

    void operator=(State st) { state = st; }
    void operator=(Errno e)  { errno = e; }

    bool operator==(State st) const { return state == st; }
    bool operator!=(State st) const { return state != st; }
    
    bool operator==(Errno e) const { return errno == e; }
    bool operator!=(Errno e) const { return errno != e; }

};

static_assert(sizeof(Directory_state) == 1);


}// impl_of
 
template <typename Sector_driver0> 
class Directory{
public:
// Types
    using Volume    = atd::FAT32::Volume<Sector_driver0>;
    using FAT_area_list = atd::FAT32::FAT_area_list<Sector_driver0>;
    using Entry	    = impl_of::Directory_entry;
    using Type      = Entry::Type;
    using Entry_info= impl_of::Entry_info;
    using Attribute = impl_of::Directory_entry::Attribute;
    using Errno	    = impl_of::Directory_errno;
    using State     = impl_of::Directory_state;

// Constructors
    // Enlaza con el directorio que se encuentra en el cluster0
    Directory(Volume& volume, uint32_t cluster0);

// ls (listado del directorio)
    // Reinicia la lectura. La siguiente llamada a `read_xxx` devolverá la
    // primera entrada que encuentre.
    void first_entry();
    
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
			 std::span<uint8_t> name);

    // Devuelve la siguiente entrada correspondiente al attribute att.
    // De esta forma se puede hacer un ls de solo los ficheros, o solo los
    // directorios.
    bool read_long_entry(Entry_info& info, 
			 std::span<uint8_t> name, 
			 Attribute att);

// cd (cambio de directorio)
    // 2 posibles formas de hacer cd:
    //	    cd("directory_name") vs cd(cluster_number)
    // La primera es muy ineficiente ya que habría que recorrer todo el
    // directorio actual hasta encontrar el cluster correspondiente.
    void cd(uint32_t cluster0);

// mkdir, mkfile (creación de ficheros)
    // mkdir
    // mkfile

// rmdir, rmfile (borrado de ficheros)
    // rmdir
    // rmfile (en unix es `rm` pero queda más claro rmfile)




// Primitivas de implementación
    // Crea una nueva entrada de nombre `name` y características dadas en
    // info. De info, ignora los access/modification times.
    // Devuelve el número de entrada de la short_entry de este nuevo fichero
    // creado ó 0 en caso de error. (0??? en general, salvo en el root
    // directory la entrada 0 corresponde al directorio actual (.) no siendo
    // una entrada válida en general, por eso puedo devolver 0 como error)
    uint32_t new_entry(const Entry_info& info, std::span<uint8_t> name);

//    void remove_entry();


// Errno = resultado de la última operación
    bool last_operation_ok() const {return errno() == Errno::ok; }
    bool last_operation_fail() const {return errno() != Errno::ok; }

    bool read_error() const {return errno() == Errno::read_error; }
    bool long_entry_corrupted() const 
		    {return errno() == Errno::long_entry_corrupted; }


// State
    bool last_entry() const {return state() == State::last_entry; }

private:
    FAT_area_list sector_;  // lista con los clusters que forman el directorio
			    
    uint32_t cluster0_; // primer cluster del directorio
    uint16_t nentry_;	// Número de entrada a la que apuntamos
			// CUIDADO: es ínidice global, no relativo al sector.
// TODO: cambiar i por nentry_
//    uint16_t i; // byte actual al que apuntamos dentro del sector
    impl_of::Directory_ertate errno_state_;
    
    static constexpr uint8_t sizeof_entry = Entry::size;

    // Este state no lo necesito fuera
    State state() const {return errno_state_.state;}
    void state(State st) {errno_state_.state = st; }

    bool ok() {errno_state_.errno = Errno::ok; return true; }
    bool errno(Errno e) {errno_state_.errno = e; return false;}
    Errno errno() const {return errno_state_.errno;}

    // hace que tanto state como errno sean ok
    void state_ok();


// Helpers
    bool read_long_entry(Entry& entry, Entry_info& info,
						    std::span<uint8_t> name);
    // Lo llamo copy porque realmente no lee ninguna entrada, se limita a
    // copiar la información de entry en [info, name]
    void copy_short_entry(const Entry& entry, Entry_info& info, 
					std::span<uint8_t> name);

// nentry
    Volume& volume() {return sector_.volume; }
    const Volume& volume() const {return sector_.volume; }

    uint16_t number_of_entries_per_sector() const
    { return volume().bytes_per_sector() / Entry::size; }

    uint16_t local_nentry(uint16_t nentry) const
    { return nentry % number_of_entries_per_sector(); }

    bool nentry_is_first_entry_of_a_sector(uint16_t nentry) const
    { return (nentry % number_of_entries_per_sector()) == 0; }
};

template <typename S> 
inline 
Directory<S>::Directory(Volume& volume, uint32_t cluster0)
    : sector_{volume, cluster0}, cluster0_{cluster0}, nentry_{0} //i{0}
{ }

template <typename S> 
inline void Directory<S>::first_entry()
{ 
    sector_.first_sector(cluster0_); 
    state_ok();
    nentry_ = 0;
    //i = 0;
}

// Esta función es idéntica a first_entry()
template <typename S>
void Directory<S>::cd(uint32_t cluster0)
{
    sector_.first_sector(cluster0);
    cluster0_ = cluster0;
    first_entry();
}

template <typename S>
inline void Directory<S>::state_ok()
{
    state(State::ok);
    errno(Errno::ok);
}

// Observar que este read no marca el error, eso se lo dejo a quien lo llame
template <typename S> 
bool Directory<S>::read(Entry& entry)
{
    if (state() == State::last_entry){
	atd::ctrace<5>() << "Trying to read EOC file\n";
	return errno(Errno::last_entry);
    }

    auto pos = local_nentry(nentry_) * Entry::size;

    auto n = 
	volume().read_global(sector_.global_sector_number(), pos,
								entry.data);
    
    if (n != Entry::size){
	atd::ctrace<3>() << "Can't read next directory entry\n";
	return errno(Errno::read_error);
    }

    ++nentry_;

    if (nentry_is_first_entry_of_a_sector(nentry_)){
	sector_.next_sector(); 

	if (sector_.last_operation_fail())
	    return false;

	if (sector_.end_of_sectors())
	    state(State::last_entry);
    }

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
	    return errno(Errno::long_entry_corrupted);
	}

	if (i * size < long_name.size()){// copiamos nombre si podemos
	    uint8_t n = std::min<uint8_t>(size, 
					long_name.size() - i * size);
	    entry.copy_long_name({long_name.begin() + i * size, n});
	}

	if (!read(entry))
	    return errno(Errno::read_error);
	
    }

// Después de las long_name entries siempre viene una short_entry
    if (entry.type() != Type::short_entry)
	return errno(Errno::long_entry_corrupted); 

    impl_of::copy(entry, info);

    return ok();
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
	if(!read(entry))
	    return errno(Errno::read_error);

    }while (entry.type() == Type::free);

    if (entry.type() == Type::last_entry){
	state(State::last_entry);
	return ok();
    }

    if (entry.type() == Type::short_entry){
	copy_short_entry(entry, info, long_name); // copia entry en info/long_name
    }

    else // == Type::long_entry
	return read_long_entry(entry, info, long_name); // entry es la last_entry
					        // entrada del long_name

    return ok();
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

// (DUDA) Al pasarle uint8_t limito la longitud del nombre a 255 caracteres. 
// ¿Algún problema? 255 es demasiado largo (long_name es el nombre del
// fichero, no todo el path).
//template <typename S>
//bool Directory<S>::find_first_free_entry(uint8_t long_name_size)
//{
//    if (state() == State::last_entry)
//	return errno(Errno::last_entry);
//
//    Entry entry;
//
//    if (!read(entry)){
//	if (sector_.end_of_sectors()){
//	return errno(Errno::end_of_sectors);
//    }
//
//
//    if (entry.type() == Entry_type::last_entry){
//	state(State::last_entry);
//	return errno(Errno::end_of_file);
//    }
//
//    if (entry.type() == Entry_type::free){
//    CONTAR CONTIGUAS PARA QUE ENTRE LONG_NAME
//    }
//
//    return ok();
//}


//template <typename S>
//uint32_t 
//Directory<S>::new_entry(const Entry_info& info , std::span<uint8_t> name)
//{
//// find_first_free_entry: 
//    uint32_t ne = find_first_free_entry(name.size()); // que entre el name
//    if (ne == 0)
//	sector_.add_cluster(0x00); // 0x00 valor por defecto en los directorios
//    
//// new_entry_impl:
//    para toda parte de name:
//	new_long_entry(name[...]);
//    new_short_entry(info);
//}



}// namespace FAT32

}// namespace dev
 

#endif


