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
    using TIMER0_counter_type          = uint8_t;
    constexpr uint8_t TIMER0_num_pin_A = 12u;
    constexpr uint8_t TIMER0_num_pin_B = 11u;
    //constexpr uint16_t TIMER0_resolution= 256u;

    // Timer1
    using TIMER1_counter_type          = uint16_t;
    constexpr uint8_t TIMER1_num_pin_A = 15u;
    constexpr uint8_t TIMER1_num_pin_B = 16u;
    // constexpr uint32_t TIMER1_resolution= 65536u;

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



}// namespace

#endif


