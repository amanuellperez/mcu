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

/*!
 *  \brief  Dispositivo de memoria.
 *
 *  Oculta todo el protocolo TWI.
 *
 *  Para ver un ejemplo de implementación ver BMP280 ó DS1307
 *
 */
template <avr::TWI_basic::Address slave_address,
	 typename TWI_master>
struct TWI_master_memory_type {

    using TWI = avr::TWI_master_ioxtream<TWI_master>;

    using iostate = typename TWI::iostate;

    // Lee una zona de memoria en el
    // device conectado via TWI y la devuelve codificada en la estructura T
    // correspondiente. La clase T contiene toda la información de cómo es
    // almacenada en el device.
    // Params: [out] st: objeto leido.
    // Return value: On success 0, on error TWI::error.
    template <typename T>
    static iostate read(T& st);

    template <typename T>
    static TWI::iostate write(const T& st);

};



// ------------------------------
// avr_TWI_master_memory_type.cxx
// ------------------------------
template <avr::TWI_basic::Address slave_address, typename TWI_master>
template <typename T>
TWI_master_memory_type<slave_address, TWI_master>::iostate
TWI_master_memory_type<slave_address, TWI_master>::read(T& st)
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


template <avr::TWI_basic::Address slave_address, typename TWI_master>
template <typename T>
TWI_master_memory_type<slave_address, TWI_master>::iostate
TWI_master_memory_type<slave_address, TWI_master>::write(const T& st)
{
    static_assert (atd::is_writeable(T::mem_type));

    TWI twi;
    twi.open(slave_address);
    

    twi << T::address << st;

    twi.close();

    return TWI::state();
    
}


}// namespace avr


#endif

