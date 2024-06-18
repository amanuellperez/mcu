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

#pragma once

#ifndef __DEV_EEPROM_IOSTREAM_H__
#define __DEV_EEPROM_IOSTREAM_H__

/****************************************************************************
 *
 *   - DESCRIPCION: Concebimos una EEPROM como un iostream.
 *
 *
 *   - HISTORIA:
 *           Manuel Perez- 14/10/2019 v0.0
 *
 ****************************************************************************/
#include <iostream>
#include "dev_EEPROM_lineal.h"
#include <string.h>


namespace dev{

/*!
 *  \brief  Buffer para gestionar una EEPROM lineal
 *
 *  Esta clase es responsable de:
 *	1.- Mantener la put area.
 *	2.- Llevar un control de la dirección donde queremos escribir en 
 *	la eeprom.
 *
 *  La forma de decidir si se está usando el buffer como put area o get area
 *  es a través del puntero gptr() y pptr(). Si es gptr() == nullptr el buffer
 *  no funciona como get area. Si pptr() == nullptr no funciona como put area.
 *
 *  De momento voy a poner la siguiente precondición:
 *	+ Si el buffer se está usando como de salida, el cliente no puede
 *	llamar a underflow (ni ninguna función de acceso a la get area).
 *	+ Si se está usando como de entrada, no se puede llamar overflow
 *	(ni ninguna función de acceso a la put area).
 *
 */
template <typename EEPROM, uint8_t BUFFER_SIZE = 8>
class EEPROM_streambuf : public std::streambuf {
public:
    using Address = typename EEPROM::Address;
    using size_type = typename EEPROM::size_type;

    EEPROM_streambuf()	{ null_state(); }
    ~EEPROM_streambuf() {   close(); }


    /// Abre un fichero que comienza en la dirección indicada
    bool open(Address address, std::ios_base::openmode mode);

    /// Cierra el fichero de forma ordenada volcando todos los datos que
    /// hubiera en el buffer.
    void close();

    EEPROM& eeprom() {return eeprom_;}
    const EEPROM& eeprom() const {return eeprom_;}

protected:
    // Buffer management
    // -----------------
    // Synchronizes the controlled sequences with the arrays (vamos, es
    // flush()). That is, if pbase() is non-null the characters [pbase(),
    // pptr()) are writen to the controlled sequence. The pointers may be
    // reset as appropiate.
    //
    // Returns: -1 on failure (cada clase derivada determinará qué considera
    // fallo).
    virtual int sync() override;


    // Ensures that there is space at the put area for at least one character
    // by saving some initial subsequence of characters starting at pbase() to
    // the output sequence and updating the poinnters to the put area (if
    // needed). If c is not eof() it is either put to the put area or directly
    // saved to the output sequence.
    //
    // Returns: traits::eof() if the function fails, or some other unspecified 
    // value to indicate success (typically returns c to indicate success, 
    // salvo cuando c == traits::eof() en cuyo caso devuelve traits::not_eof(c))
    virtual int_type overflow(int_type c = traits_type::eof()) override;

    // Ensures that at least one character is avaible in the input area by
    // updating the pointers to the input area and reading more data in from
    // the input sequence. Returns the value of that character on success or
    // traits_type::eof() on failure.
    // (Esta función no consume el caracter)
    virtual int_type underflow() override;


private:
    // Dispositivo al que conectamos
    EEPROM eeprom_;

    // put area: 
    char_type buffer_[BUFFER_SIZE];

    constexpr uint8_t buffer_size() const {return BUFFER_SIZE;}

    // Dirección actual donde escribiremos o leeremos el siguiente byte en/de
    // la EEPROM.
    // Observar que al solo usar una dirección para la get y put area, no
    // puedo usar esto como flujo de entrada/salida a la vez. O es de entrada,
    // o es de salida pero no ambos.
    Address address_ = 0;

    // Funciones de ayuda
    void put_area(size_type n)
    { 
	setp(buffer_, buffer_ + BUFFER_SIZE); 
	pbump(n);
    }

    void get_area(size_type n)
    { setg(buffer_, buffer_ + n, buffer_ + BUFFER_SIZE); }

    // put area: [pbase(), pptr()) (el end es eptr())
    void reset_put_area()
    { 
	setp(buffer_, buffer_ + BUFFER_SIZE);
	setg(buffer_, nullptr, buffer_ + BUFFER_SIZE);
    }

    // get area: [gptr(), egptr()) (el principio es eback())
    void reset_get_area()
    {
	setp(buffer_, buffer_ + BUFFER_SIZE);
	setg(buffer_, buffer_ + BUFFER_SIZE, buffer_ + BUFFER_SIZE);
    }

    // Tenemos 3 estados: 
    //	1. null_state (no estamos ni escribiendo ni leyendo)
    //	2. put_state (el buffer es la put area)
    //	3. get_state (el buffer es la get area)
    void null_state()
    {
	setp(buffer_, buffer_ + BUFFER_SIZE); 
	setg(buffer_, nullptr, buffer_ + BUFFER_SIZE);
    }


    // Returns: -1 on failure 
    int sync_put_area();
    int sync_get_area();
};


template <typename E, uint8_t P>
bool EEPROM_streambuf<E, P>::
open(Address address, std::ios_base::openmode mode) 
{ 
    // precondicion = address < max_address
    if (address >= eeprom().max_address())
	return false;

    if (mode == std::ios_base::out)
	reset_put_area(); 
    else if (mode == std::ios_base::in)
	reset_get_area();
    else
	return false;

    address_ = address; 
    
    return true;
}

template <typename E, uint8_t P>
void EEPROM_streambuf<E, P>::close() 
{
    sync();
    null_state();
}


template <typename E, uint8_t P>
int EEPROM_streambuf<E, P>::sync() 
{
    if (pptr() != nullptr)
	return sync_put_area();
    
    if (gptr() != nullptr)
	return sync_get_area();

    return -1;
}


// precondicion: pptr() != nullptr
template <typename E, uint8_t P>
int EEPROM_streambuf<E, P>::sync_put_area() 
{
    if (!eeprom_.good())
	return -1;

    if (pptr() == pbase())
	return 0;

    uint8_t* pb = reinterpret_cast<uint8_t*>(pbase());
    uint8_t* pp = reinterpret_cast<uint8_t*>(pptr());

    size_type n0 = static_cast<size_type>(pp - pb);
    size_type n = eeprom_.write(address_, pb, n0);

    address_ += n;

    if (n != n0){
	// TODO: es más eficiente hacer un copy directo (sé fijo que no se
	// van a overlap), pero tengo que escribir esa función y probarla.
	// Este caso es de esperar que apenas se dé (???). De momento no
	// pierdo tiempo con él.
	size_type resto = pptr() - pbase()  - n;
	memmove(pbase(), pbase() + n, resto);

	put_area(resto);


	return -1;
    }

    reset_put_area();

    if (!eeprom_.good())
	return -1;

    return static_cast<int>(n);
}


template <typename E, uint8_t P>
typename EEPROM_streambuf<E,P>::int_type 
EEPROM_streambuf<E, P>::overflow(int_type c)
{ 
    if (pptr() == nullptr or sync_put_area() == -1)
	return traits_type::eof();

    if (is_eof(c))
	return traits_type::not_eof(c);

    *pptr() = c;
    pbump(1);

    return traits_type::to_int_type(c);
}


// precondicion: gptr() != nullptr
template <typename E, uint8_t P>
int EEPROM_streambuf<E, P>::sync_get_area() 
{
    if (!eeprom_.good())
	return -1;

    if (gptr() != egptr())
	return (egptr() - gptr());

    uint8_t* eb = reinterpret_cast<uint8_t*>(eback());

    size_type n = eeprom_.read(address_, eb, buffer_size());

    address_ += n;

    if (n == buffer_size())
	get_area(0);
    else if (n > 0){
	memmove(egptr() - n, eback(), n);
	get_area(buffer_size() - n);
	return -1;
    }

    if (eeprom_.good())
	return n;
    else 
	return -1;
}



template <typename E, uint8_t P>
typename EEPROM_streambuf<E,P>::int_type 
EEPROM_streambuf<E,P>::underflow()
{
    if (gptr() == nullptr or sync_get_area() == -1)
	return traits_type::eof();

    return traits_type::to_int_type(*gptr());
}




/*!
 *  \brief  Flujo de salida para escribir en una EEPROM lineal.
 *
 */
// Un flujo sin abrir o cerrado tiene que estar en mal estado.
template <typename EEPROM, uint8_t BUFFER_SIZE = 8>
class EEPROM_ostream : public std::ostream{
public:
    using EEPROM_sbuf = EEPROM_streambuf<EEPROM, BUFFER_SIZE>;
    using Address = typename EEPROM_sbuf::Address;

    EEPROM_ostream() : std::ostream{&sb_} 
    { setstate(std::ios_base::badbit); }

    /// Crea un flujo de entrada abriéndolo en la dirección address.
    EEPROM_ostream(Address address) : EEPROM_ostream()
    { open(address); }

    /// Abre un fichero que comienza en la dirección indicada.
    EEPROM_ostream& open(Address address) 
    {
	clear();

	if (!sb_.open(address, std::ios_base::out))
	    setstate(std::ios_base::badbit);

	return *this;
    }

    /// Cierra el fichero de forma ordenada volcando todos los datos que
    /// hubiera en el buffer.
    void close() 
    {
	sb_.close();
	setstate(std::ios_base::badbit);
    }


    // Acceso al dispositivo
    EEPROM& eeprom() {return sb_.eeprom();}
    const EEPROM& eeprom() const {return sb_.eeprom();}



private:
    EEPROM_sbuf sb_;   

};


/*!
 *  \brief  Flujo de entrada para leer en una EEPROM lineal.
 *
 */
// Un flujo sin abrir o cerrado tiene que estar en mal estado.
template <typename EEPROM, uint8_t BUFFER_SIZE = 8>
class EEPROM_istream : public std::istream{
public:
    using EEPROM_sbuf = EEPROM_streambuf<EEPROM, BUFFER_SIZE>;
    using Address = typename EEPROM_sbuf::Address;

    EEPROM_istream() : std::istream{&sb_} 
    { setstate(std::ios_base::badbit); }

    /// Crea un flujo de entrada abriéndolo en la dirección address.
    EEPROM_istream(Address address) : EEPROM_istream()
    { open(address); }


    /// Abre un fichero que comienza en la dirección indicada.
    EEPROM_istream& open(Address address) 
    {
	clear();

	if (!sb_.open(address, std::ios_base::in))
	    setstate(std::ios_base::badbit);

	return *this;
    }

    /// Cierra el fichero de forma ordenada volcando todos los datos que
    /// hubiera en el buffer.
    void close() 
    {
	sb_.close();
	setstate(std::ios_base::badbit);
    }


    // Acceso al dispositivo
    EEPROM& eeprom() {return sb_.eeprom();}
    const EEPROM& eeprom() const {return sb_.eeprom();}


private:
    EEPROM_sbuf sb_;   

};

}// namespace

#endif


