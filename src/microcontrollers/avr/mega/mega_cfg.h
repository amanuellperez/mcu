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

#ifndef __MEGA_CFG_H__
#define __MEGA_CFG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Configuración de la familia megaAVR	
 *
 * HISTORIA
 *    Manuel Perez
 *      13/01/2019 v0.0
 *      20/10/2024 cfg::pins_28
 *
 ****************************************************************************/
#include <cstdint>  // uint8_t
#include <avr/io.h> // registros: DDRB... PORT...
#include <atd_static.h> // static_array

namespace mega_{

namespace cfg{ // ir incluyendo el resto según vaya reescribiendo codigo
// CONFIGURACIÓN DE LOS PINES
// --------------------------
// La idea es numerar los pines de acuerdo a su posición en el chip
// y no de acuerdo a los puertos B, C y D.
// Parece una numeración más natural y más fácil de mantener. (A fin de cuentas
// si conecto el pin 12, debo revisar que haya conectado el pin 12 del avr.
// Sencillo!!!)

struct pins_28{
template <uint8_t n>
static constexpr 
bool is_a_valid_pin(){
    return 
    (n == 1) or (n == 2) or (n == 3) or (n == 4) or (n == 5)
    or (n == 6) or (n == 9)
    or (n == 10) or (n == 11) or (n == 12) or (n == 13) or (n == 14)
    or (n == 15) or (n == 16) or (n == 17) or (n == 18) or (n == 19)
    or (n == 23) or (n == 24)
    or (n == 25) or (n == 26) or (n == 27) or (n == 28);
}

template <uint8_t n>
static constexpr 
auto ddr()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return &DDRC;

    if constexpr (n == 2) return &DDRD;
    if constexpr (n == 3) return &DDRD;
    if constexpr (n == 4) return &DDRD;
    if constexpr (n == 5) return &DDRD;
    if constexpr (n == 6) return &DDRD;

    if constexpr (n == 9) return &DDRB;
    if constexpr (n == 10) return &DDRB;

    if constexpr (n == 11) return &DDRD;
    if constexpr (n == 12) return &DDRD;
    if constexpr (n == 13) return &DDRD;

    if constexpr (n == 14) return &DDRB;
    if constexpr (n == 15) return &DDRB;
    if constexpr (n == 16) return &DDRB;
    if constexpr (n == 17) return &DDRB;
    if constexpr (n == 18) return &DDRB;
    if constexpr (n == 19) return &DDRB;

    if constexpr (n == 23) return &DDRC;
    if constexpr (n == 24) return &DDRC;
    if constexpr (n == 25) return &DDRC;
    if constexpr (n == 26) return &DDRC;
    if constexpr (n == 27) return &DDRC;
    if constexpr (n == 28) return &DDRC;

}

template <uint8_t n>
static constexpr 
auto port()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return &PORTC;

    if constexpr (n == 2) return &PORTD;
    if constexpr (n == 3) return &PORTD;
    if constexpr (n == 4) return &PORTD;
    if constexpr (n == 5) return &PORTD;
    if constexpr (n == 6) return &PORTD;

    if constexpr (n == 9) return &PORTB;
    if constexpr (n == 10) return &PORTB;

    if constexpr (n == 11) return &PORTD;
    if constexpr (n == 12) return &PORTD;
    if constexpr (n == 13) return &PORTD;

    if constexpr (n == 14) return &PORTB;
    if constexpr (n == 15) return &PORTB;
    if constexpr (n == 16) return &PORTB;
    if constexpr (n == 17) return &PORTB;
    if constexpr (n == 18) return &PORTB;
    if constexpr (n == 19) return &PORTB;

    if constexpr (n == 23) return &PORTC;
    if constexpr (n == 24) return &PORTC;
    if constexpr (n == 25) return &PORTC;
    if constexpr (n == 26) return &PORTC;
    if constexpr (n == 27) return &PORTC;
    if constexpr (n == 28) return &PORTC;
}

template <uint8_t n>
static constexpr 
auto pin()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return &PINC;

    if constexpr (n == 2) return &PIND;
    if constexpr (n == 3) return &PIND;
    if constexpr (n == 4) return &PIND;
    if constexpr (n == 5) return &PIND;
    if constexpr (n == 6) return &PIND;

    if constexpr (n == 9) return &PINB;
    if constexpr (n == 10) return &PINB;

    if constexpr (n == 11) return &PIND;
    if constexpr (n == 12) return &PIND;
    if constexpr (n == 13) return &PIND;

    if constexpr (n == 14) return &PINB;
    if constexpr (n == 15) return &PINB;
    if constexpr (n == 16) return &PINB;
    if constexpr (n == 17) return &PINB;
    if constexpr (n == 18) return &PINB;
    if constexpr (n == 19) return &PINB;

    if constexpr (n == 23) return &PINC;
    if constexpr (n == 24) return &PINC;
    if constexpr (n == 25) return &PINC;
    if constexpr (n == 26) return &PINC;
    if constexpr (n == 27) return &PINC;
    if constexpr (n == 28) return &PINC;
}

template <uint8_t n>
static constexpr 
auto pin_bit()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 1) return PC6;

    if constexpr (n == 2) return PD0;
    if constexpr (n == 3) return PD1;
    if constexpr (n == 4) return PD2;
    if constexpr (n == 5) return PD3;
    if constexpr (n == 6) return PD4;

    if constexpr (n == 9) return PB6;
    if constexpr (n == 10) return PB7;

    if constexpr (n == 11) return PD5;
    if constexpr (n == 12) return PD6;
    if constexpr (n == 13) return PD7;

    if constexpr (n == 14) return PB0;
    if constexpr (n == 15) return PB1;
    if constexpr (n == 16) return PB2;
    if constexpr (n == 17) return PB3;
    if constexpr (n == 18) return PB4;
    if constexpr (n == 19) return PB5;

    if constexpr (n == 23) return PC0;
    if constexpr (n == 24) return PC1;
    if constexpr (n == 25) return PC2;
    if constexpr (n == 26) return PC3;
    if constexpr (n == 27) return PC4;
    if constexpr (n == 28) return PC5;
}

template <uint8_t n>
static constexpr 
auto bitmask()
{ return (1 << pin_bit<n>()); }

template <uint8_t n>
static constexpr 
auto pcie()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 2) return PCIE2;
    if constexpr (n == 3) return PCIE2;
    if constexpr (n == 4) return PCIE2;
    if constexpr (n == 5) return PCIE2;
    if constexpr (n == 6) return PCIE2;
    if constexpr (n == 9) return PCIE0;
    if constexpr (n == 10) return PCIE0;
    if constexpr (n == 11) return PCIE2;
    if constexpr (n == 12) return PCIE2;
    if constexpr (n == 13) return PCIE2;
    if constexpr (n == 14) return PCIE0;
    if constexpr (n == 15) return PCIE0;
    if constexpr (n == 16) return PCIE0;
    if constexpr (n == 17) return PCIE0;
    if constexpr (n == 18) return PCIE0;
    if constexpr (n == 19) return PCIE0;
    if constexpr (n == 23) return PCIE1;
    if constexpr (n == 24) return PCIE1;
    if constexpr (n == 25) return PCIE1;
    if constexpr (n == 26) return PCIE1;
    if constexpr (n == 27) return PCIE1;
    if constexpr (n == 28) return PCIE1;
}



template <uint8_t n>
static constexpr 
auto pcint()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 2) return PCINT16;
    if constexpr (n == 3) return PCINT17;
    if constexpr (n == 4) return PCINT18;
    if constexpr (n == 5) return PCINT19;
    if constexpr (n == 6) return PCINT20;
    if constexpr (n == 9) return PCINT6;
    if constexpr (n == 10) return PCINT7;
    if constexpr (n == 11) return PCINT21;
    if constexpr (n == 12) return PCINT22;
    if constexpr (n == 13) return PCINT23;
    if constexpr (n == 14) return PCINT0;
    if constexpr (n == 15) return PCINT1;
    if constexpr (n == 16) return PCINT2;
    if constexpr (n == 17) return PCINT3;
    if constexpr (n == 18) return PCINT4;
    if constexpr (n == 19) return PCINT5;
    if constexpr (n == 23) return PCINT8;
    if constexpr (n == 24) return PCINT9;
    if constexpr (n == 25) return PCINT10;
    if constexpr (n == 26) return PCINT11;
    if constexpr (n == 27) return PCINT12;
    if constexpr (n == 28) return PCINT13;
}



template <uint8_t n>
static constexpr 
auto pcmsk()
{
    static_assert(is_a_valid_pin<n>(), "Wrong pin number");

    if constexpr (n == 2) return &PCMSK2;
    if constexpr (n == 3) return &PCMSK2;
    if constexpr (n == 4) return &PCMSK2;
    if constexpr (n == 5) return &PCMSK2;
    if constexpr (n == 6) return &PCMSK2;
    if constexpr (n == 9) return &PCMSK0;
    if constexpr (n == 10) return &PCMSK0;
    if constexpr (n == 11) return &PCMSK2;
    if constexpr (n == 12) return &PCMSK2;
    if constexpr (n == 13) return &PCMSK2;
    if constexpr (n == 14) return &PCMSK0;
    if constexpr (n == 15) return &PCMSK0;
    if constexpr (n == 16) return &PCMSK0;
    if constexpr (n == 17) return &PCMSK0;
    if constexpr (n == 18) return &PCMSK0;
    if constexpr (n == 19) return &PCMSK0;
    if constexpr (n == 23) return &PCMSK1;
    if constexpr (n == 24) return &PCMSK1;
    if constexpr (n == 25) return &PCMSK1;
    if constexpr (n == 26) return &PCMSK1;
    if constexpr (n == 27) return &PCMSK1;
    if constexpr (n == 28) return &PCMSK1;
}


};




// Interrupciones INT0, INT1, ...
// -------------------------------
// EXPERIMENTO: un avr puede tener varias INTx. Probemos a dejar el código
// independiente del número de interrupciones que tenga. Para ello necesito
// definir todas las posiciones de los bits como arrays. (y seguramente que
// haya que definir también los registros a los que acceder como array pero de
// momento no lo necesito)
// External Interrupt Control Register A:
// Buscamos que ISC0<0>, ISC1<0> == sean los ISC de INT0, por ello esta forma
// de definirlo:
    using ISC0 = atd::static_array<uint8_t, ISC00, ISC10>; 
    using ISC1 = atd::static_array<uint8_t, ISC01, ISC11>;

// External Interrupt Mask Register:
    using INT_POS = atd::static_array<uint8_t, INT0, INT1>;
constexpr uint8_t INT_number = INT_POS::size;

// Quiero definir INT0, INT1 como templates INT<0>, INT<1>, templates que
// asociaré a los pines correspondientes:
//	    Al Pin<4>::INT = INT<0>
//	    Al Pin<5>::INT = INT<1>
//
// Si se intenta acceder a Pin<n>::INT, con n distinto de 4 ó 5 debe de dar un
// error de compilación. 
//
// Para implementar esto necesito asociar al pin el número de INT que le
// corresponde. La siguiente clase hace eso:
// (realmente es un static_map que asocia 4->0, y 5->1.
template <uint8_t n>
inline constexpr int8_t nINT_of_pin()
{
    if constexpr (n == 4)
	return 0;

    else if constexpr (n == 5)
	return 1;

    // se especializa INT<-1> como clase vacía para que de error de compilación
    else 
	return -1;
}


// CONFIGURACIÓN DEL SPI
// ---------------------
    struct spi{
	static constexpr uint8_t SCK_pin_number  = 19u;
	static constexpr uint8_t MISO_pin_number = 18u;
	static constexpr uint8_t MOSI_pin_number= 17u;
	static constexpr uint8_t SS_pin_number = 16u;
    };

// Timer0
// ------
    struct timer0{
        using counter_type                 = uint8_t;
	static constexpr uint8_t number_of_pins = 2;
        static constexpr uint8_t OCA_pin   = 12u;
        static constexpr uint8_t OCB_pin   = 11u;
	//constexpr uint16_t resolution= 256u;
    };

 
// Timer1
// ------
    struct timer1{
        using counter_type                 = uint16_t;
	static constexpr uint8_t number_of_pins = 2;
        static constexpr uint8_t OCA_pin   = 15u;
        static constexpr uint8_t OCB_pin   = 16u;
	// constexpr uint32_t resolution= 65536u;
    };


// Timer2
// ------
    struct timer2{
        using counter_type                 = uint8_t;
	static constexpr uint8_t number_of_pins = 2;
        static constexpr uint8_t OCA_pin   = 17u;
        static constexpr uint8_t OCB_pin   = 11u; // usa OCB0!!!
	//constexpr uint16_t resolution= 256u;
	static constexpr uint8_t TOSC1_pin = 9;
	static constexpr uint8_t TOSC2_pin = 10;
    };



// TWI
// ---
    constexpr uint8_t TWI_MASK_STATUS_BITS = 0xF8;
    constexpr uint8_t TWI_MASK_BIT_RATE_PRESCALER = 0x03;
    // bits of slave address: [1,7]
    constexpr uint8_t TWI_SLAVE_ADDRESS_BIT0 = 1;
    constexpr uint8_t TWI_SLAVE_ADDRESS_BITn = 7;
   
namespace impl_of{
    // Modos comunes a transmitter/receiver mode
    struct TWI_basic_state_master_mode {
        // START has been transmitted
        static constexpr uint8_t start = 0x08;

        // Repeated START has been transmitted
        static constexpr uint8_t repeated_start = 0x10;

        // Arbitration lost in SLA+R/W or data bytes
        static constexpr uint8_t arbitration_lost = 0x38;
    };

    // Table 26-3, junto con el diagrama figure 26-12.
    struct TWI_basic_state_master_transmitter_mode {
        // SLA+W has been tramsmitted and ACK received
        static constexpr uint8_t sla_w_ack = 0x18;

        // SLA+W has been tramsmitted and NACK received
        static constexpr uint8_t sla_w_nack = 0x20;

        // Data byte has been tramsmitted and ACK received
        static constexpr uint8_t data_ack = 0x28;

        // Data byte has been tramsmitted and NACK received
        static constexpr uint8_t data_nack = 0x30;
    };

    // Table 26-4, junto con el diagrama figure 26-14.
    struct TWI_basic_state_master_receiver_mode {

        // SLA+R has been tramsmitted and ACK received
        constexpr static uint8_t sla_r_ack = 0x40;

        // SLA+R has been tramsmitted and NACK received
        constexpr static uint8_t sla_r_nack = 0x48;

        // Data byte has been received and ACK tramsmitted
        constexpr static uint8_t data_ack = 0x50;

        // Data byte has been received and NACK tramsmitted
        constexpr static uint8_t data_nack = 0x58;
    };

    // Table 26-5, junto con el diagrama figure 26-16.
    struct TWI_basic_state_slave_transmitter_mode {
	// Own SLA+R has been received; ACK has been returned
	static constexpr uint8_t sla_r            = 0xA8;

        // Arbitration lost in SLA+R/W as Master; own SLA+R has been received;
        // ACK has been returned
        static constexpr uint8_t arbitration_lost = 0xB0;

	// Data byte in TWDR has been transmitted; ACK has been received
	static constexpr uint8_t data_ack         = 0xB8;

	// Data byte in TWDR has been transmitted; NOT ACK has been received
	static constexpr uint8_t data_nack        = 0xC0;

	// Last data byte in TWDR has been transmitted. ACK has been received
	static constexpr uint8_t data_last_byte   = 0xC8;
    };



    // Table 26-6, junto con el diagrama figure 26-18.
    struct TWI_basic_state_slave_receiver_mode {

        // Own SLA+W has been received ACK has been returned
        static constexpr uint8_t sla_w = 0x60;

        // Arbitration lost in SLA+R/W as Master.
        // own SLA+W has been received; ACK has been returned
        static constexpr uint8_t arbitration_lost_sla_w = 0x68;

        // Previously addressed with own SLA+W; data has been received.
        // ACK has been returned.
        static constexpr uint8_t sla_w_data_ack = 0x80;

        // Previously addressed with own SLA+W; data has been received; NOT ACK
        // has been returned
        static constexpr uint8_t sla_w_data_nack = 0x88;

        // General call address has been received; ACK has been returned
        static constexpr uint8_t general_call = 0x70;

        // Arbitration lost in SLA+R/W as Master;
        // General call address has been received; ACK has been returned
        static constexpr uint8_t arbitration_lost_general_call = 0x78;

        // Previously addressed with general call.
        // Data has been received. ACK has been returned.
        static constexpr uint8_t general_call_data_ack = 0x90;

        // Previously addressed with general call.
        // Data has been received. NOT ACK has been returned.
        static constexpr uint8_t general_call_data_nack = 0x98;

        // A STOP condition or repeated START condition has been received while
        // still addressed as Slave
        static constexpr uint8_t stop_or_repeated_start = 0xA0;
    };
} // namespace impl_of
  
    struct TWI_basic_state{
	using master_mode = impl_of::TWI_basic_state_master_mode;
        using master_transmitter_mode =
			impl_of::TWI_basic_state_master_transmitter_mode;

        using master_receiver_mode =
			        impl_of::TWI_basic_state_master_receiver_mode;
        using slave_receiver_mode = 
				impl_of::TWI_basic_state_slave_receiver_mode;
        using slave_transmitter_mode =
			      impl_of::TWI_basic_state_slave_transmitter_mode;

        // Miscellaneous (table 26-7)
	// Bus error due to an illegal START or STOP condition
	static constexpr uint8_t bus_error = 0x00;

        // No relevant state information available. El hardware está ejecutando
        // la servicio solicitado.
        static constexpr uint8_t running   = 0xF8;
    };





// CONFIGURACIÓN DEL ADC
    // Devuelve el número de pin dentro del ADC que le corresponde al num_pin
    // del avr
    template<uint8_t num_pin> struct ADC_PIN
    { static_assert(false, "That pin is not an ADC pin");};

    template<> struct ADC_PIN<23>{ static constexpr uint8_t value = 0; };
    template<> struct ADC_PIN<24>{ static constexpr uint8_t value = 1; };
    template<> struct ADC_PIN<25>{ static constexpr uint8_t value = 2; };
    template<> struct ADC_PIN<26>{ static constexpr uint8_t value = 3; };
    template<> struct ADC_PIN<27>{ static constexpr uint8_t value = 4; };
    template<> struct ADC_PIN<28>{ static constexpr uint8_t value = 5; };

}// namespace cfg


}// namespace




#endif


