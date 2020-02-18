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
 *    18/02/2020: ixtream_of_bytes
 *
 ****************************************************************************/
#include <array>
#include <algorithm>
#include <cstddef>  // byte

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
 *  \brief  Análogo a std::stringstream pero para bytes.
 *
 *  La diferencia con std::stringstream es:
 *	1.- Son bytes, no caracteres. No los procesamos de ninguna forma.
 *	2.- Es un flujo de tamaño finito (es para los microcontroladores, 
 *	    no quiero usar memoria dinámica).
 *  
 *  Posibles nombres:
 *	+ bstream: suena bien, pero suena a flujo normal, no es de tamaño
 *	    fijo.
 *
 *	+ bfstream: la 'f' es de fix. Problema: suena a std::fstream, sería
 *	    confuso.
 *
 *	+ bfixstream: queda claro que es fijo.
 *	+ bxtream: la 'x' significa 'fix'. Los 'xtream' serían stream
 *	    finitos. De hecho procede de 'fiXsTREAM'.
 *
 *	+ byte_fixstream: nombre completo.
 *
 *  De estos de momento me quedo con xtream para indicar streams de tamaño 
 *  finitos. 
 *
 *  Posibles nombres para bytes:
 *
 *	+ ibytextream/obytextream/iobytextream: como std::stringstream
 *	+ ibyte_xtream/obyte_xstream/iobyte_xtream: mismo pero más claro.
 *	+ ibxtream/obxtream/iobxtream: abreviado.
 *	+ byte_ixtream/byte_oxtream/byte_ioxtream: más claro.
 *	+ ixtream_of_bytes/oxtream_of_bytes/ioxtream_of_bytes: largo, pero claro.
 *
 *  Me gusta el último. Es un poco largo pero claro. Si resulta muy largo
 *  siempre se pueden crear sinónimos en el futuro.
 */
// DUDA: uint8_t or int??? 
// El avr es de 8 bits, siendo más eficiente manejar uint8_t que ints (???)
// Sin embargo los unsigned generan muchos dolores de cabeza. De momento lo
// dejo como uint8_t. Pasarlo a int sería ampliar funcionalidad, no dando
// problemas.
template <uint8_t buffer_size>
class ioxtream_of_bytes{
public:

// Types
    using size_type = uint8_t;

// Construction
    ioxtream_of_bytes() : q0_{p0_()}, qe_{p0_()} { }


    // Tamaño del contenedor.
    constexpr size_type capacity() const {return buffer_size;}

    // ¿No hay datos?
    constexpr bool empty() const {return size() == 0;}

    // size = número de bytes almacenados en el flujo.
    constexpr size_type size() const {return qe_ - q0_;}

// Escritura
    template <uint8_t sz2>
    friend ioxtream_of_bytes<sz2>& operator<<(ioxtream_of_bytes<sz2>& out, char c)
    {
	// TODO: ¿le pongo un centinela? Ponerle estado? Quiero que sea lo más
	// pequeña posible!!!
	*(out.qe_) = std::byte{c};

	if (out.qe_ != out.pe_())
	    ++out.qe_; 

	return out;
    }
    

// Acceso como stream
    template <uint8_t sz2>
    friend ioxtream_of_bytes<sz2>& operator>>(ioxtream_of_bytes<sz2>& in, char& c)
    {
	c = std::to_integer<char>(*(in.q0_));
	++in.q0_;

	return in;
    }

private:
    // buffer_ = [p0, pe)
    // Los datos son [q0, pe), siendo *q0 el siguiente byte a leer.
    std::array<std::byte, buffer_size> buffer_;

    // Oculto la implementación, porque hay varias formas de hacerlo. 
    // De momento elijo la más sencilla de implementar para hacer pruebas.
    std::byte* q0_; // Datos: [q0, qe)
    std::byte* qe_; 

    constexpr std::byte* p0_() {return buffer_.begin();}
    constexpr std::byte* pe_() {return buffer_.end();}

};




}// namespace

#endif

