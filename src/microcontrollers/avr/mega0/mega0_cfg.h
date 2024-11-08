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

/***************************************************************************
 *		    CONFIGURACIÓN COMUN A TODOS LOS MEGA-0
 ***************************************************************************/
namespace cfg {

namespace private_{

// USART
// -----
// Posición de los bits en los registros
struct USART_bits{
// RXDATAH
    static constexpr uint8_t RXCIF = USART_RXCIF_bp;
    static constexpr uint8_t FERR  = USART_FERR_bp;
    static constexpr uint8_t PERR  = USART_PERR_bp;
    static constexpr uint8_t DATA8 = USART_DATA8_bp;

// STATUS
//    static constexpr uint8_t RXCIF= USART_RXCIF_bp; es igual a RXDATAH::RXCIF
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
    
    // Receiver mode
    static constexpr uint8_t RXMODE_NORMAL = USART_RXMODE_NORMAL_gc;
    static constexpr uint8_t RXMODE_CLK2X = USART_RXMODE_CLK2X_gc;
    static constexpr uint8_t RXMODE_GENAUTO = USART_RXMODE_GENAUTO_gc;
    static constexpr uint8_t RXMODE_LINAUTO = USART_RXMODE_LINAUTO_gc;

};

// SPI
// ---
// Posición de los bits en los registros
struct SPI_bits{
// CTRLA
    static constexpr uint8_t DORD   = SPI_DORD_bp;
    static constexpr uint8_t MASTER = SPI_MASTER_bp;
    static constexpr uint8_t CLK2X  = SPI_CLK2X_bp;
    static constexpr uint8_t ENABLE = SPI_ENABLE_bp;

// CTRLB
    static constexpr uint8_t BUFEN = SPI_BUFEN_bp;
    static constexpr uint8_t BUFWR = SPI_BUFWR_bp;
    static constexpr uint8_t SSD   = SPI_SSD_bp;

// INTCRTL
    static constexpr uint8_t RXCIE = SPI_RXCIE_bp;
    static constexpr uint8_t TXCIE = SPI_TXCIE_bp;
    static constexpr uint8_t DREIE = SPI_DREIE_bp;
    static constexpr uint8_t SSIE  = SPI_SSIE_bp;
    static constexpr uint8_t IE    = SPI_IE_bp;


// TODO: INTFLAGS
// TODO: INTFLAGS::IF
// TODO: INTFLAGS::WRCOL
// TODO: INTFLAGS::RXCIF
// TODO: INTFLAGS::TXCIF
// TODO: INTFLAGS::DREIF
// TODO: INTFLAGS::SSIF
// TODO: INTFLAGS::BUFOVF

};

struct SPI_values{
    // Prescaler
    static constexpr uint8_t PRESC_DIV4   = SPI_PRESC_DIV4_gc;
    static constexpr uint8_t PRESC_DIV16  = SPI_PRESC_DIV16_gc;
    static constexpr uint8_t PRESC_DIV64  = SPI_PRESC_DIV64_gc;
    static constexpr uint8_t PRESC_DIV128 = SPI_PRESC_DIV128_gc;

    // Modes
    static constexpr uint8_t MODE_0 = SPI_MODE_0_gc;
    static constexpr uint8_t MODE_1 = SPI_MODE_1_gc;
    static constexpr uint8_t MODE_2 = SPI_MODE_2_gc;
    static constexpr uint8_t MODE_3 = SPI_MODE_3_gc;
};

}// private_
} // namespace cfg
  
/***************************************************************************
 *		    CONFIGURACIÓN DE LOS MEGA-0 DE 40 pins
 ***************************************************************************/
namespace cfg_40_pins{

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
struct pins : impl_of::pins{

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

// USART0
// ------
inline auto usart0_registers() { return &USART0; }
#undef USART0

struct USART0 {
    // reg = registers
    static auto reg() {return usart0_registers();}
   
    // posiciones de los bits dentro de los registros
    using bit_pos = cfg::private_::USART_bits;

    // valores
    using value = cfg::private_::USART_values;
    
    // pines que configuran este USART
    static constexpr uint8_t TxD_pin = 33;
    static constexpr uint8_t RxD_pin = 34;
    static constexpr uint8_t XCK_pin = 35;
    static constexpr uint8_t XDIR_pin= 36;

    static void enable_Tx_pin() { PORTA.DIR |= PIN0_bm;}
    static void enable_Rx_pin() { PORTA.DIR &= ~PIN1_bm;}
    // XCK puede ser enable como in or out!!!
//    static void enable_XCK_pin() { PORTA.DIR |= PIN2_bm;}

};


// USART1
// ------
inline auto usart1_registers() { return &USART1; }
#undef USART1

struct USART1 {
    // reg = registers
    static auto reg() {return usart1_registers();}
   
    // posiciones de los bits dentro de los registros
    using bit_pos = cfg::private_::USART_bits;

    // valores
    using value = cfg::private_::USART_values;
    
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


// USART2
// ------
inline auto usart2_registers() { return &USART2; }
#undef USART2

struct USART2 {
    // reg = registers
    static auto reg() {return usart2_registers();}
   
    // posiciones de los bits dentro de los registros
    using bit_pos = cfg::private_::USART_bits;

    // valores
    using value = cfg::private_::USART_values;
    
    // pines que configuran este USART
    static constexpr uint8_t TxD_pin = 23;
    static constexpr uint8_t RxD_pin = 24;
    static constexpr uint8_t XCK_pin = 25;
    static constexpr uint8_t XDIR_pin= 26;

    static void enable_Tx_pin() { PORTF.DIR |= PIN0_bm;}
    static void enable_Rx_pin() { PORTF.DIR &= ~PIN1_bm;}
    // XCK puede ser enable como in or out!!!
//    static void enable_XCK_pin() { PORTF.DIR |= PIN2_bm;}

};


// USART3
// ------
// El atmega4809 de 40 pins no tiene USART3

/***************************************************************************
 *				SPI
 ***************************************************************************/
// SPI0
// ----
inline auto spi0_registers() { return &SPI0; }
#undef SPI0

struct SPI0 {
    // reg = registers
    static auto reg() {return spi0_registers();}
   
    // posiciones de los bits dentro de los registros
    using bit_pos = cfg::private_::SPI_bits;

    // valores
    using value = cfg::private_::SPI_values;
    
    // pines que configuran este SPI
    static constexpr uint8_t MOSI_pin = 37;
    static constexpr uint8_t MISO_pin = 38;
    static constexpr uint8_t SCK_pin  = 39;
    static constexpr uint8_t SS_pin   = 40;
};



/***************************************************************************
 *				INTERRUPTS
 ***************************************************************************/
// WARNING: es posible (???) que todos estos defines sean propios del
// atmega4809. Si es así habría que meterlos todos en atmega4809_cfg.h
// e incluir ese archivo solo si MCU = atmega4809.
//
// (???) Lo ideal sería poder anotar dentro de USART1 los números de
// interruptción, de tal manera que la llamada a la interrupción fuera:
//	void isr<USART1::receive_complete_interrupt_number>() { ... }
//
// El problema es que al escribir ISR(xx){...} realmente se está escribiendo
//  (prototipo de __vector_26 con atributos) más
//  void __vector_26(void) { ... }
//
// Y esta, supongo (?), será la función que se llame al generar la
// interrupción. ¿cómo convertir 
//	void isr<USART1::receive_complete_interrupt_number>()
//  en 
//	void __vector_26(void)
//  usando el compilador y no el preprocesador? 
//
// Solución temporal:
//	Los acrónimos de las interrupciones parecen ser únicos (los de USART
//	fijo lo son para el atmega4809). Si esto es así podemos usar el nombre
//	de la clase USART1 para generar el nombre de la ISR correspondiente:
//
//	    ISR_RXC(USART1) { ... }
//	    ISR_RXC(USART2) { ... }
//	    ...
//
//       El problema de los siguientes defines es que son cripticos:
//
//#define ISR_RXC(usart) ISR(usart ## _RXC_vect)
//#define ISR_DRE(usart) ISR(usart ## _DRE_vect)
//#define ISR_TXC(usart) ISR(usart ## _TXC_vect)

// Por ello, opto por defines que tengan significado:
//#define ISR_receive_complete(usart) ISR(usart ## _RXC_vect)
//#define ISR_data_register_empty(usart) ISR(usart ## _DRE_vect)
//#define ISR_receiver_start_frame(usart) ISR(usart ## _TXC_vect)
//
// Pero el problema con los anteriores defines es que tanto USART como SPI 
// tienen una ISR_receive_complete!!!
// Por ello me veo obligado a implementarlo de otra forma:

// ISR_receive_complete(device): // función para preprocesador
//	if (device == USART)
//		return ISR(USART_RXC_vect);
//	if (device == SPI)
//		return ISR(SPI_INT_vect);
#define ISR_receive_complete(device) ISR_ ## device ## _RXC_vect

#define ISR_USART0_RXC_vect ISR(USART0_RXC_vect)
#define ISR_USART1_RXC_vect ISR(USART1_RXC_vect)
#define ISR_USART2_RXC_vect ISR(USART2_RXC_vect)
#define ISR_USART3_RXC_vect ISR(USART3_RXC_vect)

#define ISR_SPI0_RXC_vect ISR(SPI0_INT_vect)

// Si en el futuro se ve que otros devices usan estos mismos nombres, basta
// con reescribir esto de forma análoga a ISR_receive_complete
#define ISR_data_register_empty(usart) ISR(usart ## _DRE_vect)
#define ISR_receiver_start_frame(usart) ISR(usart ## _TXC_vect)

}// namespace cfg_40_pins
 
}// namespace mega0_
 

#endif


