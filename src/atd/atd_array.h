// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ATD_ARRAY_H__
#define __ATD_ARRAY_H__
/****************************************************************************
 *
 *  - DESCRIPCION: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    17/07/2021 v0.0 Reestructurando
 *
 ****************************************************************************/
#include <tuple>    // std::tie
#include <algorithm>
#include "atd_algorithm.h"

namespace atd{

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

}// namespace




#endif


