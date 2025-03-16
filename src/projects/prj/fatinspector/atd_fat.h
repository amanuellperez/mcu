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
 *  + Duda de diseño:
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
 *  + std::span vs std::string_view:
 *	Los nombres de los ficheros son cadenas de caracteres. ¿Cómo pasarlos
 *	a las funciones? Cuando quiero rellenar el nombre en una cadena no
 *	puedo usar string_view ya que es una cadena de tipo const. Eso me
 *	obliga a usar std::span para poder escribir las cadenas. Por
 *	consistencia, lo uso también para leerlo. (necesitaba una
 *	atd::string_fix similar a std::string pero de tamaño fijo).
 *
 * DUDAS    
 *  1. Estoy copiando un puntero a Volumen dentro de la mayoría de las clases,
 *  lo cual consume memoria. ¿Es preocupante? ¿Hay que modificar eso?
 *
 * NOTACIÓN
 *  Directorios
 *  -----------
 *  1) Un directorio es un array de short entries. En principio podemos
 *     concebir el array como de tamaño infinito. Es un sparse-array, esto es,
 *     no todas las entradas del array están ocupadas. Por ello, las entries
 *     de un directorio las clasifico en:
 *	    a) entry: son entradas ocupadas. Pueden ser info o name entry.
 *	    b) free : que puede estar available (está en medio de entradas
 *	              ocupadas) o nomore (ya no hay más entries a partir de
 *	              esta entrada).
 *
 *  2) Una long_entry es un conjunto de short entries, la primera de ellas
 *     sirve para contener la información de la long_entry (fecha de creación,
 *     modificación...) y el resto contienen el long_name. Por ello, las distingo
 *     en: name_entries e info_entry.
 *
 *     Además, se colocan en orden inverso. Usando el ejemplo de la FAT
 *     specificacion, el fichero "The quick brown.fox" se escribe:
 *
 *	    +-------------------------------------------------------+
 *	    | 0x42 | wn.fox                 | check_sum             |
 *	    +-------------------------------------------------------+
 *	    | 0x01 | The quick bro          | check_sum             |
 *	    +-------------------------------------------------------+
 *	    | info_entry: 'THEQUI~FOX', creation date,...           |
 *	    +-------------------------------------------------------+
 *	
 *	Esta long_entry tiene 2 name_entries y una info_entry. Observar que
 *	las name_entries se ordenan 1, 2, ..., n, escribiendo el orden de la
 *	mayor como 0x40|n. (por eso es 0x42 la primera del ejemplo, en lugar
 *	de 0x02)
 *
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
{ free = 0, allocated, bad, 
    end_of_clusters, // es el último cluster de esa lista
    reserved, 
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
	return volume_->template sd_read<uint32_t>(sector0_ + nsector, pos);
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
	    if (!write_in_area(i, cluster, value))
		return false;
	}

	return true; 
    }

    
    bool write_in_area(uint8_t i
		      , const uint32_t& cluster, const uint32_t& value) const
    {
	auto [nsector, pos] = cluster2sector_pos(cluster);
	return 
	    volume_->template 
		sd_write<uint32_t>(sector0_area(i) + nsector, pos, value);
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
    atd::precondition<7>(i < nFATs_, "Error: FAT area number too big");
    
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
// Y llamar directamente a volume->sd_read. De esa forma evitamos el estar
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
    // Primer sector de la data area relativa a la SD card (global)
    uint32_t sd_first_sector() const
    { return data_area_first_sector_;}

    uint8_t  sectors_per_cluster() const
    { return data_area_sectors_per_cluster_;}

    // Devuelve el primer sector del `cluster` indicado. 
    // El sector devuelto es global (coordenadas de SDCard)
    uint32_t sd_first_sector_of_cluster(uint32_t cluster) const
    {return  sd_first_sector() + (cluster - 2) * sectors_per_cluster();}

    // Número de sector relativo a la SD card (es el número de sector global)
    uint32_t sd_sector_number(uint32_t cluster, uint8_t sector) const
    { return sd_first_sector_of_cluster(cluster) + sector; }



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
    Int sd_read(const uint32_t& nsector, uint16_t pos)
    { return driver_.template read<Int>(nsector, pos); }

    uint32_t sd_read(const uint32_t& nsector, const uint32_t& pos
		  , std::span<uint8_t> buf)
    {return driver_.read(nsector, pos, buf); }

    // Devuelve true si todo va bien, false si no consigue escribir.
    // Si falla mirar el state correspondiente para ver error (write_error())
    template <Type::Integer Int>
    bool sd_write( const uint32_t& nsector, uint16_t pos
		     , const Int& value)
    { return driver_.template write<Int>(nsector, pos, value) == 1; }

    uint32_t sd_write(const uint32_t& nsector, uint16_t pos
		     , const std::span<uint8_t>& buf)
    { return driver_.write(nsector, pos, buf); }

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
    auto sector0 = data_area.sd_first_sector_of_cluster(cluster);

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
    uint32_t value = read(cluster);

    if (read_error()){
	ctrace<3>() << "Volume read error\n";
	return Cluster_state::read_error;
    }

    auto state = cluster_state(value);

    // Solo actualizamos el valor de next_cluster en caso de que exista.
    if (state == Cluster_state::allocated)
	next_cluster = value;

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

static_assert(sizeof(FAT_area_list_ertate) == 1);

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
    uint32_t sd_sector_number() const;

// Errno = resultado de la última operación
    bool last_operation_ok() const {return errno() == Errno::ok; }
    bool last_operation_fail() const {return errno() != Errno::ok; }

    bool next_cluster_no_allocated() const 
    {return errno() == Errno::next_cluster_no_allocated;}

    Errno errno() const {return ertate_.errno;}

// State
    State state() const {return ertate_.state;}

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

    impl_of::FAT_area_list_ertate ertate_;

    void state(State st) {ertate_.state = st; }

    bool ok() {ertate_.errno = Errno::ok; return true; }
    bool errno(Errno e) {ertate_.errno = e; return false;}
    
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
inline uint32_t FAT_area_list<SD>::sd_sector_number() const
{ return volume.data_area.sd_first_sector_of_cluster(cluster_) + sector_; }


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
    uint32_t next_cluster{};
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
    { return sector_.volume.template sd_read<uint8_t>
				    (sector_.sd_sector_number(), i); }
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
    using Size = std::span<uint8_t>::size_type;

    if (sector_.end_of_sectors() or sector_.last_operation_fail()
	or remainder_bytes_.value() == 0)
	return 0;

    Size n = 0; 
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

/***************************************************************************
 *			     DIRECTORY_ENTRY
 ***************************************************************************/
enum class Entry_attribute: uint8_t{
    read_only = 0x01,
    hidden    = 0x02,
    system    = 0x04,
    volume_id = 0x08,
    directory = 0x10,
    archive   = 0x20,
    long_name = read_only | hidden | system | volume_id
};

// Las short entries pueden estar:
//	1. Ocupadas: pueden contener la info de la entry o parte del name.
//	2. frees   : pueden estar entre otras entries ocupadas o ser la
//	             última, de tal manera que no haya más entries después
//	             de esa.
enum class Entry_type : uint8_t{
    free_available, // == 0xE5, después de esta entry habrá alguna ocupada
    free_nomore, // == 0x00, no hay más entries ocupadas a partir de esta
    info_entry, // short entry que contiene la info. Es la clásico short_entry
    name_entry  // short entry que contiene parte del name de una long_entry
};


// (TODO) Las entries pueden ser:
//	1. info_entry
//	2. name_entry
//	o estar free.
//	Me gustaría poder tener dos tipos de entries, Info_entry y Name_entry,
//	y que después de leer la entry manejar una u otra. ¿Cómo hacerlo de
//	manera que sea sencillo su uso? (y sin usar unions, ya que lo que
//	ahora hago es algo parecido a una union)
struct Directory_entry{
// Types
    using Attribute = Entry_attribute;
    using Type = Entry_type;

// Cfg
    static constexpr uint8_t size = 32;

    // número de caracteres ASCII que admite un long_name
    static constexpr uint8_t ascii_short_name_len= 11;
    static constexpr uint8_t ascii_extension_len = 3;
    static constexpr uint8_t ascii_long_name_len = 13;

// Data
    std::array<uint8_t, size> data;

// Tipo de entrada
    Type type() const;
    void type(Type type);
    static uint8_t free_type_to_uint8_t(Type type);

    // Las entradas pueden estar o ocupadas o no. Las no ocupadas son las
    // llamadas free type, que pueden ser de dos tipos, las que están entre
    // entradas ocupadas (0xE5) y las que están al final del todo (0x00).
    bool is_free() const
    { return type() == Type::free_available or type() == Type::free_nomore; }

// info_entry (section 6, FAT specification)
    // Lee el short name de la entry copiándolo en `str`.
    // Devuelve el número de caracteres copiados.
    uint8_t read_short_name(std::span<uint8_t> str) const;

    // Escribe str en la short name
    void write_short_name(std::span<const uint8_t> str);
//    static long_name_to_short_name(

    Attribute attribute() const { return Attribute{data[11]}; }
    void attribute(Attribute att) { data[11] = static_cast<uint8_t>(att); }

    // data[12] == 0
    
    uint8_t creation_time_tenth_of_seconds() const {return data[12];}
    void creation_time_tenth_of_seconds(uint8_t t) {data[12] = t;}

    uint16_t creation_time() const 
	    {return atd::concat_bytes<uint16_t>(data[15], data[14]);}

    void creation_time(const uint16_t& t)
	    {return atd::split_bytes(t, data[15], data[14]);}

    uint16_t creation_date() const
	    {return atd::concat_bytes<uint16_t>(data[17], data[16]);}

    void creation_date(const uint16_t& t) 
	    {return atd::split_bytes(t, data[17], data[16]);}

    uint16_t last_access_date() const
	    {return atd::concat_bytes<uint16_t>(data[19], data[18]);}

    void last_access_date(const uint16_t& t) 
	    {return atd::split_bytes(t, data[19], data[18]);}

    // devuelve el cluster del file asociado con esta entrada
    uint32_t file_cluster() const
    {return atd::concat_bytes<uint32_t>(data[21], data[20], data[27], data[26]);}

    void file_cluster(const uint32_t& fc) 
    {return atd::split_bytes(fc, data[21], data[20], data[27], data[26]);}

    uint16_t last_modification_time() const
	    {return atd::concat_bytes<uint16_t>(data[23], data[22]);}

    void last_modification_time(const uint16_t& t)
	    {return atd::split_bytes(t, data[23], data[22]);}

    uint16_t last_modification_date() const
	    {return atd::concat_bytes<uint16_t>(data[25], data[24]);}

    void last_modification_date(const uint16_t& t)
	    {return atd::split_bytes(t, data[25], data[24]);}

    uint32_t file_size() const
    {return atd::concat_bytes<uint32_t>(data[31], data[30], data[29], data[28]);}

    void file_size(const uint32_t& fs) 
    {return atd::split_bytes(fs, data[31], data[30], data[29], data[28]);}

    static uint8_t 
	check_sum_of(std::span<const uint8_t, ascii_short_name_len> short_name);

    uint8_t info_entry_check_sum() const 
    { 
	std::span<const uint8_t, ascii_short_name_len> sp{data.data(),  ascii_short_name_len};
	return check_sum_of(sp);
    };

// name_entry (section 7, FAT specification)
    // Devuelve el orden de la entrada con la mask 0x40
    uint8_t extended_order_with_mask() const {return data[0];}
    void extended_order_with_mask(uint8_t x) { data[0] = x; }

    // El order de la última entrada de un long name se masquea con 0x40
    // (jejeje, yo masqueo, tu masqueas, él masquea xD. Masquear `a` es `a|0x40`)
    // Mejor: el bit número 6 order de la última entrada de un long name es 1.
    // Devuelve el orden de la entrada
    uint8_t extended_order() const {return data[0] & ~0x40; }
    bool is_last_member_of_long_name() const {return data[0] & 0x40; }

    // Devuelve el orden de la primera entrada
    static uint8_t first_order_name_entry(uint8_t order) 
    { return order | 0x40; }

    // data[1..11) = long name
    // data[11] = attribute. En este caso ATTR_LONG_NAME
    // data[12] == 0
    uint8_t name_entry_check_sum() const { return data[13]; }
    void name_entry_check_sum(uint8_t x) { data[13] = x; }

    // data[14..26) = long name
    // data[26] == 0 and data[27] == 0
    // data[28..32) = long name
    // Lee el long name de la entry copiandolo en `str`:
    //	    str = entry.name;
    // Devuelve el número de caracteres copiados
    uint8_t read_long_name(std::span<uint8_t> str);

    // Copia el nombre `str` en entry.name.
    //	    entry.name = str;
    void write_long_name(std::span<const uint8_t> str);


// Conversión de long_entry en short_entries
    // Numero de short entries que se necesitan para almacenar una long entry
    // Cada long entry está formada por una short entry más las entries para
    // almacenar el nombre (cada una de estas entries almacena solo 13
    // caracteres)
    static uint8_t nname_entries_for_store_long_entry(uint8_t name_len);
    static uint8_t nshort_entries_for_store_long_entry(uint8_t name_len)
    { return nname_entries_for_store_long_entry(name_len) + 1; }

    // lname -> sname
    static void long_name2short_name( std::span<const uint8_t> lname
			    , std::span<uint8_t, ascii_short_name_len> sname);
				    

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
//    void null_entry() { data[12] = 1; }
//    bool is_null_entry() const {return data[12] == 1;}

private:
    uint8_t read_long_name_impl(std::span<uint8_t> str);

};

// escribe la long_name como short_entry
void long_name2short_entry(uint8_t order, std::span<const uint8_t> name, 
						    Directory_entry& entry);

// Info básica que necesita un usuario de una entrada.
// Solo le falta el nombre que al ser de longitud variable dejo que sea el
// cliente quien decida cuánta memoria reservar para él.
struct Entry_info{
    using Attribute = Entry_attribute;

    static Entry_info make_empty_entry();

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

inline 
Entry_info Entry_info::make_empty_entry()
{
    Entry_info info;
    memset(&info, 0, sizeof(Entry_info));
    return info;
}

// Copia entry en info: entry -> info;
void copy(const Directory_entry& entry, Entry_info& info);

// Copia info en entry: info -> entry;
void copy(const Entry_info& info, Directory_entry& entry);


// info_entry -> [info, name]
// (@_@) Un nombre más correcto sería `info_entry2info_name` pero es un lío,
// ya que el info_name me recuerda a name_entry @_@. De momento, dejo 
// short_entry2info_name.
void short_entry2info_name(const Directory_entry& entry, Entry_info& info, 
				    std::span<uint8_t> name);
// [info, name] -> info_entry
void info_name2short_entry( const Entry_info& info
			       , std::span<const uint8_t> name
			       , Directory_entry& entry);


/***************************************************************************
 *				DIRECTORY_INDEX
 ***************************************************************************/

// Al concebir un directorio como un array de entries, tiene sentido iterar
// por las diferentes entradas usando un índice. Esta clase representa ese
// índice.

// CUIDADO:
//	Este índice permite iterar por TODAS las short entries del directorio,
//  independientemente de que estén free o empty.
//	Es parecido a std::array<int, 10> x; Tú reservas un array de 10 ints,
//  y al hacer:
//	    for (int i = 0; i < 10; ++o)
//		x[i] ...
//  no sabes en principio si está inicializado x[i] o no. Eso lo dará la
//  lógica del programa. Este índice es igual de torpe.
//
//  Los únicos accesos a la SDCard que hace es para mirar el siguiente cluster
//  de la lista enlazada que forma el directorio. No lee las entries (pretende
//  ser lo más eficiente posible (???))
template <typename Sector_driver0> 
class Directory_short_entries_index{
public:
// Types
    using Volume  = atd::FAT32::Volume<Sector_driver0>;

// Constructor
    // Precondicion: cluster0 es el cluster donde se encuentra la entrada
    // nentry.
    Directory_short_entries_index(Volume* vol, 
		    const uint32_t& cluster0, const uint32_t& nentry = 0);
    Directory_short_entries_index(Volume& vol, 
		    const uint32_t& cluster0, const uint32_t& nentry = 0);
    Directory_short_entries_index(const Directory_short_entries_index& x);
//    Directory_short_entries_index& operator=(const Directory_short_entries_index& x);

    // Reinicia el index, pero en lugar de reiniciarlo en 0 en el primer
    // cluster del directorio, lo inicia en 0 en el siguiente cluster.
    // (RRR) Este es un forward_index. Cuando ha llegado al final, a
    // end_of_array, si añadimos un cluster nuevo ¿cómo poder continuar con el
    // siguiente index? No puedo ya que end_of_array no guarda información del
    // nentry que ocupa. Por ello, opto por reiniciar el índice a 0 en el
    // siguiente cluster.
    bool reset_to_zero_in_next_cluster();

// Operations
    // Pasamos a apuntar la siguiente entrada
    Directory_short_entries_index& operator++();
    Directory_short_entries_index operator++(int);

// Info
    // Devuelve el número de sector global a la SD card
    uint32_t sd_sector_number() const
    { return sd_sector_number(cluster_, clus_sector_number()); }

    // Número de entrada relativa al sector actual
    uint32_t sec_nentry() const
    { return clus_nentry() % nentries_per_sector(); }

    // Posición del primer byte de la entrada actual dentro del sector actual
    uint32_t sec_first_byte_entry() const 
    { return sec_nentry() * sizeof_entry; }

    // Número de entrada de la short entry. (es el índice global de la short
    // entry en el array de entries del directorio)
    uint32_t nentry() const 
    { return nentry_; }

// end
    // En los directorios tenemos dos fines diferentes:
    //	1) El end of array, que indica dónde acaba el array (no hay más
    //	   entradas reservadas en la SD card)
    //	2) La last entry, que indica dónde acaban las entradas. A partir de
    //	last_entry no hay más entradas ocupadas.
    //
    //	Para no confundirlo en código distingo entre end_of_array y
    //	last_entry.
    static Directory_short_entries_index end_of_array() 
    {return Directory_short_entries_index{nullptr, 0, end_of_array_nentry}; }

    // precondición: son entradas del mismo directorio (volumen iguales)
    bool operator==(const Directory_short_entries_index& x) const
    { return x.nentry_ == nentry_; }

    bool operator!=(const Directory_short_entries_index& x) const
    { return x.nentry_ != nentry_; }

private:
// Data
    // (TODO) Como es una entrada de un directorio, tiene sentido vincularla
    // con el directorio al que pertenece en lugar del Volume. Y acceder a
    // Volume via Directory.
    Volume* volume_;	// Para poder acceder al siguiente cluster de la lista

    uint32_t cluster_;	// cluster donde se encuentra el nentry
    uint32_t nentry_;	// número de entrada (es global a todo el array)
    

// Cfg
    static constexpr uint32_t end_of_array_nentry 
					= std::numeric_limits<uint32_t>::max();
    bool is_end_of_array() const {return nentry_ == end_of_array_nentry; }

// Funciones que dependen del volumen
    using FAT_area= typename Volume::FAT_area;

    Volume& volume() {return *volume_;}
    const Volume& volume() const {return *volume_;}

    uint32_t bytes_per_sector() const {return volume().bytes_per_sector(); }
    uint32_t sectors_per_cluster() const 
	{return volume().data_area.sectors_per_cluster(); }

    // Devuelve el número de sector dentro de la SDCard que ocupa el sector
    // definido por el par (cluster, sector) (sector es relativo al cluster)
    uint32_t sd_sector_number(const uint32_t& cluster, uint8_t sector) const 
	{return volume().data_area.sd_sector_number(cluster, sector); }

    using Cluster_state = FAT_area::Cluster_state;

    Cluster_state next_cluster(const uint32_t& cluster0, 
				 uint32_t& next_cluster)
    { return volume().fat_area.next_cluster(cluster_, cluster_);}


// Helpers
    bool next();
    bool next_cluster();

    static constexpr uint8_t sizeof_entry = Directory_entry::size;
    
    // número de entries por sector
    uint32_t nentries_per_sector() const
    { return bytes_per_sector() / sizeof_entry; }

    uint32_t nentries_per_cluster() const 
    { return nentries_per_sector() * sectors_per_cluster(); }


// Las entradas las agrupamos en clusters:
//
//	    c0 -> c1 -> c2 -> ...
//
// En cada cluster hay 512 entradas (= nentries_per_cluster()), luego la
// primera entrada del cluster c será 512 * c. 
//
// Dentro de cada cluster, agrupamos las entradas por sectores:
//
//	    s0 -> s1 -> s2 -> ...
//
// Como en cada sector hay 16 entradas (= nentries_per_sector()), la
// primera entrada del sector actual s es 16 * s.
//
// Luego: nentry = 512 * c + 16 * s + e
// (siendo 512 = nentries_per_cluster() y 16 = nentries_per_sector())
//
// siendo e (= clus_sector_number) el número de entrada relativo al
// sector actual s.
//
// s = número de sector relativo al cluster c actual = clus_nsector()
// 16 * s + e = número de entry relativa al cluster c actual = clus_nentry()
//          e = número de entry relativa al sector actual = sec_nentry()
//
// Observar que si agrupamos las entradas por sectores, en lugar de
// clusters, obtenemos:
//	    nentry = 16*S + e
//	siendo S = el número de sector global al array, y e el número de entry
//	relativa al sector actual.
    
    // Número de sector relativo al cluster actual.
    uint32_t clus_nentry() const
    { return nentry_ % nentries_per_cluster(); }

    // Número de sector relativo al cluster actual
    uint32_t clus_sector_number() const
    { return clus_nentry() / nentries_per_sector(); }


    // Las entradas dentro de un cluster las numero de [0..512)
    // de un cluster será (512 * c - 1)
    bool is_last_entry_of_cluster() const
    { return clus_nentry() == nentries_per_cluster() - 1; }

// ertate
//    State state() const {return ertate_.state;}
//    void state(State st) {ertate_.state = st; }
//
//    bool ok() {ertate_.errno = Errno::ok; return true; }
//    bool errno(Errno e) {ertate_.errno = e; return false;}
//    Errno errno() const {return ertate_.errno;}
};


template <typename SD>
inline 
Directory_short_entries_index<SD>::Directory_short_entries_index(Volume* vol, 
				const uint32_t& cluster0, const uint32_t& nentry)
    : volume_{vol}, cluster_{cluster0}, nentry_{nentry}
{ }

template <typename SD>
inline 
Directory_short_entries_index<SD>::Directory_short_entries_index(Volume& vol, 
				const uint32_t& cluster0, const uint32_t& nentry)
    : Directory_short_entries_index{&vol, cluster0, nentry}
{ }

template <typename SD>
inline
Directory_short_entries_index<SD>::Directory_short_entries_index
    (const Directory_short_entries_index& x)
    : volume_{x.volume_}, cluster_{x.cluster_}, nentry_{x.nentry_}
{}


template <typename SD>
bool Directory_short_entries_index<SD>::reset_to_zero_in_next_cluster()
{
    if (!next_cluster())
	return false;

    nentry_ = 0;
    return true;
}




//template <typename SD>
//inline
//Directory_short_entries_index<SD>& 
//Directory_short_entries_index<SD>::
//    operator=(const Directory_short_entries_index& x)
//{
//    if (volume_ != x.volume_){
//	atd::ctrace<3>() << "Different directories entries!\n";
//	volume_ = x.volume_;
//    }
//
//    cluster_ = x.cluster_;
//    nentry_ = x.nentry_;
//
//    return *this;
//}


template <typename SD>
inline
Directory_short_entries_index<SD>&
	    Directory_short_entries_index<SD>::operator++()
{ 
    next();
    return *this;
}



template <typename SD>
inline
Directory_short_entries_index<SD> 
	    Directory_short_entries_index<SD>::operator++(int)
{ 
    Directory_short_entries_index tmp{*this};
    next();
    return tmp;
}

template <typename SD>
bool Directory_short_entries_index<SD>::next()
{
    if (is_last_entry_of_cluster()){
	if (!next_cluster())
	    return false;
    }

    ++nentry_;

    return true;
}


template <typename SD>
bool Directory_short_entries_index<SD>::next_cluster()
{
    auto cluster_state = next_cluster(cluster_, cluster_);

    if (cluster_state == Cluster_state::allocated)
	return true;

    if (cluster_state == Cluster_state::end_of_clusters){
	nentry_ = end_of_array_nentry;
	return false; // responde indicando que no puede calcular el siguiente
		      // cluster pero no es un error ya que no hay más.
		      // Por eso cambiamos el state a end_of_clusters.
    }

    return false; 
//    return errno(Errno::cluster_error); quiero que Index sea lo más
//    pequeña posible, por ello no quiero que tenga state.
}


/***************************************************************************
 *				DIRECTORY
 *
 * Un directorio es un array de short enties. 
 *
 * Hay 2 tipos de short entries:
 *	1. name_entry: almacenan parte del nombre de una long entry.
 *	2. info_entry: contiene el short name y la info de la long entry.
 *
 * Una long entry es un conjunto de short entries, la entry[0] es una
 * info_entry, mientras que el resto entry[1..] son name_entries que contienen
 * el nombre del fichero. Se almacenan en orden inverso en el array.
 *
 * La característica fundamental del directory es que es un sparse-array, esto
 * es, algunas de las entradas están ocupadas y otras no, por ello no se puede
 * iterar de forma secuencial sobre él, sino que hay que ir mirando si la
 * entry esta free o no.
 *
 ***************************************************************************/
enum class Directory_errno : uint8_t{
    ok = 0, // fail distinto de 0
    read_error, 
    long_entry_corrupted,
    end_of_clusters, // se ha llegado al final del último cluster
    last_entry,	    // se ha alcanzado la última entrada, no hay más
};

//enum class Directory_state : uint8_t{
//    ok          = 0,	// apunta a una entrada válida
//    last_entry  = 1	// se ha llegado a la última entrada del directorio
//};


struct Directory_ertate {
    using Errno = Directory_errno;
//    using State = Directory_state;

//    State state : 4;
    Errno errno : 4;

    // Añado este constructor para evitar warnings del compilador @_@
    //Directory_ertate() :state{State::ok}, errno{Errno::ok}{} 
    Directory_ertate() :errno{Errno::ok}{} 

//    void operator=(State st) { state = st; }
    void operator=(Errno e)  { errno = e; }

//    bool operator==(State st) const { return state == st; }
//    bool operator!=(State st) const { return state != st; }
    
    bool operator==(Errno e) const { return errno == e; }
    bool operator!=(Errno e) const { return errno != e; }

};

static_assert(sizeof(Directory_ertate) == 1);

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
//    using State     = impl_of::Directory_state;
    using Index = impl_of::Directory_short_entries_index<Sector_driver0>;

// Constructors
    // Enlaza con el directorio que se encuentra en el cluster0
    Directory(Volume& volume, uint32_t cluster0);

// ls (listado del directorio)
    // Iteramos usando índices
    Index index_begin()
	{return Index{volume(), cluster0_};}

    auto index_end() {return Index::end_of_array(); }
    
    // Lee la long entry. Deja i en la siguiente entrada a leer.
    // En caso de error devuelve index_end() y anota el error en errno.
    Index read_long_entry(Index i, Entry_info& info, 
			 std::span<uint8_t> name);

    // Devuelve la siguiente entrada correspondiente al attribute att.
    // De esta forma se puede hacer un ls de solo los ficheros, o solo los
    // directorios.
    // Deja i en la siguiente entrada a leer.
    // En caso de error devuelve index_end() y anota el error en errno.
    Index read_long_entry(Index i, Entry_info& info, 
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
    //  Lee la entrada apuntada por i
    bool read_short_entry(const Index& i, Entry& entry);

    // Crea una nueva entrada de nombre `name` y características dadas en
    // info, ignorando los access/modification times de `info`.
    // Devuelve el número de entrada de la short_entry de este nuevo fichero
    // creado ó 0 en caso de error. (0??? en general, salvo en el root
    // directory la entrada 0 corresponde al directorio actual (.) no siendo
    // una entrada válida en general, por eso puedo devolver 0 como error)
    uint32_t new_long_entry( const Entry_info& info
			   , std::span<const uint8_t> name);
    bool remove_short_entry(const uint32_t& nentry);


// Errno = resultado de la última operación
    bool last_operation_ok() const {return errno() == Errno::ok; }
    bool last_operation_fail() const {return errno() != Errno::ok; }

    bool read_error() const {return errno() == Errno::read_error; }
    bool long_entry_corrupted_error() const 
		    {return errno() == Errno::long_entry_corrupted; }
    bool last_entry_error() const {return errno() == Errno::last_entry; }


private:
// Data
    Volume vol_;
    uint32_t cluster0_; // primer cluster del directorio

    impl_of::Directory_ertate ertate_;
    
// errno
    bool ok() {ertate_.errno = Errno::ok; return true; }
    bool errno(Errno e) {ertate_.errno = e; return false;}
    Errno errno() const {return ertate_.errno;}
    bool throw_errno() const {return false; }


// Helpers
    Index read_long_entry(Index i, Entry& entry, Entry_info& info,
						    std::span<uint8_t> name);

    // Devuelve el número de short entries escritas
    uint32_t write_long_entry( Index i
			 , const Entry_info& info
			 , std::span<const uint8_t> name);

    bool write_name_entry( Index i
			 , uint8_t order, std::span<const uint8_t> name
			 , uint8_t check_sum);

    bool write_info_entry(const Index& i
			  , const Entry_info& info
			  , std::span<const uint8_t> name);

    bool write(const Index& i, Entry& entry);

    bool remove_short_entry(const Index& i);

    Index find_first_not_free_entry(Index i, Entry& entry);

    bool find_first_free_long_entry(uint8_t nshort_entries
				    , Index& i0, uint32_t& nentry_e);
    bool advance_at_most_entries_of_same_type( Entry& entry
		                  , uint8_t nshort_entries, Index& i);

// nentry
    Volume& volume() {return vol_; }
    const Volume& volume() const {return vol_; }
};

template <typename S> 
inline 
Directory<S>::Directory(Volume& volume, uint32_t cluster0)
    : vol_{volume}, cluster0_{cluster0}
{ }


// Esta función es idéntica a first_entry()
template <typename S>
void Directory<S>::cd(uint32_t cluster0)
{
    atd::ctrace<1>() << "TODO: revisar\n";
//    sector_.first_sector(cluster0);
//    cluster0_ = cluster0;
//    first_entry();
}


template <typename S> 
bool Directory<S>::read_short_entry(const Index& i, Entry& entry)
{
    if (i == Index::end_of_array()){
	atd::ctrace<5>() << "Trying to read last_entry index\n";
	return false;
    }

    auto n = volume().sd_read( i.sd_sector_number(), i.sec_first_byte_entry()
			     , entry.data);
    
    if (n != Entry::size){
	atd::ctrace<3>() << "Can't read next directory entry\n";
	return errno(Errno::read_error);
    }

    return true;
}

template <typename S> 
bool Directory<S>::write(const Index& i, Entry& entry)
{
    atd::precondition<7>(i != Index::end_of_array(), 
				    "Trying to write last_entry index\n");

    return (volume().sd_write( i.sd_sector_number(), i.sec_first_byte_entry()
			     , entry.data) == entry.data.size());
}


// Precondición: i apunta a la entrada entry, que es la primera entrada de la
// long_entry.
// El long_name de una long_entry está repartido en varias name_entries. Por
// ello, tenemos que leer todas las name_entries para poder componerlo. Cada
// name_entry tiene 13 caracteres del long_name (salvo la última name_entry
// que puede tener menos). 
template <typename S>
Directory<S>::Index
    Directory<S>::read_long_entry(Index i, Entry& entry,
					Entry_info& info, 
					std::span<uint8_t> long_name)
{
    static constexpr uint8_t esize = Entry::ascii_long_name_len;

// Garantizamos que la cadena acabe en '\0' en el caso de que sea el long_name
// justo múltiplo de size (de 13 caracteres)
    uint8_t nentry = entry.extended_order(); // == nentries de la long_name

    if (nentry * esize < long_name.size())
	long_name[nentry * esize] = '\0';

// Copiamos el name en long_name
    for (; nentry > 0; --nentry){
	uint8_t k = nentry - 1;

	if ((entry.type() != Type::name_entry) 
	    or (entry.extended_order() != nentry)){ // ¿están ordenadas?
	    errno(Errno::long_entry_corrupted);
	    return index_end();
	}

	if (k * esize < long_name.size()){// copiamos nombre si podemos
	    uint8_t n = std::min<uint8_t>(esize, long_name.size() - k * esize);
	    entry.read_long_name({long_name.begin() + k * esize, n});
	}

	++i;

	if (i == Index::end_of_array()){
	    errno(Errno::end_of_clusters);
	    return i;
	}

	if (!read_short_entry(i, entry)){
	    errno(Errno::read_error);
	    return index_end();
	}
	
    }

// Después de las long_name entries siempre viene una short_entry
    if (entry.type() != Type::info_entry){
	errno(Errno::long_entry_corrupted); 
	return index_end();
    }

    impl_of::copy(entry, info);

    ++i; // dejamos apuntando a la siguiente entrada

    return i;
}






template <typename S>
Directory<S>::Index Directory<S>::read_long_entry(Index i
				  , Entry_info& info
				  , std::span<uint8_t> long_name)
{
    using Type = Entry::Type;

    Entry entry;

    i = find_first_not_free_entry(i, entry);
    if (i == index_end())
	return i;

    if (entry.type() == Type::info_entry){
	short_entry2info_name(entry, info, long_name); // copia entry en info/long_name
	++i;
	return i;
    }

    else // == Type::long_entry
	return read_long_entry(i, entry, info, long_name); 

}


template <typename S>
Directory<S>::Index 
	    Directory<S>::read_long_entry(Index i, Entry_info& info, 
					std::span<uint8_t> long_name,
					Attribute att)
{
    while (1){
	i = read_long_entry(i, info, long_name);

	if (i == index_end())
	    return i;

	if (info.attribute == att)
	    return i;
    }

}


// Si tiene éxito, devuelve en `entry` la primera entrada not free dejando el
// Index i apuntando a dicha entrada.
template <typename S>
Directory<S>::Index
    Directory<S>::find_first_not_free_entry(Index i, Entry& entry)
{
    using Type = Entry::Type;

    if (i == Index::end_of_array())
	return i;

    while (1){

	if(!read_short_entry(i, entry)){
	    errno(Errno::read_error);
	    return index_end();
	}

	if (entry.type() == Type::free_nomore){
	    errno(Errno::last_entry);
	    return index_end();
	}

	if (entry.type() != Type::free_available)
	    return i;

	++i; // modificamos la Index
	if (i == Index::end_of_array()) // fin de los clusters
	    return i;
    }
}

// Avanza, como mucho, el índice i nshort_entries siempre y cuando sean del
// mismo tipo que entry.type(). Actualiza el valor de i0 haciendo que apunte
// al siguiente elemento del array.
// Devuelve false en caso de algún error de lectura.
// Preconcidición: entry == directory.entry[i];
template <typename S>
bool Directory<S>::
	advance_at_most_entries_of_same_type( Entry& entry
		                  , uint8_t nshort_entries
				  , Index& i)
{
    auto type = entry.type();

    for (; nshort_entries > 0; --nshort_entries){
    
	++i; 

	if (i == Index::end_of_array())
	    return true;

	if (!read_short_entry(i, entry))
	    return false;

	if (entry.type() != type)
	    return true;

    }

    return true;
}



// i0 como parámetro de entrada indica a partir de qué short entry buscar la
// free long_entry.
//
// Return:
//  Devuelve false para indicar que ha habido un error de lectura.
//  En caso de éxito anotará en [i0, nentry_e) las entradas que ocupa la
//  long_entry. Usando nentry_e - i0.nentry se puede saber si hay suficiente
//  espacio o no en el array de short entries para la long entry.
template <typename S>
bool Directory<S>::find_first_free_long_entry(uint8_t nshort_entries
		, Index& i0, uint32_t& nentry_e)
{

    while(i0 != Index::end_of_array()){

	Entry entry;
	if(!read_short_entry(i0, entry))
	    return false;

	if (entry.is_free()){
	    Index i = i0;
	    if (!advance_at_most_entries_of_same_type(entry, nshort_entries, i))
		return false;

	    if (i.nentry() - i0.nentry() == nshort_entries){
		nentry_e = i.nentry();
		return true;
	    }

	    i0 = i;
	}
	else
	    ++i0;

    }

    nentry_e = i0.nentry();
    return true;
}


// Devuelve 0 en caso de error (aunque tal como está implementada sí que está
// creando short entries) (¿corregir y que quede consistente?)
template <typename S>
uint32_t 
Directory<S>::new_long_entry( const Entry_info& info
			    , std::span<const uint8_t> name)
{
    auto nshort_entries = 
			Entry::nshort_entries_for_store_long_entry(name.size());

// 1. Encontramos/garantizamos la primera long_entry donde entra name
    Index i = index_begin();
    uint32_t nentry_e{};
    if (!find_first_free_long_entry(nshort_entries, i, nentry_e))
	return 0;

    if (nentry_e - i.nentry() < nshort_entries){ // si no hay espacio en el array
	volume().fat_area.add_cluster(0x00); // (FAT specification 6.5)
				   // 0x00 valor por defecto en los directorios
	if (i == Index::end_of_array())
	    i.reset_to_zero_in_next_cluster();
    }

// 2. Escribimos la información en la long entry
// (TODO) ¿cómo gestionar los errores? Si no se hace flush las modificaciones
// del sector no se grabarán. ¿Antes de llamar a esta función que el cliente
// haga flush, y si falla que cargue de nuevo el sector?
    if (write_long_entry(i, info, name) != nshort_entries)
	return 0;

    return nshort_entries;
}


// Una long_entry está formada por un conjunto de short entries de dos
// tipos: la mayoría solo contienen el nombre (name_entry) y la última (o
// primera realmente) contiene la info name (info_entry)
//
// precondition: hay suficientes free entries a partir de i 
//		(==> no tenemos que validar que ++i pueda fallar)
//		(???) podría fallar ++i por errores de lectura a la SD card
template <typename S>
uint32_t 
Directory<S>::write_long_entry(Index i
			      , const Entry_info& info
			      , std::span<const uint8_t> name)
{
// write_name_entries:
    static constexpr uint8_t esize = Entry::ascii_long_name_len;
    auto nentries = Entry::nname_entries_for_store_long_entry(name.size());

    std::array<uint8_t, Entry::ascii_short_name_len> short_name;
    Entry::long_name2short_name(name, short_name);

    uint8_t check_sum = Entry::check_sum_of(short_name);

    // Primera entrada: su número de orden es 0x40|order, y en general será de
    // menor longitud que el resto.
    if (!write_name_entry(i, Entry::first_order_name_entry(nentries)
		    , {name.begin() + (nentries - 1) * esize
				    , name.size() - (nentries - 1) * esize}
		    , check_sum))
	return 0;


    ++i;

    // Resto de entradas
    for (uint8_t j = 1; j < nentries; ++j, ++i){
	uint8_t k = nentries - j;

	if (!write_name_entry( i, k
			     , {name.begin() + (k - 1) * esize, esize}
			     , check_sum))
	    return j;
    }

// write_info_entry:
    if (!write_info_entry(i, info, name))
	return nentries;

    return nentries + 1;
}

template <typename S>
bool 
Directory<S>::write_info_entry( const Index& i
			      , const Entry_info& info
			      , std::span<const uint8_t> name)
{
    Entry entry;

    info_name2short_entry(info, name, entry);

    return write(i, entry);
}

template <typename S>
bool 
Directory<S>::write_name_entry( Index i
			      , uint8_t order, std::span<const uint8_t> name
			      , uint8_t check_sum)
{
    Entry entry;

    long_name2short_entry(order, name, entry);
    entry.name_entry_check_sum(check_sum);

    return write(i, entry);
}

template <typename S>
inline 
bool 
Directory<S>::remove_short_entry(const uint32_t& nentry)
{ return remove_short_entry(Index{volume(), cluster0_, nentry}); }

// ¿Qué pasa si `Index i` apunta más alla de la última entry?
// Lo que haría es perder tiempo, pero lo dejaría todo consistente.
template <typename S>
bool 
Directory<S>::remove_short_entry(const Index& i)
{
    Entry::Type type = Type::free_available;

    Entry entry;

// type = free_available or free_nomore?
    auto j = i;
    ++j;
    if (j == Index::end_of_array()){
	type = Type::free_nomore;

    } else{
	if (!read_short_entry(j, entry))
	    return errno(Errno::read_error);

	if (entry.type() == Type::free_nomore)
	    type = Type::free_nomore;
    }

    return volume().template sd_write<uint8_t>(i.sd_sector_number(), 
			       i.sec_first_byte_entry(),
			       Entry::free_type_to_uint8_t(type));
    
}



}// namespace FAT32

}// namespace dev
 

#endif


