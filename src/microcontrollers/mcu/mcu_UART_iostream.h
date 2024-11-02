// Copyright (C) 2019-2024 Manuel Perez 
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

#ifndef __MCU_UART_IOSTREAM_H__
#define __MCU_UART_IOSTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Implementación del flujo uart.
 *  
 *  TODO: el flujo da por supuesto que la configuración es la definida en
 *  UART_basic_cfg (sobre todo que el character size es de 8 bits, con 9 bits
 *  ahora no funcionaría).
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	07/10/2019 v0.0
 *	25/10/2019 '\n' --> se transforma en "\r\n". Es mucho más cómodo.
 *	18/09/2023 is_there_something_to_read
 *	09/01/2024 Deshago el cambio del 25/10/2019. Necesitaba escribir
 *	           el \r porque estaba usando el terminal `screen`. 
 *	           El problema de haber escrito ese cambio en `streambuf` es
 *	           que estaba procesando los caracteres NO funcionando la
 *	           función `put` para escribir caracteres, y no pudiendo
 *	           escribir en `uart` valores en formato binario (por ejemplo,
 *	           no podía enviar una imagen desde el micro hasta el
 *	           ordenador usando `put`). Era un error generado por usar el
 *	           terminal screen.
 *	           Ahora tengo el terminal `myterm` que no tiene ese error.
 *	           Por ello deshago el cambio dejándolo bien (?) hecho.
 *	           Problema: ya no puedo usar el terminal `screen` sino que
 *	           estoy obligado a usar `myterm` para que funcione todo. Eso
 *	           me obligará a ir mejorando poco a poco ese terminal.
 *
 *	01/11/2024 Generalizado: lo parametrizo por el traductor de USART para 
 *		   poderlo usar con cualquier dispositivo UART.
 *
 *
 ****************************************************************************/
#include <iostream>
#include <streambuf>
#include <atd_ascii.h>

namespace mcu{

/***************************************************************************
 *			UART_streambuf_unbuffered
 ***************************************************************************/
// (RRR) ¿por qué UART_streambuf_unbuffered() no llama a UART_8bits::init()?
//       En la práctica configuro el UART y luego en cada función creo un
//       objeto UART_iostream. Si llamo a init en el constructor cada vez que
//       creo el objeto UART_iostream estaría llamando a init, cosa
//       innecesaria (introduciendo ineficiencias gratuitas).
template <typename UART_8bits>
class UART_streambuf_unbuffered : public std::streambuf {
public:
    using UART = UART_8bits;

    UART_streambuf_unbuffered() : state_{State::consumido} 
    { }

    // TODO: pensarlo. ¿Pasar la configuración del UART? Sí.
    // Pero el atmega4809 puede definir la frecuencia del reloj de forma
    // dinámica. Hasta no tener definido cómo operar con el reloj, no
    // implementar esto (???)
//    // Configuración del UART_streambuf_unbuffered
//    static void init() { UART::init(); }

    // Enciende el UART. Antes de encenderlo recordar haberlo configurado.
    static void turn_on();

    // Enciende el UART. Antes de encenderlo recordar haberlo configurado.
    static void turn_off();

private:
    // TODO: ¿cómo traducir esto al inglés? consumed, not_consumed?
    // Me suena raro.
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
    virtual int sync() override 
    { 
	return UART::flush(10); // TODO: parametrizar
			       // a 9600 baudios en menos de 1 milisegundo
			       // a enviado un bytee, 10 milisegundos es
			       // excesivo, pero al añadir esta función sin
			       // timeout me dejaron de funcionar los test
			       // (si se define uart como vble global
			       // funcionaba, como vble local, no)
//	return 0; 
    }
    
    // Get area
    // --------
    // Obtains the number of characters available for input in the associated
    // input sequence if known.
    virtual std::streamsize showmanyc() override {return 0;}


    // Reads max N characters from the input sequence and stores them into s.
    // The characters are read as if by repeated call of sbumpc.
    // Returns: the number of characters assigned to s.
    virtual std::streamsize xsgetn(char_type* s, std::streamsize N) override;


    // Ensures that at least one character is avaible in the input area by
    // updating the pointers to the input area and reading more data in from
    // the input sequence. Returns the value of that character on success or
    // traits_type::eof() on failure.
    // (Esta función no consume el caracter)
    virtual int_type underflow() override;


    // Idéntica a underflow salvo que esta función avanza un caracter el valor
    // de gptr() (consume un caracter).
    virtual int_type uflow() override;


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

	put_(c);

	return 1;
    }

    // Writes up to n characters to the output sequence as if by repeated
    // calls to sputc(c).
    // Returns: the number of characters written.
    virtual std::streamsize xsputn(const char_type* s, std::streamsize n) override;


// Helper functions
// ----------------
    void put_(char_type c);
    void put_unguarded(char_type c);

    int_type receive_byte();

};


template <typename U>
inline void UART_streambuf_unbuffered<U>::turn_on()	
{                                
    if (!UART::is_receiver_enable())
	UART::enable_receiver();

    if (!UART::is_transmitter_enable())
	UART::enable_transmitter();
}

template <typename U>
inline void UART_streambuf_unbuffered<U>::turn_off()	
{                                
    if (UART::is_receiver_enable())
	UART::disable_receiver();

    if (UART::is_transmitter_enable())
	UART::disable_transmitter();
}


// (RRR) ¿por qué `inline` todas estas funciones?
//	 1. Al principio como esto era un experimento las deje dentro de la
//	    clase (por pereza, por no tener que escribir el .cpp)
//       2. Acabo de llevarlas a un .cpp esperando que el programa 
//          test/UART/iostream disminuyera en tamaño y en vez de disminuir a
//          aumentado!!! Ha pasado el .text de 12024 a 12052, así que la
//          primera impresión es que no sirve para mucho el .cpp en este caso
//          (salvo por supuesto, modularidad)
//       3. Mi intención es generalizar esta clase y pasarle como parámetro de
//          template el dispositivo real al que se conecta, en cuyo caso hay
//          que escribir todo el código aquí en el .h. Si lo muevo ahora a un
//          .cpp me toca cambiar todos los makefiles, y cuando lo generalice
//          lo volveré a mover a un .h teniendo que cambiarlo de nuevo todo.
//      Por estos motivos implemento estas funciones en el .h y no en el .cpp
//      (teniendo que ser inline)
template <typename U>
inline 
std::streamsize UART_streambuf_unbuffered<U>::xsgetn(char_type* s, std::streamsize N) 
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

template <typename U>
inline 
std::streambuf::int_type UART_streambuf_unbuffered<U>::underflow() 
{
    if (state_ == State::consumido)
	buf_ = receive_byte();

    state_ = State::no_consumido;    
    return buf_;
}

template <typename U>
inline 
std::streambuf::int_type UART_streambuf_unbuffered<U>::uflow()
{
    if (state_ == State::consumido)
	buf_ = receive_byte();

    state_ = State::consumido;

    return buf_;
}


template <typename U>
inline
std::streamsize UART_streambuf_unbuffered<U>::xsputn(const char_type* s, std::streamsize n)
{ 
    state_ = State::consumido;

    for (int i = 0; i < n; ++i)
	put_(s[i]);

    return n;
}


template <typename U>
inline
void UART_streambuf_unbuffered<U>::put_(char_type c)
{ put_unguarded(c); }


template <typename U>
inline 
void UART_streambuf_unbuffered<U>::put_unguarded(char_type c)
{
    // TODO: aqui se puede poner un contador y esperar un tiempo máximo?
    // TODO: Sí que se puede hacer. Una forma es parametrizar con el
    // max_timeout_ms a esperar. De esa forma el hardwador definirá algo
    // del tipo: `using UART_streambuf_unbuffered = mcu::UART_streambuf_unbuffered<1000>;` siendo
    // el `1000` los milisegundos que se quiere máximo esperar a que
    // responda esta función. 
    // La implementación de esta función será:
    //  uint16_t timeout = 0;
    //  while (!UART::is_transmit_complete()){
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

    UART::transmit_data_register(c);
}

template <typename U>
inline 
std::streambuf::int_type UART_streambuf_unbuffered<U>::receive_byte()
{
    while(!UART::are_there_unread_data()) 
	; 

    int_type d = static_cast<char_type>(UART::receive_data_register());

    if (static_cast<char_type>(d) == atd::ASCII::EOT)
	return traits_type::eof();

    return d;
}


/***************************************************************************
 *			    UART_iostream
 ***************************************************************************/
// TODO: crear un istream/ostream (uno enable rx, el otro tx)
template <typename UART_8bits>
class UART_iostream : public std::iostream {
public:
    UART_iostream():iostream{&sb_} { }

    /// Enciende el UART en caso de que estuviera apagado.
    /// Recordar haber configurado el UART antes de llamar a esta función.
    void turn_on() { sb_.turn_on(); }

    void turn_off() { sb_.turn_off(); }


    // TODO: nombre???
    // Mira, SIN BLOQUEAR, si hay caracteres a leer.
    // ¿Cómo hace el standard para ver si hay caracteres pendientes de leer?
    // No he encontrado la funcion correspondiente. ¿No existe? @_@ Todas las
    // funciones del standard bloquean el flujo de caracteres. No quiero que
    // lo bloquee, solo mirar.
    // La necesito sistemáticamente.
    bool is_there_something_to_read() 
    { return UART_8bits::are_there_unread_data();}

// Interrupts
    static void enable_interrupt_unread_data()
    { UART_8bits::enable_interrupt_unread_data(); }

    static void disable_interrupt_unread_data()
    { UART_8bits::disable_interrupt_unread_data(); }

private:
    UART_streambuf_unbuffered<UART_8bits> sb_;

};



}// namespace



#endif


