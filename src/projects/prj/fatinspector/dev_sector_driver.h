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
enum class Sector_driver_state :uint8_t {
    ok = 0,
    fail = 1,	    // last operation fail?
    read_error = 2,
    write_error = 3 // DUDA: usar bits o números? 0x01 << 3???
};


struct Sector_driver_struct{
    using State = Sector_driver_state;

    Sector_driver_state state : 6;
    bool modified  : 1; // indica que el sector ha sido modificado en RAM
			// con lo que hay que hacerle un flush antes de leer
			// un nuevo sector
    uint8_t locked : 1;

    Sector_driver_struct() : state{State::ok}
			    , modified{false}
			    , locked{0}{}
    
    void operator=(State st) {state = st; }
    bool operator==(State st) const {return state == st;}

    void lock() {locked = 1;}
    void unlock() {locked = 0;}


    bool ok() const {return state == State::ok;}
    bool fail() const {return state == State::fail;}
    bool read_error() const {return state == State::read_error;}

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

    using State_struct = impl_of::Sector_driver_struct;
    using State = State_struct::State;
    
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

    // Devuelve un puntero con el array de bytes que forman el sector
    // y bloquea el sector impidiendo que Sector_driver pueda modificar el
    // sector_ mientras esté bloqueado.
    uint8_t* lock_sector(const Address& nsector);
    void unlock() { state_.unlock();}

    template <typename T>
    Lock<T> lock_sector_and_view_as(const Address& nsector);

// Escritura
    // Devuelve el número de Ints escritos (0 ó 1)
    template <Type::Integer Int>
    uint8_t write(const Address& nsector, const size_type& pos
	      , const Int& value);

    bool flush();



// State
    bool ok() const {return state_ == State::ok;;}
    bool fail() const {return state_ == State::fail; }
    bool is_locked() const {return state_.is_locked();}
    bool is_unlocked() const {return state_.is_unlocked();}
	

private:
// Data
    atd::Uninitialized<Address> nsector_; // num. de sector en memoria
    Sector sector_;	// sector en memoria
    State_struct state_;
    


// Helpers
    bool is_unlocked_and_ok() const;

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
	state_.modified = true;
    }
};


template <typename SD>
bool Sector_driver<SD>::read_sector(const Address& nsector)
{
    if (!is_unlocked_and_ok())
	return false;

    if (nsector_ != nsector){
	if (!flush())
	    return false;

	if (read(nsector, sector_)){
	    state_ = State::ok;
	    state_.modified = false;

	    nsector_ = nsector;
	}
	else
	    state_ = State::read_error;
    }

    return state_ == State::ok;
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
uint8_t* Sector_driver<SD>::lock_sector(const Address& nsector)
{ 
    atd::precondition<9>(is_unlocked(), "ERROR? Locking a locked sector");

    if (!read_sector(nsector))
	return nullptr;

    state_.lock();

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
    state_ = State::ok;

    return 1;
}


template <typename SD>
bool Sector_driver<SD>::flush()
{
    if (!state_.is_modified())
	return true;

    if (state_.fail()){
	atd::ctrace<3>() << "ERROR: sector state == fail!!!\n";
	return false;
    }

    if (!write(nsector_, sector_)){
	state_ = State::write_error;
	return false;
    }

    state_.modified = false;

    return true;
}



template <typename SD>
bool Sector_driver<SD>::is_unlocked_and_ok() const
{
    if (is_locked()){
	atd::ctrace<3>() << "ERROR: Trying to access a locked sector\n";
	state_.fail();
	return false;
    }

    if (state_.fail()){
	atd::ctrace<3>() << "ERROR: sector state == fail!!!\n";
	return false;
    }

    return true;
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


