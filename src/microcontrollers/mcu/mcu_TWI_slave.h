// Copyright (C) 2019-2023 Manuel Perez 
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

#ifndef __MCU_TWI_SLAVE_H__
#define __MCU_TWI_SLAVE_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Implementación de un servidor de TWI genérico.
 *
 *  - COMENTARIOS: La idea es que se pueda usar con cualquier dispositivo TWI
 *  (ya sea avr, pic, ...). De momento supongo que dependerá del avr ya que es
 *  el único TWI que tengo.
 *	Es un proyecto de aprendizaje así que es posible que cambie bastante
 *  en el futuro.
 *
 *  - EJEMPLO: ver test/TWI/slave
 *	Recordar capturar la interrupción adecuada:
 *
 *	constexpr uint8_t buffer_size = 10;
 *	using TWI_slave = avr::TWI_slave<avr::TWI_basic, buffer_size>;
 *
 *	ISR(TWI_vect)
 *	{
 *	    TWI_slave::handle_interrupt();
 *	}
 *	
 *  - HISTORIA:
 *    Manuel Perez
 *    14/02/2020 v0.0
 *    30/10/2023 Lo independizo de avr. Primer intento de ser genérico.
 *               Para que pueda ser genérico hay que definir el interfaz del
 *               `TWI generic` que es en el que se basa esta implementación.
 *               Para poder definir ese interfaz necesito programar diferentes
 *               micros (o diferentes TWIs). 
 *
 ****************************************************************************/
#include <cstdint>  // uint8_t
		   
#include <atd_iobxtream.h>  // TWI_iobuffer
			    
namespace mcu{


enum class __TWI_slave_state{
// group of states
    listening = 0x00,
    busy = 0x01,
    waiting = 0x02,

// state
    ok		    = (1 << 2) |listening,

    wrt_be	    = (3 << 2) |waiting,
    eow_more_data   = (4 << 2) |listening,
    eow_too_many_data= (5 << 2)|listening, 
    eow		    = (6 << 2)|listening, 

    eor		    = (10 << 2) |waiting,
    rec_bf	    = (11 << 2) |waiting,
    eor_data_nack   = (12 << 2) |listening,

    transmitting    = (20 << 2) |busy,
    receiving	    = (21 << 2) |busy,

    bus_error	    = (30 << 2) |listening,
    unknown_error   = (31 << 2) |listening,

    prog_error	    = (100 << 2)|listening // error de programación
};

constexpr uint8_t __TWI_slave_state_mask_group_states = 0x03;

inline bool __TWI_slave_state_is_group(__TWI_slave_state a, __TWI_slave_state group)
{
    return 
    (static_cast<uint8_t>(a) & __TWI_slave_state_mask_group_states) == 
	static_cast<uint8_t>(group);
}

inline bool __TWI_slave_state_is_busy(__TWI_slave_state a)
{
    return __TWI_slave_state_is_group(a, __TWI_slave_state::busy);
}

inline bool __TWI_slave_state_is_waiting(__TWI_slave_state a)
{
    return __TWI_slave_state_is_group(a, __TWI_slave_state::waiting);
}

inline bool __TWI_slave_state_is_listening(__TWI_slave_state a)
{
    return __TWI_slave_state_is_group(a, __TWI_slave_state::listening);
}



template <typename Micro0, 
	  typename TWI0, typename TWI0::streamsize buffer_size0>
struct TWI_slave_cfg{
    using Micro = Micro0;
    using TWI   = TWI0;
    using streamsize = typename TWI::streamsize;
    static constexpr streamsize buffer_size = buffer_size0;
};

// La idea es que esto no sea más que una ampliacion del TWI con 1 byte
// de buffer. Le ponemos una put/get area a TWI en esta clase y ocultamos el
// protocolo al usuario.
// Responsabilidades de esta clase:
//	1.- Ponerle un buffer al hardware de TWI (que carece de el).
//	2.- Ocultar el protocolo al usuario. El usuario no tiene que saber
//	    nada de START/SLA+WR ... /STOP. 
template <typename Cfg>
class TWI_slave{
public:
// Types
    using iostate    = __TWI_slave_state;
    using Micro	     = Cfg::Micro;
    using TWI	     = Cfg::TWI;
    using streamsize = Cfg::streamsize;

// cfg data
    static constexpr streamsize buffer_size = Cfg::buffer_size;

    /// Enables TWI. En modo slave no hay que definir frecuencia de reloj!!!
    // Enables as a slave in the address SLAVE_ADDRESS.
    template <uint8_t TWI_slave_address>
    static void turn_on()
    {
	Micro::enable_interrupts();
	TWI::template slave_init<TWI_slave_address,1>(); // 1 = TWI::interrupt_enable()

	reset();
    }

    /// Lee del TWI como máximo N bytes.
    /// En caso de que el dispositivo esté ocupado o el buffer esté vacío
    /// devuelve 0.
    /// Postcondición: get_area() == empty()
    static streamsize read_buffer(uint8_t* buffer, streamsize N);

    /// Escribe en el buffer de salida buf[0, n). 
    /// Devuelve el número de bytes escritos.
    /// A día de hoy si se n > buffer_size no hace nada, ya que esta primera
    /// versión está limitada a enviar el buffer completo de una sola vez (no
    /// se puede escribir poco a poco). Si en el futuro se usa, cambiarlo y
    /// darle más flexibilidad. Esta es una versión de aprendizaje.
    static streamsize write_buffer(const uint8_t* buf, streamsize n);

    static void stop_transmission();

    // Función que va dentro de la ISR
    static void handle_interrupt();

// estados
// -------
// grupos
    static bool is_listening() {return __TWI_slave_state_is_listening(state_);}
    static bool is_busy() {return __TWI_slave_state_is_busy(state_);}
    static bool is_waiting() {return __TWI_slave_state_is_waiting(state_);}

// de lectura
    static bool eor() {return state_ == iostate::eor;}
    static bool rec_bf() { return state_ == iostate::rec_bf;}

// de escritura
    static bool wrt_be() {return state_ == iostate::wrt_be;}
    static bool eow() {return state_ == iostate::eow;}
    static bool eow_too_many_data()
			{ return state_ == iostate::eow_too_many_data; }
    static bool eow_more_data() {return state_ == iostate::eow_more_data;}

// genéricos
    static bool ok() {return state_ == iostate::ok;}

    // TODO: private
    // Estado en el que queda el slave después de ejecutar algo.
    // Sólo es válido cuando is_busy() == false!!!
    static iostate state() {return state_;}

// Da la impresión de que en polling voy a necesitar esta función bastante:
//  ENGLISH DOUBT: wait_till_no_busy??? or wait_no_busy??? @_@
    static void wait_till_no_busy() { while (is_busy()) { asm("nop"); } }

private:
    static inline volatile iostate state_;

    static inline atd::TWI_iobuffer<buffer_size> buffer_;



    // Helper functions
    // ----------------
    static void reset()
    {
	state_ = iostate::ok;
	TWI::template slave_reset<1>(); // interrupt_enable = 1
    }


// Respuesta a los estados de TWI en slave receiver mode
    static void srm_sla_w();
    static void srm_data_ack();
    static void srm_data_nack();
    static void srm_stop_or_repeated_start();


//// Respuesta a los estados de TWI en slave transmitter mode:
    static void stm_sla_r();
    static void stm_data_ack();
    static void stm_data_nack();

	// Implementación
	static void stm_send_data();

// Respuesta a miscellaneous states
    static void bus_error();
    static void unknown_error();
};


// DUDA: la datasheet admite poder responder a esto con nack, esto es, no
// reconocemos la recepción de datos. ¿Cuándo puede ser útil eso?
// ¿el slave está ocupado haciendo cosas y no puede responder y por eso
// responde con NACK? Pero basta con desconectarlo para que no responda a
// SLA+w. <-- pero esto sería como decirle al master: "error: dispositivo
// desconocido", mientras que si responde OK a SLA+W, pero luego te devuelve
// NACK al data le estas diciendo al master: "estoy ocupado, inténtalo más
// tarde".
template <typename Cfg>
void TWI_slave<Cfg>::srm_sla_w()
{
    buffer_.reset_as_input();

    state_ = iostate::receiving;
    TWI::slave_receive_data_with_ACK();
}


template <typename Cfg>
void TWI_slave<Cfg>::srm_data_ack()
{
    buffer_.in_write_one(TWI::data());

    if (buffer_.in_is_full()){
	TWI::interrupt_disable();
	state_ = iostate::rec_bf;
    }

    else{
	TWI::slave_receive_data_with_ACK();
    }

}


template <typename Cfg>
void TWI_slave<Cfg>::srm_data_nack()
{
    TWI::recover_from_bus_error();      
    state_ = iostate::eor_data_nack;
}


template <typename Cfg>
void TWI_slave<Cfg>::srm_stop_or_repeated_start()
{
    state_ = iostate::eor;

    TWI::interrupt_disable();  
}





template <typename Cfg>
void TWI_slave<Cfg>::stm_sla_r()
{
    TWI::interrupt_disable();   
    buffer_.reset_as_output();
    state_ = iostate::wrt_be;
}


template <typename Cfg>
void TWI_slave<Cfg>::stm_send_data()
{
    if (buffer_.out_size() == 1)
	TWI::slave_transmit_byte_received_NACK(buffer_.out_read_one());

    else
	TWI::slave_transmit_byte_received_ACK(buffer_.out_read_one());
}


template <typename Cfg>
void TWI_slave<Cfg>::stm_data_ack()
{
    if (buffer_.out_is_empty()){
	state_ = iostate::eow_more_data;
	reset();    // TODO: ¿no debería de recibir todos los datos 
		    // descartándolos? Este reset no creo que funcione,
		    // debería de ponerlo en otro estado (que no reconozca su
		    // SLA o algo de eso?)
	return;
    }
    stm_send_data();
}


template <typename Cfg>
void TWI_slave<Cfg>::stm_data_nack()
{
    if (buffer_.out_is_empty())
	state_ = iostate::eow;

    else 
	state_ = iostate::eow_too_many_data;

    TWI::not_addressed_slave_mode();
}



template <typename Cfg>
void TWI_slave<Cfg>::bus_error()
{
    TWI::recover_from_bus_error();      
    state_ = iostate::bus_error;

}

// Este caso lo tienen así implementado en la app-note, pero
// ¿qué estatus puede ser este desconocido? ¿error de hardware?
// ¿protección contra error de software?
template <typename Cfg>
inline void TWI_slave<Cfg>::unknown_error()
{
    TWI::not_addressed_slave_mode();
    state_ = iostate::listening;
}



template <typename Cfg>
TWI_slave<Cfg>::streamsize 
TWI_slave<Cfg>::write_buffer(const uint8_t* buf, streamsize n)
{
    if (!wrt_be() and n != 0) // precondicion
	return 0;

    if (n > buffer_.capacity()) // Devuelvo el control para que el usuario sepa
	return 0;	 // que tiene que aumentar el tamaño del buffer.

    auto res = buffer_.out_write_all_n(buf, n);

    // FUNDAMENTAL EL ORDEN:
    //	1. llamar stm_send_data: clears(TWINT)
    //	2. habilitar interrupciones.
    //
    //	Si se habilitan las interrupciones primero, se llama de nuevo SLA+R
    //	(es la último que se recibió y mientras no se haga clear a TWINT se
    //	volverá a llamar esa interrupción).
    state_ = iostate::transmitting;
    stm_send_data();
    TWI::interrupt_enable(); 

    return res;
}

template <typename Cfg>
void TWI_slave<Cfg>::stop_transmission()
{
    typename Micro::Disable_interrupts lock;   // garantizo control del flujo.
    reset();
}


// Mira a ver si hay datos que leer en el bufer del TWI. 
// En caso de que los haya los lee guardándolos en buffer.
// Si no hay datos a leer, devuelve inmediatamente el control.
// Devuelve el número de bytes leidos.
template <typename Cfg>
TWI_slave<Cfg>::streamsize 
TWI_slave<Cfg>::read_buffer(uint8_t* buffer, streamsize N)
{
    if (!is_waiting())
	return 0;
 

    streamsize n = buffer_.in_read_all_n(buffer, N);

    if (state_ == iostate::rec_bf){
	TWI::slave_receive_data_with_ACK();
	TWI::interrupt_enable();
    }

    else if (eor()){
	if (buffer_.in_is_empty()){
	    state_ = iostate::listening;
	    TWI::not_addressed_slave_mode(); 
	    TWI::interrupt_enable();
	}
    }
    else{
	state_ = iostate::prog_error;
    }

    return n;
}



// Para poder independizarlo del microcontrolador es necesario
// darle un nombre a los estados que dependan del protocolo de TWI!!!
template <typename Cfg>
void TWI_slave<Cfg>::handle_interrupt()
{
    using TWI_state = TWI::State;
    using SRM = TWI_state::slave_receiver_mode;
    using STM = TWI_state::slave_transmitter_mode;

    switch (TWI::status()){

    // slave receiver mode
    // -------------------
	case SRM::sla_w: 
	    srm_sla_w(); 
	    break;

	case SRM::sla_w_data_ack:
	    srm_data_ack();
	    break;

	case SRM::sla_w_data_nack:
	    srm_data_nack();
            break;


	case SRM::stop_or_repeated_start:
	    srm_stop_or_repeated_start();
            break;



    // slave transmitter mode
    // ----------------------
	case STM::sla_r:
	    stm_sla_r();
	    break;

	case STM::data_ack:
	    stm_data_ack();
	    break;

	case STM::data_nack:
	    stm_data_nack();
	    break;


    // miscellaneous states
    // --------------------
	case TWI_state::bus_error:
	    bus_error();
            break;

        default: // ¿Qué ha sucedido? ¡¡¡status desconocido!!!
	    unknown_error();
	    break;
    }
}


}// namespace

#endif






