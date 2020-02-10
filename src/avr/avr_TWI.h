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
 *
 *   - HISTORIA:
 *    A.Manuel L.Perez
 *    24/08/2019 v0.0
 *    27/01/2020 Divido en traductor TWI_basic y driver TWI, reescribo todo.
 *
 ****************************************************************************/

#include <cstddef>    // std::byte
//#include <atd_register.h>
#include <atd_bit.h>

#include "avr_cfg.h"

namespace avr{

// Table 26-5, junto con el diagrama figure 26-16.
struct __TWI_basic_ioreturn_slave_transmitter_mode {
    static constexpr uint8_t sla_r            = TWI_STM_SLA_R;
    static constexpr uint8_t arbitration_lost = TWI_STM_ARBITRATION_LOST;
    static constexpr uint8_t data_ack         = TWI_STM_DATA_ACK;
    static constexpr uint8_t data_nack        = TWI_STM_DATA_NACK;
    static constexpr uint8_t data_last_byte   = TWI_STM_DATA_LAST_BYTE;
};


// Table 26-6, junto con el diagrama figure 26-18.
struct __TWI_basic_ioreturn_slave_receiver_mode {
    static constexpr uint8_t sla_w = TWI_SRM_SLA_W;
    static constexpr 
    uint8_t arbitration_lost_sla_w = TWI_SRM_ARBITRATION_LOST_SLA_W;
    static constexpr uint8_t sla_w_data_ack  = TWI_SRM_SLA_W_DATA_ACK;
    static constexpr uint8_t sla_w_data_nack = TWI_SRM_SLA_W_DATA_NACK;

    static constexpr 
    uint8_t general_call = TWI_SRM_GENERAL_CALL;
    static constexpr 
    uint8_t arbitration_lost_general_call = TWI_SRM_ARBITRATION_LOST_GENERAL_CALL;
    static constexpr 
    uint8_t general_call_data_ack = TWI_SRM_GENERAL_CALL_DATA_ACK;
    static constexpr 
    uint8_t general_call_data_nack = TWI_SRM_GENERAL_CALL_DATA_NACK;

    static constexpr
    uint8_t stop_or_repeated_start = TWI_SRM_STOP_OR_REPEATED_START;
};





struct __TWI_basic_ioreturn{

    using slave_receiver_mode    = __TWI_basic_ioreturn_slave_receiver_mode;
    using slave_transmitter_mode = __TWI_basic_ioreturn_slave_transmitter_mode;

    // Miscellaneous (table 26-7)
    static constexpr uint8_t bus_error = TWI_BUS_ERROR;
    static constexpr uint8_t running   = TWI_RUNNING;
};


/*!
 *  \brief  Traductor literal de la datasheet.
 *
 *  IMPORTANTE (para principiantes):
 *	clear TWINT = write 0
 *	set TWINT = write 1
 *	Sin embargo, parece ser que es habitual en micros que cuando se quiere
 *	hacer un clear en un bit se escriba un 1, por eso la datasheet dice
 *	que si se quiere hacer clear(TWINT) hay que escribir un 1 en TWINT.
 *	Esto lo que hace es que se escriba un 0. De hecho me he tirado un buen
 *	rato (demasiado para mi gusto) mirando precisamente esto: yo escribo
 *	TWINT = 1, pero al mirar su valor resulta ser 0!!! 
 *
 *	El flujo de TWI usando interrpuciones es el siguiente:
 *	TWINT = 0 <-- a la espera de que suceda algo
 *	sucede algo y se necesita respuesta por parte del usuario:
 *	TWINT = 1 (= ey! usuario, tienes que hacer algo!!!)
 *	Si TWIE = 1 y activadas las interrupciones se lanza una interrupción,
 *	Dentro del handler de la ISR siempre tendremos TWINT = 1 (precisamente
 *	es lo que lanza la interrupción).
 *	Después de procesar la interrupción, para decirle a TWI que continue
 *	tenemos que hacer clear a TWINT, para ello:
 *	1. escribimos TWINT = 1
 *	2. el hardware traduce esto inmediatamente a TWINT= 0.
 *
 *	Recordar que mientras no hagamos clear(TWINT) SCL será igual a 0 (lo
 *	cual nos va a interesar si lleva tiempo procesar lo que estemos
 *	haciendo).
 *
 *	Recordar:
 *	    1.- Escribir un 0 en TWINT = NO HACE NADA!!!
 *	    2.- Escribir un 1 en TWINT = hace un clear.
 */
class TWI_basic{
public:
    using ioreturn = __TWI_basic_ioreturn;

    /// Selects the division factor for the bit rate generator.
    /// The bit rate generatro is a frequency divider which generatres the
    /// SCL clock frequency in the Master modes.
    static void division_factor_for_bit_rate_generator(uint8_t d)
    { TWBR = d; }
    
    /// Devuelve el division factor for the bit rate generator.
    static uint8_t division_factor_for_bit_rate_generator()
    {return TWBR;}


    /// Devuelve el status bit.
    static uint8_t status()
    { return TWSR & TWI_MASK_STATUS_BITS;}


    // Selección de la velocidad del reloj
    // -----------------------------------
    static void bit_rate_prescaler_value_1()
    { atd::write_bits<TWPS1, TWPS0>::to<0,0>::in(TWSR); }

    static void bit_rate_prescaler_value_4()
    { atd::write_bits<TWPS1, TWPS0>::to<0,1>::in(TWSR); }

    static void bit_rate_bit_rate_prescaler_value_16()
    { atd::write_bits<TWPS1, TWPS0>::to<1,0>::in(TWSR); }

    static void bit_rate_bit_rate_prescaler_value_64()
    { atd::write_bits<TWPS1, TWPS0>::to<1,1>::in(TWSR); }

    /// Devuelve los bits con el rate prescaler seleccionado.
    static uint8_t bit_rate_prescaler()
    { return TWSR & TWI_MASK_BIT_RATE_PRESCALER;}


    /// Definimos la frecuencia del reloj SCL.
    /// f_scl = frecuencia en kHz de SCL (tipica: 100 kHz y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = AVR_CLOCK_FREQUENCY_IN_HZ>
    static void SCL_frequency_in_kHz();
    



    /// ¿Queremos que en multimaster systems se responda a la general call
    /// address (0x00)?
    static void enable_general_call()
    { atd::write_bit<TWGCE>::to<1>::in(TWAR);}

    static void disable_general_call()
    { atd::write_bit<TWGCE>::to<0>::in(TWAR);}

    static bool is_general_call_enabled()
    { return atd::is_one_bit<TWGCE>::of(TWAR);}

    // Observar que TWDR es el buffer del hardware de TWI. Es un buffer
    // de 1 byte que funciona a la vez como put/get area. ¿Sería mejor
    // llamarlo buffer?
    /// Dato que queremos enviar a continuación.
    static void data(std::byte x)
    { TWDR = std::to_integer<uint8_t>(x); }

    /// Dato que hemos recibido.
    static std::byte data()
    { return std::byte{TWDR};}


    /// Enables TWI as a master.
    static void enable()
    { atd::write_bit<TWEN>::to<1>::in(TWCR);}

    /// Set up TWI slave to its initial standby state.
    /// Remember to enable interrupts after initializing the TWI.
    // TWI_on = indica si vamos a gestionar TWI usando interrupciones (=1) o
    // mediante polling (= 0). En principio no parece que tenga mucho sentido
    // usar polling para manejar el slave, pero lo dejo configurable ya que
    // carezco de experiencia.
    template <uint8_t TWI_slave_address, uint8_t TWIE_on = 1>
    static void init_slave()
    {
	atd::write_range_bits<TWI_SLAVE_ADDRESS_BIT0,TWI_SLAVE_ADDRESS_BITn>::
	    to<TWI_slave_address>::in(TWAR); // TWAR = TWI_slave_address;

	atd::write_bits<TWINT, TWEA, TWSTA, TWSTO, TWWC, TWEN, TWIE>::
	             to<  0  ,   1 ,   0  ,   0  ,   0 ,  1, TWIE_on>::in(TWCR);
    }

    /// Switched to the not addressed Slave mode (esto es, se queda a la espera
    /// de que algún master le llame).
    /// Own SLA will be recognized. GCA will be recognized if
    /// enable_general_call().
    static void not_addressed_slave_mode()
    { atd::write_bits<TWSTA, TWSTO, TWINT, TWEA>::to<0,0,1,1>::in(TWCR); }

    /// Switched to the not addressed Slave mode (esto es, se queda a la espera
    /// de que algún master le llame).
    /// no recognition of own SLA or GCA
    static void not_addressed_slave_no_recognition_mode()
    { atd::write_bits<TWSTA, TWSTO, TWINT, TWEA>::to<0,0,1,0>::in(TWCR); }

    /// Disables TWI interface.
    static void disable()
    { atd::write_bit<TWEN>::to<0>::in(TWCR);}


    /// The TWI interrupt request will be activated.
    // OJO: hay que definir globalmente las interrupciones para que esto
    // funcione: Interrupt::enable_all_interrupts();
    // Observar que hay que escribir un 0 en TWINT. Si no se escribe, no se
    // por qué pero se hace un clear de TWINT (ojo: tal como está ahora
    // implementado write_bit es normal ya que write_bits reescribe TWCR y 
    // al escribir un 1 en TWINT hacemos el clear. Sin embargo, lo he probado
    // a hacerlo directamente y sigue haciendo el clear a TWINT). Además, el
    // ejemplo de la app.note avr315 desactiva las interrupciones de esta
    // forma.
    static void interrupt_enable()
    { atd::write_bits<TWIE, TWINT>::to<1,0>::in(TWCR);}

    /// The TWI interrupt request will be disactivated.
    // Fundamental poner TWINT = 0 (no hace nada). Ver comentarios de
    // interrupt_enable
    static void interrupt_disable()
    { atd::write_bits<TWIE, TWINT>::to<0,0>::in(TWCR);}


    // Actions by Master Mode (see: tables 26-3/4)
    // --------------------------------------------
    /// A START condition will be transmitted when the bus becomes free.
    static void transmit_start()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<1,0,1>::in(TWCR); }

    /// Repeated START will be transmitted.
    static void transmit_repeated_start()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<1,0,1>::in(TWCR); }

    /// STOP condition will be transmitted.
    static void transmit_stop()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<0,1,1>::in(TWCR); }

    /// STOP condition followed by a START condition will be transmitted.
    static void transmit_stop_followed_by_start()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<1,1,1>::in(TWCR); }


    /// SLA+R/W or data byte will be transmitted.
    static void transmit_byte(uint8_t x)
    {
	TWDR = x;
	atd::write_bit<TWSTA, TWSTO, TWINT>::to<0,0,1>::in(TWCR);
    }

    /// Enviamos el byte x. Puede ser un SLA+W/R o datos.
    static void transmit_byte(std::byte x)
    { transmit_byte(std::to_integer<uint8_t>(x)); }


    /// Comenzamos a recibir el siguiente byte, devolviendo
    /// ACK cuando lo hayamos recibido.
    static void start_receive_data_with_ACK()
    { atd::write_bit<TWSTA, TWSTO, TWINT, TWEA>::to<0,0,1,1>::in(TWCR);}

    /// Comenzamos a recibir el siguiente byte, devolviendo
    /// NACK cuando lo hayamos recibido.
    static void start_receive_data_with_NACK()
    { atd::write_bit<TWSTA, TWSTO, TWINT, TWEA>::to<0,0,1,0>::in(TWCR);}


    // Actions by Slave Mode (see: tables 26-5/6)
    // ------------------------------------------
    /// Data byte will be transmitted and NACK should be received.
    static void slave_transmit_byte_received_NACK(uint8_t x)
    {
	TWDR = x;
	atd::write_bit<TWSTO, TWINT, TWEA>::to<0,1,0>::in(TWCR);
    }

    /// Data byte will be transmitted and NACK should be received.
    static void slave_transmit_byte_received_ACK(uint8_t x)
    {
	TWDR = x;
	atd::write_bit<TWSTO, TWINT, TWEA>::to<0,1,1>::in(TWCR);
    }

    /// Data byte will be received and NACK will be returned.
    static void slave_receive_data_with_NACK()
    { atd::write_bit<TWSTO, TWINT, TWEA>::to<0,1,0>::in(TWCR); }

    /// Data byte will be received and ACK will be returned.
    static void slave_receive_data_with_ACK()
    { atd::write_bit<TWSTO, TWINT, TWEA>::to<0,1,1>::in(TWCR); }

    // Miscellaneous States (table 26-7)
    // ---------------------------------
    static void recover_from_bus_error()
    { atd::write_bit<TWSTA, TWSTO, TWINT>::to<0,1,1>::in(TWCR); }


    // Other
    // -----
    /// Espera hasta acabar la transmisión en curso. Una vez acabada,
    /// se puede proceder a enviar más datos.
    static void wait_until_finished_its_current_job()
    {
	while(!atd::is_one_bit<TWINT>::of(TWCR))
	{ }
    }

    /// Compone el SLA+W command
    template <uint8_t TWI_address>
    static constexpr uint8_t SLA_W() { return (TWI_address << 1); }

    /// Compone el SLA+R command
    template <uint8_t TWI_address>
    static constexpr uint8_t SLA_R() { return ((TWI_address << 1) | 0x01); }
};



// -------------
// Estado de TWI
// -------------
enum class __TWI_state{
    good = 0, 
    send_start_fail,
    send_repeated_start_fail,
    send_address_fail,
    send_data_fail,
    receive_data_fail,
    read_send_command_fail
};

inline constexpr __TWI_state operator&(__TWI_state a, __TWI_state b)
{return static_cast<__TWI_state>(static_cast<int>(a) & static_cast<int>(b));}

inline constexpr __TWI_state operator|(__TWI_state a, __TWI_state b)
{return static_cast<__TWI_state>(static_cast<int>(a) | static_cast<int>(b));}

inline constexpr __TWI_state operator^(__TWI_state a, __TWI_state b)
{return static_cast<__TWI_state>(static_cast<int>(a) ^ static_cast<int>(b));}

inline constexpr __TWI_state operator~(__TWI_state a)
{return static_cast<__TWI_state>(~static_cast<int>(a));}

inline constexpr __TWI_state& operator&=(__TWI_state& a, __TWI_state b)
{return a = a & b;}

inline constexpr __TWI_state& operator|=(__TWI_state& a, __TWI_state b)
{return a = a | b;}

inline constexpr __TWI_state& operator^=(__TWI_state& a, __TWI_state b)
{return a = a ^ b;}

inline constexpr bool operator==(__TWI_state a, int b)
{return static_cast<int>(a) == b;}

inline constexpr bool operator!=(__TWI_state a, int b)
{return !(a == b);}



/*!
 *  \brief  Driver de TWI
 */
class TWI : public TWI_basic{
public:
    using iostate = __TWI_state;

    /// Enables TWI interface definiendo la frecuencia del SCL 
    /// a la que vamos a operar.
    /// f_scl = frecuencia en kilohercios de SCL (tipica: 100 y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = AVR_CLOCK_FREQUENCY_IN_HZ>
    struct on{
	static void as_a_master()
	{
	    SCL_frequency_in_kHz<f_scl, f_clock>();
	    enable();
	    state_ = iostate::good;
	}

	// Enables as a slave in the address SLAVE_ADDRESS.
	template <uint8_t TWI_slave_address>
	static void as_a_slave_in()
	{
	    SCL_frequency_in_kHz<f_scl, f_clock>();
	    init_slave<TWI_slave_address>();
	    state_ = iostate::good;
	}
    };



    /// Turn off TWI.
    static void off() {disable();}

    /// Recibe del dispositivo de dirección address, n bytes, escribiéndolos
    /// en data[0-n). 
    /// Return: Devuelve el número de bytes recibidos.
    /// Error: en caso de error, state() != 0.
    template <uint8_t TWI_address>
    static uint8_t read(const std::byte& command, // byte que enviamos
		 std::byte* data,	// donde almacenamos los datos
		 uint8_t n);		// número FIJO de bytes a leer




    /// Escribe los bytes data[0-n) en el dispositivo de dirección address.
    /// Return: Devuelve el número de bytes escritos.
    /// Error: en caso de error, state() != 0.
    template <uint8_t TWI_address>
    static uint8_t write(const std::byte* data, uint8_t n);

    template <uint8_t TWI_address>
    static uint8_t write(std::byte x)
    {return write<TWI_address>(&x, 1);}


// state
    static iostate state() {return state_;}


private:
// Data
    inline static iostate state_;

    // Es fundamental enviar send_stop en caso de que falle algo para poder
    // seguir con las comunicaciones. 
    struct Stop_transmission;

// Functions
    // Transmit start y espera hasta finalizar la transmisión.
    // Return:	on success 1
    //		on error 0
    static uint8_t send_start();
    static uint8_t send_repeated_start();

    // Transmit SLA+W y espera hasta finalizar la transmisión.
    // Return:	on success 1
    //		on error 0
    // Error: En caso de error state() != 0
    template <uint8_t TWI_address>
    static uint8_t send_sla_w();

    // Transmit SLA+R y espera hasta finalizar la transmisión.
    // Return:	on success 1
    //		on error 0
    // Error: En caso de error state() != 0
    template <uint8_t TWI_address>
    static uint8_t send_sla_r();

    // Transmit data[0-n) y espera hasta finalizar la transmisión.
    // Return:	the number of bytes written.
    // Error: En caso de error state() != 0
    static uint8_t send_data(const std::byte* data, uint8_t n);

    // Receive n bytes almacenandolos en data[0-n).
    // Return:	the number of bytes written.
    // Error: En caso de error state() != 0
    static uint8_t receive_data(std::byte* data, uint8_t n);
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


// Responsable de send_stop
struct TWI::Stop_transmission{
    ~Stop_transmission() {TWI_basic::transmit_stop();}
};


template <uint8_t TWI_address>
uint8_t TWI::send_sla_w()
{
    // write = 0, luego basta con hacer un shift a la izda
    transmit_byte(SLA_W<TWI_address>());

    wait_until_finished_its_current_job();

    if (status() != TWI_MTM_ADDRESS_ACK){
	state_ = iostate::send_address_fail;
	return 0;
    }

    return 1;
}


template <uint8_t TWI_address>
uint8_t TWI::send_sla_r()
{
    transmit_byte(SLA_R<TWI_address>());

    wait_until_finished_its_current_job();

    if (status() != TWI_MRM_ADDRESS_ACK){
	state_ = iostate::send_address_fail;
	return 0;
    }

    return 1;
}




// See table 26-2
template <uint8_t TWI_address>
uint8_t TWI::write(const std::byte* data, uint8_t n)
{
    // DUDA: si esta en mal estado devolvemos error???
    // DUDA: si TWI no está enable, ¿lo encendemos? (<- no, ya que 
    // hay que indicar la frecuencia) ¿error?
    state_ = iostate::good;

    Stop_transmission stop;

    if (send_start() == 0)
	return 0;

    if (send_sla_w<TWI_address>() == 0)
	return 0;

    return send_data(data, n);
}


template <uint8_t TWI_address>
uint8_t TWI::read(const std::byte& command, // byte que enviamos
	     std::byte* data,	// donde almacenamos los datos
	     uint8_t n)		// número FIJO de bytes a leer
{
    // DUDA: si esta en mal estado devolvemos error???
    // DUDA: si TWI no está enable, ¿lo encendemos? (<- no, ya que 
    // hay que indicar la frecuencia) ¿error?
    state_ = iostate::good;

    Stop_transmission stop;

    if (send_start() == 0)
	return 0;

    if (send_sla_w<TWI_address>() == 0)
	return 0;

    if (send_data(&command, 1) != 1){
	state_ = iostate::read_send_command_fail;
	return 0;
    }

    if (send_repeated_start() == 0)
	return 0;

    if (send_sla_r<TWI_address>() == 0)
	return 0;

    return receive_data(data, n);
}



}// namespace


#endif

