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

#ifndef __DEV_ONE_WIRE_H__
#define __DEV_ONE_WIRE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Implementación del protocolo 1-Wire de Maxim
 *
 * HISTORIA
 *    Manuel Perez
 *    23/08/2023 v0.0 Lo escribo de acuerdo a la explicación dada en la
 *		      datasheet del DS18B20.
 *
 ****************************************************************************/
namespace dev{

// Pin: pin que usamos para comunicarnos con los dispositivos one-wire.
//      Es de entrada-salida.
template <typename Pin0> 
class One_wire{
public:

// Transaction sequence
// Step 1: Initializacion
    void init();

// Step 2: ROM Commands
// quién está conectado? 
    search_rom

// dime tú nombre (address)? (cuando solo hay un slave conectado)
    read_rom

// Atención: quiero hablar con 
    match_rom

// Atención todo el mundo!
    skip_rom

// quién tiene el `alarm flag` set?
    alarm_search


// Step 3: Function Commands
    write_bit_zero
    write_bit_one
    read_bit

// Los del DS18B20 son
// convert_T
// write_scratchpad
// read_scratchpad
// copy_scratchpad
// recall_e2
// read_power_supply

    
private:
    using Pin = Pin0;
    
};


template <typename Pin> 
void One_wire::init()
{
    Pin::as_output();
    Pin::write_one();
    Miniclock::
}



}// namespace


#endif


