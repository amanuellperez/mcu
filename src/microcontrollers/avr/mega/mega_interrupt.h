// Copyright (C) 2019-2021 Manuel Perez 
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

#ifndef __AVR_INTERRUPT_H__
#define __AVR_INTERRUPT_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para manejar interrupciones
 *
 *   - COMENTARIOS: Las interrupciones están definidas a la hora de definir 
 *	la arquitectura. Luego las conocemos en tiempo de compilación. Es más
 *	eficiente usar templates que funciones (y si está bien hecho, podemos
 *	detectar errores en tiempo de compilación).
 *  
 *
 *   - HISTORIA:
 *    Manuel Perez
 *    27/07/2017 Pruebas iniciales.
 *    22/01/2019 La migro a templates y defino macros.
 *    06/11/2022 Quito la llamada de enable_all_interrupts(). Esto no
 *	         pertenece al traductor. Si usas el AVR tienes que saber que
 *	         tienes que llamar a enable_all_interrupts().
 *
 *	         disable_pin
 *
 *    03/12/2022 Rename: enable_all_interrupts -> enable_interrupts (es más
 *		         corto y se entiende bien)
 *
 *		 Enable_interrupt
 *    12/12/2022 Enable_interrupts/Disable_interrupts a dev_interrupt.h
 *    07/09/2023 Vuelvo a meter Enable_interrupts/Disable_interrupts en avr_.
 *
 ****************************************************************************/
#include <avr/interrupt.h>
#include <atd_bit.h>


namespace mega_{
/***************************************************************************
 *			    INTERRUPTS
 ***************************************************************************/
/// Habilita el uso de interrupciones. Además, recordar habilitar
/// cada interrupción por separado.
inline void enable_interrupts() {sei();}

/// Deshabilita el uso de interrupciones
inline void disable_interrupts() {cli();}


class Disable_interrupts{
public:
    Disable_interrupts()
    {
	sreg_ = SREG;
	disable_interrupts();
    }

    ~Disable_interrupts()
    { SREG = sreg_; }


private:
    unsigned char sreg_;
};


class Enable_interrupts{
public:
    Enable_interrupts()
    {
	sreg_ = SREG;
	enable_interrupts();
    }

    ~Enable_interrupts()
    { SREG = sreg_; }


private:
    unsigned char sreg_;
};


/***************************************************************************
 *				ISR
 ***************************************************************************/
/// Recordar definir la ISR correspondiente:
// Esto es posible que sea específico del atmega328p y algún otro? 
//
// Respuesta: No. Definir una interrupt_generic o algo asi? o mejor 
//	      mcu::interrupt?
//
// Antiguo: Si es así, hay que meterlo en el archivo de cabecera del atmega. 
//	De hecho eso parece lo lógico ya que mapeo número de pins.
//		ISR(PCINT0_vect) para pines 9, 10, 14, 15, 16, 17, 18, 19
//		ISR(PCINT1_vect) para pines 1, 23, 24, 25, 26, 27, 28
//		ISR(PCINT2_vect) para pines 2,  3,  4,  5,  6, 11, 12, 13
//	Defino todas estas macros para no tener que recordar este mapeo. 
//	Es más natural usar el número de pin.
//
// Cuidado al usar las interrupciones: si se usa USART no se pueden usar los
// pines 2 y 3.
//
// TODO: Esto no funciona bien si se generan interrupciones en un mismo
// bloque. Lo ideal sería poder definir las funciones callback que llamar en
// caso de generar una interrupción. Algo del tipo:
//	callback_interrupt_pin<2>(f2);
//	callback_interrupt_pin<3>(f3);
//
// Y que automáticamente el compilador gestione la generación del código
// correspondiente. La ISR podría ser algo del tipo:
//	ISR(???)
//	   if constexpr (requires f2){
//		if (pin2_change_from_0_to_1)
//			do_01
//		else
//			do_10
//	   }
//	   y lo mismo para el resto de pines que pertenezcan a ese bloque.
//  Pero ¿cómo implementar esto?
//	    
//
//
#define ISR_INT0	ISR(INT0_vect)
#define ISR_INT1	ISR(INT1_vect)

#define ISR_PCINT_PIN1	ISR(PCINT1_vect)
#define ISR_PCINT_PIN2	ISR(PCINT2_vect)
#define ISR_PCINT_PIN3	ISR(PCINT2_vect)
#define ISR_PCINT_PIN4	ISR(PCINT2_vect)
#define ISR_PCINT_PIN5	ISR(PCINT2_vect)
#define ISR_PCINT_PIN6	ISR(PCINT2_vect)
// VCC: #define ISR_PCINT_PIN7	ISR(PCINT2_vect)
// GND: #define ISR_PCINT_PIN8	ISR(PCINT2_vect)
#define ISR_PCINT_PIN9	ISR(PCINT0_vect)
#define ISR_PCINT_PIN10	ISR(PCINT0_vect)
#define ISR_PCINT_PIN11 ISR(PCINT2_vect)
#define ISR_PCINT_PIN12	ISR(PCINT2_vect)
#define ISR_PCINT_PIN13	ISR(PCINT2_vect)
#define ISR_PCINT_PIN14	ISR(PCINT0_vect)
#define ISR_PCINT_PIN15	ISR(PCINT0_vect)
#define ISR_PCINT_PIN16	ISR(PCINT0_vect)
#define ISR_PCINT_PIN17	ISR(PCINT0_vect)
#define ISR_PCINT_PIN18	ISR(PCINT0_vect)
#define ISR_PCINT_PIN19	ISR(PCINT0_vect)
// AVCC: #define ISR_PCINT_PIN20	ISR(PCINT2_vect)
// AREF: #define ISR_PCINT_PIN21	ISR(PCINT2_vect)
// GND:  #define ISR_PCINT_PIN22	ISR(PCINT2_vect)
#define ISR_PCINT_PIN23	ISR(PCINT1_vect)
#define ISR_PCINT_PIN24	ISR(PCINT1_vect)
#define ISR_PCINT_PIN25	ISR(PCINT1_vect)
#define ISR_PCINT_PIN26	ISR(PCINT1_vect)
#define ISR_PCINT_PIN27	ISR(PCINT1_vect)
#define ISR_PCINT_PIN28	ISR(PCINT1_vect)

// Voy a usar los nombres dados a las interrupciones en la datasheet (tabla
// 16.1). gcc les añade el _vect (???)
// ISR del watchdog
#define ISR_WDT		ISR(WDT_vect)

// ISR de los timers
#define ISR_TIMER0_COMPA ISR(TIMER0_COMPA_vect)
#define ISR_TIMER0_COMPB ISR(TIMER0_COMPB_vect)
#define ISR_TIMER0_OVF   ISR(TIMER0_OVF_vect)

#define ISR_TIMER1_CAPT  ISR(TIMER1_CAPT_vect)
#define ISR_TIMER1_COMPA ISR(TIMER1_COMPA_vect)
#define ISR_TIMER1_COMPB ISR(TIMER1_COMPB_vect)
#define ISR_TIMER1_OVF	 ISR(TIMER1_OVF_vect)

#define ISR_TIMER2_COMPA ISR(TIMER2_COMPA_vect)
#define ISR_TIMER2_COMPB ISR(TIMER2_COMPB_vect)
#define ISR_TIMER2_OVF   ISR(TIMER2_OVF_vect)

// ISR de SPI
#define ISR_SPI_STC  ISR(SPI_STC_vect)

// ISR de USART
#define ISR_USART_RX	ISR(USART_RX_vect)
#define ISR_USART_UDRE	ISR(USART_UDRE_vect)
#define ISR_USART_TX	ISR(USART_TX_vect)

// TWI
#define ISR_TWI		ISR(TWI_vect)

}// namespace


#endif


