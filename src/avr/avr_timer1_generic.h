// Copyright (C) 2021-2022 Manuel Perez 
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
 *
 ****************************************************************************/
#include "avr_timer1_basic.h"
#include "avr_timern_basic.h"
#include "avr_interrupt.h"
#include "avr_cfg.h"	// MCU_CLOCK_FREQUENCY_IN_HZ
#include "avr_micro.h"
#include "dev_interrupt.h"

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
	case Timer1::Frequency_divisor::undefined	: return 0_us;
    }

    return 0_us;
}

inline Frequency clock_frequency_in_Hz_1MHz()
{
    using namespace literals;
    using Rep = Frequency::Rep;
    switch(Timer1::frequency_divisor()){
	case Timer1::Frequency_divisor::no_preescaling	: return 1_MHz;
	case Timer1::Frequency_divisor::divide_by_8	: return 125_kHz;
	case Timer1::Frequency_divisor::divide_by_64	: return Frequency{15625ul, 0};
	case Timer1::Frequency_divisor::divide_by_256	: return Frequency{Rep{3906ul,25ul}, 0};
	case Timer1::Frequency_divisor::divide_by_1024	: return Frequency{Rep{976ul,56ul}, 0};
	case Timer1::Frequency_divisor::undefined	: return 0_Hz;
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
	case Timer1::Frequency_divisor::undefined	: return 0_us;
    }

    return 0_us;
}

inline Frequency clock_frequency_in_Hz_8MHz()
{
    using namespace literals;
    using Rep = Frequency::Rep;
    switch(Timer1::frequency_divisor()){
	case Timer1::Frequency_divisor::no_preescaling	: return 0_MHz;
	case Timer1::Frequency_divisor::divide_by_8	: return 1_MHz;
	case Timer1::Frequency_divisor::divide_by_64	: return 125_kHz;
	case Timer1::Frequency_divisor::divide_by_256	: return Frequency{Rep{31250ul,0ul}, 0};
	case Timer1::Frequency_divisor::divide_by_1024	: return Frequency{Rep{7812ul,5ul}, 0};
	case Timer1::Frequency_divisor::undefined	: return 0_Hz;
    }

    return 0_Hz;
}





template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
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

}// namespace



/***************************************************************************
 *			    Time_counter1_g
 ***************************************************************************/
/// Un Timer_counter se limita a contar microsegundos o milisegundos. Su rango
/// de valores será max_top, no más. No sirve para contar tiempo, pero son
/// ideales para medir/generar pulsos de electrónica. 
class Time_counter1_g{
public:
// Tipo de tipo
//  DUDA: ¿existe alguna función que tengan todos los dispositivos con doble
//  interfaz unsafe_/safe_ que se pueda usar para definir el concept
//  Unsafe_device? Como de momento no lo se, uso temporalmente (???)
//  is_unsafe para indicar el doble interfaz.
    static constexpr bool is_unsafe = true;

// types
    using Timer        = avr_::Timer1;
    using counter_type = typename Timer::counter_type;
    using Disable_interrupts = dev::Disable_interrupts<Micro>;

/// De momento el interfaz es static. Prohibo su construcción.
    Time_counter1_g() = delete;

/// Modo de funcionamiento: contador normal y corriente.
    static void unsafe_init(counter_type top0 = max_top()) 
    { 
	Timer::mode_CTC_top_OCR1A();
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

	Timer::mode_CTC_top_OCR1A();
	unsafe_reset();
	unsafe_top(top0);
    }

    static void init(counter_type top0 = max_top()) 
    { safe_init(top0); }

    /// Hay veces que puede ser interesante controlar cuándo el contador hace
    /// overflow. El único problema es que hay que definir la interrupción
    /// ISR_TIMER1_OVF que depende del Timer1 y no es genérico.
    // ¿Cómo independizarlo del avr este ISR_TIMER1_OVF? <-- En el dev.h
    // al seleccionar el Generic_timer<Timer1> se puede definir
    // ISR_GENERIC_TIMER_OVF = ISR_TIMER1_OVF.
    // Genera una interrupción al alcanzar el max_top (overflow).
    // Se captura con ISR_TIMER1_OVF
    static void enable_max_top_interrupt()
    { Timer::enable_overflow_interrupt();}

    static void disable_max_top_interrupt()
    { Timer::disable_overflow_interrupt();}

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
    template<uint16_t period_in_us
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() 
    {timer1_::set_clock_period_in_us<period_in_us, clock_frequency_in_hz>();}


    /// Enciende el contador configurándolo de tal manera que genera un
    /// overflow cada segundo.
    /// 
    /// Esta función es típica para que la llame Clock_s que tiene que
    /// capturar la interrupción ISR_TIMER1_COMPA cada 1 segundo. El cliente
    /// llama a esta función y garantiza que el timer queda correctamente
    /// configurado para generar esas interrupciones. 
    /// Recordar que el cliente es responsable de llamar a
    /// `enable_interrupts()` para que funcione.
    // TODO: ¿cómo poder guardar aquí la ISR que tiene que llamar el cliente y
    // que sea eficiente? ¿Algo del tipo: Time_counter1_g::ISR{ ... } ???
    // Claramente no puede ser una macro.
    template <uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on_with_overflow_every_1s();

    template <uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on_with_overflow_every_1ms();

    /// Apagamos el generador de señales.
    static void off() { Timer::off(); }


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

    /// Valor máximo que puede tener el top.
    static constexpr counter_type max_top()
    { return Timer::max(); }
};


// (RRR) Números mágicos:
//	 Para 1 MHz: 15.625 * 64 us = 1.000.000 us = 1 s
template <uint32_t clock_frequency_in_hz>
inline void Time_counter1_g::on_with_overflow_every_1s()
{
// cfg_overflow_every_1s();
    if constexpr (clock_frequency_in_hz == 1'000'000ul){
	init(15'625);
	Timer::clock_frequency_divide_by_64();
    }

    else
        static_assert(atd::always_false_v<int>,
                      "on_with_overflow_every_1s: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");

    enable_top_interrupt();

}


// (RRR) Números mágicos:
//	 Para 1 MHz: 125 * 8 us = 1.000 us = 1 ms
template <uint32_t clock_frequency_in_hz>
inline void Time_counter1_g::on_with_overflow_every_1ms()
{
// cfg_overflow_every_1s();
    if constexpr (clock_frequency_in_hz == 1'000'000ul){
	init(125);
	Timer::clock_frequency_divide_by_8();
    }

    else
        static_assert(atd::always_false_v<int>,
                      "on_with_overflow_every_1s: I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");

    enable_top_interrupt();

}


using Timer_counter1_g = Time_counter1_g;

/***************************************************************************
 *			Square_wave_generator1_g
 ***************************************************************************/
class Square_wave_generator1_g{
public:
// Hardware device
    using Timer        = avr_::Timer1;

// Interfaz static
    Square_wave_generator1_g() = delete;

// Características del Timer
    static constexpr uint8_t number_of_pins = cfg::timer1::number_of_pins;
    static constexpr uint8_t pin[] = {Timer::OCA_pin(), Timer::OCB_pin()};
    static constexpr bool is_pin(uint8_t n) 
    { return (n == pin[0] or n == pin[1]);}

    /// Apagamos el generador de señales.
    // DUDA: stop() or off()??? Hoy me suena mejor stop: generate/stop
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
    using Disable_interrupts = dev::Disable_interrupts<Micro>;

// Funciones de ayuda
    static void init(){ Timer::mode_CTC_top_ICR1();}

    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on()
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
    template <uint32_t f_clock_in_Hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static constexpr 
    uint32_t prescaler_top_to_frequency_in_Hz(uint32_t d, uint32_t M)
    { return avr_::timer_::
	    timer_prescaler_top_to_frequency_in_Hz<f_clock_in_Hz>(d, M);}

    /// Función inversa a la prescaler_top_to_frequency_in_Hz:
    /// Devuelve el par (prescaler factor, top) necesario para generar la
    /// frecuencia freq_in_Hz.
    template <uint32_t f_clock_in_Hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static constexpr 
    std::pair<uint32_t, uint32_t> 
    frequency_in_Hz_to_prescaler_top(uint32_t freq_in_Hz)
    { return avr_::timer_::
    timer_frequency_in_Hz_to_prescaler_top<Timer, f_clock_in_Hz>(freq_in_Hz); 
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




// TODO: eliminar Generic_timer a favor de clases particulares.
// PWM hay que revisarla entera. Un fallo de este
// primer intento es que no se da la duración de la onda cuadrada, solo se
// enciende o se apaga, pero ¿por cuánto tiempo? No se indica.
// De momento se me ocurren 3 formas de generar una onda cuadrada:
//	square_wave<Timer, num_pin>(frecuencia, duracion);
//	    Genera la onda midiendo el tiempo usando el Timer sin usar
//	    interrupciones.
//	    Pros : Se genera en cualquier pin.
//	           Es código genérico.
//	          
//	    Cons : No funciona para frecuencias altas.
//
//	square_wave_con_interrupciones<Timer, num_pin(freq, duracion);
//	    Pros : Genera frecuencias más altas que la versión anterior, en
//	           cualquier pin.
//	    Cons : Usa interrupciones, con lo que no es código genérico.
//	    
//
//	gen::Square_wave_generator<Timer1>(canal1 y/o 2, freq, duracion)
//	    Pros : genera la onda cuadrada por hardware, la mejor que se puede
//	           generar.
//	    Cons : solo se puede usar en los pines a los que está conectado el
//	           Timer: canal1 o 2
//	           Al usar interrupciones no es código genérico.
//
//	    
//  TODO: irlo migrando poco a poco. Lo dejo como ejemplo.
//  Cuando ya esté todo migrado eliminarlo.
class Timer1_g{
public:
// types
    using Timer        = avr_::Timer1;
    using counter_type = typename Timer::counter_type;
    using Frequency    = avr_::Frequency;
    using Time         = avr_::Time;
    using Scalar = Frequency::Scalar; // TODO: es el mismo para Microseconds
                                      // y Hertz. ¿qué nombre común usar?
                                      // ponerlo en avr_types.h

    using Disable_interrupts = dev::Disable_interrupts<Micro>;

    enum class Mode {
        fix_0x00FF, fix_0x01FF, fix_0x03FF,
        only_channel2,
        both_channels
    };

    // pines 
    static constexpr uint8_t pin_channel1 = Timer::OCA_pin();
    static constexpr uint8_t pin_channel2 = Timer::OCB_pin();

    /// Constante que nos indica si los dos canales funcionan siempre a la
    /// misma frecuencia.
    static constexpr bool ch1_and_ch2_same_frequency = true;


// Timer características
    template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static Time clock_period() {return timer1_::clock_period();}

    static Frequency clock_frequency() {return Timer::clock_frequency();}


// Timer on/off
// ------------
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on()
    {timer1_::set_clock_period_in_us<period, clock_frequency_in_hz>();}

    /// Apagamos el generador de señales.
    static void off() { Timer::off(); }



// PWM mode
// --------
    template <uint32_t top>
    static void PWM_mode_fix_frequency()
    {
	if constexpr (top == 0x00FF){
	    Timer::mode_fast_PWM_top_0x00FF();
	    mode_ = Mode::fix_0x00FF;
	}

	else if constexpr (top == 0x01FF){
	    Timer::mode_fast_PWM_top_0x01FF();
	    mode_ = Mode::fix_0x01FF;
	}

	else if constexpr (top == 0x03FF){
	    Timer::mode_fast_PWM_top_0x03FF();
	    mode_ = Mode::fix_0x03FF;
	}

	else
            static_assert(atd::always_false_v<int>,
                          "Incorrect fix_frequency. Possible values: 0x00FF, "
                          "0x01FF or 0x03FF");
    }


    static void PWM_mode_variable_pwm_only_channel2()
    { 
	Timer::mode_fast_PWM_top_OCR1A();
	mode_ = Mode::only_channel2;
    }

    static void PWM_mode_variable_pwm_both_channels()
    { 
	Timer::mode_fast_PWM_top_ICR1();
	mode_ = Mode::both_channels;
    }


    // configuración
    static void PWM_top(Scalar top)
    { 
	Disable_interrupts l; // TODO: safe/unsafe
	switch(mode_){
	    case Mode::only_channel2:
                Timer::unsafe_output_compare_register_A(
					atd::to_integer<counter_type>(top));
                break;

	    case Mode::both_channels:
                Timer::unsafe_input_capture_register(
					atd::to_integer<counter_type>(top));
                break;

	    case Mode::fix_0x00FF:
	    case Mode::fix_0x01FF:
	    case Mode::fix_0x03FF:
		// (???) el cliente no debería de llamar nunca a PWM_top si ha
		//       esta en este modo. Sería ideal poder informar al
		//       usuario de la clase de que se está confundiendo, pero
		//       esta función es dinámica. ¿cómo dar un mensaje de
		//       error? ¿habilitar trazas? (no parece buena idea,
		//       podría crecer mucho el tamaño del programa).
		//       Otra forma podría ser con un errno(). Se fija el
		//       errno aquí y el usuario cuando detecte un error que
		//       lo trace él.
		break;
	}
    }


    static counter_type PWM_top() 
    {
	Disable_interrupts l; // TODO: safe/unsafe
	switch(mode_){
	    case Mode::only_channel2:
		return Timer::unsafe_output_compare_register_A();

	    case Mode::both_channels:
		return Timer::unsafe_input_capture_register();

	    case Mode::fix_0x00FF:
		return counter_type{0x00FF};

	    case Mode::fix_0x01FF:
		return counter_type{0x01FF};

	    case Mode::fix_0x03FF:
		return counter_type{0x03FF};
	}

	return 0;
    }

    static void PWM_ch1_duty_top(Scalar duty_top)
    {
	Disable_interrupts l; // TODO: safe/unsafe
	switch(mode_){
	    case Mode::both_channels:
	    case Mode::fix_0x00FF:
	    case Mode::fix_0x01FF:
	    case Mode::fix_0x03FF:
		Timer::unsafe_output_compare_register_A(
				    atd::to_integer<counter_type>(duty_top));
		break;

	    case Mode::only_channel2:
		// ERROR: en only_channel2 no se puede fijar el OCRA
		break;
	}
    }

    static counter_type PWM_ch1_duty_top()
    {
	Disable_interrupts l; // TODO: safe/unsafe
	switch(mode_){
	    case Mode::both_channels:
	    case Mode::fix_0x00FF:
	    case Mode::fix_0x01FF:
	    case Mode::fix_0x03FF:
		return Timer::unsafe_output_compare_register_A();

	    case Mode::only_channel2:
		// ERROR: en only_channel2 no tiene sentido duty_top
		return 0;
	}

	return 0;
    }

    // En todos los modos de funcionamiento OCRB contiene el duty_top
    static void PWM_ch2_duty_top(Scalar duty_top)
    {
	Disable_interrupts l; // TODO: safe/unsafe
        Timer::unsafe_output_compare_register_B(
				    atd::to_integer<counter_type>(duty_top));
    }

    static counter_type PWM_ch2_duty_top()
    {
	Disable_interrupts l; // TODO: safe/unsafe
	return Timer::unsafe_output_compare_register_B();}


    // modos de funcionamiento
    static void PWM_ch1_non_inverting_mode()
    { Timer::PWM_pin_A_non_inverting_mode();}

    static void PWM_ch1_inverting_mode()
    { Timer:: PWM_pin_A_inverting_mode();}

    static void PWM_ch2_non_inverting_mode()
    { Timer::PWM_pin_B_non_inverting_mode();}

    static void PWM_ch2_inverting_mode()
    { Timer::PWM_pin_B_inverting_mode();}

    static void PWM_ch1_off()
    { Timer::pin_A_disconnected(); }

    static void PWM_ch2_off()
    { Timer::pin_B_disconnected(); }


private:
    inline static Mode mode_;

};

}// namespace 



#endif



