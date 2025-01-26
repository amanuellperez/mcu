// Copyright (C) 2021-2025 Manuel Perez 
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

#ifndef __ATD_ARRAY_H__
#define __ATD_ARRAY_H__
/****************************************************************************
 *
 *  - DESCRIPCION: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    17/07/2021 Reestructurando
 *		 Tenemos varios tipos de arrays:
 *		 1. De memoria de longitud fija.
 *		 2. De memoria de longitud variable.
 *
 *		 std::array es de longitud fija mientras que std::vector es de
 *		 longitud variable. Sin embargo hay una diferencia fundamental
 *		 entre los dos: 
 *		  * std::vector diferencia entre la memoria disponible y la 
 *		    memoria usada. [begin, end) = es la memoria usada.
 *		  * std::array no hace esa diferencia. [begin, end) = es la
 *		    memoria reservada.
 *		
 *		 ¿Cómo llamar al intermedio? ¿A un array que diferencie entre 
 *		 la memoria reservada y la memoria usada? En principio pensé
 *		 en llamarlo Buffer, pero como ya tenía Circular_array que es
 *		 algo similar, parece que tenga sentido llamarlo Linear_array.
 *
 *		 Tenemos los siguientes tipos de arrays:
 *		 * std::array: memoria local, no gestiona la memoria usada.
 *		 * atd::Array : memoria local, gestiona la usada. 
 *			         Distingue entre x[0..N) memoria reservada, de
 *			         x[0..size()) memoria usada.
 *		 * atd::Linear_array  : memoria local, gestiona la usada.
 *				Es más elaborada que Array: x[0..N) es la
 *				memoria reservada, pero [p0, pe) es la memoria
 *				usada (donde p0 puede ser distinto de x[0]!!!)
 *		 * atd::Circular_array: idem.
 *		 * std::vector: memoria dinámica, gestiona la usada.
 *		
 *   05/11/2022 Array_view
 *		Al intentar usar atd::Linear_array me encuentro con el
 *		problema de que al ser una template estás obligado a conocer
 *		el número máximo de elementos o definir toda la función que
 *		usa el Linear_array como template en un .h. Sin embargo, para
 *		manejar interrupciones me resulta más sencillo de programar
 *		definir las funciones dentro de los .cpp y no en los .h, no
 *		pudiendo usar Linear_array. Por eso creo este Array. 
 *		Como siempre, es experimental. El uso marcará qué arrays son
 *		los útiles.
 *
 *   16/11/2022 Array
 *   26/01/2025 Array_of_bytes_view
 *
 ****************************************************************************/
#include <tuple>    // std::tie
#include <algorithm>
#include <stdlib.h>   // size_t
#include <cstddef> // std::ptrdiff_t
#include <type_traits>
#include <iterator> // begin/end
#include <initializer_list>
#include "atd_algorithm.h"
#include "atd_type_traits.h"

namespace atd{

/***************************************************************************
 *			    CIRCULAR ARRAY
 ***************************************************************************/
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
    if (b0 == nullptr or n == 0){
	reset();
	return 0;
    }

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


/***************************************************************************
 *				LINEAR ARRAY
 ***************************************************************************/
// El interfaz que le voy a dar es similar al de un flujo de entrada/salida.
// De hecho dudé si llamarlo atd::iostream_buffer ya que es exactamente eso
// (salvo que en lugar de usar los nombres del estandar de C++, uso los de C 
// que me resultan más naturales)
template<typename T, size_t N>
class Linear_array {
public:
// Types
    using value_type	    = T;
    using pointer	    = T*;
    using const_pointer	    = const T*;
    using reference	    = T&;
    using const_reference   = const T&;
    using size_type	    = size_t;
    using difference_type   = std::ptrdiff_t;
    using iterator	    = T*;	// Garantizado que sea un puntero.
    using const_iterator    = const T*;
 
    // Construction
    constexpr Linear_array() : p0_{m0()}, pe_{m0()} {}
    constexpr Linear_array(std::initializer_list<T> v)
    {
	size_t n = std::min(v.size(), N);

	std::copy_n(v.begin(), n, a);
	p0_ = m0();
	pe_ = p0_ + n;
    }


// Funciones de escritura: escribimos siempre al final.
    /// Appends the given element to the end of the container.
    /// Es responsabilidad del usuario garantizar que end() != me()
    constexpr void push_back(const T& x);

    /// Removes the last element of the container.
    /// Es responsabilidad del usuario garantizar que size() > 0
    constexpr T pop_back();

// Funciones de lectura: 
//    Leemos desde el principio (de izda a dcha). Primero el caracter 0, luego
//    el 1, ... (justo al revés que la escritura).
    /// Añadimos el elemento x al principio del array.
    /// Es responsabilidad del usuario garantizar que begin() != m0()
    constexpr void push_front(const T& x);

    /// Extraemos el primer elemento del array.
    /// Es responsabilidad del usuario garantizar que size() > 0
    constexpr T pop_front();


// Funciones de inserción: 
    /// Inserta el elemento x en la posición p.
    /// Ejemplo: array = 12345-----
    /// al llamar insert(iterator_posicion_3, 9) modifica el array
    /// como     array = 129345----. Observar que inserta 9 entre 12 y 345.
    ///
    /// En caso de que no se pueda copiar la parte derecha, se perderá un
    /// elemento. Ejemplo: si array = '12345'end, al insertar el 9 como en el
    /// ejemplo anterior obtenemos array = '12934'end. Como se ve se pierde el
    /// 5 por no haber memoria donde escribirlo.
    constexpr void insert(iterator p, const T& x);

    constexpr void insert(size_t i, const T& x) {insert(&p0_[i], x);}

    /// Sinónimo de *p = x.
    // La doy por consistencia: si hay insert, también hay replace.
    constexpr void replace(iterator p, const T& x) {*p = x;}
    
    // (???) remove or erase? Which name is better?
    /// Remove element pointed by p.
    /// Example: if array = '12345---' the call of remove(iterator_of_3)
    /// returns array = '1245----'
    constexpr void remove(iterator p);

    constexpr void remove(size_t i) {remove(&p0_[i]);}

// Clear:
    /// Erases all elements from the container.
    constexpr void clear() {p0_ = m0(); pe_ = p0_;} 

    // iterators
    constexpr iterator begin()  {return p0_;}
    constexpr iterator end()  {return pe_;}

    constexpr const_iterator begin() const  
    {return p0_;}

    constexpr const_iterator end() const  
    {return pe_;}

    /// Cambiamos el inicio del array. 
    /// Es responsabilidad del usuario garantizar que p0_ <= pe_
    // Observar que le paso un puntero y no un iterador. Esta es una clase de
    // bajo nivel que quiero usar como arrays de C. Por eso se garantiza que
    // los iteradores son punteros.
    constexpr void begin(T* p0)  {p0_ = p0;}

    
    // capacity:
    constexpr bool empty() const 
    { return size() == size_type(0); }

    /// Devuelve el tamaño de la memoria usada.
    constexpr size_type size() const  
    {return pe_ - p0_;}

    /// Devuelve el tamaño de la memoria reservada.
    constexpr size_type max_size() const  
    {return N;}

    // elemental access:
    constexpr reference operator[](size_type n)
    {return p0_[n];}

    constexpr const_reference operator[](size_type n) const
    {return p0_[n];}


    // [m0, me) = memoria disponible
    constexpr T* m0() {return &a[0];}
    constexpr T* me() {return &a[N];}

    constexpr const T* m0() const {return &a[0];}
    constexpr const T* me() const {return &a[N];}

private:
    // memory
    T a[N];

    // [p0, pe) = memoria usada
    T* p0_;
    T* pe_;


};


template<typename T, size_t N>
inline constexpr void Linear_array<T,N>::push_back(const T& x)
{
    *pe_ = x;
    ++pe_;
}

template<typename T, size_t N>
inline constexpr void Linear_array<T,N>::push_front(const T& x)
{
    --p0_;
    *p0_ = x;
}


template <typename T, size_t N>
inline constexpr T Linear_array<T, N>::pop_back()
{
    --pe_;
    return *pe_;
}

template <typename T, size_t N>
inline constexpr T Linear_array<T, N>::pop_front()
{
    return *p0_++;
}



template <typename T, size_t N>
constexpr void Linear_array<T,N>::insert(iterator p, const T& x)
{
    static_assert(N > 0); // --pe_

    if (p != pe_){// move_one_to_the_right_of(p):

	if (pe_ == me()) // garantizamos sea posible hacer ++pe_;
	    --pe_;

	iterator q = pe_;

	for (; q != p; --q)
	    *q = q[-1];

    }

    *p = x;
    ++pe_;
}

    
template <typename T, size_t N>
constexpr void Linear_array<T,N>::remove(iterator p)
{
    ++p;

    for(; p != pe_; ++p)
	p[-1] = *p;

    --pe_;
}



/***************************************************************************
 *				ARRAY
 ***************************************************************************/
// En un array tenemos que distinguir dos cosas: la memoria reservada y la
// memoria usada (con datos). El problema con los arrays clásicos es que no
// gestionan la memoria usada, cosa que tampoco lo hace std::array. atd::Array
// gestiona esa memoria.
// Distinguimos entre:
//	    Array<T, 10> x;
//	    x[0..10) = memoria reservada. Es la que se reserva al hacer
//						    T x[10];
//	    x[0..size()) = memoria usada. Esto es lo que aporta esta clase.
//
// Array es una clase más sencilla que Linear_array.
template <typename T, size_t N>
class Array{
public:
// Types
    using value_type	    = T;
    using pointer	    = T*;
    using const_pointer	    = const T*;
    using reference	    = T&;
    using const_reference   = const T&;
    using size_type	    = size_t;
    using difference_type   = std::ptrdiff_t;
    using iterator	    = T*;	
    using const_iterator    = const T*;


// Methods
    Array() : ie_{0} { }

    reference operator[] (size_type i) {return ptr_[i];}
    const_reference operator[] (size_type i) const {return ptr_[i];}

    bool empty() const {return ie_ == 0;}
    bool full () const {return ie_ == N;}

    /// Erases all elements from the container.
    void clear() {ie_ = 0;}

    // Si se maneja el array como array de C se tiene que poder definir
    // el número de elementos usados.
    void size(size_type n) {ie_ = n;}

    /// Número de elementos almacenados en el array.
    // size() <= capacity()
    size_type size() const {return ie_;}

    /// Tamaño máximo del array (= memoria reservada)
    size_type capacity() const {return N;}

    /// Añade un elemento al final del array.
    /// Garantiza que no haya overflow. En caso de que no haya espacio
    /// no hace nada.
    void push_back(const_reference x);

    /// Borra el último elemento añadido.
    /// Garantiza que no haya underflow.
    void pop_back();

    /// Direct access to the underlying array
    pointer data() { return ptr_; }
    const_pointer data() const {return ptr_;}

// Iterators
    iterator begin() {return ptr_;}
    iterator end() {return ptr_ + ie_;}

    const_iterator begin() const {return ptr_;}
    const_iterator end() const  {return ptr_ + ie_;}

private:
// Data
    T ptr_[N];
    size_type ie_;  // índice del último elemento
		    // ptr_[0..ie_) = contiene los datos
		    // ptr_[0..N)   = memoria reservada
};


template <typename T, size_t N>
void Array<T, N>::push_back(const_reference x)
{
    if (ie_ < N){
	ptr_[ie_] = x;
	++ie_;
    }
}

template <typename T, size_t N>
inline void Array<T, N>::pop_back()
{
    if (ie_ > 0) 
	--ie_;
}



// (RRR) ¿Por qué llamarlo _view? 
//	 Porque es una view. No tiene propiedad del array. El array hay que
//	 definirlo en otro sitio.
//
//	 Array_view es un array básico de C con max_size elementos (número de
//	 elementos reservados en memoria). El problema con los arrays de C es
//	 que no se saben cuántos elementos de esos están ocupados. La variable
//	 `size` nos dice el número de elementos usados.
//
//	 Ejemplos:
//	    int x[4];
//	    x[0] = 20;
//	    x[1] = 30;
//
//	    Array_view view{x, 2, 4};
//	    view = es una view del array x, contenedor de 4 elementos, de los
//	    cuales solo 2 están ocupados.
//
//  Se trata de una generalización de std::span. Aunque de momento no está
//  implementado como std::span tendría que ser un std::span al que le
//  añadimos el distinguir entre la capacity() del array y el size().
//
//  (TODO) Lo más probable es que esta clase esté obsoleta ya que ahora tengo
//  Array que es un contenedor. En lugar de un array de C usar Array y así
//  evitamos Array_view.
template <typename T>
class Array_view{
public:
// Types
    using value_type	    = T;
    using pointer	    = T*;
    using const_pointer	    = const T*;
    using reference	    = T&;
    using const_reference   = const T&;
    using size_type	    = size_t;
    using difference_type   = std::ptrdiff_t;
    using iterator	    = T*;	
    using const_iterator    = const T*;


// Constructors
    Array_view(T* p0, size_type sz, size_type max_sz)
	: ptr_{p0}, size_{sz}, mem_size_{max_sz} { }

    template <size_t N>
    Array_view(Array<T, N>& x) : Array_view{x.data(), x.size(), N} { }

// Element access
    // (TODO) esto es un incordio. Al intentar probar Array_of_bytes_view con
    // este tipo de array me falla, ya que no es natural no poder inicializar
    // un array con a[10] = 4; Esto hay que revisarlo.
    // ¡El interfaz de un array tiene que ser el habitual! 
    reference operator[] (size_type i) {return ptr_[i];}
    const_reference operator[] (size_type i) const {return ptr_[i];}

    /// Añade un elemento al final del array.
    void push_back(const_reference x);

    /// Borra el último elemento añadido.
    void pop_back();

// Observers
    bool empty() const {return size() == 0;}
    bool full () const {return size() == capacity();}

    size_type size() const {return size_;}
    size_type capacity() const {return mem_size_;}


// Iterators
    iterator begin() {return ptr_;}
    iterator end() {return ptr_ + size();}

    const_iterator begin() const {return ptr_;}
    const_iterator end() const  {return ptr_ + size();}

private:
// Data
    T* ptr_;
    size_type size_;	
    size_type mem_size_; // = memory_size
};


// DUDA: validar size_ < mem_size_??? Solo es un condicional y evitamos
// problemas de memoria.
template <typename T>
void Array_view<T>::push_back(const_reference x)
{
    if (size_ < mem_size_){
	ptr_[size_] = x;
	++size_;
    }
}

template <typename T>
inline void Array_view<T>::pop_back()
{
    if (size_ > 0) 
	--size_;
}

/***************************************************************************
 *			    ARRAY_OF_BYTES_VIEW
 ***************************************************************************/
// Permite concebir un array de bytes como un array de otro tipo.
//
// Ejemplo: la FAT es un array de uint32_t, pero los sectores son arrays de
// bytes. Después de leer un sector quiero acceder a cada entrada de la FAT.
// Esta view facilita todo el proceso.
template <typename Array_bytes, typename T>
struct Array_of_bytes_view{

    using size_type = size_t;

    Array_of_bytes_view(Array_bytes& s0) : s{s0} {}

    T& operator[](size_type i) 
    { return *(reinterpret_cast<T*>(&s[i * sizeof(T)])); }

    Array_bytes& s;
};


}// namespace


#endif

