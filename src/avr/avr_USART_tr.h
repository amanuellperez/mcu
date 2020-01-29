// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
#ifndef __AVR_USART_TR_H__
#define __AVR_USART_TR_H__
/****************************************************************************
 *
 * - DESCRIPCION: Traductor del módulo UART del avr.
 *
 * - HISTORIA:
 *   AManuel Lopez Perez
 *	26/07/2017 v0.0 - Escrito.
 *	27/08/2019 v0.1 - Modifico el calculo de UBBR.
 *	13/10/2019 v0.2 - Creo uart_iostream dejando UART como traductor puro.
 *
 ****************************************************************************/
#include <avr/io.h>

#include "avr_cfg.h"	
//#include <atd_register.h>
#include <atd_bit.h>

#include "avr_USART_baud_rate.h"

namespace avr{


/*!
 *  \brief  Representa el puerto UART del avr.
 *
 *  Siempre que se quiera usar, llamar a UART::on().
 *
 *  Clase traductora del interfaz que suministra el UART del avr. Para
 *  escribirla basta con ir leyendo la datasheet e ir dandole un nombre a cada
 *  función.
 *
 *  Como solo
 *  hay un UART todas las funciones son static. No quiero que el usuario 
 *  pueda crear 2 UARTs pensando que está creando 2 UART. El avr solo tiene 1
 *  uart. 
 *
 *  Para facilitar el uso luego suministro un flujo:
 *	    
 *		UART::on();	// idioma: todo dispositivo hay que encenderlo
 *		auto usart = UART::as_a_stream();
 *		usart << "frecuencia = [" << f << "]\n\r";
 *
 */
class UART{
public:
    // Configuración del UART
    // -----------------------
    // Recordar que antes que hacer nada hay que configurar:
    //	    1.- El baud rate
    //	    2.- Mode of operation
    //	    3.- Frame format

    /// Devuelve el valor que existe en el data register.
    static uint8_t data_register() {return UDR0;}

    /// Escribimos un valor en el data register.
    static void data_register(uint8_t x) {UDR0 = x;}


    // -------------------------
    // Gestión de la transmisión
    // -------------------------
    /// Devuelve true si el UART ha recibido algo y está pendiente de ser
    /// leido, y false en caso de que no haya nada que leer. Si devuelve false
    /// y la última operación que realizó el UART fue leer, mantiene en el
    /// data register (UDR0) el último valor leído (hace de get area de 1 byte).
    static bool are_there_data_unread()
    // {return atd::Register{UCSR0A}.is_one_bit<RXC0>();}
    {return atd::is_one_bit<RXC0>::of(UCSR0A);}

    /// Devuelve true si se ha enviado todo el frame
    static bool is_transmit_complete()
//    { return atd::Register{UCSR0A}.is_one_bit<TXC0>(); }
    {return atd::is_one_bit<TXC0>::of(UCSR0A);}

    /// Devuelve true si el UART está ocupado transmitiendo.
    /// Para poder transmitir algo hay que esperar a que deje de transmitir.
    static bool is_ready_to_transmit()
//    {return atd::Register{UCSR0A}.is_one_bit<UDRE0>();}
    {return atd::is_one_bit<UDRE0>::of(UCSR0A);}
    

    // ------------------
    // Gestión de errores
    // ------------------
    /// Returns true if the next character in the receive buffer had
    /// a frame error when received.
    static bool is_there_a_frame_error()
//    {return atd::Register{UCSR0A}.is_one_bit<FE0>(); }
    {return atd::is_one_bit<FE0>::of(UCSR0A);}

    /// Return true if a Data OverRun condition is detected.
    static bool is_there_data_overrun()
//    {return atd::Register{UCSR0A}.is_one_bit<DOR0>(); }
    {return atd::is_one_bit<DOR0>::of(UCSR0A);}

    /// Return true if the next character in the receive buffer had a Parity
    /// Error when received and the Parity Checking was enabled.
    static bool is_there_a_parity_error()
//    {return atd::Register{UCSR0A}.is_one_bit<UPE0>(); }
    {return atd::is_one_bit<UPE0>::of(UCSR0A);}


    // ----------------------
    // Configuración del UART
    // ----------------------
    /// Double the transfer rate of communication
    static void double_transmission_speed()
    //{ atd::Register(UCSR0A).write_one_bit<U2X0> ();}
    { atd::write_bit<U2X0>::to<1>::in(UCSR0A);}

    /// Usamos la velocidad normal de transmisión
    // DUDA: en la datasheet pone que siempre que se escriba en UCSR0A
    // hay que escribir ciertos bits a 0 (vg: el bit DOR0 y otros) ???
    static void normal_transmission_speed()
    //{ atd::Register(UCSR0A).write_zero_bit<U2X0> ();}
    { atd::write_bit<U2X0>::to<0>::in(UCSR0A);}


    /// Enable multi-processor communication mode.
    static void multiprocessor_communication_mode()
    //{ atd::Register(UCSR0A).write_one_bit<MPCM0> ();}
    { atd::write_bit<MPCM0>::to<1>::in(UCSR0A);}


    // TODO: faltan los bits de interrupciones: 
    // RXCIE0
    // TXCIE0
    // UDRIE0
    // y el 9 bit (con frames de 9 bits)
    // TXB80 y RXB80

    /// Enable receiver.
    static void enable_receiver()
    // {atd::Register(UCSR0B).write_one_bit<RXEN0> ();}
    { atd::write_bit<RXEN0>::to<1>::in(UCSR0B);}

    /// Disable receiver.
    static void disable_receiver()
    // {atd::Register(UCSR0B).write_zero_bit<RXEN0> ();}
    { atd::write_bit<RXEN0>::to<0>::in(UCSR0B);}

    /// ¿Is receiver enable?
    static bool is_receiver_enable()
    // {return atd::Register{UCSR0B}.is_one_bit<RXEN0>();}
    {return atd::is_one_bit<RXEN0>::of(UCSR0B);}

    /// Enable transmitter
    static void enable_transmitter()
    // {atd::Register(UCSR0B).write_one_bit<TXEN0> ();}
    { atd::write_bit<TXEN0>::to<1>::in(UCSR0B);}

    /// Disable transmitter
    static void disable_transmitter()
    //{atd::Register(UCSR0B).write_zero_bit<TXEN0> ();}
    { atd::write_bit<TXEN0>::to<0>::in(UCSR0B);}

    /// ¿Is transmitter enable?
    static bool is_transmitter_enable()
//    {return atd::Register{UCSR0B}.is_one_bit<TXEN0>();}
    {return atd::is_one_bit<TXEN0>::of(UCSR0B);}


    /// Parity mode disabled
    static void parity_mode_disabled()
    {// mode 00
//	atd::Register(UCSR0C).write_zero_bit<UPM01, UPM00>();
	atd::write_bits<UPM01, UPM00>::to<0,0>::in(UCSR0C);
    }


    /// Parity mode enabled, even parity
    static void parity_mode_enabled_even_parity()
    {// mode 10
//	atd::Register(UCSR0C).write_one_bit <UPM01>();
//	atd::Register(UCSR0C).write_zero_bit<UPM00>();
	atd::write_bits<UPM01, UPM00>::to<1,0>::in(UCSR0C);
    }


    /// Parity mode enabled, odd parity
    static void parity_mode_enabled_odd_parity()
    {// mode 11
//	atd::Register(UCSR0C).write_one_bit <UPM01, UPM00>();
	atd::write_bits<UPM01, UPM00>::to<1,1>::in(UCSR0C);
    }
    
    /// One stop bit inserted by the transmitter. The receiver ignores
    /// this setting.
    static void one_stop_bit()
    // { atd::Register(UCSR0C).write_zero_bit <USBS0>(); }
    { atd::write_bit<USBS0>::to<0>::in(UCSR0C);}


    /// Two stop bit inserted by the transmitter. The receiver ignores
    /// this setting.
    static void two_stop_bit()
    // { atd::Register(UCSR0C).write_zero_bit <USBS0>(); } <-- ERROR!!!
    { atd::write_bit<USBS0>::to<1>::in(UCSR0C);}


    // Definimos el character size
    static void character_size_5()
    {// 000
//	atd::Register(UCSR0B).write_zero_bit<UCSZ02>();
//	atd::Register(UCSR0C).write_zero_bit<UCSZ01, UCSZ00>();
        atd::write_bit <UCSZ02>	       ::to<0>  ::in(UCSR0B);
        atd::write_bits<UCSZ01, UCSZ00>::to<0,0>::in(UCSR0C);
    }

    static void character_size_6()
    {// 001
//	atd::Register(UCSR0B).write_zero_bit<UCSZ02>();
//	atd::Register(UCSR0C).write_zero_bit<UCSZ01>();
//	atd::Register(UCSR0C).write_one_bit <UCSZ00>();
        atd::write_bit <UCSZ02>	       ::to<0>  ::in(UCSR0B);
        atd::write_bits<UCSZ01, UCSZ00>::to<0,1>::in(UCSR0C);
    }

    static void character_size_7()
    {// 010
//	atd::Register(UCSR0B).write_zero_bit<UCSZ02>();
//	atd::Register(UCSR0C).write_one_bit<UCSZ01>();
//	atd::Register(UCSR0C).write_zero_bit <UCSZ00>();
        atd::write_bit <UCSZ02>	       ::to<0>  ::in(UCSR0B);
        atd::write_bits<UCSZ01, UCSZ00>::to<1,0>::in(UCSR0C);
    }


    static void character_size_8()
    {// 011
//	atd::Register(UCSR0B).write_zero_bit<UCSZ02>();
//	atd::Register(UCSR0C).write_one_bit<UCSZ01, UCSZ00>();
        atd::write_bit <UCSZ02>	       ::to<0>  ::in(UCSR0B);
        atd::write_bits<UCSZ01, UCSZ00>::to<1,1>::in(UCSR0C);
    }

    static void character_size_9()
    {// 111
	// atd::Register(UCSR0B).write_one_bit<UCSZ02>();
	// atd::Register(UCSR0C).write_one_bit<UCSZ01, UCSZ00>();
        atd::write_bit <UCSZ02>	       ::to<1>  ::in(UCSR0B);
        atd::write_bits<UCSZ01, UCSZ00>::to<1,1>::in(UCSR0C);
    }


    static void asynchronous_mode() // modo 00
//    {atd::Register(UCSR0C).write_zero_bit<UMSEL01, UMSEL00>();}
    {atd::write_bits<UMSEL01, UMSEL00>::to<0, 0>::in(UCSR0C);}

    /// Definimos la velocidad a la que queremos transmitir datos.
    template <uint32_t f_clock, uint32_t baud_rate, uint32_t tolerance = 2>
    static void baud_speed();
};



template <uint32_t f_clock, uint32_t baud_rate, uint32_t tolerance>
void UART::baud_speed()
{
    auto [mode, ubbr] = UBBRn<f_clock, baud_rate, tolerance>();

    UBRR0H = (ubbr >> 8);
    UBRR0L = (ubbr & 0xFF);

    if (mode == UART_mode::normal)
	normal_transmission_speed();
    else
	double_transmission_speed();
}

}// namespace


#endif

