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

    static auto reg() { return Cfg::reg(); }
    using pos   = Cfg::bit_pos; // posiciones de los bits dentro de los registros
    using value = Cfg::value;

// Constructor
    SPI_basic() = delete;

// CTRLA
// CTRLA::DORD
    static void data_order_MSB();
    static void data_order_LSB();

// CTRLA::MASTER
    static void host_mode();
    static void client_mode();

// CTRLA::CLK2X
    static void clock_normal_speed();
    static void clock_double_speed();

// CTRLA::PRESC
    static void clock_peripheral_divide_by_2();
    static void clock_peripheral_divide_by_16();
    static void clock_peripheral_divide_by_64();
    static void clock_peripheral_divide_by_128();

// CTRLA::ENABLE
    static void enable();
    static void disable();
    
// CTRLB
// CTRLB::BUFEN
    static void buffer_mode_enable();
    static void buffer_mode_disable();

// TODO: CTRLB::BUFWR
// ¿cómo llamar a estas funciones?

// CTRLB::SSD
    static void enable_client_select_line();
    static void disable_client_select_line();

// CTRLB::MODE
    static void mode_0();
    static void mode_1();
    static void mode_2();
    static void mode_3();

// INTCRTL
// Interrupciones para buffer mode:
// INTCRTL::RXCIE
    static void enable_receive_complete_interrupt();
    static void disable_receive_complete_interrupt();

// INTCRTL::TXCIE
    static void enable_transfer_complete_interrupt();
    static void disable_transfer_complete_interrupt();

// INTCRTL::DREIE
    static void enable_data_register_empty_interrupt();
    static void disable_data_register_empty_interrupt();

// INTCRTL::SSIE
    static void enable_client_select_line_interrupt();
    static void disable_client_select_line_interrupt();

// INTCRTL::IE
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


// DATA
    static void data(uint8_t x);
    static uint8_t data();

};

// CTRLA
// CTRLA::DORD
template <typename C>
inline void SPI_basic<C>::data_order_MSB()
{ atd::write_bit<pos::DORD>::template to<0>::in(reg()->CTRLA); }

template <typename C>
inline void SPI_basic<C>::data_order_LSB()
{ atd::write_bit<pos::DORD>::template to<1>::in(reg()->CTRLA); }


// CTRLA::MASTER
template <typename C>
inline void SPI_basic<C>::client_mode()
{ atd::write_bit<pos::MASTER>::template to<0>::in(reg()->CTRLA); }

template <typename C>
inline void SPI_basic<C>::host_mode()
{ atd::write_bit<pos::MASTER>::template to<1>::in(reg()->CTRLA); }

// CTRLA::CLK2X
template <typename C>
inline void SPI_basic<C>::clock_normal_speed()
{ atd::write_bit<pos::CLK2X>::template to<0>::in(reg()->CTRLA); }

template <typename C>
inline void SPI_basic<C>::clock_double_speed()
{ atd::write_bit<pos::CLK2X>::template to<1>::in(reg()->CTRLA); }


// CTRLA::PRESC
template <typename C>
inline void SPI_basic<C>::clock_peripheral_divide_by_2()
{ reg()->CTRLA |= value::DIV4;}

template <typename C>
inline void SPI_basic<C>::clock_peripheral_divide_by_16()
{ reg()->CTRLA |= value::DIV16;}

template <typename C>
inline void SPI_basic<C>::clock_peripheral_divide_by_64()
{ reg()->CTRLA |= value::DIV64;}

template <typename C>
inline void SPI_basic<C>::clock_peripheral_divide_by_128()
{ reg()->CTRLA |= value::DIV128;}


// CTRLA::ENABLE
template <typename C>
inline void SPI_basic<C>::disable()
{ atd::write_bit<pos::ENABLE>::template to<0>::in(reg()->CTRLA); }

template <typename C>
inline void SPI_basic<C>::enable()
{ atd::write_bit<pos::ENABLE>::template to<1>::in(reg()->CTRLA); }

    
// CTRLB
// CTRLB::BUFEN
template <typename C>
inline void SPI_basic<C>::buffer_mode_enable()
{ atd::write_bit<pos::BUFEN>::template to<0>::in(reg()->CTRLB); }

template <typename C>
inline void SPI_basic<C>::buffer_mode_disable()
{ atd::write_bit<pos::BUFEN>::template to<1>::in(reg()->CTRLB); }


// TODO: CTRLB::BUFWR
// ¿cómo llamar a estas funciones?

// CTRLB::SSD
template <typename C>
inline void SPI_basic<C>::enable_client_select_line()
{ atd::write_bit<pos::SSD>::template to<0>::in(reg()->CTRLB); }

template <typename C>
inline void SPI_basic<C>::disable_client_select_line()
{ atd::write_bit<pos::SSD>::template to<1>::in(reg()->CTRLB); }


// CTRLB::MODE
template <typename C>
inline void SPI_basic<C>::mode_0()
{ reg()->CTRLB |= value::MODE_0;}

template <typename C>
inline void SPI_basic<C>::mode_1()
{ reg()->CTRLB |= value::MODE_1;}

template <typename C>
inline void SPI_basic<C>::mode_2()
{ reg()->CTRLB |= value::MODE_2;}

template <typename C>
inline void SPI_basic<C>::mode_3()
{ reg()->CTRLB |= value::MODE_3;}


// INTCRTL
// Interrupciones para buffer mode:
// INTCRTL::RXCIE
template <typename C>
inline void SPI_basic<C>::enable_receive_complete_interrupt()
{ atd::write_bit<pos::RXCIE>::template to<1>::in(reg()->INTCTRL); }

template <typename C>
inline void SPI_basic<C>::disable_receive_complete_interrupt()
{ atd::write_bit<pos::RXCIE>::template to<0>::in(reg()->INTCTRL); }


// INTCRTL::TXCIE
template <typename C>
inline void SPI_basic<C>::enable_transfer_complete_interrupt()
{ atd::write_bit<pos::TXCIE>::template to<1>::in(reg()->INTCTRL); }

template <typename C>
inline void SPI_basic<C>::disable_transfer_complete_interrupt()
{ atd::write_bit<pos::TXCIE>::template to<0>::in(reg()->INTCTRL); }


// INTCRTL::DREIE
template <typename C>
inline void SPI_basic<C>::enable_data_register_empty_interrupt()
{ atd::write_bit<pos::DREIE>::template to<1>::in(reg()->INTCTRL); }

template <typename C>
inline void SPI_basic<C>::disable_data_register_empty_interrupt()
{ atd::write_bit<pos::DREIE>::template to<0>::in(reg()->INTCTRL); }


// INTCRTL::SSIE
template <typename C>
inline void SPI_basic<C>::enable_client_select_line_interrupt()
{ atd::write_bit<pos::SSIE>::template to<1>::in(reg()->INTCTRL); }

template <typename C>
inline void SPI_basic<C>::disable_client_select_line_interrupt()
{ atd::write_bit<pos::SSIE>::template to<0>::in(reg()->INTCTRL); }


// INTCRTL::IE
// La siguiente es para no buffer mode
template <typename C>
inline void SPI_basic<C>::enable_interrupt()
{ atd::write_bit<pos::IE>::template to<1>::in(reg()->INTCTRL); }

template <typename C>
inline void SPI_basic<C>::disable_interrupt()
{ atd::write_bit<pos::IE>::template to<0>::in(reg()->INTCTRL); }


// TODO: INTFLAGS
// TODO: INTFLAGS::IF
// TODO: INTFLAGS::WRCOL
// TODO: INTFLAGS::RXCIF
// TODO: INTFLAGS::TXCIF
// TODO: INTFLAGS::DREIF
// TODO: INTFLAGS::SSIF
// TODO: INTFLAGS::BUFOVF


// DATA
template <typename C>
inline void SPI_basic<C>::data(uint8_t x)
{ reg()->DATA = x;}

template <typename C>
inline uint8_t SPI_basic<C>::data()
{ return reg()->DATA;}


}// mega0_
 
#endif


