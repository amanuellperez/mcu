// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __AVR_ATMEGA328P_H__
#define __AVR_ATMEGA328P_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Pines del ATMEGA328P
 *
 *
 *   - HISTORIA:
 *    A.Manuel L.Perez
 *      13/01/2019 v0.0
 *
 ****************************************************************************/

namespace avr{

// CONFIGURACIÓN DE LOS PINES
// --------------------------
// La idea es numerar los pines de acuerdo a su posición en el chip
// y no de acuerdo a los puertos B, C y D.
// Parece una numeración más natural y más fácil de mantener. (A fin de cuentas
// si conecto el pin 12, debo revisar que haya conectado el pin 12 del avr.
// Sencillo!!!)

// Las macros DDRB son del tipo *(volatile uint8_t*) direccion_ddrb;
// Estas funciones devuelven la dirección de memoria del registro que 
// siempre es válida.
//
// Los pines no válidos les asigno un 0.
constexpr volatile uint8_t* DDR[29] = {
    0
    , &DDRC, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, 0
    , 0    , &DDRB, &DDRB, &DDRD, &DDRD, &DDRD, &DDRB
    , &DDRB, &DDRB, &DDRB, &DDRB, &DDRB, 0    , 0
    , 0	   , &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC
};

constexpr volatile uint8_t* PORT[29] = {
    0
    , &PORTC, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, 0
    , 0	    , &PORTB, &PORTB, &PORTD, &PORTD, &PORTD, &PORTB
    , &PORTB, &PORTB, &PORTB, &PORTB, &PORTB, 0     , 0
    , 0	    , &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC
};

constexpr volatile uint8_t* PIN[29] = {
    0
    , &PINC, &PIND, &PIND, &PIND, &PIND, &PIND, 0
    , 0	   , &PINB, &PINB, &PIND, &PIND, &PIND, &PINB
    , &PINB, &PINB, &PINB, &PINB, &PINB, 0    , 0
    , 0    , &PINC, &PINC, &PINC, &PINC, &PINC, &PINC};


constexpr uint8_t BIT_MASK[29] = {
    0
    , 1 << PC6, 1 << PD0, 1 << PD1, 1 << PD2, 1 << PD3, 1 << PD4, 0
    , 0  , 1 << PB6, 1 << PB7, 1 << PD5, 1 << PD6, 1 << PD7, 1 << PB0
    , 1 << PB1, 1 << PB2, 1 << PB3, 1 << PB4, 1 << PB5, 0  , 0
    , 0  , 1 << PC0, 1 << PC1, 1 << PC2, 1 << PC3, 1 << PC4, 1 << PC5
    };



// CONFIGURACIÓN DEL SPI
    constexpr uint8_t SPI_num_pin_SCK  = 19u;
    constexpr uint8_t SPI_num_pin_MISO = 18u;
    constexpr uint8_t SPI_num_pin_MOSI = 17u;
    constexpr uint8_t SPI_num_pin_SS   = 16u;



// CONFIGURACIÓN DE LOS TIMERS
    // Timer0
    struct TIMER0{
        using counter_type                 = uint8_t;
        static constexpr uint8_t OCA_pin   = 12u;
        static constexpr uint8_t OCB_pin   = 11u;
	//constexpr uint16_t resolution= 256u;
    };

    // Timer1
    struct TIMER1{
        using counter_type                 = uint16_t;
        static constexpr uint8_t OCA_pin   = 15u;
        static constexpr uint8_t OCB_pin   = 16u;
	// constexpr uint32_t resolution= 65536u;
    };



// CONFIGURACIÓN DEL ADC
    // Devuelve el número de pin dentro del ADC que le corresponde al num_pin
    // del avr
    template<uint8_t num_pin> struct ADC_PIN{ static uint8_t value; };

    template<> struct ADC_PIN<23>{ static constexpr uint8_t value = 0; };
    template<> struct ADC_PIN<24>{ static constexpr uint8_t value = 1; };
    template<> struct ADC_PIN<25>{ static constexpr uint8_t value = 2; };
    template<> struct ADC_PIN<26>{ static constexpr uint8_t value = 3; };
    template<> struct ADC_PIN<27>{ static constexpr uint8_t value = 4; };
    template<> struct ADC_PIN<28>{ static constexpr uint8_t value = 5; };


// CONFIGURACIÓN DEL TWI
    constexpr uint8_t TWI_MASK_STATUS_BITS = 0xF8;
    constexpr uint8_t TWI_MASK_BIT_RATE_PRESCALER = 0x03;
    // bits of slave address: [1,7]
    constexpr uint8_t TWI_SLAVE_ADDRESS_BIT0 = 1;
    constexpr uint8_t TWI_SLAVE_ADDRESS_BITn = 7;

    // TWI Status register codes TODO: eliminar los codigo en mayusculas
    // -------------------------
    // START has been transmitted  
    constexpr static uint8_t TWI_START = 0x08; 
    // Repeated START has been transmitted
    constexpr static uint8_t TWI_REPEATED_START = 0x10;  
    // Arbitration lost in SLA+R/W or data bytes
    constexpr static uint8_t TWI_ARBITRATION_LOST = 0x38;


    // TWI Master Transmitter status codes (table 26-3)
    // ------------------------------------------------
    // SLA+W has been tramsmitted and ACK received
    constexpr static uint8_t TWI_MTM_ADDRESS_ACK = 0x18;
    // SLA+W has been tramsmitted and NACK received 
    constexpr static uint8_t TWI_MTM_ADDRESS_NACK = 0x20;
    // Data byte has been tramsmitted and ACK received
    constexpr static uint8_t TWI_MTM_DATA_ACK = 0x28;
    // Data byte has been tramsmitted and NACK received 
    constexpr static uint8_t TWI_MTM_DATA_NACK = 0x30;

    // TWI Master Receiver status codes (table 26-4)
    // ---------------------------------------------
    // SLA+R has been tramsmitted and ACK received
    constexpr static uint8_t TWI_MRM_ADDRESS_ACK = 0x40;
    // SLA+R has been tramsmitted and NACK received
    constexpr static uint8_t TWI_MRM_ADDRESS_NACK = 0x48;
    // Data byte has been received and ACK tramsmitted
    constexpr static uint8_t TWI_MRM_DATA_ACK = 0x50;
    // Data byte has been received and NACK tramsmitted
    constexpr static uint8_t TWI_MRM_DATA_NACK = 0x58;
   
    // Modos comunes a transmitter/receiver mode
    struct __TWI_basic_iostate_master_mode {
        // START has been transmitted
        static constexpr uint8_t start = 0x08;

        // Repeated START has been transmitted
        static constexpr uint8_t repeated_start = 0x10;

        // Arbitration lost in SLA+R/W or data bytes
        static constexpr uint8_t arbitration_lost = 0x38;
    };

    // Table 26-3, junto con el diagrama figure 26-12.
    struct __TWI_basic_iostate_master_transmitter_mode {
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
    struct __TWI_basic_iostate_master_receiver_mode {

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
    struct __TWI_basic_iostate_slave_transmitter_mode {
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
    struct __TWI_basic_iostate_slave_receiver_mode {

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

    struct TWI_basic_iostate{
	using master_mode = __TWI_basic_iostate_master_mode;
        using master_transmitter_mode =
			        __TWI_basic_iostate_master_transmitter_mode;

        using master_receiver_mode =
			        __TWI_basic_iostate_master_receiver_mode;
        using slave_receiver_mode = __TWI_basic_iostate_slave_receiver_mode;
        using slave_transmitter_mode =
				 __TWI_basic_iostate_slave_transmitter_mode;

        // Miscellaneous (table 26-7)
	// Bus error due to an illegal START or STOP condition
	static constexpr uint8_t bus_error = 0x00;

        // No relevant state information available. El hardware está ejecutando
        // la servicio solicitado.
        static constexpr uint8_t running   = 0xF8;
    };


}// namespace

#endif


