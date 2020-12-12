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
 *    12/12/2020 v0.1
 *
 ****************************************************************************/

#include <atd_memory.h>
#include <cstddef>  // std::byte
#include "avr_TWI_master_ioxtream.h"

namespace avr{


// syntactic sugar
using __Mem_address = std::byte;

/*!
 *  \brief  Dispositivo de memoria.
 *
 *  Oculta todo el protocolo TWI.
 *
 *  Para ver un ejemplo de implementación ver BMP280 ó DS1307
 *
 *  Para leer/escribir una zona de memoria del dispositivo definirla como
 *  struct. Dos formas de hacerlo:
 *
 *  (1) Leemos/escribimos un array de bytes y luego lo formateamos.
 *  struct __BMP280_id{
 *  // Data
 *      std::byte id;
 *  
 *  // Memory
 *      static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
 *      static constexpr std::byte address{0xD0}; // siempre std::byte!!!
 *      static constexpr uint8_t size = 1;	
 *
 *  // Format functions: transforman el array de bytes 'mem' leido del 
 *  // dispositivo en la estructura, y viceversa.
 *  // mem <-> struct
 *      static void mem_to_struct(const std::array<std::byte, size>& mem
 *  				, __BMP280_id& st);
 *      static void struct_to_mem(__BMP280_id& st, 
 *					const std::array<std::byte, size>& mem);
 *  //...
 *  };
 *
 *  (2) Leemos/escribimos directamente la struct sin necesidad de usar el
 *  array de bytes intermedio. En este caso no es necesario definir las
 *  funciones de formato. En su lugar definimos operadores<< y >>. Hay que
 *  añadir el flag use_struct_as_mem, para indicar que no vamos a usar el
 *  array de bytes intermedio.
 *  struct __BMP280_id{
 *  // Data
 *      std::byte id;
 *  
 *  // Memory
 *      static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
 *      static constexpr std::byte address{0xD0};
 *      static constexpr uint8_t size = 1;	
 *
 *  // Leemos y escribimos:
 *  template <typename Oxtream>
 *  friend Oxtream& operator<<(Oxtream& out, const __BMP280_id& st);
 *  template <typename Ixtream>
 *  friend Ixtream& operator>>(Ixtream& in, const __BMP280_id& st);
 *
 *  // NO OLVIDAR EL FLAG!!!
 *  static constexpr bool use_struct_as_mem = true;
 *
 *  };
 */
template <typename TWI_master, typename TWI_master::Address slave_address>
struct TWI_master_memory_type {

    using TWI = avr::TWI_master_ioxtream<TWI_master>;
    using iostate = typename TWI::iostate;

    using Mem_address = __Mem_address;    // Dirección de una zona de memoria dentro 
				    // del dispositivo.

    // Lee una zona de memoria en el
    // device conectado via TWI y la devuelve codificada en la estructura T
    // correspondiente. La clase T contiene toda la información de cómo es
    // almacenada en el device.
    // Params: [out] st: objeto leido.
    template <typename T>
    static iostate read(T& st);

    template <typename T>
    static iostate write(const T& st);


    // Funciones sin codificar: leemos/escribimos una zona de memoria como
    // array de bytes.
    // (RRR) ¿por qué pasar como parámetro de plantilla n?
    // En principio esta clase la estoy usando para manejar dispositivos (BMP,
    // DS1307...) y no memoria. En este tipo de dispositivos conozco en tiempo 
    // de compilación dónde y cuánto quiero leer. 
    // Lee 'n' bytes del dispositivo a partir de la dirección 'address'
    // guardando el resultado en 'mem'.
    template <Mem_address address, typename TWI_master::streamsize n>
    static iostate mem_read(std::byte* mem);

    // Escribe 'n' bytes de 'mem' a partir de la dirección 'address' en el 
    // dispositivo.
    template <Mem_address address, typename TWI_master::streamsize n>
    static iostate mem_write(const std::byte* mem);

private:
    template <typename T>
    static iostate read_without_optimization(T& st);

    template <typename T>
    static TWI::iostate write_without_optimization(const T& st);

    template <typename T>
    static iostate read_with_optimization(T& st);

    template <typename T>
    static TWI::iostate write_with_optimization(const T& st);

};



// ------------------------------
// avr_TWI_master_memory_type.cxx
// ------------------------------
// Detecta si la clase T tiene el tipo use_struct_as_mem
template <typename T, typename = void>
struct __has_use_struct_as_mem : std::false_type {};

template <typename T>
struct __has_use_struct_as_mem<T, 
	std::void_t<decltype(T::use_struct_as_mem)> > : std::true_type {};



template <typename TWI_master, typename TWI_master::Address slave_address>
template <__Mem_address mem_address, typename TWI_master::streamsize n>
inline TWI_master_memory_type<TWI_master, slave_address>::iostate
TWI_master_memory_type<TWI_master, slave_address>::mem_read(std::byte* mem)
{
    TWI twi;		    
    twi.open(slave_address);
     
    twi << mem_address;

    if (twi.error())
	return TWI::state();

    twi.read(n);
    twi.read(mem, n);

    twi.close();	

    return TWI::state();
}


template <typename TWI_master, typename TWI_master::Address slave_address>
template <__Mem_address mem_address, typename TWI_master::streamsize n>
inline TWI_master_memory_type<TWI_master, slave_address>::iostate
TWI_master_memory_type<TWI_master, slave_address>::mem_write(const std::byte* mem)
{
    TWI twi;
    twi.open(slave_address);

    if (twi.error())
	return TWI::state();

    twi << mem_address;
    twi.write(mem, n);

    twi.close();

    return TWI::state();
}



template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
inline TWI_master_memory_type<TWI_master, slave_address>::iostate
TWI_master_memory_type<TWI_master, slave_address>::read(T& st)
{
    static_assert (atd::is_readable(T::mem_type));
    
    if constexpr (__has_use_struct_as_mem<T>())
	return read_with_optimization(st);

    else 
	return read_without_optimization(st);
}


template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
inline TWI_master_memory_type<TWI_master, slave_address>::iostate
TWI_master_memory_type<TWI_master, slave_address>::write(const T& st)
{
    static_assert (atd::is_writeable(T::mem_type));

    if constexpr (__has_use_struct_as_mem<T>())
	return write_with_optimization(st);

    else 
	return write_without_optimization(st);
}





template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
TWI_master_memory_type<TWI_master, slave_address>::iostate
TWI_master_memory_type<TWI_master, slave_address>::read_without_optimization
									(T& st)
{
    static_assert (atd::is_readable(T::mem_type));

    std::array<std::byte, T::size> mem;

    mem_read<T::address, T::size>(mem.data());

    T::mem_to_struct(mem, st);

    return TWI::state();
}



// NOTA: Un error que puede generar dolores de cabeza encontrarlos es no
// inicializar 'mem'. Por eso opto por inicializarlo aqui, de esa forma queda
// garantizado que el usuario no comete ese error.
template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
TWI_master_memory_type<TWI_master, slave_address>::iostate
TWI_master_memory_type<TWI_master, slave_address>::write_without_optimization
								    (const T& st)
{
    static_assert (atd::is_writeable(T::mem_type));

    std::array<std::byte, T::size> mem;
    std::fill(mem.begin(), mem.end(), std::byte{0}); // ver nota
    T::struct_to_mem(st, mem);

    mem_write<T::address, T::size>(mem.data());

    return TWI::state();
    
}



template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
TWI_master_memory_type<TWI_master, slave_address>::iostate
TWI_master_memory_type<TWI_master, slave_address>::read_with_optimization(T& st)
{
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



template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
TWI_master_memory_type<TWI_master, slave_address>::iostate
TWI_master_memory_type<TWI_master, slave_address>::write_with_optimization
								   (const T& st)
{
    TWI twi;
    twi.open(slave_address);
    
    if (twi.error())
	return TWI::state();

    twi << T::address << st;

    twi.close();

    return TWI::state();
    
}


}// namespace avr


#endif

