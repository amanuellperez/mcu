// Copyright (C) 2021 Manuel Perez 
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

#ifndef __DEV_SIGNAL_GENERATOR_H__
#define __DEV_SIGNAL_GENERATOR_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Generador de señales
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    30/01/2021 v0.0
 *
 ****************************************************************************/
#include "generic_devices.h"

#include <stdint.h> // uint8_t
#include <atd_type_traits.h>
#include <atd_cast.h>

namespace dev{

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
template <typename Timer_n>
class Signal_generator_base{
public:
    using GT           = dev::Generic_timer<Timer_n>;
    using counter_type = typename GT::counter_type;
    using Time         = typename GT::Time;
    using Frequency    = typename GT::Frequency;

    static constexpr uint8_t pin_channel1 = GT::pin_channel1;
    static constexpr uint8_t pin_channel2 = GT::pin_channel2;

    /// Constante que nos indica si los dos canales funcionan siempre a la
    /// misma frecuencia.
    static constexpr bool ch1_and_ch2_same_frequency =
						GT::ch1_and_ch2_same_frequency;

    /// Apagamos el generador de señales.
    static void off() { GT::off(); }


// Info
    /// Periodo al que funciona internamente el timer.
    static Time clock_period() 
    {return GT::clock_period();}

    /// Frecuencia a la que funciona internamente el timer.
    static Frequency clock_frequency()
    { return GT::clock_frequency(); }

protected:
    // Clase de implementación.
    Signal_generator_base() {}

};

/*!
 *  \brief  Square_wave_generator
 *
 *  No puede generar una onda cuadrada de cualquier frecuencia, sino solo un
 *  conjunto discreto de frecuencias:
 *		f_min = f_1, f_2, f_3, ..., f_max
 *
 *  Notation:
 *      f[n-1] = previous frequency
 *      f[n]   = frecuencia actual
 *      f[n+1] = next frequency
 *  
 */
template <typename Timer_n>
class Square_wave_generator: public Signal_generator_base<Timer_n>{
public:
    using GT           = dev::Generic_timer<Timer_n>;
    using counter_type = typename GT::counter_type;
    using Time         = typename GT::Time;
    using Frequency    = typename GT::Frequency;
    using Rep	       = typename GT::Frequency::Rep;

    // Frequency
    // ---------
    /// Define la frecuencia que se genera en ch1 y ch2
    static void frequency(const Frequency& freq_sq);

    /// Define la frecuencia a generar como la siguiente a la actual.
    /// En caso de que sea la máxima, no hace nada.
    static void next_frequency();

    /// Define la frecuencia a generar como la anterior a la actual.
    /// En caso de que sea la mínima, no hace nada.
    static void previous_frequency();

// Info
// ----
    /// Frecuencia que genera (en caso de que esté encendido y funcionando).
    static Frequency frequency();

    /// Frecuencia máxima que se puede generar con el Timer usando ese 
    /// clock_period(). Para generar más grandes probar a cambiar 
    /// Timer::clock_period().
    static Frequency max_frequency();

    static Frequency min_frequency();


// on
// --
    /// Encendemos el generador de señales
    template<uint16_t period>
    static void on();
    

// square_wave
// -----------
    // Turn on/off
    static void ch1_on();
    static void ch1_off();

    static void ch2_on();
    static void ch2_off();
};


/// Encendemos el generador de señales
template <typename T>
template<uint16_t period>
inline void Square_wave_generator<T>::on() 
{ 
    GT::template on<period>();
    GT::mode_square_wave();
}

// set frequency
// -------------
template <typename T>
inline void Square_wave_generator<T>::frequency(const Frequency& freq_sq)
{
    Rep top = (GT::clock_frequency() / (2 * freq_sq)) - Rep{1};
    GT::square_wave_top(atd::to_integer<counter_type>(top));
}


template <typename T>
inline void Square_wave_generator<T>::previous_frequency()
{
    counter_type top = GT::square_wave_top();
    if (top < GT::square_wave_max_top())
	GT::square_wave_top(top + 1u);
}

template <typename T>
inline void Square_wave_generator<T>::next_frequency()
{
    counter_type top = GT::square_wave_top();
    if (top > GT::square_wave_min_top())
	GT::square_wave_top(top - 1u);
}


// info
// ----
// CUIDADO: al multiplicar por 2 le podemos hacer overflow al top!!!
template <typename T>
inline Square_wave_generator<T>::Frequency Square_wave_generator<T>::frequency()
{
    atd::same_type_with_double_bits<counter_type> top = GT::square_wave_top();
    return GT::clock_frequency() / (2 * (1u + top));
}

// CUIDADO: al sumarle 1 al max_top() estamos haciendo overflow. Es
// fundamental hacer más grande el tipo de max_top() para garantizar que no
// haya overflow.
template <typename T>
inline Square_wave_generator<T>::Frequency
Square_wave_generator<T>::min_frequency()
{
    using Int = atd::same_type_with_double_bits<counter_type>;
    Int den = Int{2} * (Int{1} + Int{GT::square_wave_max_top()});
    return GT::clock_frequency() / den;
}

template <typename T>
inline Square_wave_generator<T>::Frequency
Square_wave_generator<T>::max_frequency()
{
    return GT::clock_frequency() / (2 * (1u + GT::square_wave_min_top()));
}


// square_wave
// -----------
template <typename T>
inline void Square_wave_generator<T>::ch1_on()
//{ GT::square_wave_ch1_on(); }
{ GT::square_wave_connect_ch1(); }

template <typename T>
inline void Square_wave_generator<T>::ch1_off()
//{ GT::square_wave_ch1_off(); }
{ GT::square_wave_disconect_ch1(); }


template <typename T>
inline void Square_wave_generator<T>::ch2_on()
//{ GT::square_wave_ch2_on(); }
{ GT::square_wave_connect_ch2(); }

template <typename T>
inline void Square_wave_generator<T>::ch2_off()
// { GT::square_wave_ch2_off(); }
{ GT::square_wave_disconect_ch2(); }



/***************************************************************************
 *				PWM_generator
 ***************************************************************************/
template <typename Timer_n>
class PWM_generator : public Signal_generator_base<Timer_n>{
public:
    using GT           = dev::Generic_timer<Timer_n>;
    using counter_type = typename GT::counter_type;
    using Time  = typename GT::Time;
    using Frequency    = typename GT::Frequency;
    using Rep	       = typename GT::Frequency::Rep;

    enum class Mode{
	inverting, non_inverting
    };

    /// Encendemos el generador de señales. 
    /// Esto es, pulsamos el botón 'on'. De momento no hemos elegido modo de
    /// funcionamiento ni nada más. Recordar elegir modo, frecuencia, duty y
    /// luego encender el canal.
    template<uint16_t period>
    static void on();

// Frecuencia de funcionamiento del Signal_generator
// -------------------------------------------------
// Recordar que los 2 canales funcionan a la misma frecuencia.

    /// Define la frecuencia que se genera en ch1 y ch2
    static void frequency(const Frequency& freq_sq);

    /// Frecuencia que genera (en caso de que esté encendido y funcionando).
    static Frequency frequency();

    /// Define el periodo a generar de la señal generada en ch1 y ch2.
    static void period(const Time& T_s);

    /// Periodo que genera
    static Time period();

// configuración
// -------------
    /// Generamos PWM en los dos canales. La frecuencia queda limitada a un
    /// rango de valores suministrados por el Timer.
    template <uint32_t top>
    static void mode_fix_frequency();

    /// Generamos PWM de cualquier frecuencia y duty_cycle, pero solo en 
    /// el channel 2. El channel 1 desconectado. (OJO: se podía hacer un 
    /// toggle en channel 1, implementarlo aquí ???)
    static void mode_variable_pwm_only_channel2();

    /// Generamos PWM de cualquier frecuencia y duty_cycle. Esta función no la
    /// suministan todos los Timers.  EX: Timer0 no dispone de él.
    static void mode_variable_pwm_both_channels();


// modos de funcionamiento. Enciende el channel correspondiente
// ------------------------------------------------------------
    static void ch1_non_inverting_mode();
    static void ch1_inverting_mode();
    static void ch1_on(Mode mode);
    static void ch1_off();

    static void ch2_non_inverting_mode();
    static void ch2_inverting_mode();
    static void ch2_on(Mode mode);
    static void ch2_off();

    /// Define el duty_cycle como tanto por cien.
    /// Ejemplo: ch1_duty_cycle(30); => duty_cycle = 30%
    static void ch1_duty_cycle(uint8_t duty_cycle);
    static void ch2_duty_cycle(const Time& duty_cycle);

    // Define el duty_cycle como el número de microsegundos.
    static void ch1_duty_cycle(const Time& duty_cycle);
    static void ch2_duty_cycle(uint8_t duty_cycle);
    static Time ch1_duty_cycle();
    static Time ch2_duty_cycle();

};


/// Encendemos el generador de señales
template <typename T>
template<uint16_t period>
inline void PWM_generator<T>::on() 
{
    GT::template on<period>();
}



// OJO: es diferente de la de Square_wave_generator, por culpa del 2*
template <typename T>
inline void PWM_generator<T>::frequency(const Frequency& freq_sq)
{
    Rep top = (GT::clock_frequency() / freq_sq) - 1ul;
    GT::PWM_top(atd::to_integer<counter_type>(top));
}

// (RRR) ¿por qué definir como doble el tipo del top?
//       Observar que estamos sumandole 1 al top. Si es el Timer0 el top
//       devuelve un uint8_t igual a 0xFF. Al sumarle 1 pasaría a 0xFF + 1 =
//       0x00!!!
template <typename T>
inline PWM_generator<T>::Frequency PWM_generator<T>::frequency()
{
    atd::same_type_with_double_bits<counter_type> top = GT::PWM_top(); 
    return GT::clock_frequency() / (1u + top);
}

/// Define el periodo a generar de la señal generada en ch1 y ch2.
template <typename T>
inline void PWM_generator<T>::period(const Time& T_s)
{
    Rep top = (T_s / GT::clock_period()) - 1ul;
    GT::PWM_top(atd::to_integer<counter_type>(top));
}

template <typename Timer_n>
inline PWM_generator<Timer_n>::Time PWM_generator<Timer_n>::period()
{
    counter_type top = GT::PWM_top();
    Time T = (top + 1) * GT::clock_period();

    return T;
}


// configuración
// -------------
template <typename T>
template <uint32_t top>
inline void PWM_generator<T>::mode_fix_frequency()
{ GT::template PWM_mode_fix_frequency<top>(); }


template <typename T>
inline void PWM_generator<T>::mode_variable_pwm_only_channel2()
{ GT::PWM_mode_variable_pwm_only_channel2();}


template <typename T>
inline void PWM_generator<T>::mode_variable_pwm_both_channels()
{ GT::PWM_mode_variable_pwm_both_channels();}


// modos de funcionamiento
// -----------------------
template <typename T>
inline void PWM_generator<T>::ch1_non_inverting_mode()
{ GT::PWM_ch1_non_inverting_mode();}

template <typename T>
inline void PWM_generator<T>::ch1_inverting_mode()
{ GT::PWM_ch1_inverting_mode();}

template <typename T>
inline void PWM_generator<T>::ch2_non_inverting_mode()
{ GT::PWM_ch2_non_inverting_mode();}

template <typename T>
inline void PWM_generator<T>::ch2_inverting_mode()
{ GT::PWM_ch2_inverting_mode();}

template <typename T>
inline void PWM_generator<T>::ch1_on(Mode mode)
{
    if (mode == Mode::non_inverting)
	ch1_non_inverting_mode();
    else 
	ch1_inverting_mode();

}

template <typename T>
inline void PWM_generator<T>::ch2_on(Mode mode)
{
    if (mode == Mode::non_inverting)
	ch2_non_inverting_mode();
    else 
	ch2_inverting_mode();
}

template <typename T>
inline void PWM_generator<T>::ch1_off()
{ GT::PWM_ch1_off(); }

template <typename T>
inline void PWM_generator<T>::ch2_off()
{ GT::PWM_ch2_off(); }



// duty_cycle
// ----------
template <typename T>
inline void PWM_generator<T>::ch1_duty_cycle(uint8_t duty_cycle)
{// uint32_t clave, para que no haya overflow!!!
    // uint32_t ocr1a = GT::PWM_top();
    atd::same_type_with_double_bits<counter_type> ocr1a = GT::PWM_top();
    ocr1a = (duty_cycle*ocr1a)/100;
    GT::PWM_ch1_duty_top(atd::to_integer<counter_type>(ocr1a));
}


template <typename T>
inline void PWM_generator<T>::ch1_duty_cycle(const Time& duty_cycle)
{
    Rep ocr1a = duty_cycle / GT::clock_period();
    GT::PWM_ch1_duty_top(atd::to_integer<counter_type>(ocr1a));
}

template <typename T>
inline PWM_generator<T>::Time PWM_generator<T>::ch1_duty_cycle()
{ return GT::PWM_ch1_duty_top() * GT::clock_period(); }

template <typename T>
inline void PWM_generator<T>::ch2_duty_cycle(uint8_t duty_cycle)
{// uint32_t clave, para que no haya overflow!!!
    // uint32_t ocr1b = GT::PWM_top();
    atd::same_type_with_double_bits<counter_type> ocr1b = GT::PWM_top();
    ocr1b = (duty_cycle*ocr1b)/100;
    GT::PWM_ch2_duty_top(ocr1b);
}


template <typename T>
inline void PWM_generator<T>::ch2_duty_cycle(const Time& duty_cycle)
{
    Rep ocr1b = duty_cycle / GT::clock_period();
    GT::PWM_ch2_duty_top(atd::to_integer<counter_type>(ocr1b));
}

template <typename T>
inline PWM_generator<T>::Time PWM_generator<T>::ch2_duty_cycle()
{ return GT::PWM_ch2_duty_top() * GT::clock_period(); }



}// namespace


#endif


