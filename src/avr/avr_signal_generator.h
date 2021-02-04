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
 *  \brief  Generador de señales.
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
struct Signal_generator{
// TODO: esto son parametros de template
    using Timer = Timer1_CTC_mode;
    static constexpr uint8_t pin_channel1 = Timer1_CTC_mode::num_pin_A();
    static constexpr uint8_t pin_channel2 = Timer1_CTC_mode::num_pin_B();

    /// Encendemos el generador de señales
    template<uint16_t period>
    static void on() { Timer::template on<period>();}
    
    /// Apagamos el generador de señales.
    static void off() { Timer::off(); }

    /// Genera una onda cuadrada en el channel 1.
    // TODO: es más elegante:
    //          top = Timer::clock_frequency()/(2*freq_sq);
    static void ch1_square_wave(const Hertz& freq_sq)
    {// Generic_timer
	Microsecond T_sq0 = atd::inverse(freq_sq);
	Timer::counter_type T_sq = atd::to_integer<Timer::counter>(T_sq0);
        Timer::counter_type top =
            T_sq /
            (static_cast<uint32_t>(Timer::clock_period_in_us()) * Timer::counter_type{2});

        Timer::top_OCRA(top);
	Timer::pin_A_toggle_on_compare_match();
    }

    /// Apaga el channel 1.
    static void ch1_off()
    { Timer::pin_A_disconnected(); }

    /// Genera una onda cuadrada en el channel 2.
    static void ch2_square_wave(const Hertz& freq);

    /// Apaga el channel 2.
    static void ch2_off();
};


}// namespace


#endif


