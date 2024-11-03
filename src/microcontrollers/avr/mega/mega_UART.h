// Copyright (C) 2019-2024 Manuel Perez 
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

#ifndef __MEGA_UART_H__
#define __MEGA_UART_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Driver del UART.
 *  
 *  (RRR) ¿por qué definir UART_8bits?
 *
 *	1) En la practica, al implementar UART_iostream, daba por supuesto que
 *	   estabamos que el character size es de 8 bits.
 *
 *	2) Necesito un interfaz genérico para UART. Las clases que haya aquí
 *	   definen dicho interfaz.
 *
 *	3) La forma de leer/transmitir datos de 8 a 9 bits es diferente. La
 *	   implementación UART_8bits y UART_9bits será diferente.
 *
 *	DUDA: Sospecho que no debe ser dificil fusionar la implementación de
 *	UART_7bits con UART_8bits, pero como de momento no necesito UART_7bits
 *	no lo implemento.
 *
 * HISTORIA
 *    Manuel Perez
 *    17/09/2023 Reestructurado
 *    02/11/2024 UART_8bits
 *
 ****************************************************************************/

#include "mega_UART_basic.h"
#include "mega_import_avr.h"	// clock_frequency_in_hz

namespace mega_{
/***************************************************************************
 *				UART_8bits
 ***************************************************************************/
// Esta función configura el UART tanto en 8 bits como en los demás character
// size. Por eso no puede ser UART_8bits::init
// TODO: f_clock -> f_cpu() 
namespace impl_of{
template <typename USART, typename Cfg>
bool UART_cfg()
{                                
    USART::asynchronous_mode();

    USART::template baud_speed<f_cpu(), Cfg::baud_rate, Cfg::max_error>();
    
    if constexpr (Cfg::parity_mode_enable)
	USART::parity_mode_enable();
    else
	USART::parity_mode_disabled();

    if constexpr (Cfg::one_stop_bit)
	USART::one_stop_bit();
    else
	USART::two_stop_bit();

// character size
    if constexpr (Cfg::character_size == 8)
	USART::character_size_8(); 

    else if constexpr (Cfg::character_size == 5)
	USART::character_size_5(); 

    else if constexpr (Cfg::character_size == 6)
	USART::character_size_6(); 

    else if constexpr (Cfg::character_size == 7)
	USART::character_size_7(); 

    else
	static_assert(true, "Wrong value of character_size");

    // TODO: ¿cómo elegir character size == 9? 
    // Hay dos opciones: 9L o 9H

    return true;
}

} // impl_of


// UART_8bits
// ----------
// DUDA: 2 posibles formas de implementarlo.
//  1) Heredar de UART_basic. En este caso UART_8bits tendría acceso directo a
//     todas las funciones de UART_basic.
//  2) Como wrapper. Queda un interfaz más limpio porque controlamos qué
//     funciones son visibles y podemos dar acceso a UART_basic vía Basic.
//     De momento pruebo con esta segunda opción por tener más control sobre
//     el interfaz de UART_8bits.
class UART_8bits {
public:
// Types
    using USART = UART_basic;

// Constructor
    UART_8bits() = delete;

    template <typename UART_8bits_cfg>
    static bool init();

// Receiver
    static void enable_receiver();
    static void disable_receiver();
    static bool is_receiver_enable();

    static uint8_t receive_data_register();
    static bool are_there_unread_data();

// Transmitter
    static void enable_transmitter();
    static void disable_transmitter();
    static bool is_transmitter_enable();

    static void transmit_data_register(uint8_t c);
    static bool is_ready_to_transmit();

    // Hace un flush del flujo (fundamental llamar a esta función antes de 
    // dormir el microcontrolador. En caso de no llamarla se genera basura)
    static int flush(uint16_t time_out_ms);

// Interrupts
    static void enable_interrupt_unread_data();
    static void disable_interrupt_unread_data();

};

// init
// ----
template <typename UART_8bits_cfg>
bool UART_8bits::init()
{ return impl_of::UART_cfg<USART, UART_8bits_cfg>(); }

// Receiver
// --------
inline void UART_8bits::enable_receiver()
{ USART::enable_receiver();}

inline void UART_8bits::disable_receiver()
{ USART::disable_receiver();}

inline bool UART_8bits::is_receiver_enable()
{ return USART::is_receiver_enable();}

inline uint8_t UART_8bits::receive_data_register()
{ return USART::data_register(); }

inline bool UART_8bits::are_there_unread_data()
{ return USART::are_there_unread_data();}


// Transmitter
// -----------
inline void UART_8bits::enable_transmitter()
{ USART::enable_transmitter();}

inline void UART_8bits::disable_transmitter()
{ USART::disable_transmitter();}

inline bool UART_8bits::is_transmitter_enable()
{ return USART::is_transmitter_enable();}

inline bool UART_8bits::is_ready_to_transmit()
{ return USART::is_data_register_empty(); }

inline void UART_8bits::transmit_data_register(uint8_t c)
{ USART::data_register(c); }

// Interrupts
// ----------
inline void UART_8bits::enable_interrupt_unread_data()
{ USART::enable_interrupt_unread_data();}

inline void UART_8bits::disable_interrupt_unread_data()
{ USART::disable_interrupt_unread_data();}


}// namespace
 


#endif

