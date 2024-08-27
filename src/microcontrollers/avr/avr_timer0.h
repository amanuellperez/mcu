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

#ifndef __AVR_TIMER0_H__
#define __AVR_TIMER0_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Generic views of Timer0.
 *
 *	Leer los comentarios en Timer1.
 *    
 *  HISTORIA
 *    Manuel Perez
 *    15/02/2021 Escrito
 *    26/02/2022 timer_counter
 *    30/10/2022 Generic_timer_counter
 *    07/12/2022 dev::Generic_timer<Timer0> --> avr::Timer0_g
 *		 ¿Por qué lo había definido como template?
 *		 Lo que quiero definir es un timer que obedece al
 *		 concept "time r". No necesito usar templates para nada.
 *
 *		 square_wave_generate
 *    15/12/2022 Square_wave_generator0_g
 *    02/01/2023 Square_wave_burst_generator0_g
 *    06/07/2024 PWM0_pin
 *
 *    TODO: Square_wave_generator0_g -> SWG0_pin
 *          Square_wave_burst_generator0_g -> Burst_SWG0_pin (o algo parecido
 *					      el nombre)
 *
 ****************************************************************************/
#include "avr_timer0_basic.h"
#include "avr_cfg.h"	// clock_frequency_in_hz

#include <tuple>    // std::tie
		    
#include <atd_type_traits.h>
#include <atd_percentage.h>

namespace avr_{

namespace timer0_{

// DUDA: aunque queda muy bien que devuelvan Time y Frequency esto es un
// traductor y es de bajo nivel. Time y Frequency generan un montón de
// dependencias que no deben de tener los traductores. ¿Cambiarlo a uint32_t y
// que todo sea en microsegundos y herzios?

// avr clock at 1MHz
// -----------------
template<uint16_t period>
inline void set_clock_period_in_us_1MHz() 
{
    if constexpr (period == 1u)
	Timer0::clock_frequency_no_preescaling();
    
    else if constexpr (period == 8u)
	Timer0::clock_frequency_divide_by_8();

    else if constexpr (period == 64u)
	Timer0::clock_frequency_divide_by_64();

    else if constexpr (period == 256u)
	Timer0::clock_frequency_divide_by_256();

    else if constexpr (period == 1024u)
	Timer0::clock_frequency_divide_by_1024();

    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect Timer0 period. Try another one. "
		    "Valid ones: 1, 8, 64, 256 or 1024.");
}

// avr clock at 8MHz
// -----------------
// a 125 ns
//template<>
//inline void Timer0::clock_period_in_ns<125u, 8000000UL>() 
//{clock_frequency_no_prescaling();}

template<uint16_t period>
inline void set_clock_period_in_us_8MHz() 
{
    if constexpr (period == 1u)
	Timer0::clock_frequency_divide_by_8();

    else if constexpr (period == 8u)
	Timer0::clock_frequency_divide_by_64();

    else if constexpr (period == 32u)
	Timer0::clock_frequency_divide_by_256();
 
    else if constexpr (period == 128u)
	Timer0::clock_frequency_divide_by_1024();
 
    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect Timer0 period. Try another one. "
		    "Valid ones: 1, 8, 32 or 128.");
}


template<uint16_t period
	, uint32_t clock_frequency_in_Hz>
inline void set_clock_period_in_us()
{
    if constexpr (clock_frequency_in_Hz == 1000000UL)
	set_clock_period_in_us_1MHz<period>();

    else if constexpr (clock_frequency_in_Hz == 8000000UL)
	set_clock_period_in_us_8MHz<period>();

    else
        static_assert(atd::always_false_v<int>,
                      "set_clock_period_in_us: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}
// TODO: a .cpp?
inline Time clock_period_in_us_1MHz()
{
    using namespace literals;
    switch(Timer0::frequency_divisor()){
	case Timer0::Frequency_divisor::no_preescaling	: return 1_us;
	case Timer0::Frequency_divisor::divide_by_8	: return 8_us;
	case Timer0::Frequency_divisor::divide_by_64	: return 64_us;
	case Timer0::Frequency_divisor::divide_by_256	: return 256_us;
	case Timer0::Frequency_divisor::divide_by_1024	: return 1024_us;
	case Timer0::Frequency_divisor::off		: return 0_us;
    }

    return 0_us;
}


inline Time clock_period_in_us_8MHz()
{
    using namespace literals;
    switch(Timer0::frequency_divisor()){
	case Timer0::Frequency_divisor::no_preescaling	: return 0_us;
	case Timer0::Frequency_divisor::divide_by_8	: return 1_us;
	case Timer0::Frequency_divisor::divide_by_64	: return 8_us;
	case Timer0::Frequency_divisor::divide_by_256	: return 32_us;
	case Timer0::Frequency_divisor::divide_by_1024	: return 128_us;
	case Timer0::Frequency_divisor::off		: return 0_us;
    }

    return 0_us;
}


template<uint32_t clock_frequency_in_Hz = clock_frequency_in_hz>
inline Time clock_period()
{
    if constexpr (clock_frequency_in_Hz == 1000000UL)
	return clock_period_in_us_1MHz();

    else if constexpr (clock_frequency_in_Hz == 8000000UL)
	return clock_period_in_us_8MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_period: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}



inline Frequency clock_frequency_in_Hz_1MHz()
{
    using namespace literals;
//    using Rep = Frequency::Rep;
    switch(Timer0::frequency_divisor()){
	case Timer0::Frequency_divisor::no_preescaling	: return 1_MHz;
	case Timer0::Frequency_divisor::divide_by_8	: return 125_kHz;
	case Timer0::Frequency_divisor::divide_by_64	: 
					    //return Frequency{15625, 0};
					    return Frequency{15625};
	case Timer0::Frequency_divisor::divide_by_256	: 
					//return Frequency{Rep{3906ul,25ul}, 0};
					return Frequency{3906ul};
	case Timer0::Frequency_divisor::divide_by_1024	: 
					//return Frequency{Rep{976ul,56ul}, 0};
					return Frequency{977ul};
	case Timer0::Frequency_divisor::off		: return 0_Hz;
    }

    return 0_Hz;
}


inline Frequency clock_frequency_in_Hz_8MHz()
{
    using namespace literals;
//    using Rep = Frequency::Rep;
    switch(Timer0::frequency_divisor()){
	case Timer0::Frequency_divisor::no_preescaling  : return 0_Hz;
	case Timer0::Frequency_divisor::divide_by_8	: return 1_MHz;
	case Timer0::Frequency_divisor::divide_by_64	: return 125_kHz;
	case Timer0::Frequency_divisor::divide_by_256	: 
				//return Frequency{Rep{31250ul,0ul}, 0};
				return Frequency{31250ul};
	case Timer0::Frequency_divisor::divide_by_1024	: 
				// return Frequency{Rep{7812ul,5ul}, 0};
				return Frequency{7813ul};
	case Timer0::Frequency_divisor::off		: return 0_Hz;
    }

    return 0_Hz;
}


template<uint32_t clock_frequency_in_Hz>
inline Frequency clock_frequency()
{
    if constexpr (clock_frequency_in_Hz == 1000000UL)
	return clock_frequency_in_Hz_1MHz();

    else if constexpr (clock_frequency_in_Hz == 8000000UL)
	return clock_frequency_in_Hz_8MHz();

    else
        static_assert(atd::always_false_v<int>,
                      "clock_frequency: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}


// PWM modes
// ---------
class PWM_cfg : public timer_::PWM_cfg<Timer0>{
public:
    bool fast_mode; // if false, then mode == phase_mode
		    
    void calculate_cfg_top_0xFF(const Frequency& freq_clk,
			       const Frequency& freq_gen)
    { calculate_cfg_top_0xFF(freq_clk.value(), freq_gen.value());}

    void calculate_cfg_top_0xFF(const Frequency::Rep& freq_clk,
			       const Frequency::Rep& freq_gen);

    void calculate_cfg_top_OCRA(const Frequency& freq_clk,
			       const Frequency& freq_gen)
    { calculate_cfg_top_OCRA(freq_clk.value(), freq_gen.value());}

    // Esta función va a intentar generar la frecuencia freq_gen, pero
    // muchas veces no se va a generar exactamente. Por ello devuelve el error
    // que comete (como %)
    atd::Percentage calculate_cfg_top_OCRA(const Frequency::Rep& freq_clk,
			       const Frequency::Rep& freq_gen);

private:
    atd::Percentage calculate_cfg_top_OCRA(const Frequency::Rep& freq_clk,
			        const Frequency::Rep& freq_gen, 
			        const uint16_t& prescaler);
};


}// namespace timer0_



/***************************************************************************
 *			    Time_counter0
 ***************************************************************************/
// Un Time_counter es un contador de tiempo (vamos, un reloj).
// Los periodos ha usar serían:
//	si freq_mcu = 1 MHz, period = 1    = 1 microsegundo
//			or   period = 1024 = 1 milisegundo
//
//  Recordar que el reloj del avr tiene un 10% de incertidumbre, eso quiere
//  decir que el 1024 microsegundos no son realmente 1024 sino que cada tick
//  del contador será de 930 us a 1138 us: en la práctica podemos considerarlo
//  1 ms.
//
//  Al principio pensé en definir un interfaz donde pudieses elegir reloj: 1
//  us ó 1 ms, sin embargo hacerlo así haría que el usuario olvidase de la
//  incertidumbre del reloj del avr. Si se usa un reloj externo más exacto se
//  conseguirán mejores medidas.
//
/// Un Timer_counter se limita a contar microsegundos o milisegundos. Su rango
/// de valores será 255, no más. No sirve para contar tiempo, pero son
/// ideales para medir/generar pulsos de electrónica. 
class Time_counter0{
public:
// types
    using Timer        = avr_::Timer0;
    using counter_type = typename Timer::counter_type;
    static constexpr counter_type minus_one = static_cast<counter_type>(-1); 

/// De momento el interfaz es static. Prohibo su construcción.
    Time_counter0() = delete;

/// Modo de funcionamiento: contador normal y corriente.
    static void init(counter_type top0 = max_top()) 
    { 
	Timer::CTC_mode();
	reset();
	top(top0);
    }

    /// Hay veces que puede ser interesante controlar cuándo el contador hace
    /// overflow. El único problema es que hay que definir la interrupción
    /// ISR_TIMER0_COMPA que depende del Timer0 y no es genérico.
    // Genera una interrupción cuando llega al top.
    // Capturarla con ISR_TIMER0_COMPA
    static void enable_top_interrupt()
    { Timer::enable_output_compare_A_match_interrupt();}

    static void disable_top_interrupt()
    { Timer::disable_output_compare_A_match_interrupt();}
    
// Timer on/off
// ------------
    // (RRR) Cuando se configura el preescaler del timer del avr, este se
    //       enciende ==> configurar el periodo del reloj del timer equivale a
    //       encenderlo.
    //
    //       Podía haber llamado a esta función:
    //		    `Time_counter::cfg<1>()`
    //	    pero no reflejaría el hecho de que lo arranca.
    //
    //	    Opté por llamarla `Time_counter::on<1>();` pero el código resulta
    //	    ilegible. ¿Qué significa ese 1 que se le pasa como parámetro?
    //
    //	    Para que el código sea legible implemento esta estructura. Voy a
    //	    probar a escribir para encender el timer:
    //		    `Time_counter::turn_on_with_clock_period_of<1>::us();`
    //
    //	    Largo pero totalmente legible.
    //		
    // Time_counter::turn_on_with_period_of<1>::us(); 
    template<uint16_t period_in_us
	    , uint32_t clock_frequency_in_Hz = clock_frequency_in_hz>
    struct turn_on_with_clock_period_of{
	static void us()
	{timer0_::set_clock_period_in_us<period_in_us, clock_frequency_in_Hz>();}
    };

    template <uint32_t clock_frequency_in_hz = clock_frequency_in_hz>
    static counter_type turn_on_with_overflow_to_count_1s();

// TODO: escribirla cuando modifique Clock_ms
//    template <uint32_t clock_frequency_in_hz = clock_frequency_in_hz>
//    static void turn_on_with_overflow_every_1ms();


    /// Apagamos el generador de señales.
    static void turn_off() { Timer::off(); }


    /// Devuelve el valor del contador en ticks.
    static counter_type value() 
    { return Timer::counter(); }
    
    /// Hace que el counter = 0.
    static void reset() { Timer::counter(0); }

    /// Define el top del counter.
    static void top(counter_type top)
    {Timer::output_compare_register_A(top);}

    /// Valor del top
    static counter_type top()
    { return Timer::output_compare_register_A(); }

    /// Valor máximo que puede tener el top.
    static constexpr counter_type max_top()
    { return Timer::max(); }
};



// Turn on
// -------
// (RRR) Números mágicos:
//	 Para 1 MHz: 125 * 125 * 64 us = 1.000.000 us = 1 s
template <uint32_t clock_frequency_in_hz>
Time_counter0::counter_type 
Time_counter0::turn_on_with_overflow_to_count_1s()
{
// cfg_overflow_every_1s();
    if constexpr (clock_frequency_in_hz == 1'000'000ul){
	init(125);
	Timer::clock_frequency_divide_by_64();

	enable_top_interrupt();
	return 125;
    }

    else
        static_assert(atd::always_false_v<int>,
                      "turn_on_with_overflow_to_count_1s: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");


    return minus_one; // Nunca debería de llegar aquí
}



/***************************************************************************
 *			Square_wave_generator0_g
 ***************************************************************************/
class Square_wave_generator0_g_base{
public:
// Types
    using Timer        = avr_::Timer0;

// Características del Timer
    static constexpr uint8_t number_of_pins = cfg::timer0::number_of_pins;
    static constexpr uint8_t pin[] = {Timer::OCA_pin(), Timer::OCB_pin()};
    static constexpr bool is_pin(uint8_t n) 
    { return (n == pin[0] or n == pin[1]);}

    /// Apagamos el generador de señales.
    // DUDA: ¿dejar el pin en 0? como input? que lo gestione el cliente?
    static void stop();


// Static interface
// ----------------
    template <int npin>
    static void connect_pin();

    template <int npin>
    static void disconnect_pin();


// Dynamic interface
// -----------------
// TODO: eliminar estos connect. Solo pueden generar errores de programación.
// Se sabe en tiempo de compilación las conexiones hardware.
    static void connect_pin(uint8_t npin);

    static void disconnect_pin(uint8_t npin);

    static void disconnect_all_pins();

protected:
    using counter_type = typename Timer::counter_type;

    // Implementation class 
    Square_wave_generator0_g_base() {}

// Funciones de ayuda
    static void init(){ Timer::CTC_mode();}

    template<uint16_t period_in_us
	    , uint32_t clock_frequency_in_Hz = clock_frequency_in_hz>
    static void turn_on()
    {timer0_::set_clock_period_in_us<period_in_us, clock_frequency_in_Hz>();}

    static void top(counter_type x)
    { Timer::output_compare_register_A(x);}

    static counter_type top()
    { return Timer::output_compare_register_A();}

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

inline void Square_wave_generator0_g_base::stop() 
{ 
    Timer::off(); 
    disconnect_all_pins(); // fundamental: para que el cliente pueda asignar
			   // valores al pin
}


template <int npin>
inline void Square_wave_generator0_g_base::connect_pin()
{
    if constexpr (npin == pin[0])
    { Timer::CTC_pin_A_toggle_on_compare_match(); }

    else if constexpr (npin == pin[1])
    { Timer::CTC_pin_B_toggle_on_compare_match(); }

    else
    {static_assert(atd::always_false_v<int>, "npin out of range");}

}


template <int npin>
inline void Square_wave_generator0_g_base::disconnect_pin()
{
    if constexpr (npin == pin[0])
    { Timer::pin_A_disconnected();}

    else if constexpr (npin == pin[1])
    { Timer::pin_B_disconnected(); }

    else
    {static_assert(atd::always_false_v<int>, "npin out of range");}

}

inline void Square_wave_generator0_g_base::connect_pin(uint8_t npin)
{
    if      (npin == pin[0]) connect_pin<pin[0]>();
    else if (npin == pin[1]) connect_pin<pin[1]>();
}

inline void Square_wave_generator0_g_base::disconnect_pin(uint8_t npin)
{
    if      (npin == pin[0]) disconnect_pin<pin[0]>();
    else if (npin == pin[1]) disconnect_pin<pin[1]>();
}

inline void Square_wave_generator0_g_base::disconnect_all_pins()
{
    disconnect_pin<pin[0]>();
    disconnect_pin<pin[1]>();
}

// (RRR) ¿Por qué Square_wave_generator0_g y Square_wave_burst_generator0_g?
//
//  De momento necesito dos formas de generar ondas cuadradas:
//  (1) En música: generamos ondas cada vez de una frecuencia distinta mínimo
//      de 250 ms de largo. 
//
//  (2) En IR remote control: generamos burst de 38kHz de 500 us de longitud.
//
//  En (1) generamos muchas frecuencias sin importarnos la eficiencia (250 ms
//  para un micro a 1 MHz es una eternidad). En (2) fijamos la frecuencia pero
//  es fundamental que sea eficiente.
//
//  Square_wave_generator es responsable de generar frecuencias distintas.
//  Como cada vez que se llama a `generate` se tiene que calcular la
//  configuración del Timer se introduce en torno a 1 ms de retraso desde que
//  se llama a `generate` hasta que empieza a generarse la señal. Ventaja: muy
//  sencilla de usar. Desventaja: no funciona para generar tren de pulsos (de
//  hecho, al intentar usar esta clase en el IR Remote Control y ver que no
//  funcionaba es cuando he detectado el problema).
//
//  Square_wave_burst_generator es una clase con estado. Se calcula primero la
//  configuración que tiene que tener el Timer para generar la frecuencia
//  deseada (tarda 1 ms) y una vez calculado ya no es necesario recalcularlo
//  con lo que se pueden generar los burst de 500 us (sin el delay de 1 ms que
//  introduce Square_wave_generator).
// TODO: obsoleta: escribir SWG0_pin. Copiar de Timer1.
class Square_wave_generator0_g : public Square_wave_generator0_g_base{
public:
// Interfaz static
    Square_wave_generator0_g() = delete;

// Static interface
// ----------------
//    /// Genera la frecuencia indicada en el pin indicado.
//    /// La frecuencia pasada tiene que estar en herzios.
//    // DUDA: usar Frequency en lugar de uint32_t? Sería lo mejor...
//    template <uint8_t npin>
//    static void generate(uint32_t freq_in_Hz);

// Dynamic interface
// -----------------
    // Generación de una onda cuadrada
    // -------------------------------
    /// Genera la frecuencia indicada en el pin indicado.
    /// La frecuencia pasada tiene que estar en herzios.
    // DUDA: usar Frequency en lugar de uint32_t? Sería lo mejor...
    static void generate(uint32_t freq_in_Hz, uint8_t npin);

};

// Square_wave_burst_generator0_g
// ------------------------------
class Square_wave_burst_generator0_g: public Square_wave_generator0_g_base{
public:
    Square_wave_burst_generator0_g(uint32_t freq_in_Hz);

    template <uint8_t npin>
    void generate_burst();

private:
    uint32_t prescaler_factor_;
    counter_type top_;
};



// (RRR) inline para que sea lo más eficiente posible.
template <uint8_t npin>
inline void Square_wave_burst_generator0_g::generate_burst()
{
    connect_pin<npin>();
    top(top_); 
    Timer::prescaler(prescaler_factor_); // esto enciende el Timer
}

/***************************************************************************
 *				PWM0_pin
 ***************************************************************************/
/*!
 *  \brief  pin donde se genera una señal PWM.
 *
 *	Esta clase pretende ocultar toda la configuración del Timer0 para
 *	generar una señal PWM.
 *
 *
 * NOTACIÓN
 *	El 0 del PWM0 indica que se está usando el Timer0.
 *
 * ADVERTENCIA
 *	Los pines OCA y OCB están acoplados.
 *
 *	En el pin OCA solo se puede generar señales de frecuencia fija, ya que
 *	tenemos que fijar el top a 0xFF. El duty lo fijamos con OCRA.
 *
 *	En el pin OCB podemos generar señales de frecuencia variable, fijando
 *	el top en OCRA, y el duty con OCRB. Observar que al fijar el top a
 *	OCRA prohibimos generar una señal en el pin OCA.
 *
 *	Por ello el criterio usado a la hora de implementar PWM0_pin es:
 *	    1) Si el usuario usa el pin A, fijamos el top a 0xFF: las
 *	       frecuencias que vamos a generar son muy limitadas, pero podemos
 *	       generar señales PWM en los dos pines A y B.
 *
 *	    2) Si el usuario no usa el pin A, solo el B, usamos como top OCRA.
 *	       En este caso el rango de frecuencias es mayor pero solo podemos
 *	       usar el pin B para generar PWM.
 *
 *	La seleccion se hace al llamar a la función init().
 *
 * DUDAS
 *	La implementación se basa en suponer que los pines se van a usar para
 *	genererar PWM. El harwador al principio decidirá que hacer con el
 *	Timer0 y durante el programa no se cambiará su uso. Esto simplifica
 *	mucho el diseño y evita (fijo) muchos dolores de cabeza.
 *
 *	Otra forma de programar sería usar el Timer0 dependiendo del momento
 *	(dependiendo de que el usuario quiera hacer una cosa u otra). Para
 *	hacer eso sería mejor construir las clases PWM0_pin, en lugar de tener
 *	un static interface. A priori, da la impresión de que esta forma de
 *	programar puede generar muchos errores sobre todo si encima usamos
 *	interrupciones. 
 *
 *	¿Cuál de las dos formas es mejor? Modificar la clase para admitir el
 *	segundo estilo es sencillo, bastando con crear un constructor que
 *	llame a init. 
 *	    
 */
namespace impl_of{
// state
    // (RRR) El siguiente flag es para averiguar si el usuario va a usar los
    // dos pines o solo 1. Lo ideal sería que fuese el compilador quien
    // gestionase esto:
    //	    using pin1 = PWM0_pin<OCRA>;
    //	    using pin2 = PWM0_pin<OCRB>;
class PWM0_pin_flags{
public:
    struct Flags{
	bool pin_A_connected : 1;
	bool pin_B_connected : 1;
    };

    inline static Flags flags_{false, false};

    static void pin_A_connected() { flags_.pin_A_connected = true; }
    static void pin_B_connected() { flags_.pin_B_connected = true; }

    static bool is_pin_A_connected() { return flags_.pin_A_connected; }
    static bool is_pin_B_connected() { return flags_.pin_B_connected; }
};

}// impl_of

// Observar que los flags son comunes a PWM0_pin<11> y PWM0_pin<12> pudiendo
// modificarse desde ambas clases.
template <uint8_t npin0>
//    requires (npin0 == Timer1::OCA_pin() or npin0 == Timer1::OCB_pin())
//    En g++ 13 si defino la clase con requires luego todas las
//    implementaciones tienen que llevar ese require ==> hay que modificar
//    todas las funciones definidas fuera de la clase para añadirles el
//    requires @_@
class PWM0_pin : public impl_of::PWM0_pin_flags{
    static_assert(npin0 == Timer0::OCA_pin() or npin0 == Timer0::OCB_pin());
public:
// types
    using Timer        = avr_::Timer0;
    using PWM_signal   = avr_::PWM_signal;

// cfg
    static constexpr uint8_t number  = npin0;
    static constexpr Frequency clock_frequency = avr_::clock_frequency;

// constructor
    PWM0_pin() = delete;    // de momento static interface, aunque dudo...
    static void init();

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

    // Devuelve el duty cycle que está configurado
    static atd::Percentage duty_cycle();

    // Conecta el pin al timer
    static void connect();

    // Desconecta el pin del PWM sin modificar el estado del Timer
    // No modifico el estado del Timer por si acaso se está generando en el
    // otro pin del Timer0 una señal.
    static void disconnect();

    // Paramos el Timer
    static void stop();

    // Podemos usarlo como un pin normal y corriente de salida.
    // Corresponderían a los casos extremos de 0% ó 100% de duty cycle.
    static void write_zero();
    static void write_one();

    static bool is_zero();
    static bool is_one();

// info
    // Devuelve la frecuencia que se está generando
    static Frequency frequency();


// info para el programador
    static constexpr uint8_t pin_number_A() {return Timer0::OCA_pin();}
    static bool top_is_0xFF();

private:
// types
    using counter_type = Timer::counter_type;
    using PWM_cfg     = timer0_::PWM_cfg;

// helpers
    static void pin_as_output();
    static void generate_impl(const PWM_signal& pwm);


    static counter_type OCR();
    static void OCR(const counter_type& ocr);

    static counter_type top();

    static bool is_disconnected();
};

template <uint8_t n>
inline void PWM0_pin<n>::init()
{
    if constexpr (number == Timer::OCA_pin())
	pin_A_connected();
    else
	pin_B_connected();
}

template <uint8_t n>
inline bool PWM0_pin<n>::top_is_0xFF()
{ return is_pin_A_connected(); }

// Si se llama a esta función via el pin A ==> este pin esta conectado
// ==> el top es 0xFF ==> podemos modificar el registro OCRA. Por eso no pongo
// la validación `if(top_is_0xFF()) ...` al modificar OCRA
template <uint8_t n>
void PWM0_pin<n>::OCR(const counter_type& ocr)
{
    if constexpr (number == Timer::OCA_pin()){
//	Timer::PWM_pin_A_non_inverting_mode(); // DUDA: dar a elegirlo?
	Timer::output_compare_register_A(ocr);
    }
    else {
//	Timer::PWM_pin_B_non_inverting_mode();
	Timer::output_compare_register_B(ocr);
    }
}

template <uint8_t n>
PWM0_pin<n>::counter_type PWM0_pin<n>::OCR()
{
    if constexpr (number == Timer::OCA_pin())
	return Timer::output_compare_register_A();

    else 
	return Timer::output_compare_register_B();
}


template <uint8_t n>
void PWM0_pin<n>::generate(const PWM_signal& pwm)
{
    if (pwm.duty_cycle() == atd::Percentage{0}){
	write_zero();
	return;
    }

    if (pwm.duty_cycle() == atd::Percentage{100}){
	write_one();
	return;
    }

    generate_impl(pwm);
}

// En FAST PWM : freq_generada = clock_freq / (prescaler * (top + 1));
// En PHASE PWM: freq_generada = clock_freq / (2 * prescaler * top);
template <uint8_t n>
void PWM0_pin<n>::generate_impl(const PWM_signal& pwm)
{
    timer0_::PWM_cfg mode;

// top 0xFF
    if (top_is_0xFF()){
	mode.calculate_cfg_top_0xFF(clock_frequency, pwm.frequency());

	if (mode.fast_mode)  Timer::fast_PWM_mode_top_0xFF();
	else		     Timer::PWM_phase_correct_mode_top_0xFF();
    
	// top = 0xFF
    }

// top OCRA
    else{ 
	mode.calculate_cfg_top_OCRA(clock_frequency, pwm.frequency());

	if (mode.fast_mode)  Timer::fast_PWM_mode_top_OCRA();
	else		     Timer::PWM_phase_correct_mode_top_OCRA();

	Timer::output_compare_register_A(mode.top);
    }

// common
    Timer::counter(0); 

    counter_type ocr = pwm.duty_cycle().of(mode.top);
    OCR(ocr);
    connect();

    Timer::prescaler(mode.prescaler);
}


template <uint8_t n>
auto PWM0_pin<n>::top() -> counter_type
{
    if (top_is_0xFF())
	return 0xFF;
    else
	return Timer::output_compare_register_A();
}

template <uint8_t n>
void PWM0_pin<n>::duty_cycle(const atd::Percentage& percentage)
{
    auto t = top();

    if (t == 0) return;

    counter_type ocr = percentage.of(t);
    OCR(ocr);
}

template <uint8_t n>
atd::Percentage PWM0_pin<n>::duty_cycle()
{
    auto t = top();
    auto ocr = OCR();

    return atd::Percentage::as_ratio(ocr, t);
}



// TODO: dar a elegir el idle state  (o polarity??? = CPOL?)
template <uint8_t n>
void PWM0_pin<n>::connect()
{
    if constexpr (number == Timer::OCA_pin())
	Timer::PWM_pin_A_non_inverting_mode();
    else 
	Timer::PWM_pin_B_non_inverting_mode();
}


template <uint8_t n>
inline void PWM0_pin<n>::disconnect()
{
    if constexpr (number == Timer::OCA_pin())
	Timer::pin_A_disconnected();

    else
	Timer::pin_B_disconnected();
}

template <uint8_t n>
inline bool PWM0_pin<n>::is_disconnected()
{
    if constexpr (number == Timer::OCA_pin())
	return Timer::is_pin_A_disconnected();

    else
	return Timer::is_pin_B_disconnected();
}

template <uint8_t n>
inline void PWM0_pin<n>::stop() 
{ 
    Timer::off(); 
    write_zero();   // garantizamos un 0 como estado final
}


template <uint8_t n>
void PWM0_pin<n>::pin_as_output() {
    disconnect();
    Pin<n>::as_output();
}

template <uint8_t n>
void PWM0_pin<n>::write_zero() {
    pin_as_output();
    Pin<n>::write_zero();
}


template <uint8_t n>
void PWM0_pin<n>::write_one() {
    pin_as_output();
    Pin<n>::write_one();
}

template <uint8_t n>
bool PWM0_pin<n>::is_zero() 
{
    if (!is_disconnected()) return false;
    return Pin<n>::is_zero();
}

template <uint8_t n>
bool PWM0_pin<n>::is_one() 
{
    if (!is_disconnected()) return false;
    return Pin<n>::is_one();
}

// Observar que no mira si esta encendido o no
// (???) no depende de n esta función, a clase padre.
//       La frecuencia que se genera es la misma en ambos pines.
//  Esta función es casi idéntica a la de PWM1_pin
template <uint8_t n>
Frequency PWM0_pin<n>::frequency()
{
    using Mode = Timer0::Mode;
    Mode mode  = Timer0::mode();       

    PWM_cfg pwm;
    pwm.top       = top();
    pwm.prescaler = Timer0::prescaler();

    if (pwm.prescaler == 0) // timer apagado?
	return Frequency{0}; 

    if (mode == Mode::fast_PWM_top_0xFF or
	mode == Mode::fast_PWM_top_OCRA)
	return pwm.frequency_fast_mode(clock_frequency);

    if (mode == Mode::PWM_phase_correct_top_0xFF or
	mode == Mode::PWM_phase_correct_top_OCRA)
	return pwm.frequency_phase_mode(clock_frequency);

    return Frequency{0}; // 0 marca error en este caso ^_^'
}


}// namespace 



#endif



