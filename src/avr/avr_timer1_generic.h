// Copyright (C) 2021-2022 A.Manuel L.Perez 
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

#ifndef __AVR_TIMER1_GENERIC_H__
#define __AVR_TIMER1_GENERIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Generic_timer<Timer1>
 *    
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    15/02/2021 v0.0: Escrito
 *    26/02/2022       timer_counter
 *    30/10/2022       Generic_timer_counter
 *    07/12/2022       dev::Generic_timer<Timer0> --> avr::Generic_timer0
 *		       ¿Por qué lo había definido como template?
 *		       Lo que quiero definir es un timer que obedece al
 *		       concept "timer". No necesito usar templates para nada.
 *
 ****************************************************************************/
#include "avr_timer1_basic.h"
#include "avr_interrupt.h"

namespace avr{

/// Un Timer_counter se limita a contar microsegundos o milisegundos. Su rango
/// de valores será max_top, no más. No sirve para contar tiempo, pero son
/// ideales para medir/generar pulsos de electrónica. 
class Generic_timer_counter1{
public:
// types
    using Timer        = avr::Timer1;
    using counter_type = typename Timer::counter_type;

/// De momento el interfaz es static. Prohibo su construcción.
    Generic_timer_counter1() = delete;

/// Modo de funcionamiento: contador normal y corriente.
    static void unsafe_init(counter_type top0 = max_top()) 
    { 
	Timer::mode_CTC_top_OCR1A();
	unsafe_reset();
	unsafe_top(top0);
    }

    static void safe_init(counter_type top0 = max_top()) 
    { 
	avr::Interrupts_lock l;

	Timer::mode_CTC_top_OCR1A();
	unsafe_reset();
	unsafe_top(top0);
    }

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
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {Timer::template on<period, clock_frequency_in_hz>();}

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
	avr::Interrupts_lock l;
	return unsafe_value();
    }

    /// Hace que el counter = 0.
    static void unsafe_reset() { Timer::unsafe_counter(0); }
    static void safe_reset() 
    { 
	avr::Interrupts_lock l;
	unsafe_reset();
    }

    /// Define el top del counter.
    static void unsafe_top(counter_type top)
    {Timer::unsafe_output_compare_register_A(top);}

    static void safe_top(counter_type top)
    {
	avr::Interrupts_lock l;
	unsafe_top(top);
    }

    /// Valor del top
    static counter_type unsafe_top()
    { return Timer::unsafe_output_compare_register_A(); }

    static counter_type safe_top()
    { 
	avr::Interrupts_lock l;
	return Timer::unsafe_output_compare_register_A(); 
    }

    /// Valor máximo que puede tener el top.
    static constexpr counter_type max_top()
    { return Timer::max(); }
};



// TODO: eliminar Generic_timer a favor de clases particulares.
// La parte de onda cuadrada y PWM hay que revisarla entera. Un fallo de este
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
class Generic_timer1{
public:
// types
    using Timer        = avr::Timer1;
    using counter_type = typename Timer::counter_type;
    using Frequency    = avr::Frequency;
    using Time         = avr::Time;
    using Scalar = Frequency::Scalar; // TODO: es el mismo para Microseconds
                                      // y Hertz. ¿qué nombre común usar?
                                      // ponerlo en avr_types.h

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
    static Time clock_period() {return Timer::clock_period();}

    static Frequency clock_frequency() {return Timer::clock_frequency();}


// Timer on/off
// ------------
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {Timer::template on<period, clock_frequency_in_hz>();}

    /// Apagamos el generador de señales.
    static void off() { Timer::off(); }


// Counter mode
// ------------
// Si el timer se puede conectar a una señal de entrada (ICP) este modo
// serviría para contar el número de ticks. En este caso se cuentan ticks, no
// tiempo, por eso este 'counter mode' es diferente del 'timer counter mode'
// que cuenta tiempo.


// TODO: Obsoleto: usar Generic_timer_counter 
//       A día de hoy lo uso en Clock. Reescribir.
// Timer counter mode
// ------------------
// En este modo el timer se limita a contar tiempo. 
    /// Modo de funcionamiento: contador normal y corriente.
    static void mode_timer_counter(counter_type top = timer_counter_max_top()) 
    { 
	Timer::mode_CTC_top_OCR1A();
	timer_counter_reset();
	timer_counter_top(top);
	Timer::enable_output_compare_A_match_interrupt();
    }

    /// Devuelve el valor del contador en ticks.
    static counter_type timer_counter() 
    {
	avr::Interrupts_lock l; // TODO: safe/unsafe
	return Timer::unsafe_counter(); }
    
    /// Hace que el counter = 0.
    static void timer_counter_reset() { 
	avr::Interrupts_lock l; // TODO: safe/unsafe
	Timer::unsafe_counter(0); }

    /// Define el top del counter.
    static void timer_counter_top(counter_type top)
    {
	avr::Interrupts_lock l; // TODO: safe/unsafe
	Timer::unsafe_output_compare_register_A(top);}

    /// Valor del top
    static counter_type timer_counter_top()
    { 
	avr::Interrupts_lock l; // TODO: safe/unsafe
	return Timer::unsafe_output_compare_register_A(); }

    /// Valor máximo que puede tener el top.
    static constexpr counter_type timer_counter_max_top()
    { return Timer::max(); }



// Square wave mode
// ----------------
    static void mode_square_wave(){ Timer::mode_CTC_top_ICR1();}

    // TODO: cambiar a safe/unsafe
    static void square_wave_top(Scalar x)
    { 
	avr::Interrupts_lock l;
	Timer::unsafe_input_capture_register(atd::to_integer<counter_type>(x));}

    /// Devuelve el valor mínimo que puede tomar el top
    static constexpr counter_type square_wave_min_top()
    { return Timer::bottom();}

    /// Devuelve el valor máximo que puede tomar el top
    static constexpr counter_type square_wave_max_top()
    { return Timer::max();}

    // TODO: cambiar a safe/unsafe
    static counter_type square_wave_top()
    {
	avr::Interrupts_lock l;
	return Timer::unsafe_input_capture_register();}
    
    static void square_wave_ch1_on()
    { Timer::CTC_pin_A_toggle_on_compare_match(); }

    static void square_wave_ch2_on()
    { Timer::CTC_pin_B_toggle_on_compare_match(); }

    static void square_wave_ch1_off()
    { Timer::pin_A_disconnected(); }

    static void square_wave_ch2_off()
    { Timer::pin_B_disconnected(); }


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
	avr::Interrupts_lock l; // TODO: safe/unsafe
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
	avr::Interrupts_lock l; // TODO: safe/unsafe
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
	avr::Interrupts_lock l; // TODO: safe/unsafe
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
	avr::Interrupts_lock l; // TODO: safe/unsafe
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
	avr::Interrupts_lock l; // TODO: safe/unsafe
        Timer::unsafe_output_compare_register_B(
				    atd::to_integer<counter_type>(duty_top));
    }

    static counter_type PWM_ch2_duty_top()
    {
	avr::Interrupts_lock l; // TODO: safe/unsafe
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



