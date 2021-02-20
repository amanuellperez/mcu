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

    /// Devuelve el valor mínimo que puede tomar el top
    static counter_type square_wave_min_top()
    { return Timer::bottom();}

    /// Devuelve el valor máximo que puede tomar el top
    static counter_type square_wave_max_top()
    { return Timer::max();}

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
	switch(mode_){
	    case Mode::only_channel2:
                Timer::output_compare_register_A(
					atd::to_integer<counter_type>(top));
                break;

	    case Mode::both_channels:
                Timer::input_capture_register(
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
	switch(mode_){
	    case Mode::only_channel2:
		return Timer::output_compare_register_A();

	    case Mode::both_channels:
		return Timer::input_capture_register();

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
	switch(mode_){
	    case Mode::both_channels:
	    case Mode::fix_0x00FF:
	    case Mode::fix_0x01FF:
	    case Mode::fix_0x03FF:
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
	    case Mode::both_channels:
	    case Mode::fix_0x00FF:
	    case Mode::fix_0x01FF:
	    case Mode::fix_0x03FF:
		return Timer::output_compare_register_A();

	    case Mode::only_channel2:
		// ERROR: en only_channel2 no tiene sentido duty_top
		return 0;
	}

	return 0;
    }

    // En todos los modos de funcionamiento OCRB contiene el duty_top
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

};

}// namespace 



#endif



