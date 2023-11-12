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
 *
 *  - HISTORIA:
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
 *	TWI_master_ioxtream twi;    // antes hay que haber encendido TWI::turn_on!!!
 *	twi.open(slave_address);    // establece la conexión
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

 *  DUDA: ¿ponerle un state?
 *	Por una parte quiero que sea lo más eficiente posible, pero por otra
 *	un state es muy natural en un flujo. 
 *	De momento no lo pongo. Si lo necesito lo añado en el futuro.
 *	(Regla: ante la duda, implementación mínima. Siempre se puede añadir
 *	en el futuro sin cambiar el código actual.)
 */
template <typename TWI_master>
class TWI_master_ioxtream{
public:
    using TWI         = TWI_master;
    using Address     = TWI::Address;
    using streamsize  = TWI::streamsize;
    using iostate     = TWI::iostate;   // para depurar

// Construction
    /// Construimos la conexión conectandola
    explicit TWI_master_ioxtream(Address slave_address)
    {open(slave_address);}

    ~TWI_master_ioxtream() {close();}

// init
// El cliente es el responsable de la gestión de TWI. Es él el que se encarga
// de encenderlo o apagarlo. Por eso no suministro función `turn_on`

    /// Reinicializa el dispositivo.
    static void reset() {TWI::reset();}


// open/close
    // Inicia una transmisión. En general no llamarla directamente, ya que se
    // llama a través del constructor. 
    // DUDA: pensar este open
    // No bloquea.
    // Precondition: TWI::is_idle();
    // Return: no devuelvo nada ya que open no se debería de llamar en
    // general. Se llama a través del constructor. 
    void open(Address slave_address);

    /// Cierra la transmisión actual.
    /// Bloquea el flujo. No devuelve el control hasta que no la ha cerrado
    /// por completo.
    void close();



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
    // postcondition: TWI::state == read_or_write
    void send_start_if_needed();

    // Enviamos a TWI el comando de que lea `n` bytes. 
    void send_read_cmd(streamsize n);


    // TODO: gestión de errores. Si write no devuelve sizeof(x) error	
    // Formas posibles:
    //	    (1) Almacenar el número de bytes escritos/leidos y que se pueda
    //	        consultar despues.
    //	    (2) Ampliar el state de TWI con más errores. El problema es que no
    //	        sabría cúantos bytes se han enviado.
    template <typename T>
    TWI_master_ioxtream& write_(const T& x)
    {
//	send_start_if_needed(); (*) ver comentario en write()

	if (TWI::read_or_write()){
            TWI::write_to(slave_address_,
                          reinterpret_cast<const uint8_t*>(&x),
                          sizeof(x));
        }
	else {
	    TWI::write(reinterpret_cast<const uint8_t*>(&x), sizeof(x));
	}

//	write(reinterpret_cast<const uint8_t*>(&x), sizeof(x));
	return *this;
    }

    // TODO: gestión de errores. Si read no devuelve sizeof(x) error
    template <typename T>
    TWI_master_ioxtream& read_(T& x)
    {
//	send_read_cmd(sizeof(T));
//
//	TWI::wait_while_busy();
//
//	if (TWI::is_busy()) // ha vencido timeout 
//	    return *this;
//	
//	TWI::read_buffer(reinterpret_cast<uint8_t*>(&x), sizeof(x));
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
void TWI_master_ioxtream<T>::send_start_if_needed()
{
    if (TWI::is_idle()) 
	TWI::send_start();

    TWI::wait_while_busy();

    if (!TWI::read_or_write()) // ignoramos el state real de TWI
	TWI::send_repeated_start();
}


template <typename T>
void TWI_master_ioxtream<T>::send_read_cmd(streamsize n)
{
    send_start_if_needed();

    TWI::read_from(slave_address_, n);
}


template <typename T>
TWI_master_ioxtream<T>::streamsize
TWI_master_ioxtream<T>::read(uint8_t* q, streamsize n)
{
    send_read_cmd(n); 
    
    TWI::wait_while_busy();

    if (TWI::is_busy()) 
	return 0;

    return TWI::read_buffer(q, n);
}

// (*) Al escribir no se puede enviar repeated_start si estamos leyendo la RAM
//     del device. 
//     Ejemplo:
//	    TWI twi(slave_address);
//	    twi << ram_address;
//	    twi.write(buffer, n); <-- NO puedo enviar repeated_start
//
//	Si enviaramos repeated_start el device no sabría a qué zona de la
//	memoria queremos acceder!!!
//	    
template <typename T>
TWI_master_ioxtream<T>::streamsize 
TWI_master_ioxtream<T>::write(const uint8_t* q, streamsize n)
{
//    send_start_if_needed(); (*)

    if (TWI::read_or_write())
	return TWI::write_to(slave_address_, q, n);

    else 
	return TWI::write(q, n);

}





}// namespace


#endif

