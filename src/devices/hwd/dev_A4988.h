// Copyright (C) 2024 Manuel Perez 
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

#ifndef __DEV_A4988_H__
#define __DEV_A4988_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del driver del stepper motor A4988	
 *
 * TODO
 *	A día de hoy si se usan dos instancias de A4988 (porque haya 2
 *	motores) se van a crear dos clases diferentes A4988<pines_motor1>
 *	y A4988<pines_motor2>, duplicando el código, lo cual es ineficiente.
 *
 *	Una forma de eliminar esto es sacar todas las funciones comunes a una
 *	clase padre y que las hijas se limiten a particularizar los pines.
 *	
 *	De esa forma conseguimos:
 *	(1) "Todo las conexiones de hardware se conocen en tiempo de
 *	compilación, con lo que hay que pasarlas como parámetros de templates"
 *	para detectar errores en tiempo de compilación.
 *
 *	(2) No duplicamos código.
 *
 * HISTORIA
 *    Manuel Perez
 *    22/06/2024 Escrito
 *
 ****************************************************************************/
#include <atd_math.h>	// Sign

#include <mcu_pin.h>	

namespace dev{

// Cfg
// ---
// Admite una estructura con los números de pines correspondientes.
// Si no se pasa un pin se da por supuesto que está flotante.
template <typename Pins>
struct A4988_pins{
    using Pin = mcu::Pin_connection_type;
    static constexpr uint8_t NO_ENABLE(){ return Pin::floating;}
    static constexpr uint8_t NO_ENABLE()
		requires requires {Pins::NO_ENABLE;}
		{ return Pins::NO_ENABLE;}

    static constexpr uint8_t MS1() { return Pin::floating; }
    static constexpr uint8_t MS1()
		requires requires {Pins::MS1;}
		{ return Pins::MS1; }

    static constexpr uint8_t MS2() { return Pin::floating; }
    static constexpr uint8_t MS2()
		requires requires {Pins::MS2;}
		{ return Pins::MS2; }

    static constexpr uint8_t MS3() { return Pin::floating; }
    static constexpr uint8_t MS3()
		requires requires {Pins::MS3;}
		{ return Pins::MS3; }


    static constexpr uint8_t NO_RESET() { return Pin::floating; }
    static constexpr uint8_t NO_RESET()
		requires requires {Pins::NO_RESET;}
		{ return Pins::NO_RESET; }

    static constexpr uint8_t NO_SLEEP() { return Pin::floating; }
    static constexpr uint8_t NO_SLEEP()
		requires requires {Pins::NO_SLEEP;}
		{ return Pins::NO_SLEEP; }


    using STEP = typename Pins::STEP;
    static constexpr uint8_t DIR() { return Pins::DIR; }
    
};



// Class
// -----
template <typename Micro0, typename Pins_numbers>
class A4988_basic {
public:
// Types
    using Micro = Micro0;

    // pins
    using Pins = A4988_pins<Pins_numbers>;

    // de control de movimiento
    using STEP =  typename Pins::STEP;
    using DIR  =  typename Micro::Pin<Pins::DIR()>;

    // de configuración
    using NO_ENABLE = typename Micro::Pin<Pins::NO_ENABLE()>;
    using NO_RESET  = typename Micro::Pin<Pins::NO_RESET()>;
    using NO_SLEEP  = typename Micro::Pin<Pins::NO_SLEEP()>;

    // tipo de movimiento
    using MS1 = typename Micro::Pin<Pins::MS1()>;
    using MS2 = typename Micro::Pin<Pins::MS2()>;
    using MS3 = typename Micro::Pin<Pins::MS3()>;


    // types
    using Direction = atd::Sign;
    using Frequency = typename STEP::Frequency;
    using Hertz     = typename STEP::Hertz;
    using NSteps_t  = STEP::NPulses_t;

// Constructor 
    A4988_basic() = delete; // static interface
    
    static void init();

// Movement
// --------
    static void direction(Direction);

    // Envía al motor nsteps pulsos a la frecuencia indicada.
    static void step(const Frequency& freq, const NSteps_t& nsteps);

    // (???) ¿Tiene algún sentido estas dos funciones? 
    // step(Frequency){STEP::genera(freq);}; // el motor gira sin parar 
    // stop(){STEP::stop();};		     // se para el motor

// Cfg
// ---
// no enable pin
    static void enable()    requires (NO_ENABLE::is_a_valid_pin());
    static void disable()   requires (NO_ENABLE::is_a_valid_pin());

// no sleep pin
    static void sleep() requires (NO_SLEEP::is_a_valid_pin());
    static void awake() requires (NO_SLEEP::is_a_valid_pin());

// no reset
    // Datasheet: Reset input: [...] All STEP inputs are ingored until the
    // no_reset input is set to high.
    // El nombre "reset" parece estar mal puesto. Como lo que hace es
    // ignorar steps, al principio pensé en llamarlo ignore_steps(). Pero
    // ¿cómo llamar a la acción opuesta? Al final opto por engage/disengage 
    static void engage()    requires (NO_RESET::is_a_valid_pin());
    static void disengage() requires (NO_RESET::is_a_valid_pin());

// Modes
    // El A4988 se puede configurar para que funcione: en full step, en half
    // step, quarter step, eighth step y sixteenth step mode.
    // (???) de momento voy a imponer que para poder configurar el modo
    // dinámicamente los tres pines MS1, MS2 y MS3 tienen que estar conectados
    // al microcontrolador. (Si sabemos que el pin MS1 está conectado a Vcc,
    // podríamos configurar el driver en algunos modos, pero ¿merece la pena
    // implementar esos casos raros?)
    //
    // DUDA: por qué no puedo definir esta función después? El compilador se
    // queja.
    static constexpr bool MS123_connected()
    { return MS1::is_a_valid_pin()  and
	     MS2::is_a_valid_pin()  and MS3::is_a_valid_pin(); }
    
    // Función común a todos los steppers drivers. Indica si el step_mode
    // está fijado en tiempo de compilación o no, permitiendo alguna
    // optimización sencilla
    static constexpr bool step_mode_at_compile_time();

    enum class Mode {
	    full_step, half_step, quarter_step, eighth_step, sixteenth_step,
	    unknown };

    static Mode mode() requires (MS123_connected());
    static Mode mode() requires (!MS123_connected());


    static void full_step_mode()	requires (MS123_connected());
    static void half_step_mode()	requires (MS123_connected());
    static void quarter_step_mode()	requires (MS123_connected());
    static void eighth_step_mode()	requires (MS123_connected());
    static void sixteenth_step_mode()	requires (MS123_connected());


private:
// Types
    using SW_signal = typename STEP::SW_signal;



};


template <typename M, typename P>
void A4988_basic<M, P>::init()
{
    DIR::as_output();

    if constexpr (NO_ENABLE::is_a_valid_pin()){
	NO_ENABLE::as_output();
	enable(); // TODO: en qué estado inicial lo dejamos?
    }

    if constexpr (NO_SLEEP::is_a_valid_pin()){
	NO_SLEEP::as_output();
	awake(); // TODO: en qué estado inicial lo dejamos?
    }

    if constexpr (NO_RESET::is_a_valid_pin()){
	NO_RESET::as_output();
	engage(); // TODO: en qué estado inicial lo dejamos?
    }

    if constexpr (MS1::is_a_valid_pin()){ MS1::as_output(); }
    if constexpr (MS2::is_a_valid_pin()){ MS2::as_output(); }
    if constexpr (MS3::is_a_valid_pin()){ MS3::as_output(); }
}


// Movement
// --------
// DUDA: ¿hay que parar el motor antes de cambiar dirección? 
//	¿Qué pasa si con SWG el sofwador hace que el motor vaya dando pasos y
//	cambia la dirección? ¿Puede generar algún problema? Si el driver es
//	robusto, no. ¿Lo es?
template <typename M, typename P>
void A4988_basic<M, P>::direction(Direction dir) 
{
// stop() (???)
    if (dir == Direction::positive) 
	DIR::write_one();

    else
	DIR::write_zero();
}


template <typename M, typename P>
void A4988_basic<M, P>::step(const Frequency& freq, const NSteps_t& nsteps)
{
    SW_signal sw{freq};
    STEP::generate(sw, nsteps); 
}


// no enable
// ---------
template <typename M, typename P>
inline 
void A4988_basic<M, P>::enable()
    requires (NO_ENABLE::is_a_valid_pin())
{ NO_ENABLE::write_zero(); }

template <typename M, typename P>
inline 
void A4988_basic<M, P>::disable()
    requires (NO_ENABLE::is_a_valid_pin())
{ NO_ENABLE::write_one(); }


// no sleep
// --------
template <typename M, typename P>
inline 
void A4988_basic<M, P>::sleep()
    requires (NO_SLEEP::is_a_valid_pin())
{ NO_SLEEP::write_zero(); }

template <typename M, typename P>
inline 
void A4988_basic<M, P>::awake()
    requires (NO_SLEEP::is_a_valid_pin())
{ 
    NO_SLEEP::write_one(); 
    // La datasheet indica que despés de despertar esperar 1ms antes de enviar
    // ningún step. ¿Hago la espera aquí bloqueando al micro por 1 ms o que la
    // gestione el cliente? 
    //	A favor: el cliente se olvida del funcionamiento del driver
    //	En contra: bloqueamos 1 ms el micro, lo cual es mucho tiempo.
    //	De momento forcemos al cliente a que sea él el que espere 1 ms (pero,
    //	... cómo forzarle? como hacer que no se le olvide?)
    // (???) Micro::wait_ms(1);
}


// no reset
// --------
template <typename M, typename P>
inline 
void A4988_basic<M, P>::engage()
    requires (NO_RESET::is_a_valid_pin())
{ NO_RESET::write_one(); }

template <typename M, typename P>
inline 
void A4988_basic<M, P>::disengage()
    requires (NO_RESET::is_a_valid_pin())
{ NO_RESET::write_zero(); }


// modes
// -----
template <typename M, typename P>
void A4988_basic<M, P>::full_step_mode()	
    requires (MS123_connected())
{// 000
    MS1::write_zero();
    MS2::write_zero();
    MS3::write_zero();
}

template <typename M, typename P>
void A4988_basic<M, P>::half_step_mode()	
    requires (MS123_connected())
{// 100
    MS1::write_one();
    MS2::write_zero();
    MS3::write_zero();
}

template <typename M, typename P>
void A4988_basic<M, P>::quarter_step_mode()	
    requires (MS123_connected())
{// 010
    MS1::write_zero();
    MS2::write_one();
    MS3::write_zero();
}

template <typename M, typename P>
void A4988_basic<M, P>::eighth_step_mode()	
    requires (MS123_connected())
{// 110
    MS1::write_one();
    MS2::write_one();
    MS3::write_zero();
}

template <typename M, typename P>
void A4988_basic<M, P>::sixteenth_step_mode()	
    requires (MS123_connected())
{// 111
    MS1::write_one();
    MS2::write_one();
    MS3::write_one();
}

template <typename M, typename P>
constexpr bool A4988_basic<M, P>::step_mode_at_compile_time()
{
    if constexpr (MS123_connected())
	return false;
    else
	return true;
}


// (@_@) Aquí tenemos un ejemplo de código ilegible.
//       Se podría escribir:
//	    uint8_t x{};
//	    first_bit(x) = MS1::is_one();
//	    second_bit(x) = MS2::is_one();
//	    third_bit(x) = MS3::is_one(); // o algo parecido
//	    switch(x):
//		case 000: return full_step;
//		case 100: return half_step;
//		case 010: return quarter_step;
//		case 110: return eighth_step;
//		case 111: return sixteenth_step;
//
//	Quedando la tabla completamente legible. El problema es que 
//	es un pelín más ineficiente y genera un poco más código que el
//	siguiente monstruo:
template <typename M, typename P>
A4988_basic<M, P>::Mode A4988_basic<M, P>::mode()
    requires (MS123_connected())
{
    if (MS1::is_zero()){// 0..
	if (MS2::is_zero()){ // 00.
	    if (MS3::is_zero()) return Mode::full_step;
	    // else  001 no mode
	} else { // 01.
	    if (MS3::is_zero()) return Mode::quarter_step;
	    // else 011 no mode
	}
    } else {// 1..
	if (MS2::is_zero()){ // 10.
	    if (MS3::is_zero()) return Mode::half_step;
	    // else 101 no mode
	} else { // 11.
	    if (MS3::is_zero()) return Mode::eighth_step;
	    else                return Mode::sixteenth_step;
	}
    }

    return Mode::unknown;
}


template <typename M, typename P>
inline 
A4988_basic<M, P>::Mode A4988_basic<M, P>::mode()
    requires (!MS123_connected())
{
    if constexpr (mcu::pin<MS1>::is_floating() and
	          mcu::pin<MS2>::is_floating() and
	          mcu::pin<MS3>::is_floating())
	return Mode::full_step;
    
    else if constexpr (mcu::pin<MS1>::is_connected_to_GND() and
		       mcu::pin<MS2>::is_connected_to_GND() and
	               mcu::pin<MS3>::is_connected_to_GND())
	return Mode::full_step;

    else if constexpr (mcu::pin<MS1>::is_connected_to_VCC() and
		       mcu::pin<MS2>::is_connected_to_GND() and
	               mcu::pin<MS3>::is_connected_to_GND())
	return Mode::half_step;

    else if constexpr (mcu::pin<MS1>::is_connected_to_GND() and
		       mcu::pin<MS2>::is_connected_to_VCC() and
	               mcu::pin<MS3>::is_connected_to_GND())
	return Mode::quarter_step;

    else if constexpr (mcu::pin<MS1>::is_connected_to_VCC() and
		       mcu::pin<MS2>::is_connected_to_VCC() and
	               mcu::pin<MS3>::is_connected_to_GND())
	return Mode::eighth_step;

    else if constexpr (mcu::pin<MS1>::is_connected_to_VCC() and
		       mcu::pin<MS2>::is_connected_to_VCC() and
	               mcu::pin<MS3>::is_connected_to_VCC())
	return Mode::sixteenth_step;

    else 
	static_assert(atd::always_false_v<M>, 
		    "Wrong connection in pins: MS1, MS2, MS3");
}


}// namespace


#endif


