// Copyright (C) 2020-2023 Manuel Perez 
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

#ifndef __DEV_TWI_MASTER_IOXTREAM_H__
#define __DEV_TWI_MASTER_IOXTREAM_H__

/****************************************************************************
 *
 *  DESCRIPCION
 *	Concebimos TWI como un ioxtream.
 *	Esta clase está pensada para crear TWI_master_ioxtream en local: el
 *	constructor y destructor gestionan el coger el bus de TWI.
 *
 *
 *  TIPO DE USUARIO
 *	Normal. No necesita saber cosas de bajo nivel del protocolo TWI.
 *
 *  RESPONSABILIDADES
 *  Suministra funciones de alto nivel para leer tipos.
 *
 *  MÉTODO DE FLUJO
 *  Para leer/escribir datos podemos concebir TWI como un stream de bytes:
 *
 *	TWI_master_ioxtream twi{slave_address};
 *	twi << uint8_t{0x10}; 
 *	twi << int16_t{-23};
 *	twi >> x;
 *	twi >> y;
 *
 *  Para escribir se puede llamar al operator<< tantas veces como se quiera
 *  que todas se ejecutarán dentro de la misma twi data transfer. Para leer,
 *  el twi-master tiene que marcar el final del data transfer con NACK
 *  necesitando conocer el número de bytes que se van a transferir. Eso obliga
 *  a que cada lectura sea un data transfer independiente (<-- CUIDADO con
 *  esto). 
 *  (otra forma de implementarlo sería que explicitamente se llamase a twi >>
 *  end_of_reading; o algo parecido para marcar el final, pero casi seguro que
 *  ese estilo de programación generaría muchos dolores de cabeza porque
 *  habría que recordar llamar al end_of_reading continuamente. Por eso opto
 *  por hacer data transfers independientes a la hora de leer).
 *
 *  Si se necesita leer muchos bytes en un mismo data transfer usar
 *  directamente read(slave_address, buffer, n);
 *
 *  MÉTODO read/write
 *	
 *	TWI_master_ioxtream twi(slave_address);
 *	write(buffer, n);
 *	read (buffer, n);
 *
 *  que sirven para transferir datos al twi-device. 
 *
 *  Además, añade los operadores <</>> correspondientes.
 *
 *  TODO: Sería interesante que se pudiera indicar como parámetro si se
 *  quiere enviar como little or big endian.
 *
 *  TODO: TWI_master tiene la restricción de que todo lo envíado/recibido
 *  tiene que entrar en el buffer, así que hay que elegir un tamaño de buffer
 *  suficientemente grande, sino se producirá el error error_buffer_size. No
 *  es complicado mejorarlo, pero de momento parece sencillo y práctico enviar
 *  todo de una misma vez. A fin de cuentas esta es una versión de aprendizaje
 *  y no se si la usaré o no. 
 *
 *
 *  HISTORIA
 *    Manuel Perez
 *    24/02/2020 v0.0
 *    31/10/2023 Lo independizo de avr. Primer intento de ser genérico.
 *
 ****************************************************************************/
#include <cstdint>  // uint8_t
#include <cstddef>  // std::byte
#include <ostream>	    

namespace dev{
/*!
 *  \brief  Concebimos TWI como ioxtream.
 * 
 *  Esta clase es responsable de:
 *	1. Ocultar el protocolo de TWI.
 *	2. Concebir TWI como flujo normal y corriente.
 *
 *  Cada lectura/escritura es una transacción completa. Esto es, cada vez que
 *  se intenta leer/escribir se envía un `repeated_start` al device
 *  correspondiente. Si se quiere leer de golpe un montón de bytes usar la
 *  función `read(q, n)`
 *
 *  Ejemplo (ideal):
 *	TWI_master_ioxtream twi(slave_address);
 *	char x;
 *	int y;
 *	twi << x << y;
 *	...
 *	twi >> x >> y;
 *	...
 *	twi.close();
 *
 *	if (twi.error())
 *	    // error...
 *
 *	// NO OLVIDAR LA ISR!!!
 *	ISR(TWI_vect)
 *	{
 *	    TWI::handle_interrupt();
 *	}

 */
template <typename TWI_master0>
class TWI_master_ioxtream{
public:
    using TWI_master  = TWI_master0; // para que sea accesible por el usuario
    using TWI         = TWI_master;
    using Address     = TWI::Address;
    using streamsize  = TWI::streamsize;
    using iostate     = TWI::iostate;   // para depurar

// Construction
    /// Construimos la conexión conectandola
    explicit TWI_master_ioxtream(Address slave_address)
    {open(slave_address);}

    ~TWI_master_ioxtream() {close();}

// open/close
    // (RRR) ¿Necesito open/close cuando lo gestiona mejor el
    //        constructor/destructor?
    //       Sí. Hay devices como la cámara OV7670 que cuando se quiere leer
    //       un registro de la memoria:
    //		1º) se escribe la dirección de ese registro.
    //		2º) se cierra la conexión y espera 1 ms
    //		3º) se abre la conexión y se lee el valor del registro pedido
    //		4º) se cierra la conexión
    //	    Para está cámara sí necesito el interfaz open/close.
    // Inicia una transmisión. En general no llamarla directamente, ya que se
    // llama a través del constructor. 
    // No bloquea.
    // Precondition: TWI::is_idle();
    // Return: no devuelvo nada ya que open no se debería de llamar en
    // general. Se llama a través del constructor. 
    void open(Address slave_address);

    // Cierra la transmisión actual.
    // Bloquea el flujo. No devuelve el control hasta que no la ha cerrado
    // por completo.
    void close();

// init
// El cliente es el responsable de la gestión de TWI. Es él el que se encarga
// de encenderlo o apagarlo. Por eso no suministro función `turn_on`

    /// Reinicializa el dispositivo.
    static void reset() {TWI::reset();}





// operator<<. Ninguna bloquea.
    TWI_master_ioxtream& operator<<(std::byte c) {return write_(c);}
    TWI_master_ioxtream& operator<<(char c) {return write_(c);}
    TWI_master_ioxtream& operator<<(unsigned char c) {return write_(c);}
    TWI_master_ioxtream& operator<<(signed char c) {return write_(c);}
    TWI_master_ioxtream& operator<<(short c) {return write_(c);}
    TWI_master_ioxtream& operator<<(unsigned short c) {return write_(c);}
    TWI_master_ioxtream& operator<<(int c) {return write_(c);}
    TWI_master_ioxtream& operator<<(unsigned int c) {return write_(c);}
    TWI_master_ioxtream& operator<<(const long& c) {return write_(c);}
    TWI_master_ioxtream& operator<<(const unsigned long& c) {return write_(c);}
    TWI_master_ioxtream& operator<<(const long long& c) {return write_(c);}
    TWI_master_ioxtream& operator<<(const unsigned long long& c) {return write_(c);}

// operator>>. Todas bloquean.
    TWI_master_ioxtream& operator>>(std::byte& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(char& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(unsigned char& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(signed char& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(short& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(unsigned short& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(int& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(unsigned int& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(long& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(unsigned long& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(long long& c) {return read_(c);}
    TWI_master_ioxtream& operator>>(unsigned long long& c) {return read_(c);}
    

// Lectura/escritura en bloque

    // read: C-style
    /// Lee exáctamente n bytes metiéndolos en q[0, n). 
    /// Bloquea la ejecución. Hasta que no lee todo no devuelve el control.
    /// Precondition: se ha llamado a read(N) antes (con N >= n).
    streamsize read(uint8_t* q, streamsize n);

    // read: array-style
//    template <streamsize N>  doesn't work!!!
//    streamsize read(std::array<std::byte, N>& q) {return read(q.data(), N);}

    // write C-style
    /// Escribe q[0,n) en el flujo.
    /// No bloquea. q[0,n) se mete en el buffer interno de TWI y lo irá
    /// enviando poco a poco.
    streamsize write(const uint8_t* q, streamsize n);

    // write: array-style
//    template <streamsize N> doesn't work!!!
//    streamsize write(const std::array<std::byte, N>& q) 
//    {return write(q.data(), N);}


// ISR
//    // FUNDAMENTAL: no olvidar llamar a esta función!!! <-- la llama el
//    main, usando directamente TWI_master o TWI_multimaster. Nosotros no
//    podemos decidir aquí!!!
//    static void handle_interrupt() {TWI::handle_interrupt();}


// States
// ------
// grupos
    static bool is_idle() {return TWI::is_idle();} 
    static bool is_busy() {return TWI::is_busy();}
    static bool is_waiting() {return TWI::is_waiting();}


// ¿algún error?
    static bool error() {return TWI::error();}
    static bool error(iostate st) {return TWI::error(st);}

// errores genéricos
    static bool no_response() {return TWI::no_response();}
    static bool prog_error() {return TWI::prog_error();}
    static bool bus_error() {return TWI::state() == TWI::iostate::bus_error;}
    static bool unknown_error() {return TWI::state() == TWI::iostate::unknown_error;}


// de escritura
    static bool eow() {return TWI::eow();}
    static bool eow_data_nack() {return TWI::eow_data_nack();}

// de lectura
    static bool eor() {return TWI::eor();}
    static bool eor_bf() {return TWI::eor_bf();}
    static bool error_buffer_size() { return TWI::error_buffer_size();}

// genérico
    static bool ok() {return TWI::ok();}

// para depurar
    static iostate state() {return TWI::state();}
    static void print_state(std::ostream& out) {TWI::print_state(out);}


private:
// Data
    Address slave_address_;

// Functions

    void send_start();

    // TODO: gestión de errores. Si write no devuelve sizeof(x) error	
    // Formas posibles:
    //	    (1) Almacenar el número de bytes escritos/leidos y que se pueda
    //	        consultar despues.
    //	    (2) Ampliar el state de TWI con más errores. El problema es que no
    //	        sabría cúantos bytes se han enviado.
    template <typename T>
    TWI_master_ioxtream& write_(const T& x)
    {
	write(reinterpret_cast<const uint8_t*>(&x), sizeof(x));
	return *this;
    }

    // TODO: gestión de errores. Si read no devuelve sizeof(x) error
    template <typename T>
    TWI_master_ioxtream& read_(T& x)
    {
	read(reinterpret_cast<uint8_t*>(&x), sizeof(x));
	return *this;
    }

};

template <typename T>
inline void TWI_master_ioxtream<T>::open(Address slave_address)
{
    if (!TWI::is_idle())
	TWI::reset();

    slave_address_ = slave_address;

    TWI::send_start();
}


template <typename T>
inline void TWI_master_ioxtream<T>::close()
{
    if (TWI::is_idle())	// No estamos dentro de una transmisión
	return;

    TWI::wait_while_busy();

    // if (TWI::eow() or TWI::eor())	<--- al no ponerlo podemos cerrar
    // desde cualquier estado. Mejor descomentarlo (???)
    TWI::send_stop();
}


template <typename T>
void TWI_master_ioxtream<T>::send_start()
{
    TWI::wait_while_busy(); // FUNDAMENTAL!!! El micro va a 1MHz, TWI a 100kHz!

    if (TWI::read_or_write())
	return;

    if (TWI::is_idle()) 
	TWI::send_start();

    else 
	TWI::send_repeated_start();
}


template <typename T>
TWI_master_ioxtream<T>::streamsize
TWI_master_ioxtream<T>::read(uint8_t* q, streamsize n)
{
    send_start();
    
    TWI::read_from(slave_address_, n);
    
    TWI::wait_while_busy();

    if (TWI::is_busy()) 
	return 0;

    return TWI::read_buffer(q, n);
}

template <typename T>
TWI_master_ioxtream<T>::streamsize 
TWI_master_ioxtream<T>::write(const uint8_t* q, streamsize n)
{
    if (!TWI::is_writing())
	send_start(); 

    if (TWI::read_or_write())
	return TWI::write_to(slave_address_, q, n);

    else 
	return TWI::write(q, n);

}





}// namespace


#endif

