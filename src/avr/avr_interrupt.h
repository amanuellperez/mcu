// Copyright (C) 2019-2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
 *    A.Manuel L.Perez
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
 *
 ****************************************************************************/
#include <avr/interrupt.h>
#include <atd_bit.h>


namespace avr_{
/// Habilita el uso de interrupciones. Además, recordar habilitar
/// cada interrupción por separado.
inline void enable_interrupts() {sei();}

/// Deshabilita el uso de interrupciones
inline void disable_interrupts() {cli();}

/*!
 *  \brief  Interfaz para manejar interrupciones
 *
 */
class Interrupt{
public:
    // Tipos de interrupciones que pueden ser la INT0 y la INT1
    enum class Tipo_INT01
    {			// se genera la interrupción cuando:
	when_is_zero	// el pin vale 0
	, when_change	// cambia de 0->1 ó 1->0
	, when_falling_edge // cuando cae de 1->0
	, when_rising_edge  // cuando sube de 0->1
    };


    /// Habilitamos las interrupciones INT0
    /// Recordar definir la ISR:
    ///		ISR(INT0_vect){ <++cuerpo de la función++> }
    /// TODO: no está probada!!! Solo está probada el when_change.
    template<Tipo_INT01 tipo>
    static void enable_INT0();

    // TODO: está será copia casi literal de enable_INT0
    // static void enable_INT1(Tipo_INT01 tipo);

    /// Habilitamos las interrupciones en el pin `num_pin`
    /// Todos los pines pueden generar interrupciones cuando cambia su
    /// valor. Este es el tipo de interrupciones que habilitamos aquí.
    /// Recordar definir la ISR correspondiente. Usar las macros
    /// ISR_PCINT_PINx para ello
    template<uint8_t num_pin>
    static void enable_pin();

    /// Desactivamos las interrupciones en el pin `num_pin`
    template<uint8_t num_pin>
    static void disable_pin();


private:

    static void enable_INT0_comun()
    {
	// Habilitamos las interrupciones INT0
	// TODO: OJO: la datasheet dice que "the 1-bit in de SREG is set", 
	// aunque el libro de MAKE lo ignora. (???) (pag. 97)
	// EIMSK |= (1 << INT0);
	EIMSK = EIMSK | (1 << INT0);
    }
};


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
// ISR de los timers
#define ISR_TIMER0_COMPA ISR(TIMER0_COMPA_vect)
#define ISR_TIMER0_COMPB ISR(TIMER0_COMPB_vect)
#define ISR_TIMER0_OVF   ISR(TIMER0_OVF_vect)

#define ISR_TIMER1_CAPT  ISR(TIMER1_CAPT_vect)
#define ISR_TIMER1_COMPA ISR(TIMER1_COMPA_vect)
#define ISR_TIMER1_COMPB ISR(TIMER1_COMPB_vect)
#define ISR_TIMER1_OVF	 ISR(TIMER1_OVF_vect)

// ISR de SPI
#define ISR_SPI_STC  ISR(SPI_STC_vect)

// ISR de USART
#define ISR_USART_RX	ISR(USART_RX_vect)
#define ISR_USART_UDRE	ISR(USART_UDRE_vect)
#define ISR_USART_TX	ISR(USART_TX_vect)




// Según página 96 de la datasheet
template <>
inline void Interrupt::enable_INT0<Interrupt::Tipo_INT01::when_is_zero>()
{
    atd::write_bits<ISC01, ISC00>::to<0,0>::in(EICRA);

    enable_INT0_comun();
}




template<>
inline void Interrupt::enable_INT0<Interrupt::Tipo_INT01::when_change>()
{
    atd::write_bits<ISC01, ISC00>::to<0,1>::in(EICRA);

    enable_INT0_comun();
}


template<>
inline void Interrupt::enable_INT0<Interrupt::Tipo_INT01::when_falling_edge>()
{
    atd::write_bits<ISC01, ISC00>::to<1,0>::in(EICRA);

    enable_INT0_comun();
}


template<>
inline void Interrupt::enable_INT0<Interrupt::Tipo_INT01::when_rising_edge>()
{
    atd::write_bits<ISC01, ISC00>::to<1,1>::in(EICRA);

    enable_INT0_comun();
}




// TODO: funcionan todos los pines menos el número 1, por eso comento la
// función, para que de error si se intenta usar. 
// Revisar qué pasa 
// (OJO: puede que no funcionen debido a la forma en como conecto
// el programador al avr). A fin de cuenta este es el pin de RESET!!!
//template<>
//inline void Interrupt::enable_pin<1>()
//{
//    atd::write_one_bit<PCIE1>(PCICR);
//    atd::write_one_bit<PCINT14>(PCMSK1);
//
//}


template<>
inline void Interrupt::enable_pin<2>()
{
    atd::write_bit<PCIE2>::to<1>::in(PCICR);
    atd::write_bit<PCINT16>::to<1>::in(PCMSK2);
}

template<>
inline void Interrupt::disable_pin<2>()
{ atd::write_bit<PCINT16>::to<0>::in(PCMSK2); }


template<>
inline void Interrupt::enable_pin<3>()
{
    atd::write_bit<PCIE2>::to<1>::in(PCICR);
    atd::write_bit<PCINT17>::to<1>::in(PCMSK2);
}

template<>
inline void Interrupt::disable_pin<3>()
{ atd::write_bit<PCINT17>::to<0>::in(PCMSK2); }


template<>
inline void Interrupt::enable_pin<4>()
{
    atd::write_bit<PCIE2>::to<1>::in(PCICR);
    atd::write_bit<PCINT18>::to<1>::in(PCMSK2);
}

template<>
inline void Interrupt::disable_pin<4>()
{ atd::write_bit<PCINT18>::to<0>::in(PCMSK2); }


template<>
inline void Interrupt::enable_pin<5>()
{
    atd::write_bit<PCIE2>::to<1>::in(PCICR);
    atd::write_bit<PCINT19>::to<1>::in(PCMSK2);
}

template<>
inline void Interrupt::disable_pin<5>()
{ atd::write_bit<PCINT19>::to<0>::in(PCMSK2); }


template<>
inline void Interrupt::enable_pin<6>()
{
    atd::write_bit<PCIE2>::to<1>::in(PCICR);
    atd::write_bit<PCINT20>::to<1>::in(PCMSK2);
}

template<>
inline void Interrupt::disable_pin<6>()
{ atd::write_bit<PCINT20>::to<0>::in(PCMSK2); }


template<>
inline void Interrupt::enable_pin<9>()
{
    atd::write_bit<PCIE0>::to<1>::in(PCICR);
    atd::write_bit<PCINT6>::to<1>::in(PCMSK0);
}

template<>
inline void Interrupt::disable_pin<9>()
{ atd::write_bit<PCINT6>::to<0>::in(PCMSK0); }


template<>
inline void Interrupt::enable_pin<10>()
{
    atd::write_bit<PCIE0>::to<1>::in(PCICR);
    atd::write_bit<PCINT7>::to<1>::in(PCMSK0);
}

template<>
inline void Interrupt::disable_pin<10>()
{ atd::write_bit<PCINT7>::to<0>::in(PCMSK0); }


template<>
inline void Interrupt::enable_pin<11>()
{
    atd::write_bit<PCIE2>::to<1>::in(PCICR);
    atd::write_bit<PCINT21>::to<1>::in(PCMSK2);
}

template<>
inline void Interrupt::disable_pin<11>()
{ atd::write_bit<PCINT21>::to<0>::in(PCMSK2); }


template<>
inline void Interrupt::enable_pin<12>()
{
    atd::write_bit<PCIE2>::to<1>::in(PCICR);
    atd::write_bit<PCINT22>::to<1>::in(PCMSK2);
}

template<>
inline void Interrupt::disable_pin<12>()
{ atd::write_bit<PCINT22>::to<0>::in(PCMSK2); }


template<>
inline void Interrupt::enable_pin<13>()
{
    atd::write_bit<PCIE2>::to<1>::in(PCICR);
    atd::write_bit<PCINT23>::to<1>::in(PCMSK2);
}

template<>
inline void Interrupt::disable_pin<13>()
{ atd::write_bit<PCINT23>::to<0>::in(PCMSK2); }


template<>
inline void Interrupt::enable_pin<14>()
{
    atd::write_bit<PCIE0>::to<1>::in(PCICR);
    atd::write_bit<PCINT0>::to<1>::in(PCMSK0);
}

template<>
inline void Interrupt::disable_pin<14>()
{ atd::write_bit<PCINT0>::to<0>::in(PCMSK0); }


template<>
inline void Interrupt::enable_pin<15>()
{
    atd::write_bit<PCIE0>::to<1>::in(PCICR);
    atd::write_bit<PCINT1>::to<1>::in(PCMSK0);
}

template<>
inline void Interrupt::disable_pin<15>()
{ atd::write_bit<PCINT1>::to<0>::in(PCMSK0); }


template<>
inline void Interrupt::enable_pin<16>()
{
    atd::write_bit<PCIE0>::to<1>::in(PCICR);
    atd::write_bit<PCINT2>::to<1>::in(PCMSK0);
}

template<>
inline void Interrupt::disable_pin<16>()
{ atd::write_bit<PCINT2>::to<0>::in(PCMSK0); }


template<>
inline void Interrupt::enable_pin<17>()
{
    atd::write_bit<PCIE0>::to<1>::in(PCICR);
    atd::write_bit<PCINT3>::to<1>::in(PCMSK0);
}

template<>
inline void Interrupt::disable_pin<17>()
{ atd::write_bit<PCINT3>::to<0>::in(PCMSK0); }


template<>
inline void Interrupt::enable_pin<18>()
{
    atd::write_bit<PCIE0>::to<1>::in(PCICR);
    atd::write_bit<PCINT4>::to<1>::in(PCMSK0);
}

template<>
inline void Interrupt::disable_pin<18>()
{ atd::write_bit<PCINT4>::to<0>::in(PCMSK0); }


template<>
inline void Interrupt::enable_pin<19>()
{
    atd::write_bit<PCIE0>::to<1>::in(PCICR);
    atd::write_bit<PCINT5>::to<1>::in(PCMSK0);
}

template<>
inline void Interrupt::disable_pin<19>()
{ atd::write_bit<PCINT5>::to<0>::in(PCMSK0); }


template<>
inline void Interrupt::enable_pin<23>()
{
    atd::write_bit<PCIE1>::to<1>::in(PCICR);
    atd::write_bit<PCINT8>::to<1>::in(PCMSK1);
}

template<>
inline void Interrupt::disable_pin<23>()
{ atd::write_bit<PCINT8>::to<0>::in(PCMSK1); }


template<>
inline void Interrupt::enable_pin<24>()
{
    atd::write_bit<PCIE1>::to<1>::in(PCICR);
    atd::write_bit<PCINT9>::to<1>::in(PCMSK1);
}

template<>
inline void Interrupt::disable_pin<24>()
{ atd::write_bit<PCINT9>::to<0>::in(PCMSK1); }


template<>
inline void Interrupt::enable_pin<25>()
{
    atd::write_bit<PCIE1>::to<1>::in(PCICR);
    atd::write_bit<PCINT10>::to<1>::in(PCMSK1);
}

template<>
inline void Interrupt::disable_pin<25>()
{ atd::write_bit<PCINT10>::to<0>::in(PCMSK1); }


template<>
inline void Interrupt::enable_pin<26>()
{
    atd::write_bit<PCIE1>::to<1>::in(PCICR);
    atd::write_bit<PCINT11>::to<1>::in(PCMSK1);
}

template<>
inline void Interrupt::disable_pin<26>()
{ atd::write_bit<PCINT11>::to<0>::in(PCMSK1); }


template<>
inline void Interrupt::enable_pin<27>()
{
    atd::write_bit<PCIE1>::to<1>::in(PCICR);
    atd::write_bit<PCINT12>::to<1>::in(PCMSK1);
}

template<>
inline void Interrupt::disable_pin<27>()
{ atd::write_bit<PCINT12>::to<0>::in(PCMSK1); }


template<>
inline void Interrupt::enable_pin<28>()
{
    atd::write_bit<PCIE1>::to<1>::in(PCICR);
    atd::write_bit<PCINT13>::to<1>::in(PCMSK1);
}

template<>
inline void Interrupt::disable_pin<28>()
{ atd::write_bit<PCINT13>::to<0>::in(PCMSK1); }




/*!
 *  \brief  Clase que bloquea las interrupciones. Usamos RRI.
 *
 *  El constructor bloque las interrupciones, mientras que el destructor
 *  las deja como estaban.
 *
 */
// TODO: esta clase es genérica. Sacarla de aquí, esto es un traductor.
// Ejemplo de bloqueo en la datasheet: ver pag. 153 por ejemplo.
class Interrupts_lock{
public:
    Interrupts_lock()
    {
	sreg_ = SREG;	// Save global interrupt flag
	cli();		// Disable interrupts
    }

    ~Interrupts_lock()
    {
	SREG = sreg_;	// Restore global interrupt flag
    }


private:
    unsigned char sreg_;
};

// DUDA: Interrupts_lock y Disable_interrupt es la misma clase. ¿Qué nombre es
// mejor? Disable_interrupts encaja mejor con Enable_interrupts
class Disable_interrupts{
public:
    Disable_interrupts()
    {
	sreg_ = SREG;	// Save global interrupt flag
	cli();		// Disable interrupts
    }

    ~Disable_interrupts()
    {
	SREG = sreg_;	// Restore global interrupt flag
    }


private:
    unsigned char sreg_;
};

class Enable_interrupts{
public:
    Enable_interrupts()
    {
	sreg_ = SREG;	// Save global interrupt flag
	sei();		// Enable interrupts
    }

    ~Enable_interrupts()
    {
	SREG = sreg_;	// Restore global interrupt flag
    }


private:
    unsigned char sreg_;
};


}// namespace


#endif


