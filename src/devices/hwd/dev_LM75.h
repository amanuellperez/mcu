// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __DEV_LM75_H__
#define __DEV_LM75_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Driver del termómetro LM75.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *	    Manuel Perez- 29/08/2019 v0.0
 *
 *	    TODO: lo interesante de este termómetro es la salida O.S. que
 *	    ahora no programo. Añadir esto (ver datasheet para ejemplos
 *	    de cómo controlar un ventilador o un termostato).
 *
 *
 ****************************************************************************/
#include <atd_bit.h>

#include <avr_TWI.h>
#include "not_generic.h"

namespace dev{

// Esta clase es temporal para probar el TWI. La buena está en dev.
template <uint8_t addr>
class Thermometer_LM75{
public:
    // TODO: sacar esta enum fuera y definirla como 
    // using Error = __LM75_Error;
    /// Errores que puede dar la lectura de la temperatura.
    enum Error {
	ok = 0,   // no ha sucedido ningún error
	start_fail,    // el termómetro no envía ACK al enviar el start
	select_temperature_reg_fail, // error al intentar seleccionar el temperature_reg
	repeated_start_fail,	// no envía ACK al enviar repeated start
	receive_temperature_fail    // error al intentar recibir la temperatura
    };

    /// Actualiza el valor de la temperatura.
    /// Le preguntamos al LM75 la temperatura que hay, guardándola en 
    /// T().
    /// Devuelve 0 si todo va bien, y distinto de cero (un código de Error)
    /// si algo ha fallado.
    // TODO: eliminar el volatile. Probarlo en tlog.
    uint8_t refresh() volatile;

    // TODO: usar Q24.8 o algo asi para codificar la parte decimal y devolver
    // la temperatura completa, no a cachos.
    /// Devuelve la parte entera de la temperatura medida en ºC.
    uint8_t T() const volatile {return temp_[0];}

    /// Devuelve la parte decimal de la temperatura. Este termómetro
    /// solo da .0 ó .5 como parte decimal.
    uint8_t T_d() const volatile
    {
	if (atd::bit<7>::of(temp_[1]))
	    return 5;
	else 
	    return 0;
    }


private:
    // La dirección del LM75_1 es 1001|address, siendo address de 3 bits.
    constexpr static uint8_t address = (0b1001000 | addr);

    // Pointer register (datasheet, pag. 13)
    constexpr static std::byte temperature_reg   {0x00};
    constexpr static std::byte configuration_reg {0x01};
    constexpr static std::byte T_hyst_reg        {0x02};
    constexpr static std::byte T_os_reg          {0x03};
    constexpr static std::byte product_id_reg    {0x07};

    uint8_t temp_[2];	// 2 bytes con la temperatura
};


template <uint8_t addr>
uint8_t Thermometer_LM75<addr>::refresh() volatile
{
    not_generic::TWI::Start start;
    if (!start)
	return Error::start_fail;

    // Seleccionamos el registro de temperatura
    if (not_generic::TWI::send(address, temperature_reg) != 1)
	return Error::select_temperature_reg_fail;

    // Reenviamos otro start (vamos a leer)
    not_generic::TWI::send_repeated_start();
    if (not_generic::TWI::send_repeated_start_fail())
	return Error::repeated_start_fail;

    if (not_generic::TWI::receive(address, reinterpret_cast<volatile std::byte*>(temp_), 2) != 2u)
	return Error::receive_temperature_fail;
    
    return 0;	// ok
}


}// namespace


#endif

