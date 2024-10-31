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

#ifndef __MEGA0_USART_BASIC_H__
#define __MEGA0_USART_BASIC_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor de USART	
 *
 * COMENTARIOS
 *	Voy a definir una clase genérica USART_basic que toma como parámetro 
 *  la configuración de ese USART en concreto. De esa forma podre definir:
 *	    using USART0 = USART_basic<Cfg0>;
 *	    using USART1 = USART_basic<Cfg1>;
 *	    ...
 *
 *	El problema con este planteamiento es el de siempre: el compilador
 *  genera una clase DIFERENTE para cada parámetro Cfg0, Cfg1, ... generando
 *  código que no necesito. 
 *	Sin embargo, con los traductores en los avr no debería de surgir ese
 *  problema ya que al ser meros traductores:
 *	1) no tienen estado = son interfaces puros
 *	2) todas las funciones deberían ser inline
 *	   
 *	Pero hay que tener en cuenta esto a la hora de implementar estos
 *  traductores.
 *
 * HISTORIA
 *    Manuel Perez
 *    29/10/2024 Implementación mínima
 *
 ****************************************************************************/
#include <atd_bit.h>


namespace mega0_{

template <typename Cfg0>
class USART_basic{
public:
// syntactic sugar
    using Cfg = Cfg0;

    static auto reg() { return Cfg::reg(); }
    using pos = Cfg::bit_pos; // posiciones de los bits dentro de los registros

// Constructor
    USART_basic() = delete;

// RXDATAL
// TODO

// RXDATAH
// TODO: RXDATAH::RXCIF
// TODO: RXDATAH::RXCIF
// TODO: RXDATAH::FERR
// TODO: RXDATAH::PERR
// TODO: RXDATAH::DATA8

// TXDATAL
    static void data_register_low_byte(uint8_t x);

// TXDATAH
    static void data_register_high_byte(uint8_t x);

// STATUS
// TODO: STATUS::RXCIF
// TODO: STATUS::TXCIF

// STATUS::DREIF
    static bool is_transmit_data_empty();

// TODO: STATUS::RXSIF
// TODO: STATUS::ISFIF
// TODO: STATUS::BDF
// TODO: STATUS::WFB

// CTRLA
// TODO: CTRLA::RXCIE
// TODO: CTRLA::TXCIE
// TODO: CTRLA::DREIE
// TODO: CTRLA::RXSIE
// TODO: CTRLA::LBME
// TODO: CTRLA::ABEIE
// TODO: CTRLA::RS485

// CTRLB
// TODO: CTRLB::RXEN
// static void enable_receiver() 
// static void disable_receiver() 
// static bool is_receiver_enable()
//
//
// CTRLB::TXEN
    static void enable_transmitter();
    static void disable_transmitter();
    static bool is_transmitter_enable();

// TODO: CTRLB::SFDEN
// TODO: CTRLB::ODME
// TODO: CTRLB::RXMODE
// TODO: CTRLB::MPCM

// CTRLC - normal mode
// TODO: CTRLC::CMODE
// TODO: CTRLC::PMODE
// TODO: CTRLC::SBMODE
// TODO: CTRLC::CHSIZE

// CTRLC - host SPI mode
// TODO: CTRLC::CMODE
// TODO: CTRLC::UDORD
// TODO: CTRLC::UCPHA

// BAUD
    static void baud(uint16_t value);
    static uint16_t baud();

// CTRLD
// TODO: CTRLD::ABW

// DBGCTRL
// TODO: DBGCTRL::DBGRUN

// EVCTRL
// TODO: EVCTRL::IREI

// TXPLCTRL
// TODO

// RXPLCTRL
// TODO

};

// RXDATAL
// TODO

// RXDATAH
// TODO

// TXDATAL
template <typename C>
inline void USART_basic<C>::data_register_low_byte(uint8_t x)
{ reg()->TXDATAL = x; }

// TXDATAH
template <typename C>
inline void USART_basic<C>::data_register_high_byte(uint8_t x)
{ reg()->TXDATAH = x; }

// STATUS

// STATUS::DREIF
template <typename C>
inline bool USART_basic<C>::is_transmit_data_empty()
{ return atd::is_one_bit<pos::DREIF>::of(reg()->STATUS); }



// CTRLA
// TODO

// CTRLB
// -----
// CTRLB::TXEN
template <typename C>
inline void USART_basic<C>::enable_transmitter()
{ 
    Cfg::enable_Tx_pin();
    atd::write_bit<pos::TXEN>::template to<1>::in(reg()->CTRLB); 
}

template <typename C>
inline void USART_basic<C>::disable_transmitter()
{ atd::write_bit<pos::TXEN>::template to<0>::in(reg()->CTRLB); }

template <typename C>
inline bool USART_basic<C>::is_transmitter_enable()
{ return atd::is_one_bit<pos::TXEN>::of(reg()->CTRLB); }


// TODO: CTRLB::SFDEN
// TODO: CTRLB::ODME
// TODO: CTRLB::MPCM

// CTRLC - normal mode
// TODO

// CTRLC - host SPI mode
// TODO

// BAUD
// ----
template <typename C>
inline void USART_basic<C>::baud(uint16_t value)
{ reg()->BAUD = value; }

template <typename C>
inline uint16_t USART_basic<C>::baud() 
{ return reg()->BAUD; }

// CTRLD
// TODO

// DBGCTRL
// TODO

// EVCTRL
// TODO

// TXPLCTRL
// TODO

// RXPLCTRL
// TODO

}// namespace
 

#endif


