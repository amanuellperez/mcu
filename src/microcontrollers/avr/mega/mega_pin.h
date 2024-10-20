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

#ifndef __MEGA_PIN_H__
#define __MEGA_PIN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Clases para manejar pines del AVR
 *
 * DUDA
 *	¿Merecen la pena Output_pin/Input_pin? 
 *	En la práctica lo que estoy haciendo es:
 *	    1) Usar el interfaz static. Con lo que puedo eliminar el
 *	       constructor.
 *
 *	    2) Llamar explícitamente a Pin::as_output() or
 *	       Pin::as_input_with_pullup() en el init() del driver.
 *
 *	¿Eliminar esas clases? (???)
 *
 * HISTORIA
 *    Manuel Perez
 *      24/07/2017 Escrito
 *	27/01/2019 Reescrito, paso a usar templates.
 *	09/06/2019 Output_pin<0> y pulso_1us. 
 *	05/01/2020 Migrado a inglés.
 *	06/11/2022 toggle
 *	10/12/2022 number: lo necesito para depurar. Puedo poner cosas del
 *			     tipo "conectar osciloscopio al pin " <<
 *			     pin.number;
 *	03/09/2023 Introduzco la gestión de interrupciones de los pines dentro
 *	           de la clase Pin. Es más natural.
 *
 *	11/10/2023 Al intentar implementar la lectura de trenes de pulsos
 *	           mediante polling no funciona. Al mirar el código en
 *	           ensamblador que se genera observo que no está `inlining`
 *	           las funciones de Pin lo cual fijo está ralentizando la
 *	           ejecución. 
 *	           La clase Pin es muy básica, es fundamental que todas sus
 *	           funciones sean de verdad inline. Por ello le añado el
 *	           atributo de GCC de inline (problema: no es estandard,
 *	           aunque como este fichero es para los avrs y estos de
 *	           momento solo se pueden compilar con avr-gcc, no genera
 *	           problemas).
 *
 *	25/06/2024 Evolución de significado:
 *		   Quiero poder dejar en código documentado cómo se conectan 
 *		   los pines de un chip. Por ejemplo, en el A4988 se puede 
 *		   conectar el pin MS1 al microcontrolador, o dejarlo flotante
 *		   o conectarlo a GND o Vcc.
 *
 *		    ¿Cómo indicar eso?
 *
 *		    A4988::MS1 = Pin<14>; // conectado al pin 14 del mcu
 *		    A4988::MS1 = Pin<connected_to_GND> // conectado a GND
 *		    A4988::MS1 = Pin<connected_to_VCC> // conectado a +5V
 *		    A4988::MS1 = Pin<floating>         // sin conectar
 *	
 *		    Como se ve queda completamente documentado en código. El 
 *		    problema es que la clase Pin tiene 2 significados 
 *		    diferentes:
 *			1) Pin<14> es el pin del micro. 
 *			2) Pin<floating> no es un pin del micro, sino 
 *			   simplemente documenta como conectar el pin del chip.
 *		    Implemento este doble significado.
 *
 *	10/07/2024 Enable_change_level_interrupt
 *	13/07/2024 is_input/is_output
 *		   Reescrito internamente: uso atd::write_bit/read_bit
 *
 *	20/10/2024 Parametrizo Pin por Cfg para admitir micros de diferentes
 *	           pines.
 *
 ****************************************************************************/
#include "avr_not_generic.h" // TODO: para eliminar este archivo basta con
			     // escribir Pin como he hecho en atmega4809,
			     // pasándole la configuración a Pin.
    
#include <atd_bit.h>
#include <mcu_pin.h>

namespace avr_{

// Traductor de las interrupciones INT0, INT1, ...
//
// (RRR) ¿Por qué parametrizarlo por el número de la INT en lugar de escribir
//       clases particulares INT0 e INT1?
//       ¿por qué INT__<0> en lugar de INT0? ¿e INT__<1> en lugar de INT1?
//
//       Porque el número de interrupciones INTx depende del microcontrolador.
//       El atmega32 tiene 2, pero el atmega2560 tiene más. 
//       
template <int8_t n>
class INT__{
public:
    static void enable_change_level();
    static void enable_when_falling_edge();
    static void enable_when_rising_edge();
    static void enable_when_is_zero();
    static void disable();

private:
    static void enable_interrupt()
    {
	constexpr uint8_t i = cfg::INT_POS::at<n>;
	//atd::write_bit<cfg::INT_POS::at<n> >::template to<1>::in(EIMSK);
	atd::write_bit<i>::template to<1>::in(EIMSK);
    }

};


template <int8_t n>
void INT__<n>::enable_when_is_zero()
{
    constexpr uint8_t ISC1 = cfg::ISC1::at<n>; // = ISCn1
    constexpr uint8_t ISC0 = cfg::ISC0::at<n>; // = ISCn0

    atd::write_bits<ISC1, ISC0>::template to<0,0>::in(EICRA);
    //atd::write_bits<ISC01, ISC00>::to<0,0>::in(EICRA);
    enable_interrupt();
}

template <int8_t n>
void INT__<n>::enable_change_level()
{
    constexpr uint8_t ISC1 = cfg::ISC1::at<n>; // = ISCn1
    constexpr uint8_t ISC0 = cfg::ISC0::at<n>; // = ISCn0

    atd::write_bits<ISC1, ISC0>::template to<0,1>::in(EICRA);
    // atd::write_bits<ISC01, ISC00>::to<0,1>::in(EICRA);
    enable_interrupt();
}


template <int8_t n>
void INT__<n>::enable_when_falling_edge()
{
    constexpr uint8_t ISC1 = cfg::ISC1::at<n>; // = ISCn1
    constexpr uint8_t ISC0 = cfg::ISC0::at<n>; // = ISCn0

    atd::write_bits<ISC1, ISC0>::template to<1,0>::in(EICRA);
    // atd::write_bits<ISC01, ISC00>::to<1,0>::in(EICRA);
    enable_interrupt();
}

template <int8_t n>
void INT__<n>::enable_when_rising_edge()
{
    constexpr uint8_t ISC1 = cfg::ISC1::at<n>; // = ISCn1
    constexpr uint8_t ISC0 = cfg::ISC0::at<n>; // = ISCn0

    atd::write_bits<ISC1, ISC0>::template to<1,1>::in(EICRA);
    // atd::write_bits<ISC01, ISC00>::to<1,1>::in(EICRA);
    enable_interrupt();
}


template <int8_t n>
inline void INT__<n>::disable()
{
    constexpr uint8_t i = cfg::INT_POS::at<n>;
    atd::write_bit<i>::template to<0>::in(EIMSK);
    //atd::write_bit<cfg::INT_POS::at<n> >::template to<0>::in(EIMSK);
}

// Especialización para que de error al compilar si 
// se intenta generar una INTx en un pin no válido
template <>
class INT__<-1>{ };


/*!
 *  \brief  Clase para manejar pins
 *
 *  Este es un pin totalmente configurable. Lo podemos cambiar de entrada a
 *  salida y viceversa en medio del código.
 *
 *  Si se quiere una versión más reestringida usar Output_pin/de_entrada
 *
 */
// (RRR) Para no reescribir el nombre del Pin lo meto todo dentro de avr_. Es
// pereza, no tiene ninguna idea de diseño detras. (Mejorarlo en el futuro)
namespace private_ {

template <uint8_t n, typename Cfg>
class Pin{
public:
    using number_type = uint8_t;

    static constexpr bool is_a_valid_pin()
    {return Cfg::template is_a_valid_pin<n>(); }


// CONSTRUCCIÓN
    // constexpr Pin(){}	
    Pin() = delete;

    // No es posible copiar pins (aunque como es una template static no
    // debería de generar ningún problema)
    Pin(const Pin&)		= delete;
    Pin& operator=(const Pin&)	= delete;

// INFO
    static constexpr uint8_t number = n;

// CONFIGURAMOS EL cfg::PIN
    static void as_output() __attribute__((always_inline));
    static void as_input_with_pullup() __attribute__((always_inline));
    static void as_input_without_pullup() __attribute__((always_inline));

    static bool is_output_pin();
    static bool is_input_pin();
    static bool is_input_with_pullup_pin();
    static bool is_input_without_pullup_pin();


// FUNCIONES DE LECTURA
    // Si el bit es '1' devuelve un número distinto de cero.
    // Si el bit es '0' devuelve cero.
    static bool read() __attribute__((always_inline));

    /// Devuelve si el bit es 0 o no. 
    static bool is_zero() __attribute__((always_inline));

    /// Devuelve si el bit es 1 o no. 
    static bool is_one() __attribute__((always_inline));

// FUNCIONES DE ESCRITURA
    static void write_one() __attribute__((always_inline));
    static void write_zero() __attribute__((always_inline));

    /// Cambiamos el valor del pin (de 0 a 1 ó de 1 a 0).
    static void toggle() __attribute__((always_inline));

    /// Escribimos en el pin. Un valor distinto de cero es 1.
    static void write(uint8_t x) __attribute__((always_inline));
    

// INTERRUPTS
    // PCINT interrupts
    // ----------------
    // Genera una interrupción cada vez que el pin cambia de 0->1 ó de 1->0
    static void enable_change_level_interrupt();
    static void disable_change_level_interrupt();

    using INT = INT__<cfg::nINT_of_pin<n>()>;

private:
// En la datasheet se habla de DDRn, PORTn, ... pero en código tenemos DDRB,
// DDRC, ... Las siguientes funciones (realmente Cfg::ddr<n>()) permiten
// hablar como la datasheet: DDRn es Cfg::ddr<n>();
    static constexpr auto ddr()     {return Cfg::template ddr<n>();}
    static constexpr auto port()    {return Cfg::template port<n>();}
    static constexpr auto pin()     {return Cfg::template pin<n>();}
    static constexpr auto pin_bit() {return Cfg::template pin_bit<n>();}
    static constexpr auto bitmask() {return Cfg::template bitmask<n>();}

    static constexpr auto pcie() {return Cfg::template pcie<n>();}
    static constexpr auto pcint() {return Cfg::template pcint<n>();}
    static constexpr auto pcmsk() {return Cfg::template pcmsk<n>();}

};


// Implementación
// --------------
template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::as_output() 
//{ atd::write_bit<cfg::pin_bit<n>>::template to<1>::in(*cfg::DDR[n]); }
{ atd::write_bit<pin_bit()>::template to<1>::in(*ddr()); }

template <uint8_t n, typename Cfg>
inline bool Pin<n, Cfg>::is_output_pin()
{ return (atd::read_bit<pin_bit()>::of(*ddr()) !=  0); }


template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::as_input_with_pullup() 
{
    // de entrada:
    atd::write_bit<pin_bit()>::template to<0>::in(*ddr()); 

    // enable pull-up resistor 
    atd::write_bit<pin_bit()>::template to<1>::in(*port());
}

template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::as_input_without_pullup() 
{
    // de entrada:
    atd::write_bit<pin_bit()>::template to<0>::in(*ddr()); 

    // disable pull-up resistor 
    atd::write_bit<pin_bit()>::template to<0>::in(*port());
}

template <uint8_t n, typename Cfg>
bool Pin<n, Cfg>::is_input_pin()
{ return (atd::read_bit<pin_bit()>::of(*ddr()) ==  0); }


template <uint8_t n, typename Cfg>
bool Pin<n, Cfg>::is_input_with_pullup_pin()
{ 
    return (is_input_pin() and
	   (atd::read_bit<pin_bit()>::of(*port()) !=  0)); 
}


template <uint8_t n, typename Cfg>
bool Pin<n, Cfg>::is_input_without_pullup_pin()
{
    return (is_input_pin() and
	   (atd::read_bit<pin_bit()>::of(*port()) ==  0)); 
}


template <uint8_t n, typename Cfg>
inline bool Pin<n, Cfg>::read()
{return atd::read_bit<pin_bit()>::of(*pin()) != 0;}

template <uint8_t n, typename Cfg>
inline bool Pin<n, Cfg>::is_zero()
{return atd::read_bit<pin_bit()>::of(*pin()) == 0;}

template <uint8_t n, typename Cfg>
inline bool Pin<n, Cfg>::is_one() 
{return atd::read_bit<pin_bit()>::of(*pin()) != 0;}

template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::write_one()	
{atd::write_bit<pin_bit()>::template to<1>::in(*port()); }

template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::write_zero()	
{atd::write_bit<pin_bit()>::template to<0>::in(*port()); }

template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::toggle()  
{ (*port()) = (*port()) ^ bitmask(); }

template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::write(uint8_t x)
{
    if(x) write_one();
    else  write_zero();
}

template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::enable_change_level_interrupt()
{ 
    atd::write_bit<pcie()>::template to<1>::in(PCICR);
    atd::write_bit<pcint()>::template to<1>::in(*pcmsk());
}

template<uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::disable_change_level_interrupt()
{ atd::write_bit<pcint()>::template to<0>::in(*pcmsk()); }


} // namespace private_ 
 

// TODO: descomentar y borrar el particular
//template <uint8_t n, typename Cfg>
//using Pin = mcu::Pin<private_::Pin<n, Cfg>>::type;

// TODO: este es temporal, en lo que reestructuro el código
template <uint8_t n, typename Cfg = avr_::cfg::pins_28>
using Pin = mcu::Pin<private_::Pin<n, Cfg>>::type;


/***************************************************************************
 *			ENABLE_CHANGE_LEVEL_INTERRUPT
 ***************************************************************************/
// Equivalente a Enable_interrupts pero solo para el pin correspondiente
// TODO: quitar el valor por defecto cfg::pins_28
template <uint8_t n, typename Cfg = avr_::cfg::pins_28>
struct Enable_change_level_interrupt{
    Enable_change_level_interrupt() 
	{Pin<n, Cfg>::enable_change_level_interrupt(); }

    ~Enable_change_level_interrupt() 
	{Pin<n, Cfg>::disable_change_level_interrupt();}
};


// DUDA: borrar estas clases? En la práctica no las estoy usando.
// Me gusta más configurar los pines en el driver directamente porque así
// controlo lo que hago. 
/***************************************************************************
 *			    OUTPUT cfg::PIN
 ***************************************************************************/
/*!
 *  \brief  Creamos un pin que es de salida
 *  
 *  Observar que para que sea un pin de salida es necesario crear un objeto
 *  de este tipo.
 *
 *  (RRR) ¿Por qué necesito cfg()? 
 *        Si se usa como parámetro de template (y sospecho que es así como lo
 *        estoy usando casi siempre) necesito inicializarlo. Aquí todavía
 *        tengo dudas de si llamar a esta función init() o cfg() (???)
 *        Usándolo me resulta más natural llamarlo `as_output` ya que de esa
 *        forma se muestra la intención del código.
 */
template<uint8_t n>
class Output_pin{
public:
    static constexpr uint8_t number = n;

// Constructor
    constexpr Output_pin()
    { Pin<n>::as_output(); }

    Output_pin& operator=(const Output_pin&)	= delete;

    // static void cfg()         { Pin<n>::as_output(); }
    static void as_output()   { Pin<n>::as_output(); }

// Write pin
    static void write_one()	{Pin<n>::write_one();}
    static void write_zero()	{Pin<n>::write_zero();}
    static void write(uint8_t x) {Pin<n>::write(x);}
    static void toggle()	{Pin<n>::toggle();}

// Read pin
    static bool read() {return Pin<n>::read();}

    /// Devuelve si el bit es 0 o no. 
    static bool is_zero() {return Pin<n>::is_zero();}

    /// Devuelve si el bit es 1 o no. 
    static bool is_one() {return Pin<n>::is_one();}
};


// Usamos el pin 0, que no existe, to indicate a floating pin o a pin
// conectado a Vcc o GND.
template<>
class Output_pin<0>{ };



// Me gusta más configurar los pines en el driver directamente porque así
// controlo lo que hago. 
// Comentadas el 20/10/2024. Borrarlas si no se usan.
///***************************************************************************
// *			    INPUT cfg::PIN WITH PULLUP
// ***************************************************************************/
///*!
// *  \brief  Creamos un pin de entrada con pull-up
// *
// */
//template<uint8_t n>
//class Input_pin_with_pullup{
//public:
//    static constexpr uint8_t number = n;
//
//    Input_pin_with_pullup()
//    {init();}
//
//    static void init()
//    {Pin<n>::as_input_with_pullup();}
//
//
//    Input_pin_with_pullup& operator=(const Input_pin_with_pullup&) = delete;
//
//    /// Leemos el valor del pin (0 ó 1)
//    static bool read()
//    {return Pin<n>::read();}
//
//    /// Devuelve si el bit es 0 o no. 
//    static bool is_zero() {return Pin<n>::is_zero();}
//
//    /// Devuelve si el bit es 1 o no. 
//    static bool is_one() {return Pin<n>::is_one();}
//};
//
//
//
///***************************************************************************
// *			INPUT cfg::PIN WITHOUT PULLUP
// ***************************************************************************/
///*!
// *  \brief  Creamos un pin de entrada sin pull-up
// *
// */
//template<uint8_t n>
//class Input_pin_without_pullup{
//public:
//    static constexpr uint8_t number = n;
//
//    Input_pin_without_pullup()
//    {init();}
//
//    static void init()
//    {Pin<n>::as_input_without_pullup();}
//
//    Input_pin_without_pullup& operator=(const Input_pin_without_pullup&)	= delete;
//
//    /// Leemos el valor del pin (0 ó 1)
//    static bool read()
//    {return Pin<n>::read();}
//
//    /// Devuelve si el bit es 0 o no. 
//    static bool is_zero() {return Pin<n>::is_zero();}
//
//    /// Devuelve si el bit es 1 o no. 
//    static bool is_one() {return Pin<n>::is_one();}
//};


}// namespace avr


#endif

