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

#ifndef __AVR_UART_IOSTREAM_H__
#define __AVR_UART_IOSTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Implementación del flujo uart.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	07/10/2019 v0.0
 *	25/10/2019 '\n' --> se transforma en "\r\n". Es mucho más cómodo.
 *
 ****************************************************************************/

#include "avr_UART_basic.h"
#include "avr_cfg.h"	// MCU_CLOCK_FREQUENCY_IN_HZ

#include <iostream>
#include <streambuf>
#include <atd_ascii.h>

namespace avr_{

class UART_streambuf_unbuffered : public std::streambuf {
public:
    using UART = UART_basic;

    UART_streambuf_unbuffered() : state_{State::consumido} 
    { UART::asynchronous_mode(); } // es UART, no USART, siempre asincrono.

    // Enciende el UART. Antes de encenderlo recordar haberlo configurado.
    static void on();

    // Enciende el UART. Antes de encenderlo recordar haberlo configurado.
    static void off();

private:
    enum class State{
	consumido,   // la ultima operación fue o de escritura o de lectura
		    // consumiendo el caracter.
	no_consumido // última operación realizada de lectura. En el data 
		    // register está garantizado que está el último byte leído
    };



    // Datos
    // -----
    State state_;

    // Voy a definir una get area (= buf_) para garantizar que la
    // implementación de uflow/underflow funcionen correctamente.
    // Si no defino este buf_ no puedo trazarlo (lo trazo escribiendo en UART,
    // lo cual invalida UDR0, y por tanto no puedo probar las funciones get).
    // Aunque una vez probado podría eliminar este buf_ prefiero dejarlo, solo
    // es 1 byte y si hay que depurar en el futuro no tendré que descubrir que
    // no puedo depurarlo sin buf_.
    int_type buf_;


    // Funciones virtuales: como el streambuf está pensado para que el flujo
    // tenga un buffer, voy a redefinir todas las funciones virtuales,
    // garantizando de esta forma que no se intente acceder al buffer nunca.
    
    // Buffer management
    // -----------------
    // Synchronizes the controlled sequences with the arrays (vamos, es
    // flush()). That is, if pbase() is non-null the characters [pbase(),
    // pptr()) are writen to the controlled sequence. The pointers may be
    // reset as appropiate.
    //
    // Returns: -1 on failure (cada clase derivada determinará qué considera
    // fallo).
    virtual int sync() override { return 0; }
    
    // Get area
    // --------
    // Obtains the number of characters available for input in the associated
    // input sequence if known.
    virtual std::streamsize showmanyc() override {return 0;}


    // Reads max N characters from the input sequence and stores them into s.
    // The characters are read as if by repeated call of sbumpc.
    // Returns: the number of characters assigned to s.
    virtual std::streamsize xsgetn(char_type* s, std::streamsize N) override
    {
	std::streamsize i = 0;
	for (; i < N; ++i){
	    int_type d = receive_byte();

	    if (!is_eof(d))
		s[i] = traits_type::to_char_type(d);

	    else
		return i;
	}

	state_ = State::consumido;

	return i;
    }


    // Ensures that at least one character is avaible in the input area by
    // updating the pointers to the input area and reading more data in from
    // the input sequence. Returns the value of that character on success or
    // traits_type::eof() on failure.
    // (Esta función no consume el caracter)
    virtual int_type underflow() override
    {
	if (state_ == State::consumido)
	    buf_ = receive_byte();

	state_ = State::no_consumido;    
	return buf_;
    }


    // Idéntica a underflow salvo que esta función avanza un caracter el valor
    // de gptr() (consume un caracter).
    virtual int_type uflow() override
    {
	if (state_ == State::consumido)
	    buf_ = receive_byte();

	state_ = State::consumido;

	return buf_;
    }


    // Putback
    // -------
    // Puts a character back into the input sequence, posibly modifying the 
    // input sequence.
    virtual int_type pbackfail(int_type c = traits_type::eof()) override
    {return traits_type::eof();}


    // Put area
    // --------
    // Consumes some initial subsequence of the characters of the pending
    // sequence.
    // Returns: traits::eof() if the function fails, or some other value to
    // indicate success (typically returns c to indicate success, salvo cuando
    // c == traits::eof() en cuyo caso devuelve traits::not_eof(c)).
    virtual int_type overflow(int_type c = traits_type::eof()) override
    { 
	state_ = State::consumido;

	put(c);

	return 1;
    }

    // Writes up to n characters to the output sequence as if by repeated
    // calls to sputc(c).
    // Returns: the number of characters written.
    virtual std::streamsize xsputn(const char_type* s, std::streamsize n) override
    { 
	state_ = State::consumido;

	for (int i = 0; i < n; ++i)
	    put(s[i]);

	return n;
    }


    // Funciones ayuda
    // ---------------
    void put(char_type c)
    {
	if (c == '\r')
	    return;

	if (c == '\n')	// retorno de carro = \n\r
	    put_unguarded('\r');

	put_unguarded(c);
    }

    void put_unguarded(char_type c)
    {
	// TODO: aqui se puede poner un contador y esperar un tiempo máximo?
	// TODO: Sí que se puede hacer. Una forma es parametrizar con el
	// max_timeout_ms a esperar. De esa forma el hardwador definirá algo
	// del tipo: `using UART_iostream = mcu::UART_iostream<1000>;` siendo
	// el `1000` los milisegundos que se quiere máximo esperar a que
	// responda esta función. 
	// La implementación de esta función será:
	//  uint16_t timeout = 0;
	//  while (!UART::is_ready_to_transmit()){
	//	++timeout;
	//	if (timeout >= max_timeout_ms){
	//	    state = Statetime_out;
	//	    return;
	//	}
	//  }
	//
	//  De esta forma evitamos bloquear el programa en caso de que falle
	//  UART.
	while(!UART::is_ready_to_transmit()) // wait_mientras_esta_transmitiendo();
	    ;   

	UART::data_register(c);
    }

    int_type receive_byte()
    {
	while(!UART::are_there_data_unread()) 
	    ; 

	int_type d = static_cast<char_type>(UART::data_register());

	if (static_cast<char_type>(d) == atd::ASCII::EOT)
	    return traits_type::eof();

	return d;
    }

};


inline void UART_streambuf_unbuffered::on()	
{                                
    if (!UART::is_receiver_enable())
	UART::enable_receiver();

    if (!UART::is_transmitter_enable())
	UART::enable_transmitter();
}

inline void UART_streambuf_unbuffered::off()	
{                                
    if (UART::is_receiver_enable())
	UART::disable_receiver();

    if (UART::is_transmitter_enable())
	UART::disable_transmitter();
}



// TODO: crear un istream/ostream (uno enable rx, el otro tx)
class UART_iostream : public std::iostream {
public:
    UART_iostream():iostream{&sb_} { }

    /// Enciende el UART en caso de que estuviera apagado.
    /// Recordar haber configurado el UART antes de llamar a esta función.
    void on() { sb_.on(); }

    void off() { sb_.off(); }


private:
    UART_streambuf_unbuffered sb_;

};

template <uint32_t baud_rate = 9600u,
	  uint32_t f_clock   = MCU_CLOCK_FREQUENCY_IN_HZ,
	  uint32_t tolerance = 2>
void UART_iostream_basic_cfg()
{                                
    using UART = UART_basic;

    UART::baud_speed<f_clock, baud_rate, tolerance>();

    UART::parity_mode_disabled();

    UART::one_stop_bit();
    UART::character_size_8();
}



// El parámetro lo usamos para sobrecargar: quiero que configures el flujo
// con la configuración básica.
template <uint32_t baud_rate = 9600u,
	  uint32_t f_clock   = MCU_CLOCK_FREQUENCY_IN_HZ,
	  uint32_t tolerance = 2>
inline void basic_cfg(UART_iostream&) 
{ UART_iostream_basic_cfg<baud_rate, f_clock, tolerance>();}


}// namespace



#endif


