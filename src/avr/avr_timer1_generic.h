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

#ifndef __AVR_TIMER1_GENERIC_H__
#define __AVR_TIMER1_GENERIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Generic_timer<Timer1>
 *    
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    15/02/2021 v0.0
 *
 ****************************************************************************/
#include "avr_timer1_basic.h"
#include "gen_types.h"

namespace gen{

template <>
class Generic_timer<avr::Timer1>{
public:
// types
    using Timer        = avr::Timer1;
    using counter_type = typename Timer::counter_type;
    using Hertz        = avr::Hertz;
    using Microsecond  = avr::Microsecond;
    using Scalar       = Hertz::Scalar; // TODO: es el mismo para Microseconds 
			    // y Hertz. ¿qué nombre común usar? ponerlo en
			    // avr_types.h

// pines 
    static constexpr uint8_t pin_channel1 = Timer::OCA_pin();
    static constexpr uint8_t pin_channel2 = Timer::OCB_pin();

    /// Constante que nos indica si los dos canales funcionan siempre a la
    /// misma frecuencia.
    static constexpr bool ch1_and_ch2_same_frequency = true;


// Timer características
    template<uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static Microsecond clock_period() {return Timer::clock_period();}

    static Hertz clock_frequency() {return Timer::clock_frequency();}


// Timer on/off
// ------------
    template<uint16_t period
	    , uint32_t clock_frequency_in_hz = MCU_CLOCK_FREQUENCY_IN_HZ>
    static void on() {Timer::template on<period, clock_frequency_in_hz>();}

    /// Apagamos el generador de señales.
    static void off() { Timer::off(); }



// Square wave mode
// ----------------
    static void mode_square_wave(){ Timer::mode_CTC_top_ICR1();}

    static void square_wave_top(Scalar x)
    { Timer::input_capture_register(atd::to_integer<counter_type>(x));}

    static counter_type square_wave_top()
    { return Timer::input_capture_register();}
    
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
// En este modo el timer cuenta desde 0 hasta PWM_top. 
// El pin de salida cambiará cuando alcance duty_top y PWM_top.
// El periodo de la señal generada será PWM_top, mientras que el duty_cycle
// será duty_top / PWM_top.
    static void mode_PWM_mode() { Timer::mode_fast_PWM_top_ICR1();}

    // configuración
    static void PWM_top(Scalar top)
    { Timer::input_capture_register(atd::to_integer<counter_type>(top));}

    static counter_type PWM_top() 
    {return Timer::input_capture_register();}

    static void PWM_ch1_duty_top(Scalar duty_top)
    {
        Timer::output_compare_register_A(
				    atd::to_integer<counter_type>(duty_top));
    }

    static counter_type PWM_ch1_duty_top()
    {return Timer::output_compare_register_A();}

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



};

}// namespace 



#endif



