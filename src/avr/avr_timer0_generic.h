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

#ifndef __AVR_TIMER0_GENERIC_H__
#define __AVR_TIMER0_GENERIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Timer0_generic
 *    
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    15/02/2021 Escrito
 *    26/02/2022 timer_counter
 *    30/10/2022 Generic_timer_counter
 *    07/12/2022 dev::Generic_timer<Timer0> --> avr::Timer0_generic
 *		 ¿Por qué lo había definido como template?
 *		 Lo que quiero definir es un timer que obedece al
 *		 concept "timer". No necesito usar templates para nada.
 *
 *		 square_wave_generate
 *    15/12/2022 Square_wave_generator0_g
 *    02/01/2023 Square_wave_burst_generator0_g
 *
 ****************************************************************************/
#include "avr_timer0_basic.h"
#include "avr_cfg.h"	// MCU_CLOCK_FREQUENCY_IN_HZ

#include <atd_type_traits.h>
#include <tuple>    // std::tie
		    
namespace avr_{

/***************************************************************************
 *			    Timer0_counter_g
 ***************************************************************************/
// Un Timer_counter es un contador de tiempo.
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
// DUDA: Timer0_counter_generic?
//	 Time_counter0_generic?
//	 Time_counter0_generic?
class Time_counter0_generic{
public:
// types
    using Timer        = avr_::Timer0;
    using counter_type = typename Timer::counter_type;

/// De momento el interfaz es static. Prohibo su construcción.
    Time_counter0_generic() = delete;

/// Modo de funcionamiento: contador normal y corriente.
//TODO: rename to cfg. Realmente está configurando este dispositivo para poder
//      usarlo.  init suena a inicializar, a poner valores iniciales
    static void init(counter_type top0 = max_top()) 
    { 
	Timer::mode_CTC();
	reset();
	top(top0);
    }

    /// Hay veces que puede ser interesante controlar cuándo el contador hace
    /// overflow. El único problema es que hay que definir la interrupción
    /// ISR_TIMER0_OVF que depende del Timer0 y no es genérico.
    // ¿Cómo independizarlo del avr este ISR_TIMER0_OVF? <-- En el dev.h
    // al seleccionar el Generic_timer<Timer0> se puede definir
    // ISR_GENERIC_TIMER_OVF = ISR_TIMER0_OVF.
    // Genera una interrupción cuando llega al max_top (hace overflow)
    // Capturarla con ISR_TIMER0_OVF
    static void enable_max_top_interrupt()
    { Timer::enable_overflow_interrupt();}

    static void disable_max_top_interrupt()
    { Timer::disable_overflow_interrupt();}

    // Genera una interrupción cuando llega al top.
    // Capturarla con ISR_TIMER0_COMPA
    static void enable_top_interrupt()
    { Timer::enable_output_compare_A_match_interrupt();}

    static void disable_top_interrupt()
    { Timer::disable_output_compare_A_match_interrupt();}
    
// Timer on/off
// ------------
    template<uint16_t period_in_us
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {Timer::template on<period_in_us, clock_frequency_in_hz>();}

    /// Apagamos el generador de señales.
    static void off() { Timer::off(); }


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
    static void init(){ Timer::mode_CTC();}

    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {Timer::template on<period, clock_frequency_in_hz>();}

    static void top(counter_type x)
    { Timer::output_compare_register_A(x);}

    static counter_type top()
    { return Timer::output_compare_register_A();}

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
    Timer::clock_frequency(prescaler_factor_); // esto enciende el Timer
}


/***************************************************************************
 *				Timer0_generic
 ***************************************************************************/
// DUDA: eliminar Generic_timer a favor de clases particulares. <-- NO lo
// tengo tan claro. A veces creo que es buena idea, otras no.
// Mejor esperar a implementar otros micros y comparar los  timers.
class Timer0_generic{
public:
// types
    using Timer        = avr_::Timer0;
    using counter_type = typename Timer::counter_type;
    using Frequency    = avr_::Frequency;
    using Time         = avr_::Time;
    using Scalar       = Frequency::Scalar; // TODO: es el mismo para Microseconds
                                  // y Hertz. ¿qué nombre común usar? ponerlo en
                                  // avr_types.h

    // PWM configuration 
    enum class Mode {
        fix_0xFF,
        only_channel2
    };

//    // Indica qué canales se conectan a la hora de generar SW
//    // TODO: cambiar. Esto no es genérico. ¿Y si el SWG tiene 3 pines? ¿y 4?
//    // Mejor usar numeros.
//    enum class Connect{
//	only_channel1, only_channel2, channel1_and_2
//    };

// pines 
    static constexpr uint8_t pin_channel1 = Timer::OCA_pin();
    static constexpr uint8_t pin_channel2 = Timer::OCB_pin();

    /// ¿los dos canales funcionan siempre a la misma frecuencia?
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

// Timer counter mode: TODO: borrar esto
// ------------------
// En este modo el timer se limita a contar tiempo. 
    /// Modo de funcionamiento: contador normal y corriente.
//    static void mode_timer_counter(counter_type top = timer_counter_max_top()) 
//    { 
//	Timer::mode_CTC();
//	timer_counter_reset();
//	timer_counter_top(top);
//	Timer::enable_output_compare_A_match_interrupt();
//    }
//
//    /// Devuelve el valor del contador en ticks.
//    static counter_type timer_counter() 
//    { return Timer::counter(); }
//    
//    /// Hace que el counter = 0.
//    static void timer_counter_reset() { Timer::counter(0); }
//
//    /// Define el top del counter.
//    static void timer_counter_top(counter_type top)
//    {Timer::output_compare_register_A(top);}
//
//    /// Valor del top
//    static counter_type timer_counter_top()
//    { return Timer::output_compare_register_A(); }
//
//    /// Valor máximo que puede tener el top.
//    static constexpr counter_type timer_counter_max_top()
//    { return Timer::max(); }

// Square wave mode
// ----------------
//    /// Genera la frecuencia indicada conectándola a los canales indicados.
//    /// La frecuencia pasada tiene que estar en herzios.
//    // DUDA: usar Frequency en lugar de uint32_t? Sería lo mejor...
//    static void square_wave_generate(uint32_t freq_in_Hz, Connect);
//
//    static void mode_square_wave(){ Timer::mode_CTC();}
//
//    static void square_wave_top(Scalar x)
//    { Timer::output_compare_register_A(atd::to_integer<counter_type>(x));}
//
//    static counter_type square_wave_top()
//    { return Timer::output_compare_register_A();}
//
//    /// Devuelve el valor mínimo que puede tomar el top
//    static constexpr counter_type square_wave_min_top()
//    { return Timer::bottom();}
//
//    /// Devuelve el valor máximo que puede tomar el top
//    static constexpr counter_type square_wave_max_top()
//    { return Timer::max();}
//    
//    //static void square_wave_ch1_on() <-- TODO: borrarlo cuando actualize
//                                             //  Timer1
//    static void square_wave_connect_ch1()
//    { Timer::CTC_pin_A_toggle_on_compare_match(); }
//
//    static void square_wave_connect_ch2()
//    { Timer::CTC_pin_B_toggle_on_compare_match(); }
//
//    /// Desconecta el pin ch1 del Timer, sin apagarlo.
//    // La operatividad será: 
//    //	    1. configuras el Square_wave_generator
//    //	    2. conectas el Timer al canal 1 (y/o 2)
//    //	    3. enciendes el Timer
//    //	    ...
//    //	    Puedes desconectar los pines al Timer como quieras
//    static void square_wave_disconnect_ch1()
//    { Timer::pin_A_disconnected();}
//
//
//    /// Desconecta el pin ch2 del Timer, sin apagarlo.
//    static void square_wave_disconnect_ch2()
//    { Timer::pin_B_disconnected(); }



// PWM mode
// --------
    template <uint32_t top>
    static void PWM_mode_fix_frequency() 
    {
	Timer::mode_fast_PWM_top_0xFF();
	mode_ = Mode::fix_0xFF;
    }
    
    static void PWM_mode_variable_pwm_only_channel2()
    { 
	Timer::mode_fast_PWM_top_OCRA();
	mode_ = Mode::only_channel2;
    }


    // Timer0 no suministra esta posibilidad:
    // static void PWM_mode_variable_pwm_both_channels()
    
    // configuración
    static void PWM_top(Scalar top)
    { 
	switch(mode_){

	    case Mode::only_channel2:
                Timer::output_compare_register_A(
					atd::to_integer<counter_type>(top));
		break;

	    case Mode::fix_0xFF:
		// El usuario nunca debe de llamar esta posibilidad.
		break;
        }
    }

    static counter_type PWM_top() 
    {
	switch(mode_){
	    case Mode::only_channel2:
		return Timer::output_compare_register_A();

            case Mode::fix_0xFF: 
                return counter_type{0xFF};
	}

	return 0;
    }

    static void PWM_ch1_duty_top(Scalar duty_top)
    {
	switch(mode_){
	    case Mode::fix_0xFF:
                Timer::output_compare_register_A(
				atd::to_integer<counter_type>(duty_top));
		break;

            case Mode::only_channel2:
		// ERROR: en only_channel2 no se puede fijar el OCRA
		break;
	}
    }

    static counter_type PWM_ch1_duty_top()
    {
	switch(mode_){
	    case Mode::fix_0xFF:
		return Timer::output_compare_register_A();

            case Mode::only_channel2:
		return 0;
	}

	return 0;
    }

    static void PWM_ch2_duty_top(Scalar duty_top)
    {
        Timer::output_compare_register_B(
				    atd::to_integer<counter_type>(duty_top));
    }

    static counter_type PWM_ch2_duty_top()
    {return Timer::output_compare_register_B();}


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

//// Funciones no genéricas: conocen cómo funciona el Timer0
//    /// Devuelve la frecuencia, en Hz,  que se genera dados 
//    /// el prescaler factor d (divisor de frecuencia) y el top M. 
//    /// (pag 132 datasheet)
//    template <uint32_t f_clock_in_Hz = MCU_CLOCK_FREQUENCY_IN_HZ>
//    static constexpr 
//    uint32_t prescaler_top_to_frequency_in_Hz(uint32_t d, uint32_t M)
//    { return avr_::timer_::
//	    timer_prescaler_top_to_frequency_in_Hz<f_clock_in_Hz>(d, M);}
//
//    /// Función inversa a la prescaler_top_to_frequency_in_Hz:
//    /// Devuelve el par (prescaler factor, top) necesario para generar la
//    /// frecuencia freq_in_Hz.
//    template <uint32_t f_clock_in_Hz = MCU_CLOCK_FREQUENCY_IN_HZ>
//    static constexpr 
//    std::pair<uint32_t, uint32_t> 
//    frequency_in_Hz_to_prescaler_top(uint32_t freq_in_Hz)
//    { return avr_::timer_::
//    timer_frequency_in_Hz_to_prescaler_top<Timer, f_clock_in_Hz>(freq_in_Hz); 
//    }

};


// Alias
// -----
// DUDA: usar _g como dispositivos genéricos. Es más corto y se entiende bien
using Time_counter0_g = Time_counter0_generic;

}// namespace 



#endif



