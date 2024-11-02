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

#ifndef __MEGA0_UART_H__
#define __MEGA0_UART_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Driver de UART
 *
 * HISTORIA
 *    Manuel Perez
 *    02/11/2024 UART_8bits, calculo del baud rate
 *
 ****************************************************************************/

#include "mega0_import_avr.h"	// wait_cpu_ticks
#include "mega0_clock_frequencies.h"	// clk_per

namespace mega0_{
/***************************************************************************
 *			    CALCULO DEL BAUD RATE
 ***************************************************************************/
namespace private_{

// Table 23-1, datasheet
// Funciones: baud_register --> baud_rate
inline 
constexpr uint32_t baud_rate_normal_mode(uint32_t f_clk_per, uint32_t baud_reg)
{ return 4 * ( f_clk_per /  baud_reg); } // simplificado:
//{ return (64 * f_clk_per) / (16 * baud_reg); }

inline 
constexpr uint32_t 
    baud_rate_double_speed_mode(uint32_t f_clk_per, uint32_t baud_reg)
{ return 8 * ( f_clk_per /  baud_reg); } // simplificado:
//{ return (64 * f_clk_per) / (8 * baud_reg); }

// Funciones inversas:  baud_rate --> baud_register
inline 
constexpr uint32_t baud_reg_normal_mode(uint32_t f_clk_per, uint32_t baud_rate)
{ return 4 * (f_clk_per /  baud_rate); } // simplificado:
// { return (64 * f_clk_per) / (16 * baud_rate); }

inline 
constexpr 
uint32_t baud_reg_double_speed_mode(uint32_t f_clk_per, uint32_t baud_rate)
{ return 8 * (f_clk_per /  baud_rate); } // simplificado:
// { return (64 * f_clk_per) / (8 * baud_rate); }

enum class UART_receiver_mode{
    normal, double_speed, error
};

// Devuelve [normal_mode?, BAUD register]
inline constexpr 
std::pair<UART_receiver_mode, uint16_t> baud_register(uint32_t f_clk_per, uint32_t baud_rate)
{
    auto baud_reg = baud_reg_normal_mode(f_clk_per, baud_rate);
    if (baud_reg >= 64)
	return {UART_receiver_mode::normal, static_cast<uint16_t>(baud_reg)};

    baud_reg = baud_reg_double_speed_mode(f_clk_per, baud_rate);
    if (baud_reg >= 64)
	return {UART_receiver_mode::double_speed, 
					    static_cast<uint16_t>(baud_reg)};

    return {UART_receiver_mode::error, 0};
}




// Devuelve: 
//  true: bien configurado
//  false: error al configurarlo
template <typename USART>
inline constexpr
bool cfg_baud_rate(uint32_t f_clk_per, uint32_t baud_rate)
{
    auto [mode, baud_reg] = baud_register(f_clk_per, baud_rate);

    USART::baud(baud_reg);

    switch (mode){
	break; case UART_receiver_mode::normal: 
					USART::receiver_normal_speed_mode();
	break; case UART_receiver_mode::double_speed: 
					USART::receiver_double_speed_mode();
	break; case UART_receiver_mode::error:	    
					return false;
				    
    }

    return true;
}


}// private_
 

/***************************************************************************
 *				UART_8bits
 ***************************************************************************/
namespace default_{
struct UART_basic_cfg{
    static constexpr uint32_t baud_rate = 9'600;
    static constexpr bool parity_mode_enable = false;
    static constexpr bool one_stop_bit = true; 
};

} // default_

// (RRR) ¿Cómo se va a comunicar mi avr con otro dispositivo via UART?
//       El protocolo lo conoce el hardwador, conociendose por tanto en tiempo
//       de compilación. Por eso pasa Cfg como parámetro de template
template <typename USART_basic_cfg, typename Cfg = default_::UART_basic_cfg>
void UART_basic_cfg()
{                                
    using UART = USART_basic<USART_basic_cfg>;

    UART::asynchronous_mode();
    private_::cfg_baud_rate<UART>(clk_per(), Cfg::baud_rate);
    
    if constexpr (Cfg::parity_mode_enable)
	UART::parity_mode_enable();
    else
	UART::parity_mode_disabled();

    if constexpr (Cfg::one_stop_bit)
	UART::one_stop_bit();
    else
	UART::two_stop_bit();

    UART::character_size_8(); 
}


// UART_8bits
// ----------
template <typename Cfg>
class UART_8bits {
public:
// Types
    using Basic = USART_basic<Cfg>;

// Constructor
    UART_8bits() = delete;

    // TODO: pasar como parámetros la configuración
    static void init() { UART_basic_cfg<Cfg>(); }

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
    static int flush(uint16_t time_in_cpu_ticks);

// Interrupts
// TODO:
//    static void enable_interrupt_unread_data();
//    static void disable_interrupt_unread_data();

};

// Receiver
// --------
template <typename C>
inline void UART_8bits<C>::enable_receiver()
{ Basic::enable_receiver();}

template <typename C>
inline void UART_8bits<C>::disable_receiver()
{ Basic::disable_receiver();}

template <typename C>
inline bool UART_8bits<C>::is_receiver_enable()
{ return Basic::is_receiver_enable();}

template <typename C>
inline uint8_t UART_8bits<C>::receive_data_register()
{ return Basic::receive_data_register_low_byte(); }

template <typename C>
inline bool UART_8bits<C>::are_there_unread_data()
{ return Basic::are_there_unread_data();}


// Transmitter
// -----------
template <typename C>
inline void UART_8bits<C>::enable_transmitter()
{ Basic::enable_transmitter();}

template <typename C>
inline void UART_8bits<C>::disable_transmitter()
{ Basic::disable_transmitter();}

template <typename C>
inline bool UART_8bits<C>::is_transmitter_enable()
{ return Basic::is_transmitter_enable();}

template <typename C>
inline bool UART_8bits<C>::is_ready_to_transmit()
{ return Basic::is_transmit_data_register_empty(); }

template <typename C>
inline void UART_8bits<C>::transmit_data_register(uint8_t c)
{ Basic::transmit_data_register_low_byte(c); }

// TODO: es mentira que se espere time_in_cpu_ticks, sino
// 100*time_in_cpu_ticks. ¿Dividimos time_in_cpu_ticks / 100? 
// Revisar mcu::UART_iostream. Ahora esta llamando flush(10)!!! Si se divide
// 10 / 100 sale 0.
template <typename C>
int UART_8bits<C>::flush(uint16_t time_in_cpu_ticks)
{
    // while (!Basic::is_transmit_complete()) 
    while (!Basic::is_transmit_data_register_empty())
    {
	wait_cpu_ticks(100); // mínimo 16 ticks
	if (time_in_cpu_ticks == 0)
	    return -1;

	--time_in_cpu_ticks;
    }

//    Basic::clear_transmit_complete_flag();

    return 0;

}

// Interrupts
// ----------
//template <typename C>
//inline void UART_8bits<C>::enable_interrupt_unread_data()
//{ Basic::enable_interrupt_unread_data();}
//
//template <typename C>
//inline void UART_8bits<C>::disable_interrupt_unread_data()
//{ Basic::disable_interrupt_unread_data();}

}// mega0_

#endif


