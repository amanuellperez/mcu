// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __AVR_SIGNAL_GENERATOR_H__
#define __AVR_SIGNAL_GENERATOR_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Generador de señales
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    30/01/2021 v0.0
 *
 ****************************************************************************/
#include "avr_types.h"	// TODO: realmente será mcu_types.h

#include "avr_timer1_basic.h"	// TODO: eliminar al generalizar

namespace avr{

// TODO: este realmente es dev_signal...
// TODO: usar Generic_timer para parametrizar este signal_generator

/*!
 *  \brief  Generador de señales SF (same frequency both channels)
 *
 *  Es un generador de señales genérico vinculado a un Timer. 
 *  En principio tiene 2 canales, channel 1 and 2.
 *  Que esté vinculado al Timer quiere decir que el Timer se controla por
 *  completo a través del Signal_generator. Esto es, mientras se use este
 *  signal_generator no se debe de usar ese Timer en el resto de la
 *  aplicación.
 *
 *  Al ser genérico no debe de depender de avr.
 *
 */
namespace __avr{
class Signal_generator{
public:
// TODO: esto son parametros de template
    using Timer = Timer1;
    static constexpr uint8_t pin_channel1 = Timer::OCA_pin();
    static constexpr uint8_t pin_channel2 = Timer::OCB_pin();

    /// Constante que nos indica si los dos canales funcionan siempre a la
    /// misma frecuencia.
    static constexpr bool ch1_and_ch2_same_frequency = true;

    /// Apagamos el generador de señales.
    static void off() { Timer::off(); }

    /// Apaga el channel 1.
    static void ch1_off()
    { Timer::pin_A_disconnected(); }

    /// Apaga el channel 2.
    static void ch2_off()
    { Timer::pin_B_disconnected(); }


};

}// namespace __avr


/***************************************************************************
 *			Square_wave_generator
 ***************************************************************************/
class Square_wave_generator: public __avr::Signal_generator{
public:
    /// Define la frecuencia que se genera en ch1 y ch2
    static void frequency(const Hertz& freq_sq);

    /// Frecuencia que genera (en caso de que esté encendido y funcionando).
    static Hertz frequency();

    /// Frecuencia máxima que se puede generar con el Timer usando ese 
    /// clock_period(). Para generar más grandes probar a cambiar 
    /// Timer::clock_period().
    static Hertz max_frequency();

    /// Encendemos el generador de señales
    template<uint16_t period>
    static void on();
    

// square_wave
// -----------
    /// Enciende el channel 1.
    static void ch1_on();

    /// Enciende el channel 2. 
    static void ch2_on();
};


/// Define la frecuencia que se genera en ch1 y ch2
inline void Square_wave_generator::frequency(const Hertz& freq_sq)
{
    Hertz::Scalar two = 2;
    auto top = (Timer::clock_frequency() / (two*freq_sq)) - 1ul;
    Timer::input_capture_register(atd::to_integer<Timer::counter_type>(top));
}

/// Frecuencia que genera (en caso de que esté encendido y funcionando).
inline Hertz Square_wave_generator::frequency()
{
    auto top = Timer::input_capture_register();
    return Timer::clock_frequency() / Hertz::Rep{2*(1ul + top)};
}

inline Hertz Square_wave_generator::max_frequency()
{
    return Timer::clock_frequency() *
	   Hertz::Rep{2 * (1ul + sizeof(Timer::counter_type))};
}


/// Encendemos el generador de señales
template<uint16_t period>
inline void Square_wave_generator::on() 
{ 
    Timer::template on<period>();
    Timer::mode_CTC_top_ICR1();
}


// square_wave
// -----------
/// Enciende el channel 1.
inline void Square_wave_generator::ch1_on()
{ Timer::CTC_pin_A_toggle_on_compare_match(); }


/// Enciende el channel 2. 
inline void Square_wave_generator::ch2_on()
{ Timer::CTC_pin_B_toggle_on_compare_match(); }




/***************************************************************************
 *				PWM_generator
 ***************************************************************************/
class PWM_generator : public __avr::Signal_generator{
public:
    enum class Mode{
	inverting, non_inverting
    };

    /// Encendemos el generador de señales
    template<uint16_t period>
    static void on();

// Frecuencia de funcionamiento del Signal_generator
// -------------------------------------------------
// Recordar que los 2 canales funcionan a la misma frecuencia.

    /// Define la frecuencia que se genera en ch1 y ch2
    static void frequency(const Hertz& freq_sq);

    /// Frecuencia que genera (en caso de que esté encendido y funcionando).
    static Hertz frequency();

    /// Define el periodo a generar de la señal generada en ch1 y ch2.
    static void period(const Microsecond& T_s);

    /// Periodo que genera
    static Microsecond period();


// modos de funcionamiento. Enciende el channel correspondiente
// ------------------------------------------------------------
    static void ch1_non_inverting_mode();
    static void ch1_inverting_mode();
    static void ch1_mode(Mode mode);

    static void ch2_non_inverting_mode();
    static void ch2_inverting_mode();
    static void ch2_mode(Mode mode);

    /// Define el duty_cycle como tanto por cien.
    /// Ejemplo: ch1_duty_cycle(30); => duty_cycle = 30%
    static void ch1_duty_cycle(uint8_t duty_cycle);

    static void ch1_duty_cycle(const Microsecond duty_cycle);

    static void ch2_duty_cycle(uint8_t duty_cycle);
    static void ch2_duty_cycle(const Microsecond duty_cycle);


// syntax sugar
// ------------
    /// Enciende el channel 1, generando una PWM de frecuencia `freq` y
    /// `duty_cycle`. El duty_cycle da el tanto por cien como número entero.
    /// Ejemplo: duty_cycle = 10, es un 10%.
    // (RRR) ¿por qué no pasarle también la frecuencia?
    //       Porque la frecuencia es la misma en los dos canales. Si se la
    //       pasara aquí estaría cambiando también la del otro canal, cosa que
    //       a lo mejor no se busca, generando errores.
    static void ch1_on(uint8_t duty_cycle, Mode mode = Mode::non_inverting);

    static void ch1_on(const Microsecond duty_cycle,
                       Mode mode = Mode::non_inverting);

    static void ch2_on(const uint8_t duty_cycle,
                       Mode mode = Mode::non_inverting);

    static void ch2_on(const Microsecond duty_cycle,
                       Mode mode = Mode::non_inverting);

};





// OJO: es diferente de la de Square_wave_generator, por culpa del 2*
inline void PWM_generator::frequency(const Hertz& freq_sq)
{
    auto top = (Timer::clock_frequency() / freq_sq) - 1ul;
    Timer::input_capture_register(atd::to_integer<Timer::counter_type>(top));
}

/// Frecuencia que genera (en caso de que esté encendido y funcionando).
inline Hertz PWM_generator::frequency()
{
    auto top = Timer::input_capture_register();
    return Timer::clock_frequency() / Hertz::Rep{1ul + top};
}

/// Define el periodo a generar de la señal generada en ch1 y ch2.
inline void PWM_generator::period(const Microsecond& T_s)
{
    auto top = (T_s / Timer::clock_period()) - 1ul;
    Timer::input_capture_register(atd::to_integer<Timer::counter_type>(top));
}

inline Microsecond PWM_generator::period()
{
    auto top = Timer::input_capture_register();
    uint32_t T = (top + 1) * Timer::clock_period_in_us();

    return Microsecond{T};
}


/// Encendemos el generador de señales
template<uint16_t period>
inline void PWM_generator::on() 
{
    Timer::mode_fast_PWM_top_ICR1();
    Timer::template on<period>();
}


// modos de funcionamiento
// -----------------------
inline void PWM_generator::ch1_non_inverting_mode()
{ Timer::PWM_pin_A_non_inverting_mode();}

inline void PWM_generator::ch1_inverting_mode()
{ Timer:: PWM_pin_A_inverting_mode();}

inline void PWM_generator::ch2_non_inverting_mode()
{ Timer::PWM_pin_B_non_inverting_mode();}

inline void PWM_generator::ch2_inverting_mode()
{ Timer::PWM_pin_B_inverting_mode();}

inline void PWM_generator::ch1_mode(Mode mode)
{
    if (mode == Mode::non_inverting)
	Timer::PWM_pin_A_non_inverting_mode();
    else 
	Timer::PWM_pin_A_inverting_mode();

}

inline void PWM_generator::ch2_mode(Mode mode)
{
    if (mode == Mode::non_inverting)
	Timer::PWM_pin_B_non_inverting_mode();
    else 
	Timer::PWM_pin_B_inverting_mode();
}


// duty_cycle
// ----------
inline void PWM_generator::ch1_duty_cycle(uint8_t duty_cycle)
{// uint32_t clave, para que no haya overflow!!!
    uint32_t ocr1a = Timer::input_capture_register();
    ocr1a = (duty_cycle*ocr1a)/100;
    Timer::output_compare_register_A(ocr1a);
}


inline void PWM_generator::ch1_duty_cycle(const Microsecond duty_cycle)
{
    auto ocr1a = duty_cycle / Timer::clock_period();
    Timer::output_compare_register_A(atd::to_integer(ocr1a));
}


inline void PWM_generator::ch2_duty_cycle(uint8_t duty_cycle)
{// uint32_t clave, para que no haya overflow!!!
    uint32_t ocr1b = Timer::input_capture_register();
    ocr1b = (duty_cycle*ocr1b)/100;
    Timer::output_compare_register_B(ocr1b);
}


inline void PWM_generator::ch2_duty_cycle(const Microsecond duty_cycle)
{
    auto ocr1b = duty_cycle / Timer::clock_period();
    Timer::output_compare_register_B(atd::to_integer(ocr1b));
}


// syntax sugar
// ------------
inline void PWM_generator::ch1_on(uint8_t duty_cycle, Mode mode)
{
    ch1_duty_cycle(duty_cycle);
    ch1_mode(mode);
}


inline void PWM_generator::ch1_on(const Microsecond duty_cycle, Mode mode)
{
    ch1_duty_cycle(duty_cycle);
    ch1_mode(mode);
}


inline void PWM_generator::ch2_on(uint8_t duty_cycle, Mode mode)
{
    ch2_duty_cycle(duty_cycle);
    ch2_mode(mode);
}   


inline void PWM_generator::ch2_on(const Microsecond duty_cycle, Mode mode)
{
    ch2_duty_cycle(duty_cycle);
    ch2_mode(mode);
}




}// namespace


#endif


