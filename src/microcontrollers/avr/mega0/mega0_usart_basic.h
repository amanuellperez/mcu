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
 *               No pruebo todas las funciones. TODO: probarlas.
 *
 ****************************************************************************/
#include <atd_bit.h>


namespace mega0_{

template <typename Registers>
class USART_basic{
public:
// syntactic sugar
    using Reg = Registers;

    static auto reg() { return Reg::reg(); }
    using pos   = Reg::bit_pos; // posiciones de los bits dentro de los registros
    using value = Reg::value;

// pines a los que está conectado el SPI
    static constexpr uint8_t TxD_pin = Reg::TxD_pin;
    static constexpr uint8_t RxD_pin = Reg::RxD_pin;
    static constexpr uint8_t XCK_pin = Reg::XCK_pin;
    static constexpr uint8_t XDIR_pin= Reg::XDIR_pin;
    
// Constructor
    USART_basic() = delete;

// RXDATAL
    static uint8_t receive_data_register_low_byte();

// RXDATAH
    // Como este byte incluye los bits de status, lo leemos
    // entero y luego las funciones correspondientes traducen
    // este byte en el flag correspondiente.
    static uint8_t receive_data_register_high_byte();

// RXDATAH::RXCIF
// Este flag es igual a STATUS::RXCIF. Lo implemento allí.

// TODO: RXDATAH::BUFOVF
// static bool receive_buffer_overflow(uint8_t rxdatah);
//
// TODO: RXDATAH::FERR
// static bool receive_frame_error(uint8_t rxdatah);
// TODO: RXDATAH::PERR
// static bool receive_parity_error(uint8_t rxdatah);
// TODO: RXDATAH::DATA8

// TXDATAL
    static void transmit_data_register_low_byte(uint8_t x);

// TXDATAH
    static void transmit_data_register_high_byte(uint8_t x);

// STATUS
// STATUS::RXCIF
    static bool are_there_unread_data();

// STATUS::TXCIF
    static bool is_transmit_complete();
    static void clear_transmit_complete_flag();

// STATUS::DREIF
    static bool is_transmit_data_register_empty();

// TODO: STATUS::RXSIF
// TODO: STATUS::ISFIF
// TODO: STATUS::BDF
// TODO: STATUS::WFB

// CTRLA
// CTRLA::RXCIE
    static void enable_receive_complete_interrupt();
    static void disable_receive_complete_interrupt();

// CTRLA::TXCIE
    static void enable_transmit_complete_interrupt();
    static void disable_transmit_complete_interrupt();

// CTRLA::DREIE
    static void enable_data_register_empty_interrupt();
    static void disable_data_register_empty_interrupt();

// CTRLA::RXSIE
    static void enable_receiver_start_frame_interrupt();
    static void disable_receiver_start_frame_interrupt();

// TODO: CTRLA::LBME
// TODO: CTRLA::ABEIE
// TODO: CTRLA::RS485

// CTRLB
// CTRLB::RXEN
    static void enable_receiver();
    static void disable_receiver();
    static bool is_receiver_enable();


// CTRLB::TXEN
    static void enable_transmitter();
    static void disable_transmitter();
    static bool is_transmitter_enable();

// TODO: CTRLB::SFDEN
// TODO: CTRLB::ODME
// CTRLB::RXMODE
    // DUDA: 23.5.7: La datasheet indica que si se configura el en modo
    // asíncrono, hay que configurar el receiver en normal speed mode. ¿Es
    // correcto o una errata y querían decir síncrono? @_@
    static void receiver_normal_speed_mode();
    static void receiver_double_speed_mode();
    static void receiver_generic_auto_baud_mode();
    static void receiver_LIN_constrained_auto_baud_mode();

// TODO: CTRLB::MPCM

// CTRLC - normal mode
    static void asynchronous_mode();
//    static void synchronous_mode();
//    static void infrared_mode();
//    static void host_SPI_mode();

// CTRLC::PMODE
    static void parity_mode_disabled();
    static void parity_mode_even();
    static void parity_mode_odd();

// CTRLC::SBMODE
    static void one_stop_bit();
    static void two_stop_bit();

// CTRLC::CHSIZE
    static void character_size_5();
    static void character_size_6();
    static void character_size_7();
    static void character_size_8();

    // 9 bit, low byte first
    static void character_size_9L();

    // 9 bit, high byte first
    static void character_size_9H();
    
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
template <typename C>
inline uint8_t USART_basic<C>::receive_data_register_low_byte()
{ return reg()->RXDATAL;}

// RXDATAH
template <typename C>
inline uint8_t USART_basic<C>::receive_data_register_high_byte()
{ return reg()->RXDATAH;}


// TXDATAL
template <typename C>
inline void USART_basic<C>::transmit_data_register_low_byte(uint8_t x)
{ reg()->TXDATAL = x; }

// TXDATAH
template <typename C>
inline void USART_basic<C>::transmit_data_register_high_byte(uint8_t x)
{ reg()->TXDATAH = x; }

// STATUS

// STATUS::RXCIF
template <typename C>
inline bool USART_basic<C>::are_there_unread_data()
{ return atd::is_one_bit<pos::RXCIF>::of(reg()->STATUS); }

// STATUS::DREIF
template <typename C>
inline bool USART_basic<C>::is_transmit_data_register_empty()
{ return atd::is_one_bit<pos::DREIF>::of(reg()->STATUS); }

// STATUS::TXCIF
template <typename C>
inline bool USART_basic<C>::is_transmit_complete()
{ return atd::is_one_bit<pos::TXCIF>::of(reg()->STATUS); }

template <typename C>
inline void USART_basic<C>::clear_transmit_complete_flag()
{ atd::write_bit<pos::TXCIF>::template to<1>::in(reg()->STATUS); }


// CTRLA
// CTRLA::RXCIE
template <typename C>
inline void USART_basic<C>::enable_receive_complete_interrupt()
{ atd::write_bit<pos::RXCIE>::template to<1>::in(reg()->CTRLA); }

template <typename C>
inline void USART_basic<C>::disable_receive_complete_interrupt()
{ atd::write_bit<pos::RXCIE>::template to<0>::in(reg()->CTRLA); }


// CTRLA::TXCIE
template <typename C>
inline void USART_basic<C>::enable_transmit_complete_interrupt()
{ atd::write_bit<pos::TXCIE>::template to<1>::in(reg()->CTRLA); }

template <typename C>
inline void USART_basic<C>::disable_transmit_complete_interrupt()
{ atd::write_bit<pos::TXCIE>::template to<0>::in(reg()->CTRLA); }


// CTRLA::DREIE
template <typename C>
inline void USART_basic<C>::enable_data_register_empty_interrupt()
{ atd::write_bit<pos::DREIE>::template to<1>::in(reg()->CTRLA); }

template <typename C>
inline void USART_basic<C>::disable_data_register_empty_interrupt()
{ atd::write_bit<pos::DREIE>::template to<0>::in(reg()->CTRLA); }


// CTRLA::RXSIE
template <typename C>
inline void USART_basic<C>::enable_receiver_start_frame_interrupt()
{ atd::write_bit<pos::RXSIE>::template to<1>::in(reg()->CTRLA); }

template <typename C>
inline void USART_basic<C>::disable_receiver_start_frame_interrupt()
{ atd::write_bit<pos::RXSIE>::template to<0>::in(reg()->CTRLA); }



// CTRLB
// -----
// CTRLB::RXEN
template <typename C>
inline void USART_basic<C>::enable_receiver()
{
    Reg::enable_Rx_pin();
    atd::write_bit<pos::RXEN>::template to<1>::in(reg()->CTRLB); 
}

template <typename C>
inline void USART_basic<C>::disable_receiver()
{ atd::write_bit<pos::RXEN>::template to<0>::in(reg()->CTRLB); }

template <typename C>
inline bool USART_basic<C>::is_receiver_enable()
{ return atd::is_one_bit<pos::RXEN>::of(reg()->CTRLB); }


// CTRLB::TXEN
template <typename C>
inline void USART_basic<C>::enable_transmitter()
{ 
    Reg::enable_Tx_pin();
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

// CTRLB::RXMODE
template <typename C>
inline void USART_basic<C>::receiver_normal_speed_mode()
{ reg()->CTRLB |= value::RXMODE_NORMAL;}

template <typename C>
inline void USART_basic<C>::receiver_double_speed_mode()
{ reg()->CTRLB |= value::RXMODE_CLK2X;}

template <typename C>
inline void USART_basic<C>::receiver_generic_auto_baud_mode()
{ reg()->CTRLB |= value::RXMODE_GENAUTO;}

template <typename C>
inline void USART_basic<C>::receiver_LIN_constrained_auto_baud_mode()
{ reg()->CTRLB |= value::RXMODE_LINAUTO;}


// TODO: CTRLB::MPCM

// CTRLC - normal mode
template <typename C>
inline void USART_basic<C>::asynchronous_mode()
{ reg()->CTRLC |= value::CMODE_ASYNCHRONOUS; }

//template <typename C>
//inline void USART_basic<C>::synchronous_mode()
//{ sin probar!!!
//    Reg::enable_XCK_pin(); <-- OJO: puede ser in or out!!!
//    reg()->CTRLC |= value::CMODE_SYNCHRONOUS; 
//}
//
//template <typename C>
//inline void USART_basic<C>::infrared_mode()
//
//template <typename C>
//inline void USART_basic<C>::host_SPI_mode()

// CRTLC::PMODE
template <typename C>
inline void USART_basic<C>::parity_mode_disabled()
{ reg()->CTRLC |= value::PMODE_DISABLED; }

template <typename C>
inline void USART_basic<C>::parity_mode_even()
{ reg()->CTRLC |= value::PMODE_EVEN; }

template <typename C>
inline void USART_basic<C>::parity_mode_odd()
{ reg()->CTRLC |= value::PMODE_ODD; }


// CTRLC::SBMODE
template <typename C>
inline void USART_basic<C>::one_stop_bit()
{ atd::write_bit<pos::SBMODE>::template to<0>::in(reg()->CTRLC); }

template <typename C>
inline void USART_basic<C>::two_stop_bit()
{ atd::write_bit<pos::SBMODE>::template to<1>::in(reg()->CTRLC); }

// CTRLC::CHSIZE
template <typename C>
inline void USART_basic<C>::character_size_5()
{ reg()->CTRLC |= value::CHSIZE_5BIT; }

template <typename C>
inline void USART_basic<C>::character_size_6()
{ reg()->CTRLC |= value::CHSIZE_6BIT; }

template <typename C>
inline void USART_basic<C>::character_size_7()
{ reg()->CTRLC |= value::CHSIZE_7BIT; }

template <typename C>
inline void USART_basic<C>::character_size_8()
{ reg()->CTRLC |= value::CHSIZE_8BIT; }

template <typename C>
inline void USART_basic<C>::character_size_9L()
{ reg()->CTRLC |= value::CHSIZE_9BITL; }

template <typename C>
inline void USART_basic<C>::character_size_9H()
{ reg()->CTRLC |= value::CHSIZE_9BITH; }


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


