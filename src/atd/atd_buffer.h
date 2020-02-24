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

#ifndef __ATD_BUFFER_H__
#define __ATD_BUFFER_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Primera versión de buffer para TWI.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    12/02/2020: v0.0 - TWI_iobuffer 
 *    22/02/2020: Circular_array, ioxtream_of_bytes
 *
 ****************************************************************************/
#include <array>
#include <algorithm>
#include <cstddef>  // std::byte
#include <tuple>    // std::tie
#include "atd_algorithm.h"

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
    void in_write_one(std::byte b);

    /// Lee el buffer de entrada copiandolo a partir de q. Copia como máximo N
    /// bytes.
    /// Returns: el número de bytes movidos del buffer de entrada a q.
    /// Llamando n a ese número, se tiene que [q, n) son los bytes copiados
    /// del input_buffer.
    size_type in_read_all_n(std::byte* q, size_type N);

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
    size_type out_write_all_n(const std::byte* q, size_type n);

    /// Lee el buffer de entrada copiandolo a partir de q. Copia como máximo N
    /// bytes.
    /// WARNING: no mira que se lea fuera del buffer!!! El usuario tendrá
    /// que mirar if (out_is_empty()) ... para saber si queda algo que leer o
    /// no.
    std::byte out_read_one();

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
    std::array<std::byte, buffer_size> buffer_; // = [p0, pe)
    std::byte* pa_;

    constexpr std::byte* p0_() {return buffer_.begin();}
    constexpr std::byte* pe_() {return buffer_.end();}

    constexpr const std::byte* p0_() const {return buffer_.begin();}
    constexpr const std::byte* pe_() const {return buffer_.end();}

};

template <uint8_t sz>
inline void TWI_iobuffer<sz>::in_write_one(std::byte b)
{
    *pa_ = b;
    ++pa_;
}

template <uint8_t sz>
TWI_iobuffer<sz>::size_type TWI_iobuffer<sz>::in_read_all_n(std::byte* q,
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
inline std::byte TWI_iobuffer<sz>::out_read_one()
{
    std::byte b = *pa_;
    ++pa_;
    return b;
}

template <uint8_t sz>
TWI_iobuffer<sz>::size_type
TWI_iobuffer<sz>::out_write_all_n(const std::byte* q, size_type n_q)
{
    if (!out_is_empty()) // realmente es: assert(!out_is_empty());
	return 0;	    

    size_type n = std::min(n_q, capacity());

    pa_ = pe_() - n;
    std::copy_n(q, n, pa_);

    return n;
    
}

/*!
 *  \brief  Array circular.
 */
// DUDA: buffer_size de qué tipo? size_t como std::array? uint8_t por
// cuestiones de eficiencia? int? La regla es evitar los unsigneds!
// Parametrizar el tipo y que el usuario elija?
template <typename T, int N>
class Circular_array{
public:
// Types
    using size_type	    = int;

// Construcción
    Circular_array();

// Acceso
    /// Escribe exactamente n elementos en el buffer. Escribe b[0, n).
    /// En caso de no haber espacio suficiente no hace nada.
    /// Returns:  el número de elementos escritos. Cero si no hace nada.
    size_type ewrite(const T* b, size_type n);

    /// Lee exactamente n elementos del buffer almacenándolos en b[0, n).
    /// Leerlos supone sacarlos del buffer.
    /// En caso de no haya n elementos para leer no hace nada.
    /// Returns:  el número de elementos leídos. Cero si no lee nada (porque
    /// haya pocos elementos).
    size_type eread(T* b, size_type n);

    /// Escribe 'b' en el buffer.
    /// precondition: !is_full();
    void write(const T& b);

    /// Lee 'b' del buffer, extrayéndolo. 
    /// precondition: !is_empty();
    /// Return: la misma referencia 'b'.
    T& read(T& b);
    
    /// Lee 'b' del buffer, extrayéndolo. 
    /// precondition: !is_empty();
    /// Return: la misma referencia 'b'.
    /// CUIDADO: se devuelve T por valor!!! Esta pensado para bytes, chars...
    /// así que no hay problema.
    T read()
    {
	T b;
	return read(b);
    }

    // Vacía el buffer.
    void reset();

// Info
    bool is_empty() const {return (q0_ == qe_ and q0_ == p0_);}
    bool is_full() const  
    { return (q0_ == p0_ and qe_ == pe_()) or (q0_ == qe_ and q0_ != p0_); }

    /// Número máximo de elementos que podemos almacenar en el buffer.
    constexpr size_type capacity() const {return N;}

    /// Número de elementos que hay escritos en el buffer pendientes de ser
    /// leídos.
    size_type size() const;

    /// Número de elementos que se pueden escribir en el buffer.
    size_type available() const {return capacity() - size();}

private:
// Datos
    T p0_[N];
    
    // Contenedor: [p0_, pe_)
    T* pe_() {return  p0_ + N;}
    const T* pe_() const {return  p0_ + N;}

    T* q0_; // Datos: [q0_...pe_, p0_... qe_) (circular)
    T* qe_;

// Funciones de ayuda

    void eread_circular(T* b0, size_type n);
    void eread_lineal(T* b0, size_type n);

};


template <typename T, int N>
Circular_array<T, N>::Circular_array():q0_{p0_}, qe_{p0_}
{ }

template <typename T, int N>
inline void Circular_array<T, N>::reset()
{
    q0_ = p0_;
    qe_ = p0_;
}


template <typename T, int N>
Circular_array<T, N>::size_type 
    Circular_array<T, N>::size() const 
{
    if (is_empty())
	return 0;

    if (qe_ > q0_)
	return qe_ - q0_;

    return (pe_() - q0_) + (qe_ - p0_);
}



template <typename T, int N>
Circular_array<T, N>::size_type 
    Circular_array<T, N>::ewrite(const T* b0, size_type n)
{
    if (available() < n)
	return 0;

    const T* be = b0 + n;
    std::tie(b0, qe_) = atd::copy(b0, be, qe_, pe_());

    if (b0 == be)
	return n;

    qe_ = p0_;
    std::tie(b0, qe_) = atd::copy(b0, be, qe_, q0_);

    // postcondition: b0 == be and qe_ <= q0_
    return n;
}


// precondition: !is_full();
template <typename T, int N>
void Circular_array<T, N>::write(const T& b)
{
    *qe_ = b;
    ++qe_;

    if (qe_ == pe_() and q0_ != p0_)
	qe_ = p0_;

}


// precondition: !is_empty();
template <typename T, int N>
T& Circular_array<T, N>::read(T& b)
{
    b = *q0_;

    ++q0_;


    if (q0_ == qe_){ // is_empty!!!
	q0_ = p0_;
	qe_ = p0_;
    }
    else if (q0_ == pe_())
	q0_ = p0_;

    return b;
}



// precondition: q0_ < qe_
template <typename T, int N>
inline void Circular_array<T, N>::eread_lineal(T* b, size_type n)
{
    std::copy_n(q0_, n, b);

    q0_ += n;

    if (q0_ == qe_)
	reset();
}

// precondition: qe_ <= q0_
template <typename T, int N>
void Circular_array<T, N>::eread_circular(T* b0, size_type n)
{
    if (pe_() - q0_ >= n){
	std::copy_n(q0_, n, b0);
	q0_ += n;

	if (q0_ == qe_)
	    q0_ = p0_;
    }

    else{
	T* be = b0 + n;
	std::tie(std::ignore, b0) = atd::copy(q0_, pe_(), b0, be);
	size_type nc = pe_() - q0_; // núm. copiados
	
	std::tie(q0_, b0) = atd::copy(p0_, p0_ + n - nc, b0, be);

	if (q0_ == qe_)
	    reset();
    }
}



template <typename T, int N>
Circular_array<T, N>::size_type 
Circular_array<T, N>::eread(T* b, size_type n)
{
    if (size() < n or n == 0)
	return 0;

    if (q0_ < qe_)
	eread_lineal(b, n);

    else
	eread_circular(b, n);

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
    Circular_array<std::byte, N> buffer_;


    template <typename T>
    iobxtream& write(const T& x)
    {
	buffer_.ewrite(reinterpret_cast<const std::byte*>(&x), sizeof(x));
	return *this;
    }

    template <typename T>
    iobxtream& read(T& x)
    {
	buffer_.eread(reinterpret_cast<std::byte*>(&x), sizeof(x));
	return *this;
    }

};


}// namespace

#endif

