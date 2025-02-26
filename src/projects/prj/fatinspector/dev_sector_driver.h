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
// (RRR) ¿Por qué Sector_driver?
//      El static interface es el usado para acceder al dispositivo físico,
//      pero ¿para qué necesito un objeto de tipo Sector_driver?
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

    Sector_driver_state state : 7;
    uint8_t locked : 1;

    Sector_driver_struct() : state{State::ok}, locked{0}{}
    
    void operator=(State st) {state = st; }
    bool operator==(State st) const {return state == st;}

    void lock() {locked = 1;}
    void unlock() {locked = 0;}


    bool ok() const {return state == State::ok;}
    bool fail() const {return state == State::fail;}
    bool read_error() const {return state == State::read_error;}

    bool is_locked() const {return locked == 1;}
    bool is_unlocked() const {return locked == 0;}


};

}

// Se puede bloquear la lectura de un sector para que el cliente pueda leer
// directamente el array cargado en memoria, evitando que otras llamadas a
// read/write modificaran su contenido o lo invalidaran.
template <typename SDCard0> // Dispositivo físico de almacenamiento
class Sector_driver{
public:
    using SDCard = SDCard0;

    using State_struct = impl_of::Sector_driver_struct;
    using State = State_struct::State;

    static constexpr uint16_t sector_size = 512; // TODO: ???

    using size_type = uint16_t;  // máximo 512
    using Sector = std::array<uint8_t, sector_size>;
    using Sector_span = std::span<uint8_t, sector_size>;
    using Address = typename SDCard::Address;

    static_assert(sector_size == SDCard::block_size);

    // Los reinterpret_cast solo funcionan si el micro es little endian, ya
    // que estamos leyendo sectores de FAT32 que son little endian.
    static_assert(std::endian::native == std::endian::little);
    

// Constructor
    Sector_driver(){}

// Lectura del sector actual
    // Devuelve el tipo T del sector nsector de la posición pos
    template <Type::Integer Int>
    Int read(const Address& nsector, const size_type& pos);

    template <typename T>
    T* sector_pointer_as(const Address& nsector);

// Escritura
    // Devuelve el número de Ints escritos (0 ó 1)
    // (DUDA) Puede que una mejora en eficiencia sea mirar si en el sector_
    //        actual se ha escrito o no algo. Si no se ha escrito nada no es
    //        necesario hacer el write correspondiente ahorrando esa operación
    //        que puede(?) ser costosa en tiempo. 
    //        El control se podría hacer via un flag del state para ahorrar
    //        memoria.
    template <Type::Integer Int>
    uint8_t write(const Address& nsector, const size_type& pos
	      , const Int& value);

    bool flush();

// State
    void lock();
    void unlock() { state_.unlock();}


// Info
    bool ok() const {return state_ == State::ok;;}
    bool fail() const {return state_ == State::fail; }
    bool is_locked() const {return state_.is_locked();}
    bool is_unlocked() const {return state_.is_unlocked();}
	

// Static interface
// (DUDA) ¿Necesito realmente que esta función sea public?
// Lo usa MBR. Reescribir MBR y hacer private esta función
    static bool read(const Address& add, Sector_span sector)
    {
	atd::ctrace<100>() << "SDCard::read(" << add << ")\n";

	auto r = SDCard::read(add, sector);
	return r.ok();
    }
private:
// Data
    atd::Uninitialized<Address> nsector_; // num. de sector en memoria
    Sector sector_;	// sector en memoria
    State_struct state_;
    


// Helpers
    bool is_unlocked_and_ok() const;


// Static interface
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
    { *(reinterpret_cast<Int*>(&sector_[pos * sizeof(Int)])) = value;}
};



template <typename SD>
    template <Type::Integer Int>
Int Sector_driver<SD>::read(const Address& nsector, const size_type& pos)
{
    if (!is_unlocked_and_ok())
	return 0;

    if (nsector_ != nsector){
	if (read (nsector, sector_)){
	    state_ = State::ok;
	    nsector_ = nsector;
	}
	else
	    state_ = State::read_error;
    }

//	return *(reinterpret_cast<Int*>(&sector_[pos * sizeof(Int)]));
    return sector_read<Int>(pos);
}


template <typename SD>
    template <typename T>
T* Sector_driver<SD>::sector_pointer_as(const Address& nsector)
{ 
    atd::precondition<9>(is_unlocked(), 
				"An error? You forgot to lock the sector");

    if (nsector_ != nsector){
	if (read (nsector, sector_))
	    state_ = State::ok;
	else{
	    state_ = State::read_error;
	    return nullptr;
	}
    }

    return reinterpret_cast<T*>(&sector_); 
}



template <typename SD>
    template <Type::Integer Int>
uint8_t Sector_driver<SD>::
	    write(const Address& nsector, const size_type& pos
		 , const Int& value)
{
    if (!is_unlocked_and_ok())
	return 0;

    if (nsector_ != nsector){
	if (!write(nsector_, sector_)){
	    state_ = State::write_error;
	    return 0;
	}

	if (read (nsector, sector_)){
	    nsector_ = nsector;
	}
	else{
	    state_ = State::read_error;
	    return 0;
	}
    }

    sector_write(pos, value);
    state_ = State::ok;

    return 1;
}

template <typename SD>
bool Sector_driver<SD>::flush()
{
    if (!is_unlocked_and_ok())
	return false;

    if (!write(nsector_, sector_)){
	state_ = State::write_error;
	return false;
    }

    return true;
}

template <typename SD>
inline void Sector_driver<SD>::lock() 
{
    if constexpr (atd::trace_level<9>()){
	if (is_locked())
	    atd::ctrace<3>() << "ERROR: locking a locked sector\n";
    }
    state_.lock();
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

}// namespace dev


#endif


