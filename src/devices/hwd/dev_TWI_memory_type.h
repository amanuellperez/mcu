// Copyright (C) 2020 Manuel Perez 
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

#ifndef __DEV_TWI_MEMORY_TYPE_H__
#define __DEV_TWI_MEMORY_TYPE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Concebimos el dispositivo TWI como una memoria.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    28/04/2020 v0.0
 *    12/12/2020 v0.1 
 *
 ****************************************************************************/
#include <cstddef>  // uint8_t
#include <array>
#include <algorithm>

#include <atd_memory.h>
		 
#include <mcu_TWI_master_ioxtream.h>

namespace dev{


// syntactic sugar
using __Mem_address = uint8_t;

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
 *  struct BMP280_id{
 *  // Data
 *      uint8_t id;
 *  
 *  // Memory
 *      static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
 *      static constexpr uint8_t address{0xD0}; // siempre uint8_t!!!
 *      static constexpr uint8_t size = 1;	
 *
 *  // Format functions: transforman el array de bytes 'mem' leido del 
 *  // dispositivo en la estructura, y viceversa.
 *  // mem <-> struct
 *      static void mem_to_struct(const std::array<uint8_t, size>& mem
 *  				, BMP280_id& st);
 *      static void struct_to_mem(BMP280_id& st, 
 *					const std::array<uint8_t, size>& mem);
 *  //...
 *  };
 *
 *  (2) Leemos/escribimos directamente la struct sin necesidad de usar el
 *  array de bytes intermedio. En este caso no es necesario definir las
 *  funciones de formato. En su lugar definimos operadores<< y >>. Hay que
 *  añadir el flag use_struct_as_mem, para indicar que no vamos a usar el
 *  array de bytes intermedio.
 *  struct BMP280_id{
 *  // Data
 *      uint8_t id;
 *  
 *  // Memory
 *      static constexpr atd::Memory_type mem_type = atd::Memory_type::read_only;
 *      static constexpr uint8_t address{0xD0};
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
struct TWI_memory_type {

// Configuración de la conexión con el dispositivo
    using TWI     = mcu::TWI_master_ioxtream<TWI_master>;
    using Slave_address = TWI_master::Address;
    using iostate = typename TWI::iostate;

// Tipos para configurar un segmento de memoria
// Un segmento de memoria queda definido dando:
//    using Memory_type = atd::Memory_type;
    using Mem_address = __Mem_address;
//    using Size        = uint8_t;

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
    static iostate mem_read(uint8_t* mem);

    // Escribe 'n' bytes de 'mem' a partir de la dirección 'address' en el 
    // dispositivo.
    template <Mem_address address, typename TWI_master::streamsize n>
    static iostate mem_write(const uint8_t* mem);

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
// avr_TWI_memory_type.cxx
// ------------------------------
namespace private_{
// Detecta si la clase T tiene el tipo use_struct_as_mem
template <typename T, typename = void>
struct has_use_struct_as_mem : std::false_type {};

template <typename T>
struct has_use_struct_as_mem<T, 
	std::void_t<decltype(T::use_struct_as_mem)> > : std::true_type {};
}// private_


template <typename TWI_master, typename TWI_master::Address slave_address>
template <__Mem_address mem_address, typename TWI_master::streamsize n>
inline TWI_memory_type<TWI_master, slave_address>::iostate
TWI_memory_type<TWI_master, slave_address>::mem_read(uint8_t* mem)
{
    TWI twi(slave_address);		    
     
    twi << mem_address;

    if (twi.error())
	return TWI::state();

    twi.read(mem, n);

    twi.close();	

    return TWI::state();
}


template <typename TWI_master, typename TWI_master::Address slave_address>
template <__Mem_address mem_address, typename TWI_master::streamsize n>
inline TWI_memory_type<TWI_master, slave_address>::iostate
TWI_memory_type<TWI_master, slave_address>::mem_write(const uint8_t* mem)
{
    TWI twi(slave_address);

    if (twi.error())
	return TWI::state();

    twi << mem_address;
    twi.write(mem, n);

    twi.close();

    return TWI::state();
}



template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
inline TWI_memory_type<TWI_master, slave_address>::iostate
TWI_memory_type<TWI_master, slave_address>::read(T& st)
{
    static_assert (atd::is_readable(T::mem_type));
    
    if constexpr (private_::has_use_struct_as_mem<T>())
	return read_with_optimization(st);

    else 
	return read_without_optimization(st);
}


template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
inline TWI_memory_type<TWI_master, slave_address>::iostate
TWI_memory_type<TWI_master, slave_address>::write(const T& st)
{
    static_assert (atd::is_writeable(T::mem_type));

    if constexpr (private_::has_use_struct_as_mem<T>())
	return write_with_optimization(st);

    else 
	return write_without_optimization(st);
}





template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
TWI_memory_type<TWI_master, slave_address>::iostate
TWI_memory_type<TWI_master, slave_address>::read_without_optimization(T& st)
{
    static_assert (atd::is_readable(T::mem_type));

    std::array<uint8_t, T::size> mem;

    mem_read<T::address, T::size>(mem.data());

    T::mem_to_struct(mem, st);

    return TWI::state();
}



// NOTA: Un error que puede generar dolores de cabeza encontrarlos es no
// inicializar 'mem'. Por eso opto por inicializarlo aqui, de esa forma queda
// garantizado que el usuario no comete ese error.
template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
TWI_memory_type<TWI_master, slave_address>::iostate
TWI_memory_type<TWI_master, slave_address>::write_without_optimization
								    (const T& st)
{
    static_assert (atd::is_writeable(T::mem_type));

    std::array<uint8_t, T::size> mem;
    std::fill(mem.begin(), mem.end(), uint8_t{0}); // ver nota
    T::struct_to_mem(st, mem);

    mem_write<T::address, T::size>(mem.data());

    return TWI::state();
    
}



template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
TWI_memory_type<TWI_master, slave_address>::iostate
TWI_memory_type<TWI_master, slave_address>::read_with_optimization(T& st)
{
    TWI twi(slave_address);
     
    twi << T::address;

    if (twi.error())
	return TWI::state();

    twi >> st;

    twi.close();

    return TWI::state();
}



template <typename TWI_master, typename TWI_master::Address slave_address>
template <typename T>
TWI_memory_type<TWI_master, slave_address>::iostate
TWI_memory_type<TWI_master, slave_address>::write_with_optimization
								   (const T& st)
{
    TWI twi(slave_address);
    
    if (twi.error())
	return TWI::state();

    twi << T::address << st;

    twi.close();

    return TWI::state();
    
}


}// namespace avr


#endif

