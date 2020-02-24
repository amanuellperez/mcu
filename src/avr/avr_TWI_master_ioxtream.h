// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    24/02/2020 v0.0
 *
 ****************************************************************************/
#include "avr_TWI_master.h"


namespace avr{
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
 *  DUDA: ¿ponerle un state?
 *	Por una parte quiero que sea lo más eficiente posible, pero por otra
 *	un state es muy natural en un flujo. 
 *	De momento no lo pongo. Si lo necesito lo añado en el futuro.
 *	(Regla: ante la duda, implementación mínima. Siempre se puede añadir
 *	en el futuro sin cambiar el código actual.)
 */
template <typename TWI_basic, uint8_t buffer_size>
class TWI_master_ioxtream{
public:
    using TWI = TWI_master<TWI_basic, buffer_size>;   
    using Address = TWI::Address;
    using streamsize = TWI::streamsize;

    /// Enables TWI interface definiendo la frecuencia del SCL 
    /// a la que vamos a operar.
    /// f_scl = frecuencia en kilohercios de SCL (tipica: 100 y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {TWI::on<f_scl, f_clock>();}

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
    /// Return: true, todo va bien; false, error (se viola la precondición)
    void read(streamsize n);


// operator<<. Ninguna bloquea.
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
    

private:
// Data
    // Solo podemos conectarnos a un slave a la vez, por eso lo defino static.
    static Address slave_addres_;


    template <typename T>
    TWI_master_ioxtream& write_(const T& x)
    {
AQUIII: solo falta write_ y read_
	TWI::(reinterpret_cast<const std::byte*>(&x), sizeof(x));
	return *this;
    }

    template <typename T>
    TWI_master_ioxtream& read_(T& x)
    {
	buffer_.eread(reinterpret_cast<std::byte*>(&x), sizeof(x));
	return *this;
    }

};

template <typename T, uint8_t bsz>
inline bool TWI_master_ioxtream::open(Address slave_address)
{
    if (!TWI::is_idle())
	return false;

    TWI::send_start();
    return true;
}


template <typename T, uint8_t bsz>
inline void TWI_master_ioxtream::close()
{
    TWI::wait_till_no_busy();

    // if (TWI::eow() or TWI::eor())	<--- al no ponerlo podemos cerrar
    // desde cualquier estado. Mejor descomentarlo (???)
    TWI::send_stop();
}

template <typename T, uint8_t bsz>
bool TWI_master_ioxtream::read(streamsize n)
{
    if (TWI::is_idle()) // no envio start para obligar al usuario a llamar
	return false;	// a open, pasando la dirección del slave.

    TWI::wait_till_no_busy();

    if (!TWI::read_or_write())
	TWI::send_repeated_start();

    TWI::read_from(slave_address_, n);

    return true;
}




}// namespace


#endif

