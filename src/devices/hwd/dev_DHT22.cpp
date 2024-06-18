// Copyright (C) 2019-2020 Manuel Perez 
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

#include "dev_DHT22.h"
#include "avr_time.h"
#include "avr_timer0.h"
#include "avr_interrupt.h"


namespace dev{
DHT22::DHT22(uint8_t num_pin)
    : num_pin_{num_pin}
{
    Pin::de_entrada_sin_pullup(num_pin);
}


// Esta función es la que se comunica con el DHT22.
// Es fundamental que sea totalmente eficiente.
//
// PROBLEMA: Uso Pin local porque creo que es más eficiente que si lo 
// meto dentro de la clase. De hecho esta clase es la que está añadiendo
// ineficiencia a esta función. ¿cómo mejorarlo?
DHT22::Error DHT22::read_data(uint8_t* bit_high, uint8_t* bit_low)
{
//    volatile uint8_t* port_debug = Pin::port(13);
//    uint8_t bit_mask_debug= Pin::bit_mask(13);
    Timer0::no_preescaling();
    Pin pin{num_pin_};

    // Pin needs to start HIGH, wait until it is HIGH with a timeout
//    (*port_debug) |= bit_mask_debug;
    pin.de_entrada_sin_pullup();

    Interrupts_lock lock;
    Timer0::reset();
    while(pin.is_zero()){
	if (Timer0::value() >= 100u)
	    return Error::BUS_HUNG;
    }
//    (*port_debug) &= ~bit_mask_debug;

//    (*port_debug) |= bit_mask_debug;

    // Send the activate pulse
    pin.de_salida();
    pin.write_zero();
    delay_ms(20);	// spec is 1 to 10ms
    pin.de_entrada_sin_pullup();
//    (*port_debug) &= ~bit_mask_debug;

//    (*port_debug) |= bit_mask_debug;
    // Find the start of the ACK signal
    Timer0::reset();
    while (pin.is_one()){
	if (Timer0::value() >= 50u)
	    return Error::NOT_PRESENT;
    }
//    (*port_debug) &= ~bit_mask_debug;

//    (*port_debug) |= bit_mask_debug;
    // Find the transition of the ACK signal
    Timer0::reset();
    while (pin.is_zero()){
	if (Timer0::value() >= 100u)
	    return Error::ACK_TOO_LONG;
    }
//    (*port_debug) &= ~bit_mask_debug;

//    (*port_debug) |= bit_mask_debug;
    // Find the end of the ACK signal
    Timer0::reset();
    while (pin.is_one()){
	if (Timer0::value() >= 100u)
	    return Error::ACK_TOO_LONG;
    }
//    (*port_debug) &= ~bit_mask_debug;

    // Read the 40 bit data stream
    for(uint8_t i = 0; i < DATA_BIT_COUNT; i++)
    {
	// Find the start of the sync pulse
//    (*port_debug) |= bit_mask_debug;
	Timer0::reset();
	while (pin.is_zero()){
	    if (Timer0::value() >= 70u)
		return Error::SYNC_TIMEOUT;
	}
	bit_low[i] = Timer0::value();
//    (*port_debug) &= ~bit_mask_debug;

//    (*port_debug) |= bit_mask_debug;
	// Measure the width of the data pulse
	Timer0::reset();
	while (pin.is_one()){
	    if (Timer0::value() >= 70u)
		return Error::DATA_TIMEOUT;
	}
//    (*port_debug) &= ~bit_mask_debug;

	bit_high[i] = Timer0::value();
    }

    return Error::NONE;
}


// precondición: pin es de entrada con pull-up
// postcondición: pin es de entrada con pull-up
DHT22::Error DHT22::read()
{
    uint8_t bit_high[DATA_BIT_COUNT];
    uint8_t bit_low[DATA_BIT_COUNT];

    auto err = read_data(bit_high, bit_low);
    if (err != Error::NONE)
	return err;


    // translate bit_high
    // 26~28us == logical 0
    // 70us    == logical 1
    int16_t raw_humidity = 0;
    int16_t raw_temperature = 0;
    uint8_t checkSum = 0;
    for(uint8_t i = 0; i < 16u; i++)
    {
	if( bit_high[i] > bit_low[i] ) 
		raw_humidity |= (1 << (15 - i));
    }

    for(uint8_t i = 0; i < 16; i++)
    {
	if( bit_high[i + 16] > bit_low[i + 16] ) 
		raw_temperature |= (1 << (15 - i));
    }

    for(uint8_t i = 0; i < 8; i++)
    {
	if(bit_high[i + 32] > bit_low[i+32]) 
		checkSum |= (1 << (7 - i));
    }

    // calculate checksum
    uint8_t data1 = raw_humidity >> 8;
    uint8_t data2 = raw_humidity & 0xFF;
    uint8_t data3 = raw_temperature >> 8;
    uint8_t data4 = raw_temperature & 0xFF;

    if( checkSum == ( (data1 + data2 + data3 + data4) & 0xFF ) )
    {
	// raw data to sensor values
	RH_parte_entera  = static_cast<uint8_t>(raw_humidity / 10);
	RH_parte_decimal = static_cast<uint8_t>(raw_humidity % 10);

	if(raw_temperature & 0x8000) // Check if temperature is below zero, non standard way of encoding negative numbers!
	{
	    raw_temperature &= 0x7FFF; // Remove signal bit
	    T_parte_entera  = static_cast<int8_t>((raw_temperature / 10.0) * -1);
	    T_parte_decimal = static_cast<uint8_t>(raw_temperature % 10);
	} else
	{
	    T_parte_entera  = static_cast<int8_t>(raw_temperature / 10.0);
	    T_parte_decimal = static_cast<uint8_t>(raw_temperature % 10);	    
	}

	return Error::NONE;
    }

    return Error::CHECKSUM;
}

}// namespace

