// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __AVR_TWI_MASTER_MEMORY_TYPE_H__
#define __AVR_TWI_MASTER_MEMORY_TYPE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Concebimos el dispositivo TWI como una memoria.
 *
 *  - TODO: este fichero no depende del avr. Es una capa colocada encima de
 *  TWI. ¿meterlo en namespace mcu? Tampoco quiero crear demasiados
 *  namespaces. (???)
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    28/04/2020 v0.0
 *
 ****************************************************************************/

#include <atd_memory.h>
#include "avr_TWI_master_ioxtream.h"

namespace avr{


// TODO: quiero que sea <-- BORRAR (28/04/2020)
//	write(TWI, q, n); <-- es más estandard!!! TWI sería el flujo!!!
//  en lugar de 
//	write(TWI, slave_addres, q, n);
//  luego le tenemos que pasar el slave de alguna otra forma.
//  1. write(TWI<slave_address>, q, n);
//  2. write(TWI_slave{slave}, q, n);
//  3. ???
//template <typename T, uint8_t bsz>
//static TWI_master_ioxtream<T, bsz>::streamsize
//    write(TWI_master_ioxtream<T, bsz>& twi,
//	  typename TWI_master_ioxtream<T, bsz>::Address slave_address,
//	  const std::byte* q,
//	  typename TWI_master_ioxtream<T, bsz>::streamsize n)
//{
//    twi.open(slave_address);
//     
//    return twi.write(q, n); // twi.close() lo llama el destructor
//}

// Dispositivo de tipo memoria.
template <typename avr::TWI_basic::Address slave_address,
	 typename TWI_master>
//          typename avr::TWI_basic::streamsize TWI_buffer_size>
struct TWI_master_memory_type {

    // using TWI = avr::TWI_master_ioxtream<avr::TWI_basic, TWI_buffer_size>;
    using TWI = avr::TWI_master_ioxtream<TWI_master>;

    using iostate = typename TWI::iostate;

    // TODO: sacarla fuera de la struct.
    // Lee una zona de memoria en el
    // device conectado via TWI y la devuelve codificada en la estructura T
    // correspondiente. La clase T contiene toda la información de cómo es
    // almacenada en el device.
    // Params: [out] st: objeto leido.
    // Return value: On success 0, on error TWI::error.
    template <typename T>
    static iostate read(T& st)
    {
	static_assert (atd::is_readable(T::mem_type));

	TWI twi;
	twi.open(slave_address);
	 
	twi << T::address;

	if (twi.error())
	    return TWI::state();

	twi.read(T::size);

	twi >> st;

	twi.close();

	return TWI::state();
    }


    template <typename T>
    static TWI::iostate write(const T& st)
    {
	static_assert (atd::is_writeable(T::mem_type));

	TWI twi;
	twi.open(slave_address);
	

	twi << T::address << st;

	twi.close();

	return TWI::state();
	
    }


};


}// namespace avr


#endif

