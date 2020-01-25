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

#ifndef __AVR_TWI_H__
#define __AVR_TWI_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Traductor del TWI (I2C)
 *
 *   - COMENTARIOS: En principio pensaba hacer un traductor literal de
 *	la datasheet. Sin embargo, todas las peticiones se hacen a través
 *	del registro TWCR donde como mínimo hay que escribir los bits TWEN y
 *	TWINT. Al final doy las dos versiones:
 *	    1.- El traductor literal.
 *	    2.- Funciones que acceden directamente al registro TWCR
 *		(en principio, este interfaz parece el más sencillo de usar).
 *
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez- 24/08/2019 v0.0
 *
 ****************************************************************************/
#include <cstddef>    // std::byte
#include <atd_register.h>

#include "avr_cfg.h"

namespace avr{

class TWI{
public:
    /// Selects the division factor for the bit rate generator.
    /// The bit rate generatro is a frequency divider which generatres the
    /// SCL clock frequency in the Master modes.
    static void division_factor_for_bit_rate_generator(uint8_t d)
    { TWBR = d; }
    
    /// Devuelve el division factor for the bit rate generator.
    static uint8_t division_factor_for_bit_rate_generator()
    {return TWBR;}


    /// Devuelve el status bit.
    static uint8_t status_bits()
    { return TWSR & TWI_MASK_STATUS_BITS;}

    // Selección de la velocidad del reloj
    static void bit_rate_prescaler_value_1()
    {// 00
	atd::Register{TWSR}.write_zero_bit<TWPS1, TWPS0>();
    }

    static void bit_rate_prescaler_value_4()
    {// 01
	atd::Register{TWSR}.write_zero_bit<TWPS1>();
	atd::Register{TWSR}.write_one_bit<TWPS0>();
    }

    static void bit_rate_bit_rate_prescaler_value_16()
    {// 10
	atd::Register{TWSR}.write_one_bit<TWPS1>();
	atd::Register{TWSR}.write_zero_bit<TWPS0>();
    }

    static void bit_rate_bit_rate_prescaler_value_64()
    {// 11
	atd::Register{TWSR}.write_zero_bit<TWPS1, TWPS0>();
    }

    /// Devuelve el bit rate prescaler seleccionado.
    static uint8_t bit_rate_prescaler()
    { return TWSR & TWI_MASK_BIT_RATE_PRESCALER;}


    /// Definimos la frecuencia del reloj SCL.
    /// f_scl = frecuencia en kilohercios de SCL (tipica: 100 y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = AVR_CLOCK_FREQUENCY_IN_HZ>
    static void SCL_frequency_in_kHz();
    

    /// Dirección que tiene el avr si funciona como slave.
    static void slave_address(uint8_t addr)
    {	// TODO: esta función depende del atmega328p!!! Generalizarla!!!
	// Que sirva para cualquier controlador.
	uint8_t tmp = TWAR & 0x01;  // El address son los bits 1-7 de TWAR.
	TWAR = tmp | (addr << 1);
    }


    /// ¿Queremos que en multimaster systems se responda a la general call
    /// address (0x00)?
    static void enable_general_call_recognition()
    { atd::Register{TWAR}.write_one_bit<TWGCE>(); }

    static void disable_general_call_recognition()
    { atd::Register{TWAR}.write_zero_bit<TWGCE>(); }

    static uint8_t is_general_call_recognition_enabled()
    { return atd::Register{TWAR}.bit<TWGCE>(); }

    /// Dato que queremos enviar a continuación.
    static void data(std::byte x)
    { TWDR = std::to_integer<uint8_t>(x); }

    /// Dato que hemos recibido.
    static std::byte data()
    { return std::byte{TWDR};}


    /// Starts the operation of the TWI, so all accesses to the TWI must
    /// be complete before calling this function.
    static void start_next_operation()
    { atd::Register{TWCR}.write_zero_bit<TWINT>(); }

    /// It TWI has finished its current job and expects application software
    /// response, return true. Otherwise, false.
    static bool has_finished_its_current_job()
    { return atd::Register{TWCR}.bit<TWINT>(); }

    /// Generate an ACK pulse (see datasheet for conditions)
    static void generate_ACK()
    { atd::Register{TWCR}.write_one_bit<TWEA>(); }

    /// Generates a start (start transmision)
    static void generate_START_condition()
    { atd::Register{TWCR}.write_one_bit<TWSTA>(); }

    /// This function must be call when the START condition has been
    /// transmitted.
    static void clear_START_condition()
    { atd::Register{TWCR}.write_zero_bit<TWSTA>(); }

    /// Generate a stop (stop transmision)
    static void generate_STOP_condition()
    { atd::Register{TWCR}.write_one_bit<TWSTO>(); }

    // TODO: write_collision()

    /// Enables TWI interface.
    static void enable()
    { atd::Register{TWCR}.write_one_bit<TWEN>(); }


    /// Enables TWI interface, definiendo la frecuencia del SCL a la 
    /// que vamos a operar.
    /// f_scl = frecuencia en kilohercios de SCL (tipica: 100 y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = AVR_CLOCK_FREQUENCY_IN_HZ>
    static void enable_and_set_SCL_frequency_in_kHz()
    {
	SCL_frequency_in_kHz<f_scl, f_clock>();
	enable();
    }


    /// Disables TWI interface.
    static void disable()
    { atd::Register{TWCR}.write_zero_bit<TWEN>(); }


    /// The TWI interrupt request will be activated.
    static void interrupt_enable()
    { atd::Register{TWCR}.write_one_bit<TWIE>(); }

    /// The TWI interrupt request will be disactivated.
    static void interrupt_disable()
    { atd::Register{TWCR}.write_zero_bit<TWIE>(); }

    // TODO: falta el registro TWAMR


    // -------------------------------------------------------------------
    // Otro estilo de programación: accedemos a los registros directamente
    // -------------------------------------------------------------------
    /// Enviamos una START condition.
    // Observar que esta forma de implementarla, escribiendo 
    // todos los bits de TWCR a la vez tiene el problema de
    // que no permite al usuario enviar un start habilitando
    // las interrupciones (bit TWIE). Es necesario hacer otra
    // función start: start_con_interrupciones para hacerlo.
    static void send_start()
    {
	TWCR =	(1 << TWEN)|	// enable(); (ver datasheet table 26-2)
		(1 << TWINT)|	// start_next_operation();
		(1 << TWSTA);	// generate_START_condition();
    }

    /// Enviamos una repeated START condition.
    static void send_repeated_start()
    {
	wait_until_finished_its_current_job();
	send_start();
    }

    /// true si ha fallado el envío de la start condition.
    static bool send_start_fail()
    {
	wait_until_finished_its_current_job();
	return (status_bits() != SRC_START);
    }

    /// true si ha fallado el envío de la repeated start condition.
    static bool send_repeated_start_fail()
    {
	wait_until_finished_its_current_job();
	return (status_bits() != SRC_REPEATED_START);
    }


    /// Enviamos una STOP condition.
    static void send_stop()
    {
	TWCR =	(1 << TWEN)|	// enable(); (ver datasheet table 26-2)
		(1 << TWINT)|	// start_next_operation();
		(1 << TWSTO);	// generate_START_condition();
    }

    /// Devuelve distinto de cero si ha ido algo mal en el envío 
    /// de la stop condición.
//    static uint8_t send_stop_fail(); <-- curiosamente en la datasheet
//    no menciona que se pueda comprobar que el stop falle. Total, hemos
//    acabado al enviar stop. ¿qué mas da?


    /// Enviamos el byte x. Puede ser un SLA+W/R o datos.
    static void send_byte(std::byte x)
    {
	TWDR = std::to_integer<uint8_t>(x);
	TWCR =	(1 << TWEN)|	// enable(); (ver datasheet table 26-2)
		(1 << TWINT);	// start_next_operation();
    }

    /// true si el envío de SLA + W (address + read) ha sido ACK. 
    static bool answer_send_address_ACK()
    {
	wait_until_finished_its_current_job();
	return (status_bits() == SRC_MTM_ADRESS_ACK);
    }

    /// true si el envío de SLA + W (address + read) ha sido NACK. 
    static bool answer_send_address_NACK()
    { return !answer_send_address_ACK();}

    /// true si el envío de SLA + R (address + read) ha sido ACK. 
    static bool answer_receive_address_ACK()
    {
	wait_until_finished_its_current_job();
	return (status_bits() == SRC_MRM_ADRESS_ACK);
    }

    /// true si el envío de SLA + R (address + read) ha sido NACK. 
    static bool answer_receive_address_NACK()
    { return !answer_receive_address_ACK();}


    /// true si la respuesta del send_data ha sido ACK.
    static bool answer_send_data_ACK()
    {
	wait_until_finished_its_current_job();
	return (status_bits() == SRC_MTM_DATA_ACK);
    }

    /// true si la respuesta del send_data ha sido NACK.
    static bool answer_send_data_NACK()
    {
	return !answer_send_data_ACK();
    }

    /// Recibimos un byte envíando ACK en caso de éxito.
    /// Metemos en x el byte recibido.
    /// En caso de que todo vaya bien devuelve 0, en caso de error
    /// devuelve un valor distinto de 0.
    static bool receive_data_with_ACK(std::byte& x);

    /// Devuelve 1 si se ha recibido un dato y se ha enviado ACK como
    /// respuesta. 0 en caso contrario.
    static bool answer_receive_data_with_ACK_ok()
    {
	wait_until_finished_its_current_job();
	return (status_bits() == SRC_MRM_DATA_ACK);
    }

    /// Devuelve 1 si no se ha recibido un dato o no se ha generado ACK
    /// como respuesta. 
    static bool answer_receive_data_with_ACK_fail()
    { return !answer_receive_data_with_ACK_ok();}



    /// Recibimos un byte envíando NACK en caso de éxito.
    /// Metemos en x el byte recibido.
    /// En caso de que todo vaya bien devuelve 0, en caso de error
    /// devuelve un valor distinto de 0.
    static bool receive_data_with_NACK(std::byte& x);

    /// Devuelve 1 si se ha recibido un dato y se ha enviado NACK como
    /// respuesta. 0 en caso contrario.
    static bool answer_receive_data_with_NACK_ok()
    {
	wait_until_finished_its_current_job();
	return (status_bits() == SRC_MRM_DATA_NACK);
    }

    /// Devuelve 1 si no se ha recibido un dato o no se ha generado NACK
    /// como respuesta. 
    static bool answer_receive_data_with_NACK_fail()
    { return !answer_receive_data_with_NACK_ok();}


    /// Espera hasta acabar la transmisión en curso. Una vez acabada,
    /// se puede proceder a enviar más datos.
    static void wait_until_finished_its_current_job()
    {
	while(!has_finished_its_current_job())
	    ;
    }

    /// Envía al dispositivo de dirección address, los bytes data[0-n).
    /// No espera a que la transmisión haya finalizado.
    /// Devuelve el número de bytes enviados.
    static uint8_t send(uint8_t address, std::byte x);

    static uint8_t send(uint8_t address, const std::byte* data, uint8_t n);

    /// Recibe del dispositivo de dirección address, n bytes, escribiéndolos
    /// en data[0-n). Al recibir el último byte se envia NACK.
    /// Devuelve el número de bytes recibidos.
    static uint8_t receive(uint8_t address, std::byte* data, uint8_t n);

    // Es fundamental enviar send_stop en caso de que falle algo para poder
    // seguir con las comunicaciones. Start se encarga de llamar a send_start
    // y el destructor de llamar a send_stop.
    struct Start;

    // Status register codes
    // ---------------------
    // General TWI Master staus codes (table 26-3)
    constexpr static uint8_t SRC_START = 0x08;  // START has been transmitted  
    constexpr static uint8_t SRC_REPEATED_START = 0x10;  // Repeated START has been transmitted
    constexpr static uint8_t SRC_ARBITRATION_LOST = 0x38;  // Arbitration lost

    // TWI Master Transmitter status codes (table 26-3)
    constexpr static uint8_t SRC_MTM_ADRESS_ACK = 0x18;  // SLA+W has been tramsmitted and ACK received
    constexpr static uint8_t SRC_MTM_ADRESS_NACK = 0x20;  // SLA+W has been tramsmitted and NACK received 
    constexpr static uint8_t SRC_MTM_DATA_ACK = 0x28;  // Data byte has been tramsmitted and ACK received
    constexpr static uint8_t SRC_MTM_DATA_NACK = 0x30;  // Data byte has been tramsmitted and NACK received 

    // TWI Master Receiver status codes  
    constexpr static uint8_t SRC_MRM_ADRESS_ACK = 0x40;  // SLA+R has been tramsmitted and ACK received
    constexpr static uint8_t SRC_MRM_ADRESS_NACK = 0x48;  // SLA+R has been tramsmitted and NACK received
    constexpr static uint8_t SRC_MRM_DATA_ACK = 0x50;  // Data byte has been received and ACK tramsmitted
    constexpr static uint8_t SRC_MRM_DATA_NACK = 0x58;  // Data byte has been received and NACK tramsmitted
//
//    // TWI Slave Transmitter status codes
//    constexpr static uint8_t SRC_STX_ADR_ACK = 0xA8;  // Own SLA+R has been received; ACK has been returned
//    constexpr static uint8_t SRC_STX_ADR_ACK_M_ARB_LOST = 0xB0;  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
//    constexpr static uint8_t SRC_STX_DATA_ACK = 0xB8;  // Data byte in TWDR has been transmitted; ACK has been received
//    constexpr static uint8_t SRC_STX_DATA_NACK = 0xC0;  // Data byte in TWDR has been transmitted; NOT ACK has been received
//    constexpr static uint8_t SRC_STX_DATA_ACK_LAST_BYTE = 0xC8;  // Last data byte in TWDR has been transmitted (TWEA = 0); ACK has been received
//
//    // TWI Slave Receiver status codes
//    constexpr static uint8_t SRC_SRX_ADR_ACK = 0x60;  // Own SLA+W has been received ACK has been returned
//    constexpr static uint8_t SRC_SRX_ADR_ACK_M_ARB_LOST = 0x68;  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
//    constexpr static uint8_t SRC_SRX_GEN_ACK = 0x70;  // General call address has been received; ACK has been returned
//    constexpr static uint8_t SRC_SRX_GEN_ACK_M_ARB_LOST = 0x78;  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
//    constexpr static uint8_t SRC_SRX_ADR_DATA_ACK = 0x80;  // Previously addressed with own SLA+W; data has been received; ACK has been returned
//    constexpr static uint8_t SRC_SRX_ADR_DATA_NACK = 0x88;  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
//    constexpr static uint8_t SRC_SRX_GEN_DATA_ACK = 0x90;  // Previously addressed with general call; data has been received; ACK has been returned
//    constexpr static uint8_t SRC_SRX_GEN_DATA_NACK = 0x98;  // Previously addressed with general call; data has been received; NOT ACK has been returned
//    constexpr static uint8_t SRC_SRX_STOP_RESTART = 0xA0;  // A STOP condition or repeated START condition has been received while still addressed as Slave
//
    // TWI Miscellaneous status codes
    constexpr static uint8_t SRC_NO_STATE = 0xF8;  // No relevant state information available
    constexpr static uint8_t SRC_BUS_ERROR = 0x00;  // Bus error due to an illegal START or STOP condition


};


// Los siguientes datos proceden de la tabla 1-1 de la application note
// AVR315.
template <>
inline void TWI::SCL_frequency_in_kHz<400u, 16000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 12;
}


template <>
inline void TWI::SCL_frequency_in_kHz<100u, 16000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 72;
}


template <>
inline void TWI::SCL_frequency_in_kHz<400u, 14400000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 10;
}

template <>
inline void TWI::SCL_frequency_in_kHz<100u, 14400000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 64;
}


template <>
inline void TWI::SCL_frequency_in_kHz<400u, 12000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 7;
}


template <>
inline void TWI::SCL_frequency_in_kHz<100u, 12000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 52;
}

template <>
inline void TWI::SCL_frequency_in_kHz<400u, 8000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 2;
}


template <>
inline void TWI::SCL_frequency_in_kHz<100u, 8000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 32;
}

template <>
inline void TWI::SCL_frequency_in_kHz<100u, 4000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 12;
}


template <>
inline void TWI::SCL_frequency_in_kHz<100u, 3600000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 10;
}


template <>
inline void TWI::SCL_frequency_in_kHz<100u, 2000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 2;
}

template <>
inline void TWI::SCL_frequency_in_kHz<50u, 2000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 12;
}

template <>
inline void TWI::SCL_frequency_in_kHz<50u, 1000000uL>()
{
    bit_rate_prescaler_value_1();
    TWBR = 2;
}


struct TWI::Start{
    Start() {TWI::send_start();}
    ~Start() {TWI::send_stop();}

    explicit operator bool() const {return !TWI::send_start_fail();}

};

}// namespace


#endif

