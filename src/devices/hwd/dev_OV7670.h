// Copyright (C) 2023 Manuel Perez 
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

#ifndef __DEV_OV7670_H__
#define __DEV_OV7670_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor para la cámara OV7670
 *
 * HISTORIA
 *    Manuel Perez
 *    01/12/2023 Primeros experimentos
 *
 ****************************************************************************/
#include <stdint.h>

namespace dev{

// Result
namespace impl_of{

enum class OV7670_state{
    ok,
    twi_fail_connection,
    write_register_index_fail,
    write_register_data_fail,
    read_register_index_fail,
    read_register_data_fail
};

}// impl_of

// Como el slave_address es único, solo es posible tener una cámara de este
// estilo conectada. Por eso todas las funciones son static.
template <typename Micro0, typename TWI_master_ioxtream0>
class OV7670{
public:
    using Micro      = Micro0;
    using TWI	     = TWI_master_ioxtream0;
    using TWI_master = typename TWI::TWI_master;
    using Address    = typename TWI::Address;
    using Result     = impl_of::OV7670_state;

    static constexpr Address slave_address = 0x21; // es única
	
    // Miramos a ver si hay comunicación con el twi-device
    // Return: true (found device); false (no response)
    static bool probe();

// Commands
    static bool reset();

// Funciones de bajo nivel para acceder a los registros
    // register[i] = x;
    static bool write_register(uint8_t i, uint8_t x);

    // x = register[i];
    static uint8_t read_register(uint8_t i);

// Gestión de errores
    // En caso de que la última operación haya fallado, aquí se pueden
    // encontrar los detalles.
    static Result result_last_operation() {return result_;}
    static bool last_operation_is_ok() {return result_ == Result::ok;}
    static bool last_operation_fail() {return !last_operation_is_ok();}

private:
    inline static Result result_;

// Cfg
    static constexpr uint8_t null_uint8_t = 0xBA; // 0xBAD!!!
    static constexpr uint16_t null_uint16_t= 0xBAD0; 
};

// TODO: escribir array de registros. Esperar 300 ms al final
template <typename M, typename T>
bool OV7670<M, T>::write_register(uint8_t i, uint8_t x)
{
    TWI twi{slave_address};
    if (twi.error()) {
	result_ = Result::twi_fail_connection;
	return false;
    }

    twi << i;
    if (twi.error()) {
	result_ = Result::write_register_index_fail;
	return false;
    }

    twi << x;
    if (twi.error()) {
	result_ = Result::write_register_data_fail;
	return false;
    }

    twi.close();

    Micro::wait_ms(1); //TODO: según la datasheet máximo 300 ms en cambiar un registro!!!

    result_ = Result::ok;
    return true;
}


template <typename M, typename T>
uint8_t OV7670<M, T>::read_register(uint8_t i)
{
    TWI twi{slave_address};
    if (twi.error()) {
	result_ = Result::twi_fail_connection;
	return null_uint8_t;
    }

    twi << i;
    if (twi.error()) {
	result_ = Result::read_register_index_fail;
	return null_uint8_t;
    }

    twi.close();
    Micro::wait_ms(1);  // FUNDAMENTAL!!!

    uint8_t res{};
    twi.open(slave_address);
    twi >> res;

    if (twi.error()) {
	result_ = Result::read_register_data_fail;
	return null_uint8_t;
    }

    twi.close();
    Micro::wait_ms(1); 
    
    result_ = Result::ok;
    return res;
}



template <typename M, typename T>
inline bool OV7670<M, T>::probe()
{ return TWI_master::probe(slave_address); }

template <typename M, typename T>
bool OV7670<M, T>::reset()
{
    result_ = Result::ok;

    if (write_register(0x12, 0x80) == false)
	return false;

    Micro::wait_ms(100);    // TODO: segun la datasheet 1 ms máximo
			
    return true;
    
}

}// dev
 
#endif
