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

#ifndef __DEV_COUNTER_CD4017B_H__
#define __DEV_COUNTER_CD4017B_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz del contador CD4017B
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           Manuel Perez- 11/06/2019 Escrito
 *
 ****************************************************************************/
#include "not_generic.h"

namespace dev{

// TODO: parametrizarlo por 
//  opción 1: <Micro, num_pins>
//  opción 2: <Micro, Pin_clock, Pin_reset, ...>
template <uint8_t num_pin_clock,
          uint8_t num_pin_reset = 0,	// = 0 no conectado al mcu
          uint8_t num_pin_clock_inhibit = 0>
class Counter_CD4017B {
public:
    Counter_CD4017B()
    {
	reset_.write_zero();
	clock_.write_zero();
	clock_inhibit_.write_zero();
    }

    /// Pasa al siguiente estado. Cuenta uno más.
    void next() {
//	clock_.pulse_of_1us();
	clock_.write_one();
	//Micro::wait_us(1);  
	not_generic::wait_us(1);  
	clock_.write_zero();
    }

    /// Reset el contador, volviendo a encender el pin 0.
    void reset() 
    {
	//reset_.pulse_of_1us();
	reset_.write_one();
	//Micro::wait_us(1);  
	not_generic::wait_us(1);  
	reset_.write_zero();

    }

    /// Enable el contador.
    void enable() {clock_inhibit_.write_zero();}

    /// Disable el contador.
    void disable() {clock_inhibit_.write_one();}

private:
    // TODO: cambiarlo por:
    //	    Micro::Output_pin<num_pin_clock> clock_;
    //	    Aunque, mejor usar el interfaz static?
    //		    using Clock_pin = Micro::Pin<num_pin_clock>;
    //	    y en el constructor definirlo como salida??? 
    not_generic::Output_pin<num_pin_clock> clock_;
    not_generic::Output_pin<num_pin_reset> reset_;
    not_generic::Output_pin<num_pin_clock_inhibit> clock_inhibit_;
};



// Idioma: forzamos a que el usuario construya la clase con el orden de los
// pines adecuados.
template <uint8_t n>
struct Counter_CD4017B_pin_clock {};

template <uint8_t n>
struct Counter_CD4017B_pin_reset {};

template <uint8_t n>
struct Counter_CD4017B_pin_clock_inhibit {};


template <uint8_t num_pin_clock,
          uint8_t num_pin_reset = 0,        
          uint8_t num_pin_clock_inhibit = 0>
Counter_CD4017B<num_pin_clock, num_pin_reset, num_pin_clock_inhibit>
    counter_CD4017B(Counter_CD4017B_pin_clock<num_pin_clock>,
                    Counter_CD4017B_pin_reset<num_pin_reset>
		   =Counter_CD4017B_pin_reset<0>{},
                    Counter_CD4017B_pin_clock_inhibit<num_pin_clock_inhibit>
		   =Counter_CD4017B_pin_clock_inhibit<0>{})
{
    return Counter_CD4017B<num_pin_clock,
                           num_pin_reset,
                           num_pin_clock_inhibit>{};
}

}// namespace

#endif



