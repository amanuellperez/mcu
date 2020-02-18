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

#ifndef __AVR_TWI_MASTER_H__
#define __AVR_TWI_MASTER_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Implementación de un master de TWI genérico con buffer.
 *
 *  - COMENTARIOS: La idea es que se pueda usar con cualquier dispositivo TWI
 *  (ya sea avr, pic, ...). De momento supongo que dependerá del avr ya que es
 *  el único TWI que tengo.
 *	Es un proyecto de aprendizaje así que es posible que cambie bastante
 *  en el futuro.
 *
 *  - TIPO DE USUARIO: de bajo nivel. Tiene que saber lo que hace. No impongo
 *  muchas precondiciones en código.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    14/02/2020 v0.0
 *
 ****************************************************************************/
#include "avr_interrupt.h" 
#include "mcu_cfg.h"

#include <atd_buffer.h>



namespace avr{

enum class __TWI_master_state{
// group of states
    idle = 0x00,
    busy = 0x01,
    waiting = 0x02,

// state
    ok		    = (1 << 2) |idle,
    no_response	    = (2 << 2) |idle,
    eow_data_nack   = (3 << 2) |idle,
    eow		    = (4 << 2) |waiting,

    eor_bf	    = (5 << 2) |waiting,    // buffer full
    eor		    = (6 << 2) |waiting,

    transmitting    = (20 << 2) |busy,
    receiving	    = (21 << 2) |busy,

    bus_error	    = (30 << 2) |idle,
    unknown_error   = (31 << 2) |idle
};

constexpr uint8_t __TWI_master_state_mask_group_states = 0x03;

bool __TWI_master_state_is_group(__TWI_master_state a, __TWI_master_state group)
{
    return 
    (static_cast<uint8_t>(a) & __TWI_master_state_mask_group_states) == 
	static_cast<uint8_t>(group);
}

bool __TWI_master_state_is_busy(__TWI_master_state a)
{
    return __TWI_master_state_is_group(a, __TWI_master_state::busy);
}

bool __TWI_master_state_is_waiting(__TWI_master_state a)
{
    return __TWI_master_state_is_group(a, __TWI_master_state::waiting);
}

bool __TWI_master_state_is_idle(__TWI_master_state a)
{
    return __TWI_master_state_is_group(a, __TWI_master_state::idle);
}


// La idea es que esto no sea más que una ampliacion del TWI con 1 byte
// de buffer. Le ponemos una put/get area a TWI en esta clase y ocultamos el
// protocolo al usuario.
// Responsabilidades de esta clase:
//	1. Ponerle un buffer al hardware de TWI (que carece de el).
//
// IDEA CLAVE: toda la clase tiene que estar implementada usando la notación
// del protocolo TWI (I2C), no usar nada del avr. De esa forma se podrá
// desvincular del avr en el futuro.
//
//  DUDAS 
//	+ Si hay un error de protocolo y el master quiere recibir datos pero 
//	  el slave no envía datos se queda colgado esperando a recibir datos. 
//	  En principio como funcionamos por interrupciones no bloqueamos el 
//	  microcontrolador, pero ¿cómo desbloquear la comunicación?
//
template <typename TWI, uint8_t buffer_size>
class TWI_master{
public:
    using iostate= __TWI_master_state;
    using streamsize = uint8_t;


    /// Enables TWI interface definiendo la frecuencia del SCL 
    /// a la que vamos a operar.
    /// f_scl = frecuencia en kilohercios de SCL (tipica: 100 y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on()
    {
	TWI::template SCL_frequency_in_kHz<f_scl, f_clock>();
	avr::enable_all_interrupts();

	init();
    }

    /// Pone el master en receiver mode y comienza a leer. Lee exactamente 
    /// 'n' bytes. En caso de que n > buffer_size no hace nada.
    /// Returns: número de bytes que va a leer.
    /// Devuelve 0 cuando: 
    ///		1. El dispositivo está ocupado.
    ///		2. Cuando n > buffer_size or n == 0.
    /// En caso contrario siempre devuelve 'n'.
    // FUTURO: si se usa esto bastante, relajarlo. Si n > buffer_size que 
    // lea n buffer_size, y pase a eor_bne, esperando al usuario que llame a
    // read_buffer, y después continue leyendo.

    template <uint8_t slave_address> 
    static streamsize read_from(streamsize n)
    {
	slave_address_ = slave_address;
	return read(n);
    }


    /// Lee el buffer del dispositivo almacenándolos en q[0,N).
    /// Lee como máximo N bytes.
    /// precondition: state() == iostate::eor_bf
    /// postcondition: state() == iostate::eor
    /// Returns: number of bytes leidos.
    /// OJO: si n > N de momento no esta pensado que se pueda seguir llamando
    /// a read_buffer para acabar de vaciarlo. Lo dejo para el futuro
    /// (realmente merece la pena? es posible que no. Todo es más sencillo
    /// si los buffers son suficientemente grandes).
    static streamsize read_buffer(std::byte* q, streamsize N);



    /// Escribe en el buffer de salida buf[0, n) y comienza el envío por TWI.
    /// Para ver si ha acabado la transmisión hay que mirar if (is_busy())
    /// wait();
    /// Devuelve el número de bytes escritos.
    /// A día de hoy si se n > buffer_size no hace nada (devuelve 0), 
    /// ya que esta primera
    /// versión está limitada a enviar el buffer completo de una sola vez (no
    /// se puede escribir poco a poco). Si en el futuro se usa, cambiarlo y
    /// darle más flexibilidad. Esta es una versión de aprendizaje.
    // Observar que tal como está implementado se puede pasar de
    // waiting/eor_bf a transmitting. De momento no estoy imponiendo en código
    // el diagrama de estados. Doy por supuesto que el usuario sabe lo que
    // hace.
    template <uint8_t slave_address> // tiene que estar definido en tiempo de compilación!!!
    static streamsize write_to(const std::byte* buf, streamsize n)
    {
	if (is_busy())	// precondition
	    return 0;

	buffer_.reset_as_output();

	if (buffer_.out_write_all_n(buf, n) != n)
	    return 0;

	slave_address_ = slave_address;

	twi_transmit_start();

	state_ = iostate::transmitting;
	TWI::interrupt_enable();

	return n;
    }

    /// Finaliza la transmisión. Envía STOP.
    static void stop_transmission();

    /// Reinicializa el dispositivo.
    /// En caso de que se quede colgado intentando leer, llamar a esta
    /// función.
    static void reset()
    {
	TWI::master_reset();
	init();
    }

    // Función que va dentro de la ISR
    static void handle_interrupt();

// estados
// -------
// grupos
    static bool is_idle() {return __TWI_master_state_is_idle(state_);}
    static bool is_busy() {return __TWI_master_state_is_busy(state_);}
    static bool is_waiting() {return __TWI_master_state_is_waiting(state_);}

// errores genéricos
    static bool no_response() {return state_ == iostate::no_response;}

// de escritura
    static bool eow() {return state_ == iostate::eow;}
    static bool eow_data_nack() {return state_ == iostate::eow_data_nack;}

// de lectura
    static bool eor() {return state_ == iostate::eor;}
    static bool eor_bf() {return state_ == iostate::eor_bf;}

// genérico
    static bool ok() {return state_ == iostate::ok;}

private:
    // FUNDAMENTAL: busy_ tiene que ser volatile, ya que lo voy a usar:
    //	    while (is_busy()) { ; }
    //	Si no lo se define volatile el compilador optimiza y eso se convierte
    //	en un bucle infinito (y tardas un par de horas en encontrar el error.
    //	Antes funcionaba bien y de repente dejó de funcionar!!!)
    static inline volatile iostate state_;

    static inline atd::TWI_iobuffer<buffer_size> buffer_;
    static inline streamsize nread_;   // num. bytes a leer (solo para receiver mode)

    // (RRR) El usuario pide leer o escribir. Para ello:
    //	     1. Enviamos start.
    //	     2. Cuando TWI responde que ha acabado el envio, enviamos
    //		SLA+w/r. Es en este momento cuando necesitamos el
    //		slave_address.
    //
    //	     Dos formas de implementar esto:
    //	     1. En write_to<slave_address>(...) enviamos start, esperamos a que
    //	        TWI responda que lo ha enviado y enviamos sla+w. No
    //	        necesitamos memorizar slave_address pero bloqueamos write a la
    //	        espera de que se finalice el envio del 'start'.
    //	     2. En write_to<slave_address>(...) enviamos start y memorizamos
    //	        slave_address_, devolviendo el control al usuario. Cuando TWI
    //	        responde que ha enviado 'start' enviamos sla+w. No bloqueamos
    //	        write, pero tenemos que memorizar. De momento pruebo a usar
    //	        esta opción ya que una idea de diseño de este TWI_master es
    //	        que no bloque nunca.
    //
    static inline uint8_t slave_address_;


    // Helper functions
    // ----------------
    static void init()
    {
	TWI::enable();
	state_ = iostate::ok;
	TWI::interrupt_disable();   // es master, nunca va a estar a la escucha
    }

    // Implementación de read_from
    static streamsize read(streamsize n);

    // Inicia una nueva transmisión, enviando START o REPEATED START,
    // dependiendo de si estabamos en medio de una transmisión o no.
    static void twi_transmit_start();

// Respuesta a los estados comunes
    static void mm_start(uint8_t slave_address);
    static void mm_repeated_start(uint8_t slave_address);

// Respuesta a los estados de TWI en master receiver mode
    static void mrm_sla_r_ack();
    static void mrm_sla_r_nack();
    static void mrm_data_ack();
    static void mrm_data_nack();

    // Implementación:
	static void mrm_receive_next_byte();


// Respuesta a los estados de TWI en master transmitter mode:
    static void mtm_sla_w_ack();
    static void mtm_sla_w_nack();
    static void mtm_data_ack();
    static void mtm_data_nack();
    
    // Implementación:
	static void mm_stop(iostate st);

	static void mtm_send_next_byte();
	static void mtm_error(iostate st);


// Respuesta a miscellaneous states
    static void bus_error();
    static void unknown_error();
};


template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::mm_start(uint8_t slave_address)
{
    if (state_ == iostate::transmitting)
	TWI::master_transmit_sla_w(slave_address);

    else if (state_ == iostate::receiving)
	TWI::master_transmit_sla_r(slave_address);
}


template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::mm_repeated_start(uint8_t slave_address)
{
    mm_start(slave_address);
}   


template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::mtm_send_next_byte()
{
    if (!buffer_.out_is_empty())
	TWI::master_transmit_byte(buffer_.out_read_one());

    else{
	TWI::interrupt_disable();
	state_ = iostate::eow;
    }
}

template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::mtm_sla_w_ack()
{
    mtm_send_next_byte();
}


template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::mm_stop(iostate st)
{
    TWI::master_transmit_stop();
    TWI::interrupt_disable();
    state_ = st;
}

template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::mtm_sla_w_nack()
{
    mm_stop(iostate::no_response);
}


template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::mtm_data_ack()
{
    mtm_send_next_byte();
}


template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::mtm_data_nack()
{
    mm_stop(iostate::eow_data_nack);
}




template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::mtm_error(iostate st)
{
    TWI::interrupt_disable();
    state_ = st;
    TWI::recover_from_bus_error();      

}

template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::bus_error()
{
    mtm_error(iostate::bus_error);
}

// Este caso lo tienen así implementado en la app-note, pero
// ¿qué estatus puede ser este desconocido? ¿error de hardware?
// ¿protección contra error de software?
template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::unknown_error()
{
    mtm_error(iostate::unknown_error);
}


template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::mrm_sla_r_ack()
{
    mrm_receive_next_byte();
}

template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::mrm_sla_r_nack()
{
    mm_stop(iostate::no_response);
}

template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::mrm_receive_next_byte()
{
    if (nread_ == 1)
	TWI::master_receive_data_with_NACK();

    else
	TWI::master_receive_data_with_ACK();
}



template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::mrm_data_ack()
{
    buffer_.in_write_one(TWI::data());
    --nread_;
    mrm_receive_next_byte();
}



// Recibimos el último byte.
template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::mrm_data_nack()
{
    buffer_.in_write_one(TWI::data());
    TWI::interrupt_disable();
    state_ = iostate::eor_bf;
}


//	if (is_busy() or is_waiting()) master_transmit_repeated_start();
//	else master_transmit_start();
template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::twi_transmit_start()
{
    if (is_idle())
	TWI::master_transmit_start();

    else
	TWI::master_transmit_repeated_start();
}

template <typename TWI, uint8_t bsz>
TWI_master<TWI, bsz>::streamsize TWI_master<TWI, bsz>::read(streamsize n)
{
    if (is_busy() or n == 0)  // = precondition
	return 0;

    if (n > buffer_.capacity()) // reestricción temporal. ¿eliminarlo en el futuro?
	return 0;

    nread_ = n;
    buffer_.reset_as_input();
 
    twi_transmit_start();
    state_ = iostate::receiving;

    TWI::interrupt_enable();

    return n;
}


template <typename TWI, uint8_t bsz>
TWI_master<TWI, bsz>::streamsize 
TWI_master<TWI, bsz>::read_buffer(std::byte* q, streamsize N)
{
    if (!eor_bf())  // precondicion
	return 0;
 
    streamsize n = buffer_.in_read_all_n(q, N);

    state_ = iostate::eor;

    return n;
}


template <typename TWI, uint8_t bsz>
inline void TWI_master<TWI, bsz>::stop_transmission()
{
    TWI::master_transmit_stop();
    TWI::interrupt_disable();
    state_ = iostate::ok;
}

// Para poder independizarlo del microcontrolador es necesario
// darle un nombre a los estados que dependan del protocolo de TWI!!!
template <typename TWI, uint8_t bsz>
void TWI_master<TWI, bsz>::handle_interrupt()
{
    using TWI_iostate = TWI_basic_iostate;
    using MM = TWI_iostate::master_mode;
    using MRM = TWI_basic_iostate::master_receiver_mode;
    using MTM = TWI_basic_iostate::master_transmitter_mode;

    switch (TWI::status()){
    // modos comunes a transmitter/receiver mode
    // -----------------------------------------
	case MM::start:
	    mm_start(slave_address_);
	    break;

	case MM::repeated_start:
	    mm_repeated_start(slave_address_);
	    break;


    // master receiver mode
    // -------------------
	case MRM::sla_r_ack:
	    mrm_sla_r_ack();
	    break;

	case MRM::sla_r_nack:
	    mrm_sla_r_nack();
	    break;

	case MRM::data_ack:
	    mrm_data_ack();
	    break;

	case MRM::data_nack:
	    mrm_data_nack();
	    break;



    // master transmitter mode
    // ----------------------
	case MTM::sla_w_ack:
	    mtm_sla_w_ack();
	    break;

	case MTM::sla_w_nack:
	    mtm_sla_w_nack();
	    break;

	case MTM::data_ack:
	    mtm_data_ack();
	    break;

	case MTM::data_nack:
	    mtm_data_nack();
	    break;



    // miscellaneous states
    // --------------------
	case TWI_iostate::bus_error:
	    bus_error();
            break;

        default: // ¿Qué ha sucedido? ¡¡¡status desconocido!!!
	    unknown_error();
	    break;
    }
}


}// namespace

#endif






