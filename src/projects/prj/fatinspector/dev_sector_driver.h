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

#ifndef __DEV_SECTOR_DRIVER_H__
#define __DEV_SECTOR_DRIVER_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Drivers usados para acceder a un dispositivo físico de almacenamiento
 *	(como por ejemplo una SD card).
 *
 * HISTORIA
 *    Manuel Perez
 *    26/02/2025 Experimentando con Sector_driver
 *
 ****************************************************************************/
#include <algorithm>	// fill
#include <bit>	// endianness
#include <atd_basic_types.h> // atd::Uninitialized
#include <atd_trace.h>
#include <atd_contracts.h>  // precondition

namespace dev{




// Sector_driver es el driver que se usa para acceder a un dispositivo físico
// que almacena la memoria en sectores (en este caso la SDCard).
// (RRR) ¿Por qué uso Sector_span?
//       Al querer implementar Volume::read(Boot_sector) como Boot_sector es
//       una struct necesito convertirla a un array de bytes. Este array de
//       bytes es el que paso a read.
//       Por otra parte, si se quiere leer un sector usaremos Sector que es un
//       std::array. Para leerlo necesito pasarlo a read.
//       std::span es una view de los arrays de C como de std::array
//       fusionando los dos interfaces.
//
// (RRR) ¿Para qué necesito un objeto de tipo Sector_driver?
//      Al empezar a implementar FAT32::Volume encuentro con que necesito
//      cargar sectores en memoria para poder leerlos: los cargo en FAT_area,
//      los cargo en File, ... y lo hago dentro de funciones generando el
//      siguiente problema:
//	    File::f()
//	    {
//		Sector s; // reserva 512 bytes
//		FAT_area::g(); // internamente reserva 512 bytes
//	    }
//	En este escenario mínimo estoy reservando 1K de RAM (y el atmega32 
//	solo tiene 2K).
//	Para evitar esto uso Sector_driver. Como alguien tiene que ser
//	responsable de gestionar la memoria usada para leer los sectores opto
//	por que sea Volume:
//	    class FAT32::Volume{
//		...
//		Sector_driver driver;
//	    };
//
//	Y cualquier función que quiera acceder a los bytes de este disco tendrá
//	que hacerlo obligatoriamente via Volume. 
//
namespace impl_of{
enum class Sector_driver_errno : uint8_t {
    ok = 0, // != 0 significa error
    read_error = 1,
    write_error = 2,
    lock_sector = 3
};


struct Sector_driver_struct{
    using Errno = Sector_driver_errno;

    Errno errno   : 6;
    bool modified : 1; // indica que el sector ha sido modificado en RAM
			// con lo que hay que hacerle un flush antes de leer
			// un nuevo sector
    uint8_t locked : 1;

    Sector_driver_struct() : errno{Errno::ok}
			    , modified{false}
			    , locked{0}{}
    
    void operator=(Errno e) {errno = e; }
    bool operator==(Errno e) const {return errno == e;}

    void lock() {locked = 1;}
    void unlock() {locked = 0;}


    bool ok() const {return errno == Errno::ok;}
    bool fail() const {return !ok(); }
    bool read_error() const {return errno == Errno::read_error;}
    bool write_error() const {return errno == Errno::write_error;}

    bool is_locked() const {return locked == 1;}
    bool is_unlocked() const {return locked == 0;}

    bool is_modified() const {return modified;}

};

// Garanticemos que el compilador no usa más memoria de la necesaria:
static_assert(sizeof(Sector_driver_struct) == 1);


template <typename SDCard, typename T>
struct Sector_driver_lock;

}

// Se puede bloquear la lectura de un sector para que el cliente pueda leer
// directamente el array cargado en memoria, evitando que otras llamadas a
// read/write modificaran su contenido o lo invalidaran.
//
//
// ¿Qué significa bloquear el driver?
// Que queremos acceder al array de bytes del sector directamente y no quiero
// que se modifique su contenido. Solo afecta a la función `read` no a write.
// Podré escribir el sector en memoria si quiero o no.
template <typename SDCard0> // Dispositivo físico de almacenamiento
class Sector_driver{
public:
// Types
    using SDCard = SDCard0;
    using Errno = impl_of::Sector_driver_struct::Errno;
    
    // Este es el tamaño del sector de la SD card que supongo (?) es igual al
    // tamaño del sector usado por FAT32. Pero Sector_driver es genérico y no
    // sabe si se va a usar para implementar FAT32 u otra cosa. Esta clase no
    // debe de saber nada de FAT32!!!
    static constexpr uint16_t sector_size = 512; 

    using size_type = uint16_t;  // máximo 512
    using Sector = std::array<uint8_t, sector_size>;
    using Sector_span = std::span<uint8_t, sector_size>;
    using Address = typename SDCard::Address;

    template <typename T>
    using Lock = impl_of::Sector_driver_lock<SDCard0, T>;

    static_assert(sector_size == SDCard::block_size);

    // Los reinterpret_cast solo funcionan si el micro es little endian, ya
    // que estamos leyendo sectores de FAT32 que son little endian.
    static_assert(std::endian::native == std::endian::little);
    

// Constructor
    Sector_driver(){}
    ~Sector_driver() { flush(); }

// Lectura del sector actual
    // Devuelve el tipo T del sector nsector de la posición pos
    template <Type::Integer Int>
    Int read(const Address& nsector, const size_type& pos);

    // Intenta leer buf.size() bytes almacenándolos en buf.
    // Los bytes los lee del sector `nsector` a partir de la posición pos.
    // Intenta hacer:	    
    //		buf[0..size()) = sector[pos.. pos + size())
    // Si no hay suficientes elementos en el sector devuelve un número menor
    // que buf.size() y anota en errno end_of_sector.
    size_type read( const Address& nsector, const size_type& pos
		  , std::span<uint8_t> buf);

    // Devuelve un puntero con el array de bytes que forman el sector
    // y bloquea el sector impidiendo que Sector_driver pueda modificar el
    // sector_ mientras esté bloqueado.
    uint8_t* lock_sector(const Address& nsector);
    void unlock() { errno_state_.unlock();}

    template <typename T>
    Lock<T> lock_sector_and_view_as(const Address& nsector);

// Escritura
    // Devuelve el número de Ints escritos (0 ó 1)
    template <Type::Integer Int>
    uint8_t write(const Address& nsector, const size_type& pos
	      , const Int& value);

    bool flush();


// Algorithms
    // Rellena los sectores [sector0 .. sector0 + n) con el valor `value`
    // Esto lo necesitamos, por ejemplo, cuando creamos un directorio todos
    // los valores tienen que ser 0x00.
    //
    // Devuelve el número de sectores inicializados con value.
    // (si todo va bien debería devolver `n`).
    Address fill_n(const Address& sector0, const Address& n, uint8_t value);

// Errno
    bool last_operation_ok() const {return errno() == Errno::ok; }
    bool last_operation_fail() const {return errno() != Errno::ok; }

    bool read_error() const {return errno() == Errno::read_error; }
    bool write_error() const {return errno() == Errno::write_error; }

    bool is_locked() const {return errno_state_.is_locked();}
    bool is_unlocked() const {return errno_state_.is_unlocked();}
	

private:
// Data
    atd::Uninitialized<Address> nsector_; // num. de sector en memoria
    Sector sector_;	// sector en memoria
    impl_of::Sector_driver_struct errno_state_;
    
// State/errno
    void state_modified(bool m) {errno_state_.modified = m;}
    void state_lock() {errno_state_.locked = true;}
    bool state_is_modified() const {return errno_state_.modified;}

    bool ok() {errno_state_.errno = Errno::ok; return true; }
    bool errno(Errno e) {errno_state_.errno = e; return false;}
    Errno errno() const {return errno_state_.errno;}

// Helpers
    // Lee el sector nsector de la SD card cargándolo en memoria
    bool read_sector(const Address& nsector);

// Static interface
    static bool read(const Address& add, Sector_span sector)
    {
	atd::ctrace<100>() << "SDCard::read(" << add << ")\n";

	auto r = SDCard::read(add, sector);
	return r.ok();
    }

    static bool write(const Address& add, Sector_span sector)
    {
	atd::ctrace<100>() << "SDCard::write(" << add << ")\n";

	auto r = SDCard::write(add, sector);
	return r.ok();
    }

// (DUDA) Estas dos funciones son las que dependen de la endiannes. 
//        En nuestro caso, las escrituras/lecturas en FAT32 son little-endian.
//        En caso de que el micro fuese big-endian hay que cambiar estas dos
//        funciones.
    template <Type::Integer Int>
    Int sector_read(size_type pos)
    { return *(reinterpret_cast<Int*>(&sector_[pos * sizeof(Int)]));}

    template <Type::Integer Int>
    void sector_write(size_type pos, const Int& value)
    { 
	*(reinterpret_cast<Int*>(&sector_[pos * sizeof(Int)])) = value;
	state_modified(true);
    }
};


template <typename SD>
bool Sector_driver<SD>::read_sector(const Address& nsector)
{
    if (is_locked()){
	atd::ctrace<3>() << "ERROR: Trying to access a locked sector\n";
	return errno(Errno::lock_sector);
    }

    if (nsector_ != nsector){
	if (!flush())
	    return false;

	if (!read(nsector, sector_))
	    return errno(Errno::read_error);

	state_modified(false);

	nsector_ = nsector;
    }

    return ok();
}



template <typename SD>
    template <Type::Integer Int>
Int Sector_driver<SD>::read(const Address& nsector, const size_type& pos)
{
    if (!read_sector(nsector))
	return 0;

    return sector_read<Int>(pos);
}


template <typename SD>
Sector_driver<SD>::size_type 
    Sector_driver<SD>::read( const Address& nsector, const size_type& pos
		  , std::span<uint8_t> buf)
{
    if (!read_sector(nsector))
	return 0;

    auto n = std::min(buf.size(), sector_size - pos);
    std::copy_n(sector_.begin() + pos, n, buf.begin());

    return n;
}





template <typename SD>
uint8_t* Sector_driver<SD>::lock_sector(const Address& nsector)
{ 
    atd::precondition<9>(is_unlocked(), "ERROR? Locking a locked sector");

    if (!read_sector(nsector))
	return nullptr;

    state_lock();

    return sector_.data();
}



template <typename SD>
    template <Type::Integer Int>
uint8_t Sector_driver<SD>::
	    write(const Address& nsector, const size_type& pos
		 , const Int& value)
{
    if (!read_sector(nsector))
	return 0;

    sector_write(pos, value);

    errno(Errno::ok);

    return 1;
}


template <typename SD>
bool Sector_driver<SD>::flush()
{
    if (!state_is_modified())
	return true;

    if (!write(nsector_, sector_))
	return errno(Errno::write_error);

    state_modified(false);

    return true;
}


template <typename SD>
Sector_driver<SD>::Address 
Sector_driver<SD>::fill_n(const Address& sector0
			, const Address& n, uint8_t value)
{
    if (n == 0) return 0;

    if (!flush())
	return 0;

    std::fill(sector_.begin(), sector_.end(), value);

    for (Address i = 0; i < n; ++i){
	if (!write(sector0 + i, sector_))
	    return i;
    }

    return n;
}



/***************************************************************************
 *			    SECTOR_DRIVER_LOCK
 *
 * Smart pointer para bloquear un sector de Sector_driver. Gestiona
 * desbloquear el sector en el destructor (básicamente es lo que hace).
 *
 ***************************************************************************/
namespace impl_of{

template < typename SDCard
	 , typename T>
struct Sector_driver_lock{
// Types
    using SD = Sector_driver<SDCard>;
    using Address   = typename SD::Address;
    using pointer   = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

// Data
    SD& sd; 
    pointer ptr;

// Constructors
    Sector_driver_lock(SD& sd0, const Address& nsector) : sd{sd0} 
    { ptr = reinterpret_cast<pointer>(sd.template lock_sector(nsector)); }

    ~Sector_driver_lock() { sd.unlock();}

// Pointer access
    reference operator*() { return *ptr; }
    const_reference operator*() const { return *ptr; }

    pointer operator->() {return ptr;}
    const_pointer operator->() const {return ptr;}

// State
    bool is_null() const {return ptr == nullptr;}
};

} // impl_of


template <typename SD>
    template <typename T>
auto Sector_driver<SD>::lock_sector_and_view_as(const Address& nsector)
-> typename Sector_driver<SD>::Lock<T>
{ return Lock<T>{*this, nsector}; }

}// namespace dev


#endif


