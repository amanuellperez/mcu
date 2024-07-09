// Copyright (C) 2021-2024 Manuel Perez 
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

#ifndef __AVR_TIMER1_GENERIC_H__
#define __AVR_TIMER1_GENERIC_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Diferentes formas de usar el Timer1
 *
 *	El Timer1 del avr realmente tiene 4 diferentes usos:
 *	(1) Pulse_counter: cuenta el número de pulsos en un pin 
 *		           TODO: implementar. El nombre Pulse_counter es
 *		           temporal. ¿dejarlo? Esta capacidad no la tiene el
 *		           Timer0 ni el Timer2.
 *
 *	(2) Time_counter : medidor de tiempo (de pulsos de reloj).
 *	(3) SWG_pin      : generador de una señal cuadrada en un pin.
 *	(4) PWM_pin      : generador de una señal PWM en un pin.
 *
 *	Esas, creo, son los distintos usos que podemos dar al hardware
 *	correspondiente. Al traductor del hardware lo llamo directamente
 *	Timer1, mientras que a sus usos los nombres anteriores.
 *
 *  COMENTARIOS ANTIGUOS
 *	Estas clases sirven para separar las responsabilidades del Timer.
 *  Un Timer mezcla distintos dispositivos: un contador, un generador de onda
 *  cuadrada, otro de PWM. Las clases definidas aquí separan todas esos
 *  dispositivos para que sea más sencillo usarlas.
 *	En principio tengo 2 formas mínimo de concebirlo:
 *	1) Square_wave_generator: concibo el Timer como un generador de onda
 *	   cuadrada. Esta concepción corresponde con el diseño del Timer del
 *	   avr.
 *
 *	2) SW_pin (square_wave_pin): a fin de cuentas lo que queremos es
 *	   generar una onda cuadrada en un pin concreto. Parece más natural
 *	   definirlo de esta forma. De hecho, para que quede documentado en
 *	   `dev.h` y detectar errores en tiempo de compilación se puede
 *	   definir como:
 *		using SW_pin = Micro::SW_pin<15>;
 *
 *	   De esa forma se deja en código que se quiere generar una onda
 *	   cuadrada en el pin número 15. 
 *
 *	   Aunque esa parece una forma muy sencilla de definir el SW_pin, en
 *	   la definición anterior no se indica el Timer que se usa. Creo (???)
 *	   que es conveniente dejarlo indicado en `dev.h` para que el
 *	   programador sea consciente de que se está usando el Timer1. Por
 *	   ello de momento opto por
 *		using SW_pin = Micro::SW1_pin<15>;
 *
 *	   Problema: si definimos dos pines
 *		using SW_pin1 = Micro::SW1_pin<15>;
 *		using SW_pin2 = Micro::SW1_pin<16>;
 *	    al usar ambos pines el mismo Timer van a estar acoplados y el
 *	    usuario tendrá que ser consciente de ello.
 *	
 *	    Voy a probar a escribir PWM1_pin a ver si es más sencilla de
 *	    manejar que PWM_generator. El uso lo dirá.
 *
 *
 *  HISTORIA
 *    Manuel Perez
 *    15/02/2021 Escrito
 *    26/02/2022 timer_counter
 *    30/10/2022 Generic_timer_counter
 *    07/12/2022 dev::Generic_timer<Timer0> --> avr::Timer1_g
 *		 ¿Por qué lo había definido como template?
 *		 Lo que quiero definir es un timer que obedece al
 *		 concept "timer". No necesito usar templates para nada.
 *    15/12/2022 Square_wave_generator1_g
 *    04/06/2024 PWM1_pin
 *    22/06/2024 SWG1_pin
 *
 ****************************************************************************/
#include "avr_timer1_basic.h"
#include "avr_timern_basic.h"
#include "avr_interrupt.h"
#include "avr_cfg.h"	// clock_frequency_in_hz
#include "avr_micro.h"

#include <atd_math.h>
namespace avr_{

namespace timer1_{

// avr clock at 1MHz
// -----------------
template<uint16_t period>
inline void set_clock_period_in_us_1MHz() 
{
    if constexpr (period == 1u)
	Timer1::clock_frequency_no_preescaling();
    
    else if constexpr (period == 8u)
	Timer1::clock_frequency_divide_by_8();

    else if constexpr (period == 64u)
	Timer1::clock_frequency_divide_by_64();

    else if constexpr (period == 256u)
	Timer1::clock_frequency_divide_by_256();

    else if constexpr (period == 1024u)
	Timer1::clock_frequency_divide_by_1024();

    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect Timer1 period. Try another one. "
		    "Valid ones: 1, 8, 64, 256 or 1024.");
}

// TODO: a .cpp???
inline Time clock_period_in_us_1MHz()
{
    using namespace literals;
    switch(Timer1::frequency_divisor()){
	case Timer1::Frequency_divisor::no_preescaling	: return 1_us;
	case Timer1::Frequency_divisor::divide_by_8	: return 8_us;
	case Timer1::Frequency_divisor::divide_by_64	: return 64_us;
	case Timer1::Frequency_divisor::divide_by_256	: return 256_us;
	case Timer1::Frequency_divisor::divide_by_1024	: return 1024_us;
	case Timer1::Frequency_divisor::off		: return 0_us;
    }

    return 0_us;
}

inline Frequency clock_frequency_in_Hz_1MHz()
{
    using namespace literals;

    switch(Timer1::frequency_divisor()){
	case Timer1::Frequency_divisor::no_preescaling	: return 1_MHz;
	case Timer1::Frequency_divisor::divide_by_8	: return 125_kHz;
	case Timer1::Frequency_divisor::divide_by_64	: 
					return Frequency{15'625ul};
	case Timer1::Frequency_divisor::divide_by_256	: 
					return Frequency{3'906ul};
	case Timer1::Frequency_divisor::divide_by_1024	: 
					return Frequency{977};
	case Timer1::Frequency_divisor::off		: return 0_Hz;
    }

    return 0_Hz;
}


// avr clock at 8MHz
// -----------------
//// a 125 ns
//template<>
//inline void set_clock_period_in_ns<125u, 8000000UL>() 
//{Timer1::clock_frequency_no_preescaling();}

template<uint16_t period>
inline void set_clock_period_in_us_8MHz() 
{
    if constexpr (period == 1u)
	Timer1::clock_frequency_divide_by_8();

    else if constexpr (period == 8u)
	Timer1::clock_frequency_divide_by_64();

    else if constexpr (period == 32u)
	Timer1::clock_frequency_divide_by_256();
 
    else if constexpr (period == 128u)
	Timer1::clock_frequency_divide_by_1024();
 
    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect Timer1 period. Try another one. "
		    "Valid ones: 1, 8, 32 or 128.");
}


inline Time clock_period_in_us_8MHz()
{
    using namespace literals;
    switch(Timer1::frequency_divisor()){
	case Timer1::Frequency_divisor::no_preescaling	: return 0_us;
	case Timer1::Frequency_divisor::divide_by_8	: return 1_us;
	case Timer1::Frequency_divisor::divide_by_64	: return 8_us;
	case Timer1::Frequency_divisor::divide_by_256	: return 32_us;
	case Timer1::Frequency_divisor::divide_by_1024	: return 128_us;
	case Timer1::Frequency_divisor::off		: return 0_us;
    }

    return 0_us;
}

inline Frequency clock_frequency_in_Hz_8MHz()
{
    using namespace literals;
    switch(Timer1::frequency_divisor()){
	case Timer1::Frequency_divisor::no_preescaling	: return 0_MHz;
	case Timer1::Frequency_divisor::divide_by_8	: return 1_MHz;
	case Timer1::Frequency_divisor::divide_by_64	: return 125_kHz;
	case Timer1::Frequency_divisor::divide_by_256	: 
				return Frequency{31'250ul};
	case Timer1::Frequency_divisor::divide_by_1024	: 
				return Frequency{7'813ul};// exacto: 7812.5 +-10%
	case Timer1::Frequency_divisor::off		: return 0_Hz;
    }

    return 0_Hz;
}





template<uint32_t clock_frequency_in_hz = avr_::clock_frequency_in_hz>
inline Time clock_period()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	return clock_period_in_us_1MHz();

    else if constexpr (clock_frequency_in_hz == 8000000UL)
	return clock_period_in_us_8MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_period: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}


template<uint16_t period
	, uint32_t clock_frequency_in_hz>
inline void set_clock_period_in_us()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	set_clock_period_in_us_1MHz<period>();

    else if constexpr (clock_frequency_in_hz == 8000000UL)
	set_clock_period_in_us_8MHz<period>();

    else
        static_assert(atd::always_false_v<int>,
                      "set_clock_period_in_us: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}



template<uint32_t clock_frequency_in_hz>
inline Frequency clock_frequency()
{
    if constexpr (clock_frequency_in_hz == 1000000UL)
	return clock_frequency_in_Hz_1MHz();

    else if constexpr (clock_frequency_in_hz == 8000000UL)
	return clock_frequency_in_Hz_8MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_frequency: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}




// PWM modes
// ---------
class PWM_mode : public timer_::PWM_mode<Timer1>{
public:
    bool fast_mode; // if false, then mode == phase_mode
		    
    // Experimental: por eso nombre tan raro.
    // Para ver el método 1 ver util/timer1/main.cpp
    void calculate_cfg_method2(const Frequency& freq_clk,
			       const Frequency& freq_gen)
    { calculate_cfg_method2(freq_clk.value(), freq_gen.value());}

    void calculate_cfg_method2(const Frequency::Rep& freq_clk,
			       const Frequency::Rep& freq_gen);
};

}// namespace timer1_



/***************************************************************************
 *			    Time_counter1
 ***************************************************************************/
/// Un Timer_counter se limita a contar microsegundos o milisegundos. Su rango
/// de valores será max_top, no más. No sirve para contar tiempo, pero son
/// ideales para medir/generar pulsos de electrónica. 
class Time_counter1{
public:
// Tipo de tipo
//  DUDA: ¿existe alguna función que tengan todos los dispositivos con doble
//  interfaz unsafe_/safe_ que se pueda usar para definir el concept
//  Unsafe_device? Como de momento no lo se, uso temporalmente (???)
//  is_unsafe para indicar el doble interfaz.
    static constexpr bool is_unsafe = true;

// Types
// -----
    using Timer        = avr_::Timer1;
    using counter_type = typename Timer::counter_type;
    using Disable_interrupts = avr_::Disable_interrupts;
    static constexpr counter_type minus_one = static_cast<counter_type>(-1); 

// Initialization
// --------------
/// De momento el interfaz es static. Prohibo su construcción.
    Time_counter1() = delete;

/// Modo de funcionamiento: contador normal y corriente.
    static void unsafe_init(counter_type top0 = max_top()) 
    { 
	Timer::CTC_mode_top_OCRA();
	unsafe_reset();
	unsafe_top(top0);
    }


    // DUDA: Creo que la forma más sencilla de hablar es usar funciones 
    // unsafe_init/init; unsafe_value/value, ... en lugar de usar las
    // correspondientes safe_init, safe_value, ...
    // TODO: Después de usar esta clase si veo que no uso el interfaz safe_
    // borrarlo a favor del mismo sin safe_ (o al contrario, dejar safe_ si es
    // el que se usa y borrar el otro)
    static void safe_init(counter_type top0 = max_top()) 
    { 
	Disable_interrupts l;
	unsafe_init(top0);
    }

    static void init(counter_type top0 = max_top()) 
    { safe_init(top0); }


// Interrupts
// ----------
    /// Hay veces que puede ser interesante controlar cuándo el contador hace
    /// overflow. El único problema es que hay que definir la interrupción
    /// ISR_TIMER1_OVF que depende del Timer1 y no es genérico.
    // ¿Cómo independizarlo del avr este ISR_TIMER1_OVF? <-- En el dev.h
    // al seleccionar el Time_counter1 se puede definir
    // ISR_GENERIC_TIMER_OVF = ISR_TIMER1_OVF.

// DUDA:enable_top_interrupt() vs enable_interrupt()
//      Me gusta más la segunda, a fin de cuentas es la única interrupción del
//      dispositivo. Pero enable_top_interrupt() es más descriptiva, le
//      recuerda al usuario de esta clase que la interrupción salta al llegar
//      al top. ¿Lo más dificil en programación? ¡Elegir los nombres! @_@
    // Genera una interrupción al alcanzar el top.
    /// Se captura con ISR_TIMER1_COMPA
    static void enable_top_interrupt()
    { Timer::enable_output_compare_A_match_interrupt();}

    static void disable_top_interrupt()
    { Timer::disable_output_compare_A_match_interrupt();}

// Timer on/off
// ------------
    /// Enciende el contador. El periodo interno del timer es el indicado por
    /// period_in_us. 
    /// Ejemplo: si period_in_us = 1, el contador avanzará 1 cada
    /// microsegundo.
    ///
    /// Esta función será la típica usada en los Miniclocks que miden el
    /// tiempo directamente del contador.
    // Leer notas en Time_counter0
    template<uint16_t period_in_us
	    , uint32_t clock_frequency_in_hz = avr_::clock_frequency_in_hz>
    struct turn_on_with_clock_period_of{
	static void us()
	{timer1_::set_clock_period_in_us<period_in_us, clock_frequency_in_hz>();}
    };


    /// Enciende el contador configurándolo de tal manera que genera un
    /// overflow cada segundo.
    /// 
    /// Esta función es típica para que la llame Clock_s que tiene que
    /// capturar la interrupción ISR_TIMER1_COMPA cada 1 segundo. El cliente
    /// llama a esta función y garantiza que el timer queda correctamente
    /// configurado para generar esas interrupciones. 
    /// Devuelve el número de ticks (veces que hay que llamar a la interrción
    /// para que se haya alcanzado 1s)
    /// Recordar que el cliente es responsable de llamar a `enable_interrupts()`
    /// para que funcione.
    template <uint32_t clock_frequency_in_hz = clock_frequency_in_hz>
    static counter_type turn_on_with_overflow_to_count_1s();

    template <uint32_t clock_frequency_in_hz = clock_frequency_in_hz>
    static void turn_on_with_overflow_every_1ms();

    /// Apagamos el contador
    static void turn_off() { Timer::off(); }


// Value
// -----
    /// Devuelve el valor del contador en ticks.
    /// Versión fast: no bloquea las interrupciones. El usuario es responsable
    /// de bloquear las interrupciones antes de llamar a esta función.
    /// Ejemplo: si se llama dentro de una ISR el mcu bloquea las
    /// interrupciones por defecto, luego es mejor llamar esta versión fast
    static counter_type unsafe_value()
    { return Timer::unsafe_counter(); }
    
    /// Devuelve el valor del contador en ticks.
    /// Esta versión bloquea las interrupciones: fácil de usar, pero más
    /// lento.
    static counter_type safe_value()
    {  
	Disable_interrupts l;
	return unsafe_value();
    }

    static counter_type value() {return safe_value();}

    /// Hace que el counter = 0.
    static void unsafe_reset() { Timer::unsafe_counter(0); }
    static void safe_reset() 
    { 
	Disable_interrupts l;
	unsafe_reset();
    }
    static void reset()  {safe_reset();}

// Top
// ---
    /// Define el top del counter.
    static void unsafe_top(counter_type top)
    {Timer::unsafe_output_compare_register_A(top);}

    static void safe_top(counter_type top)
    {
	Disable_interrupts l;
	unsafe_top(top);
    }

    static void top(counter_type top0) {safe_top(top0);}

    /// Valor del top
    static counter_type unsafe_top()
    { return Timer::unsafe_output_compare_register_A(); }

    static counter_type safe_top()
    { 
	Disable_interrupts l;
	return Timer::unsafe_output_compare_register_A(); 
    }

    static counter_type top() {return safe_top();}


// Max top
// -------
    /// Valor máximo que puede tener el top.
    static constexpr counter_type max_top()
    { return Timer::max(); }
};


// Turn on
// -------
// (RRR) Números mágicos:
//	 Para 1 MHz: 15.625 * 64 us = 1.000.000 us = 1 s
template <uint32_t clock_frequency_in_hz>
Time_counter1::counter_type 
Time_counter1::turn_on_with_overflow_to_count_1s()
{
// cfg_overflow_every_1s();
    if constexpr (clock_frequency_in_hz == 1'000'000ul){
	init(15'625);
	Timer::clock_frequency_divide_by_64();

	enable_top_interrupt();
	return 1; // 1 
    }

    else
        static_assert(atd::always_false_v<int>,
                      "turn_on_with_overflow_to_count_1s: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");


    return minus_one; // Nunca debería de llegar aquí
}


// (RRR) Números mágicos:
//	 Para 1 MHz: 125 * 8 us = 1.000 us = 1 ms
template <uint32_t clock_frequency_in_hz>
inline void Time_counter1::turn_on_with_overflow_every_1ms()
{
// cfg_overflow_every_1s();
    if constexpr (clock_frequency_in_hz == 1'000'000ul){
	init(125);
	Timer::clock_frequency_divide_by_8();
    }

    else
        static_assert(atd::always_false_v<int>,
                      "turn_on_with_overflow_to_count_1s: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");

    enable_top_interrupt();

}


/***************************************************************************
 *			Square_wave_generator1_g
 ***************************************************************************/
// TODO: obsoleta (???) Usar mejor SWG1_pin
class Square_wave_generator1_g{
public:
// Hardware device
// ---------------
    using Timer        = avr_::Timer1;

// Características del Timer
// -------------------------
    static constexpr uint8_t number_of_pins = cfg::timer1::number_of_pins;

    // pin[0], pin[1], ..., pin[number_of_pins - 1] = pins a los que podemos
    // conectar el SWG
    static constexpr uint8_t pin[] = {Timer::OCA_pin(), Timer::OCB_pin()};

    /// Nos indica si el pin número `n` es uno de los pines del SQ
    static constexpr bool is_pin(uint8_t n) 
				    { return (n == pin[0] or n == pin[1]);}

// Initialization
// --------------
    Square_wave_generator1_g() = delete;
    // init(): no hay


// Stop
// ----
    /// Apagamos el generador de señales.
    // DUDA: stop() or off()??? Hoy me suena mejor stop: generate/stop
    //       Pero ¿qué es lo que haces? Paras el dispositivo pero lo dejas
    //       encendio, o lo apagas? El primer caso sería `stop`, el segundo
    //       sería `turn_off` (DUDA2: ¿es correcto en inglés `turn on/off a
    //       timer? 
    static void stop();


// Static interface
// ----------------
    /// Genera la frecuencia indicada en el pin indicado.
    /// La frecuencia pasada tiene que estar en herzios.
    // DUDA: usar Frequency en lugar de uint32_t? Sería lo mejor...
    template <uint8_t npin>
    static void generate(uint32_t freq_in_Hz);

    template <int npin>
    static void connect_pin();

    template <int npin>
    static void disconnect_pin();


// Dynamic interface
// -----------------
    /// Genera la frecuencia indicada en el pin indicado.
    /// La frecuencia pasada tiene que estar en herzios.
    // DUDA: usar Frequency en lugar de uint32_t? Sería lo mejor...
    static void generate(uint32_t freq_in_Hz, uint8_t npin);

    static void connect_pin(uint8_t npin);

    static void disconnect_pin(uint8_t npin);

    static void disconnect_all_pins();

private:
    using counter_type = typename Timer::counter_type;
    using Disable_interrupts = avr_::Disable_interrupts;

// Funciones de ayuda
    static void init(){ Timer::CTC_mode_top_ICR();}

    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = clock_frequency_in_hz>
    static void turn_on()
    {timer1_::set_clock_period_in_us<period, clock_frequency_in_hz>();}

    static void top(counter_type x)
    { 
	Disable_interrupts l;
	Timer::unsafe_input_capture_register(atd::to_integer<counter_type>(x));
    }

    static counter_type top() // = safe_top()
    {
	Disable_interrupts l;
	return Timer::unsafe_input_capture_register();
    }

    /// Devuelve el valor mínimo que puede tomar el top
    static constexpr counter_type min_top()
    { return Timer::bottom();}

    /// Devuelve el valor máximo que puede tomar el top
    static constexpr counter_type max_top()
    { return Timer::max();}

// Funciones no genéricas: conocen cómo funciona el Timer0
    /// Devuelve la frecuencia, en Hz,  que se genera dados 
    /// el prescaler factor d (divisor de frecuencia) y el top M. 
    /// (pag 132 datasheet)
    template <uint32_t f_clock_in_Hz = clock_frequency_in_hz>
    static constexpr 
    uint32_t prescaler_top_to_frequency_in_Hz(uint32_t d, uint32_t M)
    { return avr_::timer_::CTC_mode::
	    prescaler_top_to_frequency_in_Hz<f_clock_in_Hz>(d, M);}

    /// Función inversa a la prescaler_top_to_frequency_in_Hz:
    /// Devuelve el par (prescaler factor, top) necesario para generar la
    /// frecuencia freq_in_Hz.
    template <uint32_t f_clock_in_Hz = clock_frequency_in_hz>
    static constexpr 
    std::pair<uint32_t, uint32_t> 
    frequency_in_Hz_to_prescaler_top(uint32_t freq_in_Hz)
    { return avr_::timer_::CTC_mode::
    frequency_in_Hz_to_prescaler_top<Timer, f_clock_in_Hz>(freq_in_Hz); 
    }
};

inline void Square_wave_generator1_g::stop()
{ 
    Timer::off(); 
    disconnect_all_pins();
}

template <int npin>
inline void Square_wave_generator1_g::connect_pin()
{
    if constexpr (npin == pin[0])
    { Timer::CTC_pin_A_toggle_on_compare_match(); }

    else if constexpr (npin == pin[1])
    { Timer::CTC_pin_B_toggle_on_compare_match(); }

    else
    {static_assert(atd::always_false_v<int>, "npin out of range");}

}


template <int npin>
inline void Square_wave_generator1_g::disconnect_pin()
{
    if constexpr (npin == pin[0])
    { Timer::pin_A_disconnected();}

    else if constexpr (npin == pin[1])
    { Timer::pin_B_disconnected(); }

    else
    {static_assert(atd::always_false_v<int>, "npin out of range");}

}

inline void Square_wave_generator1_g::connect_pin(uint8_t npin)
{
    if      (npin == pin[0]) connect_pin<pin[0]>();
    else if (npin == pin[1]) connect_pin<pin[1]>();
}

inline void Square_wave_generator1_g::disconnect_pin(uint8_t npin)
{
    if      (npin == pin[0]) disconnect_pin<pin[0]>();
    else if (npin == pin[1]) disconnect_pin<pin[1]>();
}

inline void Square_wave_generator1_g::disconnect_all_pins()
{
    disconnect_pin<pin[0]>();
    disconnect_pin<pin[1]>();
}

/***************************************************************************
 *				SWG_pin
 ***************************************************************************/
/*!
 *  \brief pin donde se genera una señal cuadrada 
 *	    
 *  Esta clase oculta la configuración del Timer1 para generar ondas
 *  cuadradas.
 *
 *  Leer el comentario a PWM1_pin, es aplicable casi todo aquí.
 *
 */
template <uint8_t npin0>
//    requires (npin0 == Timer1::OCA_pin() or npin0 == Timer1::OCB_pin())
//    En g++ 13 si defino la clase con requires luego todas las
//    implementaciones tienen que llevar ese require ==> hay que modificar
//    todas las funciones definidas fuera de la clase para añadirles el
//    requires @_@ <-- por eso, por pereza prefiero usar el static_assert
class SWG1_pin{
    static_assert(npin0 == Timer1::OCA_pin() or npin0 == Timer1::OCB_pin());
public:
// types
    using Timer        = avr_::Timer1;
    using SW_signal    = avr_::SW_signal;
    using counter_type = Timer::counter_type;
    using NPulses_t    = uint16_t;
    using Frequency    = avr_::Frequency;
    using Hertz	       = avr_::Hertz;

// cfg
    static constexpr uint8_t number  = npin0;
    static constexpr Frequency clock_frequency = avr_::clock_frequency;

// basic interface
    // Genera la señal SW indicada. Enciende el Timer en caso de que
    // estuviera apagado. Cuidado si se manejan los dos pins asociados a ese
    // timer: cambiará la frecuencia del otro pin.
    // [in/out] SW_signal indica la señal que se quiere generar.
    //          Como no es posible generar cualquier señal, como salida indica
    //          la señal que realmente se va a generar. <-- TODO: falta
    static void generate(const SW_signal& sw);

    // Genera un número fijo de pulsos de la SW_signal.
    // IMPORTANTE: Para poder usar esta función:
    //	1) Activar las interrupciones globalmente.
    //	2) Implementar la función: 
    //		    ISR_TIMER1_CAPT {SWG1_pin::handle_interrupt();}
    static void generate(const SW_signal& sw, const NPulses_t& npulses);

    // Conecta el pin al Timer.
    // Al llamar a generate se hace de forma automática.
    // Esto se podría usar para, sin llamar a generate, conectar y desconectar
    // el pin a la señal generada. ¿Servirá para algo? El uso lo dirá.
    static void connect();

    // Desconecta el pin del SWG sin modificar el estado del Timer
    // No modifico el estado del Timer por si acaso se está generando en el
    // otro pin del Timer1 una señal.
    static void disconnect();

    // Paramos el Timer
    // DUDA: Ahora al llamar a stop() está dejando el pin que estaba conectado
    // en HIGH. ¿poner el pin como salida por defecto en 0 o que lo gestione
    // el cliente?
    static void stop();

    // Podemos usarlo como un pin normal y corriente de salida.
    // Corresponderían a los casos extremos de 0% ó 100% de duty cycle.
    static void write_zero();
    static void write_one();

// Interrupts
    // La interrupción que usa esta clase es
    //	    ISR_TIMER1_CAPT
    static void handle_interrupt();
private:
// Data
    // TODO(DUDA?): SWG1_pin<15> y SWG1_pin<16> son dos clases distintas, y
    // por tanto cada una de ellas tiene su propio npulses_. Sin embargo las
    // dos clases están acopladas: si se generan 10 pulsos a 100Hz en el
    // SWG1_pin<15> también se van a generar esos mismos pulsos en
    // SWG1_pin<16>. Una optimización sería que npulses_ sea común a ambas
    // clases, lo mismo que la clase generate(sw, npulses); (<-- ahora se
    // duplica el código de esta función si se crean las dos clases).
    // Una posible forma de hacerlo sería con una clase padre que defina las
    // partes en común. ¿Merece la pena hacerlo? Se crearán dos clases
    // asociadas a los pines 15 y 16 a la vez? Están acopladas ... Usémoslo y
    // ya veremos.
    //
    // (RRR) volatile porque se modifica en interrupción
    inline static volatile NPulses_t npulses_; 

// Helpers
    static void pin_as_output();
}; 


template <uint8_t n>
void SWG1_pin<n>::generate(const SW_signal& sw)
{
    write_zero(); // garantizamos empezar a contar correctamente (para
		  // npulses_)

    using CTC = timer_::CTC_mode;
    auto [d, t] = CTC::frequency_to_prescaler_top
			    <Timer, clock_frequency.value()> (sw.frequency());

    Timer::CTC_mode_top_ICR();

    {
	Disable_interrupts l;

	Timer::unsafe_counter(0); 
	Timer::unsafe_input_capture_register(atd::to_integer<counter_type>(t));
    }

    connect();
    Timer::prescaler(d); // esto enciende el Timer
}


template <uint8_t n>
inline 
void SWG1_pin<n>::generate(const SW_signal& sw, const NPulses_t& npulses)
{
    if (npulses == 0)
	return;

    if (atd::overflow<NPulses_t>(2 * npulses)){
	// error("SWG1_pin::generate: overflow");
	return; // Al no hacer nada el programador detectará, espero, el error
    }

    npulses_ = 2*npulses;
    generate(sw);
    Timer::enable_input_capture_interrupt();
}

template <uint8_t n>
inline 
void SWG1_pin<n>::handle_interrupt()
{
    npulses_ = npulses_ - 1;
    
    if (npulses_ == 0){
	stop();
	Timer::disable_input_capture_interrupt();
    }
}


template <uint8_t n>
inline void SWG1_pin<n>::connect()
{
    if constexpr (number == Timer::OCA_pin())
	Timer::CTC_pin_A_toggle_on_compare_match(); 

    else
	Timer::CTC_pin_B_toggle_on_compare_match(); 
}

template <uint8_t n>
inline void SWG1_pin<n>::disconnect()
{
    if constexpr (number == Timer::OCA_pin())
	Timer::pin_A_disconnected();

    else
	Timer::pin_B_disconnected();
}


template <uint8_t n>
inline void SWG1_pin<n>::stop() 
{ 
    Timer::off(); 
    disconnect();
}


template <uint8_t n>
void SWG1_pin<n>::pin_as_output() {
    disconnect();
    Pin<n>::as_output();
}

template <uint8_t n>
void SWG1_pin<n>::write_zero() {
    pin_as_output();
    Pin<n>::write_zero();
}


template <uint8_t n>
void SWG1_pin<n>::write_one() {
    pin_as_output();
    Pin<n>::write_one();
}


/***************************************************************************
 *				PWM1_pin
 ***************************************************************************/

/*!
 *  \brief  pin donde se genera una señal PWM.
 *
 *	Esta clase pretende ocultar toda la configuración del Timer1 para
 *	generar una señal PWM.
 *
 *	De momento es una implementación mínima, pero la idea es que esta
 *	clase elija la configuración óptima para generar una señal.
 *	El cliente de esta clase no debe de saber nada de cómo se configura el
 *	Timer1 del avr, solo se limita a pedirle que genere una señal PWM de
 *	100kHz de frecuencia y un 30% de duty cycle en el pin indicado y
 *	mágicamente aparecerá esa señal, sin saber nada de la configuración.
 *	
 *	Por ello la meto dentro de generic: permite usar el Timer1 
 *	como generador de señales PWM sin necesidad de saber cómo funciona por
 *	dentro.
 *
 * NOTACIÓN
 *	El 1 del PWM1 indica que se está usando el Timer1.
 *
 *
 * ADVERTENCIA
 *	Los pines OCA y OCB están acoplados. Cambiar la frecuencia en 
 *	uno de ellos, cambia la del otro. Idem si paramos el timer: paramos 
 *	los dos pines a la vez. 
 * (???) ¿Desacoplarlo? ¿Gestionarlo internamente con algo parecido a share_ptr? 
 */
template <uint8_t npin0>
//    requires (npin0 == Timer1::OCA_pin() or npin0 == Timer1::OCB_pin())
//    En g++ 13 si defino la clase con requires luego todas las
//    implementaciones tienen que llevar ese require ==> hay que modificar
//    todas las funciones definidas fuera de la clase para añadirles el
//    requires @_@
class PWM1_pin{
    static_assert(npin0 == Timer1::OCA_pin() or npin0 == Timer1::OCB_pin());
public:
// types
    using Timer        = avr_::Timer1;
    using PWM_signal   = avr_::PWM_signal;

// cfg
    static constexpr uint8_t number  = npin0;
    static constexpr Frequency clock_frequency = avr_::clock_frequency;

// constructor
    PWM1_pin() = delete; // de momento static interface
    static void init() {};// debería registrar los pines que se usa como PWM0?

// basic interface
    // Genera la señal PWM indicada. Enciende el Timer en caso de que
    // estuviera apagado. Cuidado si se manejan los dos pins asociados a ese
    // timer: cambiará la frecuencia del otro pin.
    // [in/out] PWM_signal indica la señal que se quiere generar.
    //          Como no es posible generar cualquier señal, como salida indica
    //          la señal que realmente se va a generar. TODO: se me olvido!!!
    //          (por eso lo pongo temporalmente como const)
    static void generate(const PWM_signal& pwm);

    // Cambia el duty cycle sin modificar la frecuencia
    static void duty_cycle(const atd::Percentage& p);

    // Desconecta el pin del PWM sin modificar el estado del Timer
    // No modifico el estado del Timer por si acaso se está generando en el
    // otro pin del Timer1 una señal.
    static void disconnect();

    // Paramos el Timer
    static void stop();

    // Podemos usarlo como un pin normal y corriente de salida.
    // Corresponderían a los casos extremos de 0% ó 100% de duty cycle.
    static void write_zero();
    static void write_one();

// info
    // Devuelve la frecuencia que se está generando
    static Frequency frequency();

private:
// types
    using counter_type = typename Timer::counter_type;
    using PWM_mode     = timer1_::PWM_mode;

// helpers
    static void pin_as_output();
    static void cfg_duty_cycle(const Timer::counter_type& ocr);
    static counter_type top();
    static void generate_impl(const PWM_signal& pwm);
};


template <uint8_t n>
inline PWM1_pin<n>::counter_type PWM1_pin<n>::top()
{
    Disable_interrupts l;
    return Timer::unsafe_input_capture_register();
}


template <uint8_t n>
void PWM1_pin<n>::cfg_duty_cycle(const Timer::counter_type& ocr)
{
    if constexpr (number == Timer::OCA_pin()){
	Timer::PWM_pin_A_non_inverting_mode(); // DUDA: dar a elegirlo?
	Timer::unsafe_output_compare_register_A(ocr);
    }
    else {
	Timer::PWM_pin_B_non_inverting_mode();
	Timer::unsafe_output_compare_register_B(ocr);
    }
}


template <uint8_t n>
void PWM1_pin<n>::generate(const PWM_signal& pwm)
{
    if (pwm.duty_cycle() == 0){
	write_zero();
	return;
    }

    if (pwm.duty_cycle() == 100){
	write_one();
	return;
    }

    generate_impl(pwm);
}

// En FAST PWM : freq_generada = clock_freq / (prescaler * (top + 1));
// En PHASE PWM: freq_generada = clock_freq / (2 * prescaler * top);
// DUDA: ICR1 no es doble buffered, lo que puede generar el problema de que al
// cambiarlo se genere algún pulso incorrecto. ¿Complicar el código para
// gestionar eso? De momento, no: esto es una versión experimental pensada
// para los motores donde eso no generará problemas. 
template <uint8_t n>
void PWM1_pin<n>::generate_impl(const PWM_signal& pwm)
{
    timer1_::PWM_mode mode;
    mode.calculate_cfg_method2(clock_frequency, pwm.frequency());

    if (mode.fast_mode)  Timer::fast_PWM_mode_top_ICR();
    else		 Timer::PWM_phase_and_frequency_correct_mode_top_ICR();

    { // unsafe operations ==> Disable_interrupts 
    Disable_interrupts l; 
    Timer::unsafe_counter(0); 
    Timer::unsafe_input_capture_register(mode.top);

    Timer::counter_type ocr = pwm.duty_cycle().of(mode.top);
    cfg_duty_cycle(ocr);
    }

    Timer::prescaler(mode.prescaler);
}

template <uint8_t n>
void PWM1_pin<n>::duty_cycle(const atd::Percentage& percentage)
{
    { // unsafe operations ==> Disable_interrupts 
    Disable_interrupts l; 
    auto top = Timer::unsafe_input_capture_register();

    Timer::counter_type ocr = percentage.of(top);
    cfg_duty_cycle(ocr);
    }
}

template <uint8_t n>
inline void PWM1_pin<n>::disconnect()
{
    if constexpr (number == Timer::OCA_pin())
	Timer::pin_A_disconnected();

    else
	Timer::pin_B_disconnected();
}


template <uint8_t n>
inline void PWM1_pin<n>::stop() 
{ 
    Timer::off(); 
    write_zero();   // garantizamos un 0 como estado final
}


template <uint8_t n>
void PWM1_pin<n>::pin_as_output() {
    disconnect();
    Pin<n>::as_output();
}

template <uint8_t n>
void PWM1_pin<n>::write_zero() {
    pin_as_output();
    Pin<n>::write_zero();
}


template <uint8_t n>
void PWM1_pin<n>::write_one() {
    pin_as_output();
    Pin<n>::write_one();
}

//  Esta función es casi idéntica a la de PWM0_pin
template <uint8_t n>
Frequency PWM1_pin<n>::frequency()
{
    using Mode = Timer1::Mode;
    Mode mode  = Timer1::mode();       

    PWM_mode pwm;
    pwm.top       = top();
    pwm.prescaler = Timer1::prescaler();

    if (pwm.prescaler == 0) // timer apagado?
	return Frequency{0}; 

    if (mode == Mode::fast_PWM_top_ICR)
	return pwm.frequency_fast_mode(clock_frequency);

    if (mode == Mode::PWM_phase_and_frequency_correct_top_ICR)
	return pwm.frequency_phase_mode(clock_frequency);

    return Frequency{0}; // 0 marca error en este caso ^_^'
    
}


}// namespace 



#endif



