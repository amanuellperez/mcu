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
 *  Las señales generadas por el avr en los dos canales son siempre de la
 *  misma frecuencia!!! Para indicarlo llamo a esta clase SF, de tal manera
 *  que el cliente al usarla recuerde que no puede generar 2 ondas de
 *  frecuencia diferente en cada canal.
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


class Square_wave_generator: public __avr::Signal_generator{
public:
    /// Define la frecuencia que se genera en ch1 y ch2
    static void frequency(const Hertz& freq_sq)
    {
	auto top = (Timer::clock_frequency() / (2*freq_sq)) - 1ul;
	Timer::input_capture_register(atd::to_integer<Timer::counter_type>(top));
    }

    /// Frecuencia que genera (en caso de que esté encendido y funcionando).
    static Hertz frequency()
    {
	auto top = Timer::input_capture_register();
        return Timer::clock_frequency() / Hertz::Rep{2*(1ul + top)};
    }

    /// Frecuencia máxima que se puede generar con el Timer usando ese 
    /// clock_period(). Para generar más grandes probar a cambiar 
    /// Timer::clock_period().
    static Hertz max_frequency()
    {
        return Timer::clock_frequency() *
               Hertz::Rep{2 * (1ul + sizeof(Timer::counter_type))};
    }


    /// Encendemos el generador de señales
    template<uint16_t period>
    static void on() 
    { 
	Timer::template on<period>();
        Timer::mode_CTC_top_ICR1();
    }
    

// square_wave
// -----------
    /// Enciende el channel 1.
    static void ch1_on()
    { Timer::CTC_pin_A_toggle_on_compare_match(); }


    /// Enciende el channel 2. 
    static void ch2_on()
    { Timer::CTC_pin_B_toggle_on_compare_match(); }


};


class PWM_generator : public __avr::Signal_generator{
public:
    enum class Mode{
	inverting, non_inverting
    };

    /// Define la frecuencia que se genera en ch1 y ch2
    // OJO: es diferente de la de Square_wave_generator, por culpa del 2*
    static void frequency(const Hertz& freq_sq)
    {
	auto top = (Timer::clock_frequency() / freq_sq) - 1ul;
	Timer::input_capture_register(atd::to_integer<Timer::counter_type>(top));
    }

    /// Frecuencia que genera (en caso de que esté encendido y funcionando).
    static Hertz frequency()
    {
	auto top = Timer::input_capture_register();
        return Timer::clock_frequency() / Hertz::Rep{1ul + top};
    }

    /// Define el periodo a generar de la señal generada en ch1 y ch2.
    static void period(const Microsecond& T_s)
    {
	auto top = (T_s / Timer::clock_period()) - 1ul;
	Timer::input_capture_register(atd::to_integer<Timer::counter_type>(top));
    }

    static Microsecond period()
    {
	auto top = Timer::input_capture_register();
	uint32_t T = (top + 1) * Timer::clock_period_in_us();

	return Microsecond{T};
    }


    /// Encendemos el generador de señales
    template<uint16_t period>
    static void on() 
    {
        Timer::mode_fast_PWM_top_ICR1();
	Timer::template on<period>();
    }

    static void ch1_non_inverting_mode()
    { Timer::PWM_pin_A_non_inverting_mode();}

    static void ch1_inverting_mode()
    { Timer:: PWM_pin_A_inverting_mode();}

    static void ch2_non_inverting_mode()
    { Timer::PWM_pin_B_non_inverting_mode();}

    static void ch2_inverting_mode()
    { Timer::PWM_pin_B_inverting_mode();}


    /// Enciende el channel 1, generando una PWM de frecuencia `freq` y
    /// `duty_cycle`. El duty_cycle da el tanto por cien como número entero.
    /// Ejemplo: duty_cycle = 10, es un 10%.
    // (RRR) ¿por qué no pasarle también la frecuencia?
    //       Porque la frecuencia es la misma en los dos canales. Si se la
    //       pasara aquí estaría cambiando también la del otro canal, cosa que
    //       a lo mejor no se busca, generando errores.
    static void ch1_on(uint8_t duty_cycle,
                       Mode mode = Mode::non_inverting)
    {// uint32_t clave, para que no haya overflow!!!
	uint32_t ocr1a = Timer::input_capture_register();
	ocr1a = (duty_cycle*ocr1a)/100;
	
	ch1_on_ocr1a(ocr1a, mode);
    }


    static void ch1_on(const Microsecond duty_cycle,
                       Mode mode = Mode::non_inverting)
    {
	auto ocr1a = duty_cycle / Timer::clock_period();
	ch1_on_ocr1a(atd::to_integer(ocr1a), mode);
    }



    /// Enciende el channel 2.
    static void ch2_on(const uint8_t duty_cycle,
                       Mode mode = Mode::non_inverting)
    {
	uint32_t ocr1b = Timer::input_capture_register();
	ocr1b = (duty_cycle*ocr1b)/100;

        Timer::output_compare_register_B(ocr1b);

	if (mode == Mode::non_inverting)
	    Timer::PWM_pin_B_non_inverting_mode();
	else 
	    Timer::PWM_pin_B_inverting_mode();
    }


private:
    static void ch1_on_ocr1a(uint32_t ocr1a, Mode mode)
    {
        Timer::output_compare_register_A(ocr1a);

	if (mode == Mode::non_inverting)
	    Timer::PWM_pin_A_non_inverting_mode();
	else 
	    Timer::PWM_pin_A_inverting_mode();

    }
};

}// namespace


#endif


