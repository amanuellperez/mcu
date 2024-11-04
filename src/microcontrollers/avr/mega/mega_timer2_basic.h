// Copyright (C) 2023 Manuel Perez 
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

#ifndef __MEGA_TIMER2_BASIC_H__
#define __MEGA_TIMER2_BASIC_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del timer 2 del AVR. Es un timer de 8 bits.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *
 *      10/09/2023 Escrito
 *
 ****************************************************************************/
#include <avr/io.h> // registros: DDRB... PORT...
#include "mega_timern_basic.h"

#include <atd_bit.h>
#include <atd_type_traits.h>

#include "mega_interrupt.h"
#include "mega_pin.h"
#include "mega_clock_frequencies.h"	

#include <array>

namespace mega_{

// La creo como clase para luego poderla pasar como parámetro a templates
// (por ejemplo, a Contador). Si la defino como namespace no puedo hacerlo.
class Timer2{

public:
// CARACTERÍSTICAS DEL TIMER
    using counter_type = cfg::timer0::counter_type;

    /// The counter reaches the bottom when it becomes zero.
    static constexpr counter_type bottom() {return counter_type{0};}

    /// Maximum value that reaches the counter.
    static constexpr counter_type max() 
    {return std::numeric_limits<counter_type>::max();}

    // Devuelve el número de pin al que está conectado la salida A del 
    // generador de ondas.
    static constexpr counter_type OCA_pin() {return cfg::timer0::OCA_pin; }

    // Devuelve el número de pin al que está conectado la salida B del 
    // generador de ondas.
    static constexpr counter_type OCB_pin() {return cfg::timer0::OCB_pin;}


// CONFIGURACIÓN DEL RELOJ
    // (RRR) ¿por qué definir explícitamente aquí los prescaler_factor?
    //       Los podía definir en cfg::timer0, pero la implementación de esta
    //       clase conoce los prescalers en las funciones
    //       clock_frequency_no_prescaling, ... O se generalizan esas
    //       funciones metiendolas en cfg::timer0 o no mejor hacerlo todo
    //       concreto.
    //
    // Array con los posibles prescaler factors del timer.
    static constexpr std::array<uint16_t, 8> 
				prescaler_factor = {1, 8, 32, 64, 
				                    64, 128, 256, 1024};

    enum class Frequency_divisor{
		    undefined,    
		    timer_stopped,
		    no_prescaling,
		    divide_by_8,
		    divide_by_32,
		    divide_by_64,
		    divide_by_128,
		    divide_by_256,
		    divide_by_1024};

    // Obtenemos el divisor de frecuencia que se aplica al reloj del micro.
    static Frequency_divisor frequency_divisor();

    // Selección del reloj y de su velocidad (según tabla 19-10)
    // Establecemos el divisor de frecuencia a aplicar al reloj del micro.
    static void clock_frequency_no_prescaling();
    static void clock_frequency_divide_by_8();
    static void clock_frequency_divide_by_32();
    static void clock_frequency_divide_by_64();
    static void clock_frequency_divide_by_128();
    static void clock_frequency_divide_by_256();
    static void clock_frequency_divide_by_1024();


    /// divisor de frecuencias (prescaler_factor)
    /// Enciende el timer (si prescaler_factor != 0) o lo apaga (si es == 0)
    static constexpr void clock_frequency(uint32_t prescaler_factor);

    /// Frecuencia a la que funciona internamente el timer.
    /// Se cumple que clock_frequency() = 1 / clock_period();
    template <uint32_t clock_frequency_in_Hz = clock_cpu()>
    static Frequency clock_frequency();


    /// Paramos el timer.
    static void off();
    static void stop() {off();} // Hoy me gusta más stop que off @_@
				// Que el uso decida una u otra forma de
				// hablar


// SELECCIÓN DE RELOJ EXTERNO

//    static void external_clock_falling_edge();
//
//    /// External clock source on T0 pin. Clock on rising edge.
//    static void external_clock_rising_edge();


// ACCESO A REGISTROS
    static counter_type counter() {return TCNT2;}
    static void counter(counter_type x) { TCNT2 = x;}

    static counter_type output_compare_register_A() {return OCR2A;}
    static void output_compare_register_A(counter_type x){OCR2A = x;}

    static counter_type output_compare_register_B() {return OCR2B;}
    static void output_compare_register_B(counter_type x) {OCR2B = x;}


// WAVEFORM GENERATION MODES (table 19-9)
    static void normal_mode();

    static void CTC_mode();

    static void fast_PWM_mode_top_OCRA();
    static void fast_PWM_mode_top_0xFF();

    static void PWM_phase_correct_mode_top_OCRA();
    static void PWM_phase_correct_mode_top_0xFF();


// PINS OPERATION 
    // comunes a todas las tablas (19-3 y siguientes)
    static void pin_A_disconnected();
    static void pin_B_disconnected();

    // table 19-3 and 19-6
    static void CTC_pin_A_toggle_on_compare_match();
    static void CTC_pin_A_clear_on_compare_match();
    static void CTC_pin_A_set_on_compare_match();

    static void CTC_pin_B_toggle_on_compare_match();
    static void CTC_pin_B_clear_on_compare_match();
    static void CTC_pin_B_set_on_compare_match();

    // tables 19-4,5, 7 and 8.
    static void PWM_pin_A_non_inverting_mode();
    static void PWM_pin_A_inverting_mode();
    static void PWM_pin_B_non_inverting_mode();
    static void PWM_pin_B_inverting_mode();

    // Solo para los modos mode_PWM_phase_correct_top_OCRA y
    // mode_fast_PWM_top_OCRA
    static void PWM_pin_A_toggle_on_compare_match();

    // La datasheet indica que no hay toggle mode en el pin B.
//    static void PWM_pin_B_toggle_on_compare_match();


// INTERRUPCIONES
//  Recordar que hay que llamar a avr::enable_interrupts(); para que
//  se puedan usar las interrupciones.
    /// Se captura con ISR_TIMER0_OVF
    static void enable_overflow_interrupt();
    static void disable_overflow_interrupt();

    /// Se captura con ISR_TIMER0_COMPA
    static void enable_output_compare_A_match_interrupt();
    static void disable_output_compare_A_match_interrupt();

    /// Se captura con ISR_TIMER0_COMPB
    static void enable_output_compare_B_match_interrupt();
    static void disable_output_compare_B_match_interrupt();

    static void disable_interrupts();

    static void clear_pending_interrupts();

// ASYNCHRONOUS MODE
    // CUIDADO: Según la datasheet
    //	    This should be done before asynchronous operation is selected 
    // ¿Qué es esto? 
//    static void enable_external_asynchronous_clock();
//    static void disable_external_asynchronous_clock();

    // CUIDADO: hay que seguir un protocolo a la hora de habilitar este modo.
    // Leer la datasheet al respecto o ver test.
    static void enable_asynchronous_mode();
    static void disable_asynchronous_mode();

    // En modo asincrono los registros no se actualizan inmediatamente, sino
    // que tardan 2 ciclos de reloj. Esto puede generar un problema si se
    // quiere usar este timer para despertar el avr: si se llama a `sleep`
    // antes de que los registros se hayan actualizado, el avr no despertará
    // ya que el timer no habrá quedado en modo asincrono. Por eso hay que
    // esperar a que se actualicen los registros.
    static bool are_registers_ready();

    // La siguiente función bloquea al micro no devolviendo el control hasta
    // que se hayan actualizado los registros del Timer2 (ver ASSR register,
    // datasheet)
    static void wait_till_registers_are_ready();

// TODO: Faltan FOC2A/FOC2B
};


inline Timer2::Frequency_divisor Timer2::frequency_divisor()
{ 
    switch(atd::read_bits<CS22, CS21, CS20>::of(TCCR2B)){
	case atd::zero<uint8_t>::with_bits<CS22, CS21, CS20>::to<0,0,0>():
	    return Frequency_divisor::timer_stopped;

	case atd::zero<uint8_t>::with_bits<CS22, CS21, CS20>::to<0,0,1>():
	    return Frequency_divisor::no_prescaling;

	case atd::zero<uint8_t>::with_bits<CS22, CS21, CS20>::to<0,1,0>():
	    return Frequency_divisor::divide_by_8;

	case atd::zero<uint8_t>::with_bits<CS22, CS21, CS20>::to<0,1,1>():
	    return Frequency_divisor::divide_by_32;

	case atd::zero<uint8_t>::with_bits<CS22, CS21, CS20>::to<1,0,0>():
	    return Frequency_divisor::divide_by_64;

	case atd::zero<uint8_t>::with_bits<CS22, CS21, CS20>::to<1,0,1>():
	    return Frequency_divisor::divide_by_128;

	case atd::zero<uint8_t>::with_bits<CS22, CS21, CS20>::to<1,1,0>():
	    return Frequency_divisor::divide_by_256;

	case atd::zero<uint8_t>::with_bits<CS22, CS21, CS20>::to<1,1,1>():
	    return Frequency_divisor::divide_by_1024;
    }

    return Frequency_divisor::undefined;
}


// preescaler
// ----------
inline void Timer2::off()
{ atd::write_bits<CS22, CS21, CS20>::to<0,0,0>::in(TCCR2B); }

inline void Timer2::clock_frequency_no_prescaling() 
{ atd::write_bits<CS22, CS21, CS20>::to<0,0,1>::in(TCCR2B); }

inline void Timer2::clock_frequency_divide_by_8()
{ atd::write_bits<CS22, CS21, CS20>::to<0,1,0>::in(TCCR2B); }

inline void Timer2::clock_frequency_divide_by_32()
{ atd::write_bits<CS22, CS21, CS20>::to<0,1,1>::in(TCCR2B); }

inline void Timer2::clock_frequency_divide_by_64()
{ atd::write_bits<CS22, CS21, CS20>::to<1,0,0>::in(TCCR2B); }

inline void Timer2::clock_frequency_divide_by_128()
{ atd::write_bits<CS22, CS21, CS20>::to<1,0,1>::in(TCCR2B); }

inline void Timer2::clock_frequency_divide_by_256()
{ atd::write_bits<CS22, CS21, CS20>::to<1,1,0>::in(TCCR2B); }

inline void Timer2::clock_frequency_divide_by_1024()
{ atd::write_bits<CS22, CS21, CS20>::to<1,1,1>::in(TCCR2B); }


// DUDA: ¿cómo gestionar los errores de programación?
inline constexpr void Timer2::clock_frequency(uint32_t prescaler_factor)
{
    switch (prescaler_factor){
	break; case 8   : clock_frequency_divide_by_8();
	break; case 32  : clock_frequency_divide_by_32();
	break; case 64  : clock_frequency_divide_by_64();
	break; case 128 : clock_frequency_divide_by_128();
	break; case 256 : clock_frequency_divide_by_256();
	break; case 1024: clock_frequency_divide_by_1024();
	break; default  : clock_frequency_no_prescaling();
    }
}


// Modos de funcionamiento
// -----------------------
inline void Timer2::normal_mode()
{
    atd::write_bits<WGM22>::to<0>::in(TCCR2B);
    atd::write_bits<WGM21, WGM20>::to<0,0>::in(TCCR2A);
}

inline void Timer2::PWM_phase_correct_mode_top_0xFF()
{
    atd::write_bits<WGM22>::to<0>::in(TCCR2B);
    atd::write_bits<WGM21, WGM20>::to<0,1>::in(TCCR2A);
}

inline void Timer2::CTC_mode()
{
    atd::write_bits<WGM22>::to<0>::in(TCCR2B);
    atd::write_bits<WGM21, WGM20>::to<1,0>::in(TCCR2A);
}

inline void Timer2::fast_PWM_mode_top_0xFF()
{
    atd::write_bits<WGM22>::to<0>::in(TCCR2B);
    atd::write_bits<WGM21, WGM20>::to<1,1>::in(TCCR2A);
}

inline void Timer2::PWM_phase_correct_mode_top_OCRA()
{
    atd::write_bits<WGM22>::to<1>::in(TCCR2B);
    atd::write_bits<WGM21, WGM20>::to<0,1>::in(TCCR2A);
}

inline void Timer2::fast_PWM_mode_top_OCRA()
{
    atd::write_bits<WGM22>::to<1>::in(TCCR2B);
    atd::write_bits<WGM21, WGM20>::to<1,1>::in(TCCR2A);
}


// pins
// ----
inline void Timer2::pin_A_disconnected()
{ 
    atd::write_bits<COM2A1, COM2A0>::to<0,0>::in(TCCR2A); 
    // (???) ¿En qué estado dejar el puerto pin_B()?
}

inline void Timer2::pin_B_disconnected()
{ 
    atd::write_bits<COM2B1, COM2B0>::to<0,0>::in(TCCR2A); 
    // (???) ¿En qué estado dejar el puerto pin_B()?
}


inline void Timer2::CTC_pin_A_toggle_on_compare_match()
{ 
    atd::write_bits<COM2A1, COM2A0>::to<0,1>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}

inline void Timer2::CTC_pin_A_clear_on_compare_match()
{ 
    atd::write_bits<COM2A1, COM2A0>::to<1,0>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}

inline void Timer2::CTC_pin_A_set_on_compare_match()
{ 
    atd::write_bits<COM2A1, COM2A0>::to<1,1>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}




inline void Timer2::CTC_pin_B_toggle_on_compare_match()
{ 
    atd::write_bits<COM2B1, COM2B0>::to<0,1>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer2::CTC_pin_B_clear_on_compare_match()
{ 
    atd::write_bits<COM2B1, COM2B0>::to<1,0>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer2::CTC_pin_B_set_on_compare_match()
{ 
    atd::write_bits<COM2B1, COM2B0>::to<1,1>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer2::PWM_pin_A_non_inverting_mode()
{ 
    atd::write_bits<COM2A1, COM2A0>::to<1,0>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}


inline void Timer2::PWM_pin_A_inverting_mode()
{ 
    atd::write_bits<COM2A1, COM2A0>::to<1,1>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}


inline void Timer2::PWM_pin_B_non_inverting_mode()
{ 
    atd::write_bits<COM2B1, COM2B0>::to<1,0>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer2::PWM_pin_B_inverting_mode()
{ 
    atd::write_bits<COM2B1, COM2B0>::to<1,1>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCB_pin()>::as_output();
}


inline void Timer2::PWM_pin_A_toggle_on_compare_match()
{ 
    atd::write_bits<COM2A1, COM2A0>::to<0,1>::in(TCCR2A); 

    // Obligatorio definirlo como de salida. 
    Pin<OCA_pin()>::as_output();
}



// Interrupciones
// --------------
inline void Timer2::enable_overflow_interrupt()
{ atd::write_bits<TOIE2>::to<1>::in(TIMSK2); }

inline void Timer2::disable_overflow_interrupt()
{ atd::write_bits<TOIE2>::to<0>::in(TIMSK2); }

inline void Timer2::enable_output_compare_A_match_interrupt()
{ atd::write_bits<OCIE2A>::to<1>::in(TIMSK2); }

inline void Timer2::disable_output_compare_A_match_interrupt()
{ atd::write_bits<OCIE2A>::to<0>::in(TIMSK2); }

inline void Timer2::enable_output_compare_B_match_interrupt()
{ atd::write_bits<OCIE2B>::to<1>::in(TIMSK2); }

inline void Timer2::disable_output_compare_B_match_interrupt()
{ atd::write_bits<OCIE2B>::to<0>::in(TIMSK2); }

inline void Timer2::disable_interrupts()
{ atd::write_bits<OCIE2B, OCIE2A, TOIE2>::to<0,0,0>::in(TIMSK2); }

// CUIDADO. La datasheet dice:
//	Alternatively, OCFB is cleared by writing a logic one to the flag.
inline void Timer2::clear_pending_interrupts()
{ atd::write_bits<OCF2B, OCF2A, TOV2>::to<1,1,1>::in(TIFR2); }


// ASYNCHRONOUS MODE
// -----------------
//inline void Timer2::enable_external_asynchronous_clock()
//{ atd::write_bit<EXCLK>::to<1>::in(ASSR); }
//
//inline void Timer2::disable_external_asynchronous_clock()
//{ atd::write_bit<EXCLK>::to<0>::in(ASSR); }

inline void Timer2::enable_asynchronous_mode()
{ atd::write_bit<AS2>::to<1>::in(ASSR); }

inline void Timer2::disable_asynchronous_mode()
{ atd::write_bit<AS2>::to<0>::in(ASSR); }

inline bool Timer2::are_registers_ready()
{ return 
    (atd::read_bits<TCN2UB, OCR2AUB, OCR2BUB, TCR2AUB, TCR2BUB>::of(ASSR) == 0); }

inline void Timer2::wait_till_registers_are_ready()
{ while (!are_registers_ready()) 
    { ; } // do nothing, wait
}


}// namespace avr


#endif

