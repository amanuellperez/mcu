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

#ifndef __MEGA0_CFG_H__
#define __MEGA0_CFG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Configuración de la familia de micros mega-0 avr.	
 *
 * HISTORIA
 *    Manuel Perez
 *    18/10/2024 Empezando...
 *		 DUDAS: creo (???) que los registros de todos los micros de
 *		 una misma familia se llaman igual. Por ello basta con definir
 *		 una única configuración para todos ellos comunes (salvo en
 *		 pines, que hay que clasificarlos por número de pins).
 *
 ****************************************************************************/

#include <type_traits>

#include <avr/io.h>

namespace mega0_{
namespace cfg{

/***************************************************************************
 *				PINS    
 ***************************************************************************/
// CONFIGURACIÓN DE LOS PINES
// --------------------------
// La idea es numerar los pines de acuerdo a su posición en el chip
// y no de acuerdo a los puertos B, C y D.

// impl_of::pins
// -------------
// Cosas comunes a todos los mega0
namespace impl_of{
struct pins{
    static constexpr auto pullupen_mask = PORT_PULLUPEN_bm;
};
}// impl_of


// pins_40
// -------
// Pines del atmega4809 de 40 pins
struct pins_40 : impl_of::pins{

// datasheet: pag 20, nota 4:
// The 40-pin version of the ATmega4809 is using the die of the 48-pin 
// ATmega4809 but offers fewer connected pads. For this reason, 
// the pins PB[5:0] and PC[7:6] must be disabled (INPUT_DISABLE) 
// or enable pull-ups (PULLUPEN).
//
// Esto lo ejecuto en esta función init que solo es propia del chip de 40
// pins.
static constexpr void init()
{
    // PB[5:0] disabled
    PORTB.PIN0CTRL |= PORT_ISC_INPUT_DISABLE_gc; 
    PORTB.PIN1CTRL |= PORT_ISC_INPUT_DISABLE_gc; 
    PORTB.PIN2CTRL |= PORT_ISC_INPUT_DISABLE_gc; 
    PORTB.PIN3CTRL |= PORT_ISC_INPUT_DISABLE_gc; 
    PORTB.PIN4CTRL |= PORT_ISC_INPUT_DISABLE_gc; 

    // PC[7:6] disabled
    PORTC.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc; 
    PORTC.PIN7CTRL |= PORT_ISC_INPUT_DISABLE_gc; 
}

template <uint8_t n>
static constexpr 
bool is_a_valid_pin(){
    return 
    (n == 1) or (n == 2) or (n == 3) or (n == 4)
    or (n == 7) or (n == 8) or (n ==  9) 
    or (n == 10) or (n == 11) or (n == 12) or (n == 13) or (n == 14) or (n == 15) 
    or (n == 16) or (n == 19) 
    or (n == 20) or (n == 21) or (n == 22) or (n == 23) or (n == 24) or (n == 25) 
    or (n == 26) or (n == 27) or (n == 28) or (n == 29) 
    or (n == 33) or (n == 34) or (n == 35) 
    or (n == 36) or (n == 37) or (n == 38) or (n == 39) or (n == 40);
}

template <uint8_t n>
static constexpr 
auto PORT()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return &PORTC;
    if constexpr (n == 2) return &PORTC;
    if constexpr (n == 3) return &PORTC;
    if constexpr (n == 4) return &PORTC;
    if constexpr (n == 7) return &PORTC;
    if constexpr (n == 8) return &PORTC;

    if constexpr (n ==  9) return &PORTD;
    if constexpr (n == 10) return &PORTD;
    if constexpr (n == 11) return &PORTD;
    if constexpr (n == 12) return &PORTD;
    if constexpr (n == 13) return &PORTD;
    if constexpr (n == 14) return &PORTD;
    if constexpr (n == 15) return &PORTD;
    if constexpr (n == 16) return &PORTD;

    if constexpr (n == 19) return &PORTE;
    if constexpr (n == 20) return &PORTE;
    if constexpr (n == 21) return &PORTE;
    if constexpr (n == 22) return &PORTE;

    if constexpr (n == 23) return &PORTF;
    if constexpr (n == 24) return &PORTF;
    if constexpr (n == 25) return &PORTF;
    if constexpr (n == 26) return &PORTF;
    if constexpr (n == 27) return &PORTF;
    if constexpr (n == 28) return &PORTF;
    if constexpr (n == 29) return &PORTF;

    if constexpr (n == 33) return &PORTA;
    if constexpr (n == 34) return &PORTA;
    if constexpr (n == 35) return &PORTA;
    if constexpr (n == 36) return &PORTA;
    if constexpr (n == 37) return &PORTA;
    if constexpr (n == 38) return &PORTA;
    if constexpr (n == 39) return &PORTA;
    if constexpr (n == 40) return &PORTA;
}


template <uint8_t n>
static constexpr 
auto bitmask()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return PIN0_bm;
    if constexpr (n == 2) return PIN1_bm;
    if constexpr (n == 3) return PIN2_bm;
    if constexpr (n == 4) return PIN3_bm;
    if constexpr (n == 7) return PIN4_bm;
    if constexpr (n == 8) return PIN5_bm;

    if constexpr (n ==  9) return PIN0_bm;
    if constexpr (n == 10) return PIN1_bm;
    if constexpr (n == 11) return PIN2_bm;
    if constexpr (n == 12) return PIN3_bm;
    if constexpr (n == 13) return PIN4_bm;
    if constexpr (n == 14) return PIN5_bm;
    if constexpr (n == 15) return PIN6_bm;
    if constexpr (n == 16) return PIN7_bm;

    if constexpr (n == 19) return PIN0_bm;
    if constexpr (n == 20) return PIN1_bm;
    if constexpr (n == 21) return PIN2_bm;
    if constexpr (n == 22) return PIN3_bm;

    if constexpr (n == 23) return PIN0_bm;
    if constexpr (n == 24) return PIN1_bm;
    if constexpr (n == 25) return PIN2_bm;
    if constexpr (n == 26) return PIN3_bm;
    if constexpr (n == 27) return PIN4_bm;
    if constexpr (n == 28) return PIN5_bm;
    if constexpr (n == 29) return PIN6_bm;

    if constexpr (n == 33) return PIN0_bm;
    if constexpr (n == 34) return PIN1_bm;
    if constexpr (n == 35) return PIN2_bm;
    if constexpr (n == 36) return PIN3_bm;
    if constexpr (n == 37) return PIN4_bm;
    if constexpr (n == 38) return PIN5_bm;
    if constexpr (n == 39) return PIN6_bm;
    if constexpr (n == 40) return PIN7_bm;
}

template <uint8_t n>
static constexpr 
auto pin_ctrl()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return &(PORTC.PIN0CTRL);
    if constexpr (n == 2) return &(PORTC.PIN1CTRL);
    if constexpr (n == 3) return &(PORTC.PIN2CTRL);
    if constexpr (n == 4) return &(PORTC.PIN3CTRL);
    if constexpr (n == 7) return &(PORTC.PIN4CTRL);
    if constexpr (n == 8) return &(PORTC.PIN5CTRL);

    if constexpr (n ==  9) return &(PORTD.PIN0CTRL);
    if constexpr (n == 10) return &(PORTD.PIN1CTRL);
    if constexpr (n == 11) return &(PORTD.PIN2CTRL);
    if constexpr (n == 12) return &(PORTD.PIN3CTRL);
    if constexpr (n == 13) return &(PORTD.PIN4CTRL);
    if constexpr (n == 14) return &(PORTD.PIN5CTRL);
    if constexpr (n == 15) return &(PORTD.PIN6CTRL);
    if constexpr (n == 16) return &(PORTD.PIN7CTRL);

    if constexpr (n == 19) return &(PORTE.PIN0CTRL);
    if constexpr (n == 20) return &(PORTE.PIN1CTRL);
    if constexpr (n == 21) return &(PORTE.PIN2CTRL);
    if constexpr (n == 22) return &(PORTE.PIN3CTRL);

    if constexpr (n == 23) return &(PORTF.PIN0CTRL);
    if constexpr (n == 24) return &(PORTF.PIN1CTRL);
    if constexpr (n == 25) return &(PORTF.PIN2CTRL);
    if constexpr (n == 26) return &(PORTF.PIN3CTRL);
    if constexpr (n == 27) return &(PORTF.PIN4CTRL);
    if constexpr (n == 28) return &(PORTF.PIN5CTRL);
    if constexpr (n == 29) return &(PORTF.PIN6CTRL);

    if constexpr (n == 33) return &(PORTA.PIN0CTRL);
    if constexpr (n == 34) return &(PORTA.PIN1CTRL);
    if constexpr (n == 35) return &(PORTA.PIN2CTRL);
    if constexpr (n == 36) return &(PORTA.PIN3CTRL);
    if constexpr (n == 37) return &(PORTA.PIN4CTRL);
    if constexpr (n == 38) return &(PORTA.PIN5CTRL);
    if constexpr (n == 39) return &(PORTA.PIN6CTRL);
    if constexpr (n == 40) return &(PORTA.PIN7CTRL);
}
};


/***************************************************************************
 *				USART
 ***************************************************************************/
namespace private_{

// Posición de los bits en los registros
struct USART_bits{
// RXDATAH
    static constexpr uint8_t RXCIF = USART_RXCIF_bp;
    static constexpr uint8_t FERR  = USART_FERR_bp;
    static constexpr uint8_t PERR  = USART_PERR_bp;
    static constexpr uint8_t DATA8 = USART_DATA8_bp;

// STATUS
//    static constexpr uint8_t RXCIF= USART_RXCIF_bp;
    static constexpr uint8_t TXCIF= USART_TXCIF_bp;
    static constexpr uint8_t DREIF= USART_DREIF_bp;
    static constexpr uint8_t RXSIF= USART_RXSIF_bp;
    static constexpr uint8_t ISFIF= USART_ISFIF_bp;
    static constexpr uint8_t BDF  = USART_BDF_bp;
    static constexpr uint8_t WFB  = USART_WFB_bp;

// CTRLA
    static constexpr uint8_t RXCIE = USART_RXCIE_bp;
    static constexpr uint8_t TXCIE = USART_TXCIE_bp;
    static constexpr uint8_t DREIE = USART_DREIE_bp;
    static constexpr uint8_t RXSIE = USART_RXSIE_bp;
    static constexpr uint8_t LBME = USART_LBME_bp;
    static constexpr uint8_t ABEIE = USART_ABEIE_bp;

// CTRLB
    static constexpr uint8_t RXEN = USART_RXEN_bp;
    static constexpr uint8_t TXEN = USART_TXEN_bp;
    static constexpr uint8_t SFDEN = USART_SFDEN_bp;
    static constexpr uint8_t ODME = USART_ODME_bp;
    static constexpr uint8_t MPCM = USART_MPCM_bp;

// CTRLC
    static constexpr uint8_t SBMODE = USART_SBMODE_bp;

// TODO



};

// Diferentes valores que pueden tomar los registros
struct USART_values{
    // Modes
    static constexpr uint8_t CMODE_ASYNCHRONOUS = USART_CMODE_ASYNCHRONOUS_gc;
    static constexpr uint8_t CMODE_SYNCHRONOUS = USART_CMODE_SYNCHRONOUS_gc;
    static constexpr uint8_t CMODE_IRCOM = USART_CMODE_IRCOM_gc;
    static constexpr uint8_t CMODE_MSPI = USART_CMODE_MSPI_gc;

    // Parity
    static constexpr uint8_t PMODE_DISABLED = USART_PMODE_DISABLED_gc;
    static constexpr uint8_t PMODE_EVEN = USART_PMODE_EVEN_gc;
    static constexpr uint8_t PMODE_ODD = USART_PMODE_ODD_gc;

    // Character size
    static constexpr uint8_t CHSIZE_5BIT = USART_CHSIZE_5BIT_gc;
    static constexpr uint8_t CHSIZE_6BIT = USART_CHSIZE_6BIT_gc;
    static constexpr uint8_t CHSIZE_7BIT = USART_CHSIZE_7BIT_gc;
    static constexpr uint8_t CHSIZE_8BIT = USART_CHSIZE_8BIT_gc;

    static constexpr uint8_t CHSIZE_9BITL = USART_CHSIZE_9BITL_gc;
    static constexpr uint8_t CHSIZE_9BITH = USART_CHSIZE_9BITH_gc;
};

}// private_

// USART1
// ------
inline auto usart1_registers() { return &USART1; }
#undef USART1

struct USART1 {
    // reg = registers
    static auto reg() {return usart1_registers();}
   
    // posiciones de los bits dentro de los registros
    using bit_pos = private_::USART_bits;

    // valores
    using value = private_::USART_values;
    
    // pines que configuran este USART
    static constexpr uint8_t TxD_pin = 1;
    static constexpr uint8_t RxD_pin = 2;
    static constexpr uint8_t XCK_pin = 3;
    static constexpr uint8_t XDIR_pin= 4;

    static void enable_Tx_pin() { PORTC.DIR |= PIN0_bm;}
    static void enable_Rx_pin() { PORTC.DIR &= ~PIN1_bm;}
    // XCK puede ser enable como in or out!!!
//    static void enable_XCK_pin() { PORTC.DIR |= PIN2_bm;}
};


}// namespace cfg
 
}// namespace mega0_
 

#endif


