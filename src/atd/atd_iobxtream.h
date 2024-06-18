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

#ifndef __ATD_IOBXTREAM_H__
#define __ATD_IOBXTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Primera versión de buffer para TWI.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    12/02/2020 v0.0 - TWI_iobuffer 
 *    22/02/2020 Circular_array, ioxtream_of_bytes
 *    17/06/2021 Reestructurado.
 *    01/11/2023 Elimino uint8_t a favor de uint8_t, es más sencillo de
 *               manejar.
 *
 ****************************************************************************/
#include <array>
#include <algorithm>
#include "atd_algorithm.h"
#include "atd_array.h"	// Circular_array

namespace atd{

/*!
 *  \brief  Buffer tanto de entrada como de salida para TWI.
 *
 *  Solo puede funcionar como de entrada o como de salida.
 *  
 *  TODO: el nombre es feo. Lo dejo como TWI para indicar que es el buffer que
 *  voy a usar en TWI. Tiene las siguientes características:
 *	    1. Puede funcionar o de entrada o de salida, nunca de las dos 
 *	       cosas a la vez.
 *	    2. Es de capacidad fija, definida en tiempo de compilación por
 *	       buffer_size.
 *	    3. El buffer de entrada es de tipo write_one_by_one/read_all.
 *	    4. El buffer de salida es de tipo write_all/read_one_by_one.
 *  ¿Qué nombre genérico darle a este tipo de buffer? Lo puedo dejar como twi
 *  (???) Observar que lo que suministramos son 2 buffers (que sean de
 *  entrada/salida le da el significado el usuario): uno
 *  write_one_by_one/read_all y el otro write_all/read_one_by_one. 
 *  ¿qué nombre genérico darle?
 */
template <uint8_t buffer_size>
class TWI_iobuffer{
public:
// Types
    using size_type = uint8_t;

// Buffer de entrada
// -----------------
//	  Se trata de un buffer de tipo write_one_by_one/read_all
//	  [p0, pa) = bytes pendientes de leer por el usuario.
    /// Definimos el buffer como de entrada. Estado inicial: vacío.
    constexpr void reset_as_input() {pa_ = p0_();}

    /// Writes byte b in input buffer.
    /// WARNING: no mira que se escriba fuera del buffer!!! El usuario tendrá
    /// que mirar if (in_is_full()) ... para saber si está lleno o no. En caso
    /// de estar lleno tendrá que vaciarlo leyendolo.
    void in_write_one(uint8_t b);

    /// Lee el buffer de entrada copiandolo a partir de q. Copia como máximo N
    /// bytes.
    /// Returns: el número de bytes movidos del buffer de entrada a q.
    /// Llamando n a ese número, se tiene que [q, n) son los bytes copiados
    /// del input_buffer.
    size_type in_read_all_n(uint8_t* q, size_type N);

    /// Checks whether the input buffer is empty.
    bool in_is_empty() const {return pa_ == p0_();}

    /// Checks whether the input buffer is full.
    bool in_is_full() const {return pa_ == pe_();}


// Buffer de salida
// ----------------
//	  Se trata de un buffer de tipo write_all/read_one_by_one.
//	  [pa, pe) = bytes pendientes de leer por el device.
//
//	  Idioma:   1. Se escribe en el buffer.
//		    2. Se lee uno tras otro TODOS los bytes del buffer.
//		    3. Cuando el buffer está vacío se puede volver a escribir, 
//		       no antes.
//
//	Este idioma simplemente es para simplificar out_write_all. En el
//	futuro, si se ve necesario relajar este idioma.
 
    /// Definimos el buffer como de salida. Estado inicial: vacío.
    constexpr void reset_as_output() {pa_ = pe_();}

    /// Intenta escribir los bytes q[0, n) en el buffer. Si n > capacity(),
    /// llena el buffer no sobrepasando la capacidad del buffer.
    /// Returns: número de bytes escritos. 
    size_type out_write_all_n(const uint8_t* q, size_type n);

    /// Lee el buffer de entrada copiandolo a partir de q. Copia como máximo N
    /// bytes.
    /// WARNING: no mira que se lea fuera del buffer!!! El usuario tendrá
    /// que mirar if (out_is_empty()) ... para saber si queda algo que leer o
    /// no.
    uint8_t out_read_one();

    /// Checks whether the output buffer is empty. (si hay datos que enviar
    /// al dispositivo o no).
    bool out_is_empty() const {return pa_ == pe_();}

    /// Número de bytes que quedan pendientes de enviar al dispositivo.
    size_type out_size() const {return pe_() - pa_;}

    // ¿Necesito esta para algo?
//    /// Checks whether the output buffer is full.
//    bool out_is_full() const {return pa_ == p0_();}

// Info
    constexpr size_type capacity() const {return buffer_size;}

private:
    std::array<uint8_t, buffer_size> buffer_; // = [p0, pe)
    uint8_t* pa_;

    constexpr uint8_t* p0_() {return buffer_.begin();}
    constexpr uint8_t* pe_() {return buffer_.end();}

    constexpr const uint8_t* p0_() const {return buffer_.begin();}
    constexpr const uint8_t* pe_() const {return buffer_.end();}

};

template <uint8_t sz>
inline void TWI_iobuffer<sz>::in_write_one(uint8_t b)
{
    *pa_ = b;
    ++pa_;
}

template <uint8_t sz>
TWI_iobuffer<sz>::size_type TWI_iobuffer<sz>::in_read_all_n(uint8_t* q,
                                                            size_type N_q)
{
    size_type n_p = pa_ - p0_();

    size_type n = std::min(N_q, n_p);
	
    std::copy_n(p0_(), n, q);

    if (n < n_p)
	pa_ = std::shift_left(p0_(), pa_, n);
    else
	reset_as_input();

    return n;

}

template <uint8_t sz>
inline uint8_t TWI_iobuffer<sz>::out_read_one()
{
    uint8_t b = *pa_;
    ++pa_;
    return b;
}

template <uint8_t sz>
TWI_iobuffer<sz>::size_type
TWI_iobuffer<sz>::out_write_all_n(const uint8_t* q, size_type n_q)
{
    if (!out_is_empty()) // realmente es: assert(!out_is_empty());
	return 0;	    

    size_type n = std::min(n_q, capacity());

    pa_ = pe_() - n;
    std::copy_n(q, n, pa_);

    return n;
    
}


/*!
 *  \brief  Flujo de bytes de tamaño fijo. 
 *
 *  Almacena directamente los objetos como bytes, no como chars, a diferencia
 *  de stringstream y los flujos iostream.
 *
 *  Los bxtreams son fix_stream de bytes.
 *
 *  TODO: se le puede pasar otro parámetro de template para indicar si
 *  queremos que se codifique en little endian o en big endian.
 *
 *  TODO: de momento no introduzco state, dejo que sea el usuario el que mire
 *  si hay suficiente espacio antes de escribir. (quiero que sea lo más
 *  pequeña posible)
 */
template <int N>
class iobxtream{
public:
    using size_type = int;

// Info
    /// Número de bytes escritos en el flujo pendientes de leer.
    size_type size() const {return buffer_.size();}

    /// Número de bytes que podemos escribir en el flujo.
    size_type available() const {return buffer_.available();}

    bool is_empty() const {return buffer_.is_empty();}

// Varios
    /// Vacía el flujo, reiniciandolo, como si lo acabaramos de construir.
    void reset() {buffer_.reset();}

// operator<<
    iobxtream& operator<<(char c) {return write(c);}
    iobxtream& operator<<(unsigned char c) {return write(c);}
    iobxtream& operator<<(signed char c) {return write(c);}
    iobxtream& operator<<(short c) {return write(c);}
    iobxtream& operator<<(unsigned short c) {return write(c);}
    iobxtream& operator<<(int c) {return write(c);}
    iobxtream& operator<<(unsigned int c) {return write(c);}
    iobxtream& operator<<(const long& c) {return write(c);}
    iobxtream& operator<<(const unsigned long& c) {return write(c);}
    iobxtream& operator<<(const long long& c) {return write(c);}
    iobxtream& operator<<(const unsigned long long& c) {return write(c);}

// operator>>
    iobxtream& operator>>(char& c) {return read(c);}
    iobxtream& operator>>(unsigned char& c) {return read(c);}
    iobxtream& operator>>(signed char& c) {return read(c);}
    iobxtream& operator>>(short& c) {return read(c);}
    iobxtream& operator>>(unsigned short& c) {return read(c);}
    iobxtream& operator>>(int& c) {return read(c);}
    iobxtream& operator>>(unsigned int& c) {return read(c);}
    iobxtream& operator>>(long& c) {return read(c);}
    iobxtream& operator>>(unsigned long& c) {return read(c);}
    iobxtream& operator>>(long long& c) {return read(c);}
    iobxtream& operator>>(unsigned long long& c) {return read(c);}

private:
    Circular_array<uint8_t, N> buffer_;


    template <typename T>
    iobxtream& write(const T& x)
    {
	buffer_.ewrite(reinterpret_cast<const uint8_t*>(&x), sizeof(x));
	return *this;
    }

    template <typename T>
    iobxtream& read(T& x)
    {
	buffer_.eread(reinterpret_cast<uint8_t*>(&x), sizeof(x));
	return *this;
    }

};


}// namespace

#endif

