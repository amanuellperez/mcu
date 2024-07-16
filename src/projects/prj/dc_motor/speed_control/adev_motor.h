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

#ifndef __ADEV_MOTOR_H__
#define __ADEV_MOTOR_H__

#include <atd_percentage.h>
#include <atd_math.h>
#include <atd_magnitude.h>
#include <atd_minifloat.h>

#include <mcu_time.h>

namespace adev{ // ampliacion de dev. Son clases que puede que sea interesante
		// pasarlas a dev

// Dudas paranoicas
// ----------------
//	    ¿Qué dirección es positiva y cual negativa?
//
// La pregunta es: yo soy el motor, o yo miro al motor.
// Voy a considerar que yo MIRO al motor, ya que es lo que habitualmente hago.
// Aunque si la clase es DC_Motor, ¿no quiere decir que es el motor? Pero
// claro, cuando yo le pido a la clase: DC_Motor.turn_right() le estoy
// diciendo que gire a la derecha ¿desde MI punto de vista o desde SU punto de
// vista? xD
//
// Voy a suponer (el uso dirá si es correcta o no la suposición) que yo
// me pongo DELANTE del motor y miro cómo gira. Si gira en el sentido
// matemático de ángulos positivos eso es girar hacia la izquierda;
// el sentido negativo es girar hacia la derecha
template <typename H_Bridge_t>
class DC_Motor{
public:
    using H_Bridge  = H_Bridge_t;

    using Direction = atd::Sign;

// Constructor
    DC_Motor() = delete;	// static interface
    static void init() {} 

// Moving functions
    static void stop();

    static void turn_right(const atd::Percentage& right);
    static void turn_left( const atd::Percentage& left);

    static void turn(Direction dir, const atd::Percentage& p);

// Info
    // Devuelve el percentage de la señal PWM generada para controlar
    // la velocidad del motor
    static atd::Percentage percentage();

    // Devuelve el sentido de giro del motor
    static Direction direction();
};


template <typename D>
inline 
void DC_Motor<D>::turn_right(const atd::Percentage& p)
{ H_Bridge::voltage(atd::Sign::negative, p); }

template <typename D>
inline 
void DC_Motor<D>::turn_left(const atd::Percentage& p)
{ H_Bridge::voltage(atd::Sign::positive, p); }

template <typename D>
void DC_Motor<D>::turn(Direction dir, const atd::Percentage& p)
{
    if (dir == Direction::positive)
	turn_left(p);
    else
	turn_right(p);
}


template <typename D>
inline void DC_Motor<D>::stop()
{ H_Bridge::stop(); }

template <typename D>
inline atd::Percentage DC_Motor<D>::percentage()
{ return H_Bridge::voltage_percentage(); }

template <typename D>
inline DC_Motor<D>::Direction DC_Motor<D>::direction()
{ return H_Bridge::voltage_sign(); }

/***************************************************************************
 *			    ENCODER_DISK_OPTOCOUPLER
 ***************************************************************************/
// Cfg
// ---
namespace default_cfg{
struct Encoder_disk_optocoupler {
    static constexpr uint16_t abort_time_ms = 400;
};
}

//struct Encoder_disk_optocoupler_cfg : default_cfg::Encoder_disk_optocoupler{
//    using Micro	       = Micro_t;
//    using Miniclock_ms = Miniclock_ms_t;
//    static constexpr uint8_t optocoupler_pin = optocoupler_pin_n;
//    static constexpr uint8_t disk_slots_number = disk_slots_number_n;
//    static constexpr uint16_t abort_time_ms = abort_time_ms_n;
//};

// TODO: Problema: si mido la velocidad de varios motores usando esta clase
//	 se va a generar una clase por cada Motor. Esto va a hacer que se
//	 duplique código: por ejemplo, la funcion measure_speed_in_ms se
//	 generara por cada motor y solo depende del pin. Crear una clase padre
//	 que incluya todo lo comun. (de paso evitar llamar a
//	 Miniclock_ms::init() más de una vez en esas clases):
//		    Encoder_base<Miniclock>
//			    ^
//			    |
//		    Encoder<Miniclock, pin>
//
//	La base tiene la función Miniclock::init, y measure_speed_in_ms(pin)
//	que es la que llama la hija. De esa forma no duplicamos código.
//	
//
// (???) Nombre? 
//       Corresponde con hardware directamente: es un encoder disk que se le
//       coloca un slotted optocoupler para medir la rotación
// Observar que realmente lo que hace la clase es medir el tiempo de duración
// de un pulso del optocoupler.
template <typename Cfg>
class Encoder_disk_optocoupler{
public:
// Types
    using Micro	       = Cfg::Micro;
    using Miniclock_ms = Cfg::Miniclock_ms;
    using Pin       = typename Micro::Pin<Cfg::optocoupler_pin>;
    using time_type = typename Miniclock_ms::counter_type;
    using Angular_speed = atd::Degrees_per_second<atd::Float16>;
    using RPM       = atd::Float16;

// Cfg
    // Me gusta más N; ^_^'
    static constexpr uint8_t N = Cfg::disk_slots_number ;
    static constexpr uint8_t disk_slots_number= N; // mejor que N???

// Constructor
    Encoder_disk_optocoupler() = delete;
    static void init();

// measure
    // Devuelve el tiempo, en milisegundos, que transcurre desde que 
    // empieza un pulso de luz hasta el siguiente pulso de luz.
    // Esto es, el tiempo que tarda el encoder al girar de un slot a otro.
    // Devuelve 0 en caso de error.
    static time_type measure_speed_in_ms();

    static Angular_speed measure_speed_dps();
    static atd::Float16 measure_speed_rpm();


// Interrupt
    // Maneja la interrupción correspondiente al pin optocoupler_pin
    static void handle_interrupt();

// Units conversion 
    static constexpr time_type speed_in_rpm_to_time_in_ms(const RPM& rpm);

private:
    // Tiempo máximo que esperamos antes de cancelar una operación
    static constexpr uint16_t abort_time_ms = Cfg::abort_time_ms;
    
    struct Wait_1_ms{
	void operator()() {Micro::wait_ms(1);}
    };

    template <typename Pred>
    using wait_till = mcu::wait_till<Wait_1_ms, Pred>;

// Units conversion
    // El número de ranuras del sensor queda fijado por hardware ==> template
    // parameter
    // _dps = degrees per second
    static constexpr 
	    Angular_speed time_in_ms_to_speed_in_dps(uint16_t period_ms);

    static constexpr 
	    atd::Float16 time_in_ms_to_speed_in_rpm(uint16_t period_ms);
};


// DUDA: quién inicializa Miniclock_ms
//       Miniclock_ms puede ser usado por diferentes clases a la vez, siempre
//       y cuando no intenten medir tiempos simultaneamente. Si lo inicializa
//       cada clase que lo usa se llamara varias veces a init (aunque tampoco
//       habra muchas clases) con lo que es ineficiente; pero si no lo
//       inicializa la clase queda el riesgo de olvidarse inicializarse.
//       O se pone la disciplina de: 
//	    a) todo dispositivo definido lo inicializa el cliente.
//	 o podemos poner que
//	    b) solo los dispositivos finales que se van a usar en el programa
//	       se inicializan.
//
//	¿Qué es mejor? De momento prefiero inicializarlo aquí.
template <typename C>
void Encoder_disk_optocoupler<C>::init()
{ 
    Pin::as_input_without_pullup(); 
    Miniclock_ms::init(); 
}

template <typename C>
void Encoder_disk_optocoupler<C>::handle_interrupt()
{
    // Medimos transiciones 0 -> 1
    if (Pin::is_one()){

	if (Miniclock_ms::is_off())
	    Miniclock_ms::start();

	else{
	    Miniclock_ms::stop();
	    Pin::disable_change_level_interrupt(); 
	}
    }
}

template <typename C>
Encoder_disk_optocoupler<C>::time_type
    Encoder_disk_optocoupler<C>::measure_speed_in_ms()
{
    using Enable_change_level_interrupt = typename Micro::Enable_change_level_interrupt<Pin::number>;
    Enable_change_level_interrupt inter{};

    Miniclock_ms::reset();

    if (wait_till([]{ return Miniclock_ms::is_off(); })
		._at_most_ms(abort_time_ms)) 
	    return 0;


    if (wait_till([]{ return Miniclock_ms::is_on(); })
		._at_most_ms(abort_time_ms)) 
	    return 0;

    return Miniclock_ms::time();
}


template <typename C>
constexpr 
Encoder_disk_optocoupler<C>::Angular_speed 
	Encoder_disk_optocoupler<C>::time_in_ms_to_speed_in_dps(uint16_t period_ms)
{ // 360'000 en vez de 360 porque el period_ms esta en milisegundos 
    // return atd::Degree<atd::Float16>{360'000} / (N * period_ms); 
    atd::Float16::Rep d = N * period_ms;
    auto res = atd::Float16{360'000} / atd::Float16{d};
    return res;
}

template <typename C>
constexpr 
atd::Float16 
    Encoder_disk_optocoupler<C>::time_in_ms_to_speed_in_rpm(uint16_t period_ms)
{
    atd::Float16::Rep d = N * period_ms;
    return atd::Float16{60'000} / atd::Float16{d};
}

// (RRR) ¿Puede haber overflow?
//	 No ya que res entra en un uint16_t:
//		    60'000 / d < 60'000 < 2^16
template <typename C>
constexpr 
Encoder_disk_optocoupler<C>::time_type
    Encoder_disk_optocoupler<C>::speed_in_rpm_to_time_in_ms(const RPM& rpm)
{
    RPM d = RPM{N} * rpm;
    auto res = RPM{60'000} / d;
    return res.integer_part<uint16_t>();
}


template <typename C>
Encoder_disk_optocoupler<C>::Angular_speed 
    Encoder_disk_optocoupler<C>::measure_speed_dps()
{ 
    auto t = measure_speed_in_ms();
    if (t == 0) return 0;

    return time_in_ms_to_speed_in_dps(t);
}


template <typename C>
atd::Float16 Encoder_disk_optocoupler<C>::measure_speed_rpm()
{ 
    auto t = measure_speed_in_ms();
    if (t == 0) return 0;

    return time_in_ms_to_speed_in_rpm(t);
}


/***************************************************************************
 *			    SPEED CONTROL MOTOR
 ***************************************************************************/
namespace default_cfg {
struct Speed_control_motor {
    // Incrementamos el percentage de 5 en 5 hasta alcanzar más-menos la
    // velocidad solicitada a `turn`
    static constexpr uint8_t turn_increment_first_try = 5;

    // El motor que tengo no empieza con cualquier señal de PWM. 
    // De hecho empieza a partir de 25% de duty_cycle. Esta variable indica 
    // qué percentage aplica la función `turn` cuando quiere arrancar el
    // motor.
    static constexpr uint8_t turn_percentage_start = 25;

    // El motor tiene una inercia: desde que le pides que empiece a girar a
    // una velocidad hasta que alcanza esa velocidad tarda un tiempo. El
    // tiempo de espera del micro viene dado por esta variable
    // TODO: se puede complicar la función turn y que compare el tiempo de
    // giro actual con la última medida para saber si está acelerando o no. Si
    // no acelera no cambiar el percentage y repetir. Si el valor no cambia en
    // 10 mediciones, por ejemplo, es que el motor ya no puede alcanzar una
    // velocidad mayor y devolvería el control. Esa podría ser una forma de
    // evitar tener que definir `turn_time_ms_inertia`(???)
    static constexpr uint8_t turn_time_ms_inertia = 1;

};
}

// struct Speed_control_motor_cfg : default_cfg::Speed_control_motor {
//	using Micro = ...
//	using Motor = ...
//	using Speedmeter = ...
//	static constexpr uint8_t time_ms_intertia = 0;
// };
//
// Speed_control_motor = es un motor con un medidor de velocidad de giro
// (???) ¿Cómo llamarlo en ingles? 
// Es un motor en el que se puede fijar la velocidad de giro.
//
template <typename Cfg>
class Speed_control_motor{
public:
// types
    using Micro	    = Cfg::Micro;
    using Motor     = Cfg::Motor;
    using Speedmeter= Cfg::Speedmeter;
    using Direction = typename Motor::Direction;
    using RPM       = atd::Float16;
    using time_type = typename Speedmeter::time_type;


// construction
    Speed_control_motor() = delete; 
    static void init();

// Movement
    static void stop();

    static void turn_right(const RPM& rpm);
    static void turn_left( const RPM& rpm);

    static void turn(Direction dir, const RPM& rpm);

private:
// cfg
    static constexpr uint8_t turn_time_ms_inertia = Cfg::turn_time_ms_inertia;
    static constexpr uint8_t turn_percentage_start = Cfg::turn_percentage_start;
    static constexpr uint8_t turn_increment_first_try 
						= Cfg::turn_increment_first_try;
					 
// turn implementation
    static void turn_impl(Direction direction, const time_type& time_ms0);
    static void turn_increasing_time(int8_t incr, Direction direction, 
						 const time_type& time_ms0);
    static void turn_decreasing_time(int8_t incr, Direction direction, 
						 const time_type& time_ms0);

};

template <typename C>
inline void Speed_control_motor<C>::init()
{
    Motor::init();
    Speedmeter::init();
}

template <typename C>
inline void Speed_control_motor<C>::stop()
{ Motor::stop(); }


template <typename C>
inline
void Speed_control_motor<C>::turn_right(const RPM& rpm)
{ turn(Direction::negative, rpm); }

template <typename C>
void Speed_control_motor<C>::turn_left(const RPM& rpm)
{ turn(Direction::positive, rpm); }


template <typename C>
void Speed_control_motor<C>::turn(Direction direction, const RPM& rpm)
{
    if (rpm == 0){
	Motor::stop();
	return;
    }

    auto direction0 = Motor::direction();
    if (direction != direction0)
	Motor::stop();

    auto time_ms0 = Speedmeter::speed_in_rpm_to_time_in_ms(rpm);

    turn_impl(direction, time_ms0);
}


// Intenta llegar a la velocidad deseada 
template <typename C>
void Speed_control_motor<C>::turn_impl(Direction direction, 
				       const time_type& time_ms0)
{
    auto t = Speedmeter::measure_speed_in_ms();

    if (0 < t and t < time_ms0){
	turn_increasing_time(turn_increment_first_try, direction, time_ms0);
	turn_decreasing_time(1, direction, time_ms0);
    }

    else{ // el caso t == 0 es cuando el motor está parado, realmente t = infinito
	turn_decreasing_time(turn_increment_first_try, direction, time_ms0);
	turn_increasing_time(1, direction, time_ms0);
    }
}


template <typename C>
void Speed_control_motor<C>::turn_increasing_time(int8_t incr, 
						 Direction direction, 
						 const time_type& time_ms0)
{
    auto p = Motor::percentage().as_uint();

    for( ; p >= turn_percentage_start ; p -= incr){

	Motor::turn(direction, p);
	if constexpr (turn_time_ms_inertia != 0)
	    Micro::wait_ms(turn_time_ms_inertia);

	auto t = Speedmeter::measure_speed_in_ms();

//	if (t == 0) // error al leer: el motor no ha arrancado
//	    continue;

	if (t >= time_ms0) return;
    }
}

// precondition: time_ms0 > 0
template <typename C>
void Speed_control_motor<C>::turn_decreasing_time(int8_t incr, 
						 Direction direction, 
						 const time_type& time_ms0)
{
    auto p = Motor::percentage().as_uint();

    if (p == 0)
	p = turn_percentage_start ;

    for ( ; p <= 100; p += incr){

	Motor::turn(direction, p);

	if constexpr (turn_time_ms_inertia != 0)
	    Micro::wait_ms(turn_time_ms_inertia);

	auto t = Speedmeter::measure_speed_in_ms();

	if (t == 0) // motor parado ==> tiempo = infinito > time_ms0
	    continue;

	if (t <= time_ms0) 
	    return;
    }
}


}// namespace
 
#endif


