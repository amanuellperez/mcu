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
 *  - TODO: Dar la opción de lanzar una interrupción cuando acabe de hacer
 *  algo (cuando pase de busy a no_busy). De esa forma el usuario no tendrá
 *  que estar haciendo polling continuamente.  (ANTES DE HACERLO: ¿merece la
 *  pena? ¿Lo voy a usar?)
 *
 *  - TODO: La mejor forma de usarlo es teniendo un buffer suficientemente
 *  grande para que se lea/escriba cualquier mensaje. Sin embargo está casi
 *  (basta con que read_buffer se pueda llamar tambien en estado
 *  receiving/nread_ > 0) preparado para que se pueda
 *  enviar/recibir mensajes de cualquier tamaño independientemente del buffer
 *  interno usado. Revisarlo e implementarlo (ANTES DE HACERLO: ¿merece la
 *  pena? ¿Lo voy a usar?)
 *  
 *  - TODO: Esta clase es totalmente independiente del avr::TWI (bueno, ahora
 *  depende porque es el primer intento). Sacarlo de avr. Debería de estar
 *  en... dev? (no es un dispositivo, sino un ampliador de hardware)
 *  drivers???
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

enum class __TWI_master_state : uint8_t {
// group of states: gg (2 bits)
    idle    = 0x00,
    busy    = 0x01,
    waiting = 0x02,

// error: e (1 bit) 
// (RRR) para que sea sencillo saber si hay un error o no
    error = 0x04,

// state: 5 bits (hasta 32)
    ok = (1 << 3) | idle,

    read_or_write = (2 << 3) | waiting,
    sla_w         = (3 << 3) | busy,
    sla_r         = (4 << 3) | busy,
    no_response   = (5 << 3) | error | idle,

// transmitting
    transmitting      = (10 << 3) | busy,
    eow               = (11 << 3) | waiting,
    eow_data_nack     = (12 << 3) | error | idle,
    error_buffer_size = (13 << 3) | error | idle, // buffer pequeño

// receiving
    receiving         = (20 << 3) | busy,
    eor_bf            = (21 << 3) | waiting, // buffer full
    eor               = (22 << 3) | waiting,

// errors
    bus_error         = (25 << 3) | error | idle,
    unknown_error     = (26 << 3) | error | idle,
    prog_error        = (27 << 3) | error | idle // error de programación
};

constexpr uint8_t __TWI_master_state_mask_group_states = 0x03;
constexpr uint8_t __TWI_master_state_mask_error = 0x04;

inline bool __TWI_master_state_error(__TWI_master_state a)
{
    return (static_cast<uint8_t>(a) & __TWI_master_state_mask_error);
}

inline bool __TWI_master_state_is_group(__TWI_master_state a, __TWI_master_state group)
{
    return 
    (static_cast<uint8_t>(a) & __TWI_master_state_mask_group_states) == 
	static_cast<uint8_t>(group);
}

inline bool __TWI_master_state_is_busy(__TWI_master_state a)
{
    return __TWI_master_state_is_group(a, __TWI_master_state::busy);
}

inline bool __TWI_master_state_is_waiting(__TWI_master_state a)
{
    return __TWI_master_state_is_group(a, __TWI_master_state::waiting);
}

inline bool __TWI_master_state_is_idle(__TWI_master_state a)
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
template <typename TWI, typename TWI::streamsize buffer_size>
class TWI_master{
public:
// Types
    using iostate    = __TWI_master_state;
    using streamsize = typename TWI::streamsize;
    using Address    = typename TWI::Address;

// on/off (off (???))
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

    /// Inicia la transmisión. Envía START.
    /// Elimina el estado en el que estuviera TWI perdiéndose la información
    /// de cómo fue la última transmisión.
    /// Precondición: is_idle(); // = no está en medio de una transmisión.
    static void send_start();

    /// Reinicia la transmisión. Envía REPEATED START.
    static void send_repeated_start();

    /// Finaliza la transmisión. Envía STOP.
    static void send_stop();

    /// Reinicializa el dispositivo, dejándolo en el estado inicial.
    /// Lo deja en estado 'idle::ok'.
    /// En caso de que se quede colgado intentando leer, llamar a esta
    /// función.
    static void reset()
    {
	TWI::master_reset();
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
    template <Address slave_address> 
    static streamsize read_from(streamsize n)
    { return read_from(slave_address, n); }

    // versión 2 de read_from. La necesito en TWI_master_ioxtream
    static streamsize read_from(Address slave_address, streamsize n)
    {
	slave_address_ = slave_address;
	return read(n);
    }

    /// Lee el buffer del dispositivo almacenándolos en q[0,N).
    /// Lee como máximo N bytes.
    /// precondition: state() == iostate::eor_bf
    /// postcondition: state() == iostate::eor or eor_bf (si no vacío todo el
    /// buffer).
    /// Returns: number of bytes leidos.
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
    template <Address slave_address> // tiene que estar definido en tiempo de compilación!!!
    static streamsize write_to(const std::byte* buf, streamsize n)
    { return write_to(slave_address, buf, n); }

    // versión 2 de write_to. La necesito en TWI_master_ioxtream
    static streamsize
    write_to(Address slave_address, const std::byte* buf, streamsize n);

    /// Escribe en el buffer buf[0, n) y envía los datos vía TWI.
    // Observar que no lo llamo write_buffer, ya que sería mentira que solo 
    // escribimos en el buffer. El pseudocódigo de write es:
    //	write(q, n):
    //	    write_buffer(q, n);
    //	    send_buffer(q, n);   // no lo hacemos así sino con interrupciones.
    //	por eso no lo llamo write_buffer.
    //
    //	El caso de read_buffer es diferente ya que read_buffer se limita a
    //	leer datos del buffer, no de TWI. Los nombres no mienten.
    static streamsize write(const std::byte* buf, streamsize n);

	    
    // Función que va dentro de la ISR
    static void handle_interrupt();

// states
// ------
// groups
    static bool is_idle() {return __TWI_master_state_is_idle(state_);}
    static bool is_busy() {return __TWI_master_state_is_busy(state_);}
    static bool is_waiting() {return __TWI_master_state_is_waiting(state_);}

// is there an error?
    static bool error() {return __TWI_master_state_error(state_);}
    static bool error(iostate st) {return __TWI_master_state_error(st);}

// generic errors
    static bool no_response() {return state_ == iostate::no_response;}
    static bool prog_error() {return state_ == iostate::prog_error;}

// inicio transmision
    static bool read_or_write() {return state_ == iostate::read_or_write;}

// transmitting
    static bool transmitting() {return state_ == iostate::transmitting;}
    static bool eow() {return state_ == iostate::eow;}
    static bool eow_data_nack() {return state_ == iostate::eow_data_nack;}
    static bool error_buffer_size()
			    { return state_ == iostate::error_buffer_size; }

// receiving
    static bool receiving() {return state_ == iostate::receiving;}
    static bool eor() {return state_ == iostate::eor;}
    static bool eor_bf() {return state_ == iostate::eor_bf;}

// generics
    static bool ok() {return state_ == iostate::ok;}

    static iostate state() {return state_;}

// Da la impresión de que en polling voy a necesitar esta función bastante:
//  ENGLISH DOUBT: wait_till_no_busy??? or wait_no_busy??? @_@
    static void wait_till_no_busy() { while (is_busy()) { asm("nop"); } }

private:
    // FUNDAMENTAL: busy_ tiene que ser volatile, ya que lo voy a usar:
    //	    while (is_busy()) { ; }
    //	Si no lo se define volatile el compilador optimiza y eso se convierte
    //	en un bucle infinito (y tardas un par de horas en encontrar el error.
    //	Antes funcionaba bien y de repente dejó de funcionar!!!)
    static inline volatile iostate state_;

    static inline atd::Circular_array<std::byte, buffer_size> buffer_;
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
    static inline Address slave_address_;


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


    // Cuando no se satisface la precondición del diagrama de estados
    // generamos este error: invalid_state(); 
    // Esto puede suceder: ya sea porque el slave no respondió y se intenta
    // seguir con la comunicación, o por algún error por parte del
    // programador. Mantenemos el error inicial que causo todo (por ejemplo,
    // si el slave no responde interesa saber que fue así y no que se llamó a
    // write con un estado no válido).
    static void invalid_state()
    {
	iostate st = error()? state_: iostate::prog_error;
	reset(); // cortamos la transmisión, desconectando el dispositivo
	state_ = st;
    }

// Respuesta a las interrupciones
// ------------------------------
// Respuesta a los estados comunes
    static void mm_start(Address slave_address);
    static void mm_repeated_start(Address slave_address);

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


template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::mm_start(Address slave_address)
{
    if (state_ == iostate::sla_w)
	TWI::master_transmit_sla_w(slave_address);

    else if (state_ == iostate::sla_r)
	TWI::master_transmit_sla_r(slave_address);

    else
	invalid_state();
}


template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::mm_repeated_start(Address slave_address)
{
    mm_start(slave_address);
}   


template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::mtm_send_next_byte()
{
    if (!buffer_.is_empty()){
	TWI::master_transmit_byte(buffer_.read());
    }
    else{
	TWI::interrupt_disable();
	state_ = iostate::eow;
    }
}

template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::mtm_sla_w_ack()
{
    state_ = iostate::transmitting;
    mtm_send_next_byte();
}


template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::mm_stop(iostate st)
{
    TWI::master_transmit_stop();
    TWI::interrupt_disable();
    state_ = st;
}

template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::mtm_sla_w_nack()
{
    mm_stop(iostate::no_response);
}


template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::mtm_data_ack()
{
    mtm_send_next_byte();
}


template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::mtm_data_nack()
{
    mm_stop(iostate::eow_data_nack);
}




template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::mtm_error(iostate st)
{
    TWI::interrupt_disable();
    state_ = st;
    TWI::recover_from_bus_error();      

}

template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::bus_error()
{
    mtm_error(iostate::bus_error);
}

// Este caso lo tienen así implementado en la app-note, pero
// ¿qué estatus puede ser este desconocido? ¿error de hardware?
// ¿protección contra error de software?
template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::unknown_error()
{
    mtm_error(iostate::unknown_error);
}


template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::mrm_sla_r_ack()
{
    state_ = iostate::receiving;
    mrm_receive_next_byte();
}

template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::mrm_sla_r_nack()
{
    mm_stop(iostate::no_response);
}

template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::mrm_receive_next_byte()
{
    if (nread_ == 1)
	TWI::master_receive_data_with_NACK();

    else
	TWI::master_receive_data_with_ACK();
}



template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::mrm_data_ack()
{
    buffer_.write(TWI::data()); // buffer_ << TWI::data();
    --nread_;
    mrm_receive_next_byte();
}



// Recibimos el último byte.
template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::mrm_data_nack()
{
    buffer_.write(TWI::data()); // buffer_ << TWI::data();
    TWI::interrupt_disable();
    state_ = iostate::eor_bf;
}



// versión 2 de write_to. La necesito en TWI_master_ioxtream
template <typename TWI, typename TWI::streamsize bsz>
TWI_master<TWI, bsz>::streamsize TWI_master<TWI, bsz>
    ::write_to(Address slave_address, const std::byte* buf, streamsize n)
{
    if (state_ != iostate::read_or_write){	// precondition
	invalid_state();
	return 0;
    }

    buffer_.reset();

    if (buffer_.ewrite(buf, n) != n)
	return 0;

    slave_address_ = slave_address;

    state_ = iostate::sla_w;
    TWI::interrupt_enable();

    return n;
}


template <typename TWI, typename TWI::streamsize bsz>
TWI_master<TWI, bsz>::streamsize TWI_master<TWI, bsz>::
    write(const std::byte* buf, streamsize n)
{
    Interrupts_lock lock;	// fundamental, ya que se llamará desde 'busy'

    // OJO: el micro funciona mucho más rápido que TWI, por eso si se llama
    // a write_to(q,n), y  a continuación a write(q,n) puede que todavía no le
    // haya dado tiempo a cambiar de estado, pasando de sla_w a transmitting.
    if (!(state_ == iostate::sla_w or transmitting() or eow())){  // pre.
	invalid_state();
	return 0;
    }

    if (buffer_.ewrite(buf, n) != n)
	return 0;

    if (state_ == iostate::eow){
	state_ = iostate::transmitting;
	mtm_send_next_byte();
	TWI::interrupt_enable();
    }

    return n;
}



template <typename TWI, typename TWI::streamsize bsz>
TWI_master<TWI, bsz>::streamsize TWI_master<TWI, bsz>::read(streamsize n)
{
    if (state_ != iostate::read_or_write or n == 0){ // precondition
	invalid_state();
	return 0;
    }


    if (n > buffer_.capacity()){ // reestricción temporal. ¿eliminarlo en el futuro?
	state_ = iostate::error_buffer_size;
	return 0;
    }

    nread_ = n;
    buffer_.reset();
 
    state_ = iostate::sla_r;

    TWI::interrupt_enable();

    return n;
}


template <typename TWI, typename TWI::streamsize bsz>
TWI_master<TWI, bsz>::streamsize 
TWI_master<TWI, bsz>::read_buffer(std::byte* q, streamsize N)
{
    if (!eor_bf()){  // precondicion
	invalid_state();
	return 0;
    }
 
    streamsize n = buffer_.eread(q, N);

    if (buffer_.is_empty())
	state_ = iostate::eor;

    return n;
}

template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::send_start()
{
    if (!is_idle()){ // pre: no puede estar en medio de una transmisión
	invalid_state();
	return;
    }

    TWI::master_transmit_start();
    state_ = iostate::read_or_write;
}


template <typename TWI, typename TWI::streamsize bsz>
void TWI_master<TWI, bsz>::send_repeated_start()
{
    if (state_ != iostate::eow and state_ != iostate::eor){ // precondition
	invalid_state();
	return;
    }

    TWI::master_transmit_repeated_start();
    state_ = iostate::read_or_write;
}



// No valido la precondición: if (state_ == eor or state_ == eow).
// De esta forma se puede parar la transmisión desde cualquier estado.
template <typename TWI, typename TWI::streamsize bsz>
inline void TWI_master<TWI, bsz>::send_stop()
{
    if (is_idle()) // No esta en medio de una transmisión; no es necesario stop
	return;

    TWI::master_transmit_stop();
    TWI::interrupt_disable();
    state_ = iostate::ok;
}

// Para poder independizarlo del microcontrolador es necesario
// darle un nombre a los estados que dependan del protocolo de TWI!!!
template <typename TWI, typename TWI::streamsize bsz>
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






