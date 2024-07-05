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

#ifndef __AVR_TIMER0_GENERIC_H__
#define __AVR_TIMER0_GENERIC_H__
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
 *
 *    TODO: Time_counter0 -> Counter0
 *          Square_wave_generator0_g -> SWG0_pin
 *          Square_wave_burst_generator0_g -> Burst_SWG0_pin (o algo parecido
 *					      el nombre)
 *
 ****************************************************************************/
#include "avr_timer0_basic.h"
#include "avr_cfg.h"	// clock_frequency_in_hz

#include <atd_type_traits.h>
#include <tuple>    // std::tie
		    
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
	case Timer0::Frequency_divisor::undefined	: return 0_us;
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
	case Timer0::Frequency_divisor::undefined	: return 0_us;
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
	case Timer0::Frequency_divisor::undefined	: return 0_Hz;
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
	case Timer0::Frequency_divisor::undefined	: return 0_Hz;
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


}// namespace



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
    Timer::clock_frequency(prescaler_factor_); // esto enciende el Timer
}


/***************************************************************************
 *				Timer0_g
 ***************************************************************************/
// TODO: eliminar Generic_timer a favor de clases particulares. 
class Timer0_g{
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
    template<uint32_t clock_frequency_in_Hz = clock_frequency_in_hz>
    static Time clock_period() {return timer0_::clock_period();}

    static Frequency clock_frequency() {return Timer::clock_frequency();}


// Timer on/off
// ------------
    template<uint16_t period_in_us
	    , uint32_t clock_frequency_in_Hz = clock_frequency_in_hz>
    static void turn_on()
    {timer0_::set_clock_period_in_us<period_in_us, clock_frequency_in_Hz>();}

    /// Apagamos el generador de señales.
    static void turn_off() { Timer::off(); }



// PWM mode
// --------
    template <uint32_t top>
    static void PWM_mode_fix_frequency() 
    {
	Timer::fast_PWM_mode_top_0xFF();
	mode_ = Mode::fix_0xFF;
    }
    
    static void PWM_mode_variable_pwm_only_channel2()
    { 
	Timer::fast_PWM_mode_top_OCRA();
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
//    template <uint32_t f_clock_in_Hz = clock_frequency_in_hz>
//    static constexpr 
//    uint32_t prescaler_top_to_frequency_in_Hz(uint32_t d, uint32_t M)
//    { return avr_::timer_::
//	    timer_prescaler_top_to_frequency_in_Hz<f_clock_in_Hz>(d, M);}
//
//    /// Función inversa a la prescaler_top_to_frequency_in_Hz:
//    /// Devuelve el par (prescaler factor, top) necesario para generar la
//    /// frecuencia freq_in_Hz.
//    template <uint32_t f_clock_in_Hz = clock_frequency_in_hz>
//    static constexpr 
//    std::pair<uint32_t, uint32_t> 
//    frequency_in_Hz_to_prescaler_top(uint32_t freq_in_Hz)
//    { return avr_::timer_::
//    timer_frequency_in_Hz_to_prescaler_top<Timer, f_clock_in_Hz>(freq_in_Hz); 
//    }

};

}// namespace 



#endif



