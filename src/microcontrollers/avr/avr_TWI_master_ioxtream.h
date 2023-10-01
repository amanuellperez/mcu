// Copyright (C) 2020 Manuel Perez 
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

#ifndef __AVR_TWI_MASTER_IOXTREAM_H__
#define __AVR_TWI_MASTER_IOXTREAM_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Concebimos TWI como un ioxtream.
 *
 *  - COMENTARIOS: ¿Qué es un ioxtream? La 'x' indica que no se va a hacer
 *	ningún tipo de procesamiento en los bytes. Que es un flujo de bytes,
 *	a diferencia de los iostreams que son flujos de caracteres.
 *
 *  - TIPO DE USUARIO: normal. No necesita saber cosas de bajo nivel del
 *	protocolo TWI.
 *
 *  - TODO: Sería interesante que se pudiera indicar como parámetro si se
 *  quiere enviar como little or big endian.
 *
 *  - TODO: TWI_master tiene la restricción de que todo lo envíado/recibido
 *  tiene que entrar en el buffer, así que hay que elegir un tamaño de buffer
 *  suficientemente grande, sino se producirá el error error_buffer_size. No
 *  es complicado mejorarlo, pero de momento parece sencillo y práctico enviar
 *  todo de una misma vez. A fin de cuentas esta es una versión de aprendizaje
 *  y no se si la usaré o no. 
 *
 *  - TODO: esta clase no depende del avr::TWI. Sacarla de avr (hacerlo a la
 *  vez que se saque TWI_master/slave).
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    24/02/2020 v0.0
 *
 ****************************************************************************/
#include "avr_TWI_master.h"


namespace avr_{
/*!
 *  \brief  Concebimos TWI como ioxtream.
 * 
 *  Esta clase es responsable de:
 *	1. Ocultar el protocolo de TWI.
 *	2. Concebir TWI como flujo normal y corriente.
 *
 *  Ejemplo (ideal):
 *	TWI_master_ioxtream twi;    // antes hay que haber encendido TWI::on!!!
 *	twi.open(slave_address);    // establece la conexión
 *	char x;
 *	int y;
 *	twi << x << y;
 *	...
 *	twi.read(n);	    // ¿cómo poder eliminar esto? TWI necesita marcar
 *			    // el último byte enviado, ¿cómo saber cuál es el 
 *			    // último? twi << last(x); ???
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

 *  TODO: Tal como está hecho ahora no se puede escribir después de leer.
 *	Habría que cerrar el flujo y volverlo a abrir. Pero creo que la forma
 *	habitual de manejar twi será: 1. escribo; 2. leo; 3. cierro. En
 *	principio no necesito escribir después de leer.
 *
 *  DUDA: ¿ponerle un state?
 *	Por una parte quiero que sea lo más eficiente posible, pero por otra
 *	un state es muy natural en un flujo. 
 *	De momento no lo pongo. Si lo necesito lo añado en el futuro.
 *	(Regla: ante la duda, implementación mínima. Siempre se puede añadir
 *	en el futuro sin cambiar el código actual.)
 */
//template <typename TWI_basic, uint8_t buffer_size>
template <typename TWI_master>
class TWI_master_ioxtream{
public:
    // using TWI = TWI_master<TWI_basic, buffer_size>;   
    using TWI = TWI_master;
    using Address = TWI::Address;
    using streamsize = TWI::streamsize;
    using iostate = TWI::iostate;   // para depurar

// Destructor
    TWI_master_ioxtream() {}

    /// Construimos la conexión conectandola
    explicit TWI_master_ioxtream(Address slave_address)
    {open(slave_address);}

    ~TWI_master_ioxtream() {close();}

// init
// El cliente es el responsable de la gestión de TWI. Es él el que se encarga
// de encenderlo o apagarlo.
//    /// Enables TWI interface definiendo la frecuencia del SCL 
//    /// a la que vamos a operar.
//    /// f_scl = frecuencia en kilohercios de SCL (tipica: 100 y 400 kHz).
//    /// f_clock = frecuencia a la que funciona el reloj del avr.
//    template <uint16_t f_scl, uint32_t f_clock = MCU_CLOCK_FREQUENCY_IN_HZ>
//    static void on() {TWI::template on<f_scl, f_clock>();}

    /// Reinicializa el dispositivo.
    static void reset() {TWI::reset();}


// open/close
    /// Inicia una transmisión. 
    /// No bloquea.
    /// Precondition: TWI::is_idle();
    /// Return: true, todo va bien; false, error (se viola la precondición)
    bool open(Address slave_address);

    /// Cierra la transmisión actual.
    /// Bloquea el flujo. No devuelve el control hasta que no la ha cerrado
    /// por completo.
    void close();

    /// Vamos a leer exactamente n bytes de TWI.
    /// No bloquea.
    /// Precondición: se ha llamado a open antes.
    void read(streamsize n);


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
    streamsize read(std::byte* q, streamsize n);

    // read: array-style
//    template <streamsize N>  doesn't work!!!
//    streamsize read(std::array<std::byte, N>& q) {return read(q.data(), N);}

    // write C-style
    /// Escribe q[0,n) en el flujo.
    /// No bloquea. q[0,n) se mete en el buffer interno de TWI y lo irá
    /// enviando poco a poco.
    streamsize write(const std::byte* q, streamsize n);

    // write: array-style
//    template <streamsize N> doesn't work!!!
//    streamsize write(const std::array<std::byte, N>& q) 
//    {return write(q.data(), N);}


// ISR
//    // FUNDAMENTAL: no olvidar llamar a esta función!!! <-- la llama el
//    main, usando directamente TWI_master o TWI_multimaster. Nosotros no
//    podemos decidir aquí!!!
//    static void handle_interrupt() {TWI::handle_interrupt();}


// estados
// -------
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


private:
// Data
    Address slave_address_;


    template <typename T>
    TWI_master_ioxtream& write_(const T& x)
    {// TODO: gestión de errores. Si write no devuelve sizeof(x) error	
	if (TWI::read_or_write()){
            TWI::write_to(slave_address_,
                          reinterpret_cast<const std::byte*>(&x),
                          sizeof(x));
        }
	else {
	    TWI::write(reinterpret_cast<const std::byte*>(&x), sizeof(x));
	}

	return *this;
    }

    template <typename T>
    TWI_master_ioxtream& read_(T& x)
    {// TODO: gestión de errores. Si read no devuelve sizeof(x) error
	TWI::wait_till_no_busy();
	
	TWI::read_buffer(reinterpret_cast<std::byte*>(&x), sizeof(x));
	return *this;
    }

};

template <typename T>
inline bool TWI_master_ioxtream<T>::open(Address slave_address)
{
    if (!TWI::is_idle())
	return false; // ¿hacer mejor TWI::reset()?

    slave_address_ = slave_address;

    TWI::send_start();

    return true;
}


template <typename T>
inline void TWI_master_ioxtream<T>::close()
{
    if (TWI::is_idle())	// No estamos dentro de una transmisión
	return;

    TWI::wait_till_no_busy();

    // if (TWI::eow() or TWI::eor())	<--- al no ponerlo podemos cerrar
    // desde cualquier estado. Mejor descomentarlo (???)
    TWI::send_stop();
}

template <typename T>
void TWI_master_ioxtream<T>::read(streamsize n)
{
    if (TWI::is_idle()) 
	TWI::send_start();

    TWI::wait_till_no_busy();

    if (!TWI::read_or_write()) // ignoramos el state real de TWI
	TWI::send_repeated_start();

    TWI::read_from(slave_address_, n);
}


// (*) Al principio pensé en que el usuario tuviese que elegir entre llamar a
// read(n) o a read(q, n), para no tener que escribir código del tipo:
//	twi.read(n);
//	twi.read(q, n);
//  que parece un poco redundante.
//  Sin embargo, nada más que escribí el primer programa me confundí, ya que
//  la forma que tengo de operar es:
//	read_object(st){
//	    twi.open();
//	    twi << cmd;
//	    twi.read(size_object);
//	    twi >> st; 
//	}
//
//  y el operator>> unas veces se limita a llamar a operator>>, mientras que
//  otras llama a read(q,n)!!!  Luego con esta forma de razonar llamaré a
//  read(n) y luego a read(q,n).
//	
template <typename T>
TWI_master_ioxtream<T>::streamsize
TWI_master_ioxtream<T>::read(std::byte* q, streamsize n)
{
//    read(n); (*)
    
    TWI::wait_till_no_busy();

    return TWI::read_buffer(q, n);
}


// pre: state() == read_or_write() or state() == transmitting()
template <typename T>
TWI_master_ioxtream<T>::streamsize 
TWI_master_ioxtream<T>::write(const std::byte* q, streamsize n)
{
    if (TWI::read_or_write())
	return TWI::write_to(slave_address_, q, n);

    else 
	return TWI::write(q, n);
}




}// namespace


#endif

