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

#ifndef __AVR_SIGNAL_GENERATOR_SF_H__
#define __AVR_SIGNAL_GENERATOR_SF_H__
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
struct Signal_generator_sf{
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
    static void frequency(const Hertz& freq_sq)
    {// Generic_timer
// (---) Esta opción no funciona.
//	using Int = Timer::counter_type;
//	Microsecond T_sq0 = atd::inverse(freq_sq);
//	Int T_sq = atd::to_integer<Int>(T_sq0.value());
//        Int top =
//            T_sq /
//            (static_cast<Int>(Timer::clock_period_in_us()) * Int{2});

	uint32_t tmp = uint32_t{2} * static_cast<uint32_t>(Timer::clock_period_in_us())
		  * atd::to_integer(freq_sq.value());
	Timer::counter_type top = uint32_t{1000000u}/tmp;

        Timer::top_OCRA(top);
    }

    static Hertz frequency()
    {
	auto top = Timer::output_compare_register_A();
        uint32_t tmp = uint32_t{2} *
                       static_cast<uint32_t>(Timer::clock_period_in_us()) *
                       uint32_t{top};

	uint32_t freq = uint32_t{1000000u}/tmp;
	return Hertz{freq};
    }

    /// Enciende el channel 1.
    static void ch1_on()
    {Timer::pin_A_toggle_on_compare_match();}

    /// Apaga el channel 1.
    static void ch1_off()
    { Timer::pin_A_disconnected(); }


    /// Enciende el channel 2.
    static void ch2_on()
    {Timer::pin_B_toggle_on_compare_match();}

    /// Apaga el channel 2.
    static void ch2_off()
    { Timer::pin_B_disconnected(); }

};


}// namespace


#endif


