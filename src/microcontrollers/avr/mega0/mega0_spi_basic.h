// Copyright (C) 2024 Manuel Perez 
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

#ifndef __MEGA0_SPI_BASIC_H__
#define __MEGA0_SPI_BASIC_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del SPI	
 *
 * HISTORIA
 *    Manuel Perez
 *    06/11/2024 Implementación mínima
 *
 ****************************************************************************/
#include <atd_bit.h>

namespace mega0_{

template <typename Cfg0>
class SPI_basic{
public:
// syntactic sugar
    using Cfg = Cfg0;

// Constructor
    SPI_basic() = delete;

// TODO: CTRLA
// CTRLA::DORD
    static void data_order_MSB();
    static void data_order_LSB();

// TODO: CTRLA::MASTER
    static void host_mode();
    static void client_mode();

// TODO: CTRLA::CLK2X
    static void clock_normal_speed();
    static void clock_double_speed();

// TODO: CTRLA::PRESC
    static void clock_peripheral_divide_by_2();
    static void clock_peripheral_divide_by_16();
    static void clock_peripheral_divide_by_64();
    static void clock_peripheral_divide_by_128();

// TODO: CTRLA::ENABLE
    static void enable();
    static void disable();
    
// TODO: CTRLB
// TODO: CTRLB::BUFEN
    static void buffer_mode_enable();
    static void buffer_mode_disable();

// TODO: CTRLB::BUFWR
// ¿cómo llamar a estas funciones?

// TODO: CTRLB::SSD
    static void enable_client_select_line();
    static void disable_client_select_line();

// TODO: CTRLB::MODE
    static void mode_0();
    static void mode_1();
    static void mode_2();
    static void mode_3();

// TODO: INTCRTL
// Interrupciones para buffer mode:
// TODO: INTCRTL::RXCIE
    static void enable_receive_complete_interrupt();
    static void disable_receive_complete_interrupt();

// TODO: INTCRTL::TXCIE
    static void enable_transfer_complete_interrupt();
    static void disable_transfer_complete_interrupt();

// TODO: INTCRTL::DREIE
    static void enable_data_register_empty_interrupt();
    static void disable_data_register_empty_interrupt();

// TODO: INTCRTL::SSIE
    static void enable_client_select_line_interrupt();
    static void disable_client_select_line_interrupt();

// TODO: INTCRTL::IE
// La siguiente es para no buffer mode
    static void enable_interrupt();
    static void disable_interrupt();

// TODO: INTFLAGS
// TODO: INTFLAGS::IF
// TODO: INTFLAGS::WRCOL
// TODO: INTFLAGS::RXCIF
// TODO: INTFLAGS::TXCIF
// TODO: INTFLAGS::DREIF
// TODO: INTFLAGS::SSIF
// TODO: INTFLAGS::BUFOVF


// TODO: DATA
    static void data(uint8_t x);
    static uint8_t data();

private:
    
};
}// mega0_
 
#endif


