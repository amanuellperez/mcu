// Copyright (C) 2021-2024 Manuel Perez 
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

#ifndef __MEGA_ADC_HWD_H__
#define __MEGA_ADC_HWD_H__

/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para manejar el ADC del avr.
 *	    Un ADC es un conversor de analógico a digital, por esto 
 *	el interfaz habla de "convertir".
 *	
 *   - CONEXIONES:
 *	    Recordar conectar siempre el AVCC.
 *
 *   - HISTORIA:
 *    Manuel Perez
 *	26/07/2017 Escrito v0.0
 *	29/01/2018 v0.1
 *	05/08/2019 Acabo el traductor. Faltan probarlo todo. ¿cómo?
 *	30/03/2020 Reestructurado: saco las funciones de alto nivel.
 *		   Lo dejo como traductor puro.
 *	13/06/2024 Lo dejo como traductor puro (o eso espero)
 *
 ****************************************************************************/

#include <atd_bit.h>
#include <avr/io.h> // registros: DDRB... PORT...
		    
#include "mega_import_avr.h"
#include "mega_registers.h"	// ((TODO)) parametrizar Cfg

namespace mega_{
namespace hwd{

// Al definir avr la macro ADC, no me deja crear la clase ADC, así que lo
// primero que hay que hacer es eliminar ADC. El problema es que necesito esta
// macro para usar el ADC
namespace impl_of{
inline auto ADC_macro() {return ADC;}
}
#undef ADC

/*!
 *  \brief  Interfaz al ADC que suministra el AVR
 *
 *  Básicamente tiene dos modos de funcionamiento:
 *	    single mode: 
 *		donde si no pedimos una conversión no la ejecuta.
 *
 *	    free running mode:	
 *		que al acabar una conversión empieza otra automáticamente.
 *
 */
class ADC{
public:
// Types
    using AREF_type = uint16_t;

// Constructor
    // Static interface only
    ADC() = delete;


// ADC Voltage Reference Selection
// -------------------------------
    /// Conectamos AREF externamente.
    /// Datasheet: AREF, Internal Vref turned off
    static void AREF_external();

    /// Tomamos AREF = AVCC.
    /// Hay que conectar un condensador entre el pin AREF
    /// GND. 
    /// La datasheet lo llama "AVCC with external capacitor at AREF pin"
    static void AREF_internal_to_AVCC();

    /// Datasheet: Internal 1.1V Voltage Reference with external capacitor 
    /// at AREF pin
    static void AREF_internal_to_1_1V();



// ADC Left Adjust Result
// ----------------------
    /// Left adjust result. Esto es, cuando lea un valor los va a meter
    /// empezando a rellenar ADCH y dejando los 8 bits menos significativos
    /// de ADCL (los que están a la derecha) en 0.
    /// Por defecto los ajusta a la derecha.
    //
    // Si es right_adjust_result:
    //		    <--- ADCH ----> |<---- ADCL ---->
    //		    - - - - - - 9 8 | 7 6 5 4 3 2 1 0
    //
    //	Si es left_adjust_result:
    //		    <--- ADCH ----> |<---- ADCL ---->
    //		    9 8 7 6 5 4 3 2 | 1 0 - - - - - - 
    //
    static void left_adjust_result();

    /// Esta es la opción por defecto.
    /// Ajusta a la derecha ADCH-ADCL. Ver left_adjust_result.
    static void right_adjust_result();

// Analog channel selection
// ------------------------
    /// Seleccionamos el pin en el que queremos que el ADC lea.
    /// El número de pin pasado es el pin del chip real. De esta forma
    /// es más sencillo depurar: conecto el ADC al pin 28, siendo 28 el pin
    /// real del IC. Además, en caso de intentar conectarlo a un pin que no
    /// sea del ADC daría un error en tiempo de compilación.
    template <uint8_t num_pin>
    static void select_pin();

    // Analog channel selection temperature sensor
    static void select_temperature_sensor();

    // Analog channel selection 1.1V(V_BG)
    static void select_channel_1_1V();

    // Analog channel selection 0V(GND)
    static void select_channel_0V();


// Enable
// ------
    /// Enable ADC. Esta sería la función on()
    static void enable();

    /// Disable ADC.
    static void disable();


// Start conversion
    /// Indicamos al ADC que proceda a leer del pin el valor.
    /// En single mode: inicia una conversión. 
    /// En free running mode: write this bit to one to start the first
    /// conversion.
    static void start_conversion();

    /// El ADC ¿ha acabado la conversión?
    // En single mode: ADCSRA.ADSC será 1 mientras la conversión esté en
    // progreso. Pasará a 1 cuando acabe.
    static bool is_the_conversion_complete();



// Interrupt Flag
// --------------
    // TODO: read? 
    //       interrupt_flag_clear() ?


// Interrupts
// ----------
    /// Activamos the ADC Conversion Complete Interrupt.
    /// When this bit is written to one and the I-bit in SREG is set, the
    /// ADC Conversion Conversion Interrupt is activated.
    static void interrupt_enable();

    /// Desactivamos the ADC Conversion Complete Interrupt.
    static void interrupt_disable();


// Clock prescaler
// ---------------
    /// Definimos la velocidad del ADC (¿cómo preescalamos el reloj del AVR?)
    ///
    /// Recordar que el ADC para que funcione su reloj tiene que 
    /// estar entre 50 kHz y 200 kHz.
    /// Si F_CPU = 1MHz, al dividir el reloj entre 16 obtenemos una velocidad
    /// de muestreo de 16'2 kHz que es válida.
    static void clock_frequency_divide_by_2();
    static void clock_frequency_divide_by_4();
    static void clock_frequency_divide_by_8();
    static void clock_frequency_divide_by_16();
    static void clock_frequency_divide_by_32();
    static void clock_frequency_divide_by_64();
    static void clock_frequency_divide_by_128();


// data
// ----
    /// Devuelve el valor leído.
    /// Precondición: el ADC tiene que haber acabado de leer.
    /// El ADC lee 10 bits que se devuelven en un uint16_t.
    /// El registro ADC guarda el valor leido en arefs 
    /// (1 aref = AREF/1024 mV)
    static uint16_t ADC_in_arefs();

    /// Devuelve el byte más significativo del valor leído.
    /// Si se llama a left_adjust_result() en este byte estará el valor
    /// leído (como tanto por 255 = 8 bits)
    static uint8_t ADCH_in_arefs();

    // TODO: analog_comparator_multiplexer_enable();
    
// Trigger source
    static void single_conversion_mode();

    // The ADC will start a conversion on a positive edge of the selected 
    // trigger signal. Hay que seleccionar la trigger signal.
    static void auto_trigger_mode();

    // Si se selecciona auto_trigger_mode, indicar qué tipo:
    static void auto_trigger_source_free_running_mode();
    static void auto_trigger_source_analog_comparator();
    static void auto_trigger_source_external_interrup_request_0();
    static void auto_trigger_source_timer0_compare_match_A();
    static void auto_trigger_source_timer0_overflow();
    static void auto_trigger_source_timer1_compare_match_B();
    static void auto_trigger_source_timer1_overflow();
    static void auto_trigger_source_timer1_capture_event();


// TODO: registro DIDR0 (???)


private:
    // Seleccionamos el pin en el que queremos que el ADC lea
    // Observar que el num_pin_ADC es el del ADC (ADC0, ... ADC5 ...)
    // no el numero de pin del chip.
    template <uint8_t num_pin_ADC>
    static void select_pin_()
    {// los 4 bits menos significativos de ADMUX contienen el pin que se lee
	ADMUX = (0xf0 & ADMUX) | num_pin_ADC;
    }

};

// ADMUX register
// --------------
inline void ADC::AREF_external()
{// 00
    atd::write_bits<REFS1, REFS0>::to<0,0>::in(ADMUX);
}

inline void ADC::AREF_internal_to_AVCC()
{// 01
    atd::write_bits<REFS1, REFS0>::to<0,1>::in(ADMUX);
}


inline void ADC::AREF_internal_to_1_1V()
{// 11
    atd::write_bits<REFS1, REFS0>::to<1,1>::in(ADMUX);
}

inline void ADC::left_adjust_result()
{atd::write_bits<ADLAR>::to<1>::in(ADMUX);}

inline void ADC::right_adjust_result()
{atd::write_bits<ADLAR>::to<0>::in(ADMUX);}


template <uint8_t num_pin>
inline void ADC::select_pin()
{ select_pin_<cfg::ADC_PIN<num_pin>::value>(); }

inline void ADC::select_temperature_sensor()
{// 1000 
    atd::write_bits<MUX3, MUX2, MUX1, MUX0>::to<1,0,0,0>::in(ADMUX); 
}

inline void ADC::select_channel_1_1V()
{// 1110
    atd::write_bits<MUX3, MUX2, MUX1, MUX0>::to<1,1,1,0>::in(ADMUX); 
}

inline void ADC::select_channel_0V()
{// 1111
    atd::write_bits<MUX3, MUX2, MUX1, MUX0>::to<1,1,1,1>::in(ADMUX); 
}

inline void ADC::enable() 
{atd::write_bits<ADEN>::to<1>::in(ADCSRA);}

inline void ADC::disable() 
{atd::write_bits<ADEN>::to<0>::in(ADCSRA);}


// ADCSRA register
// ---------------
inline void ADC::start_conversion() 
{atd::write_bits<ADSC>::to<1>::in(ADCSRA);}

inline bool ADC::is_the_conversion_complete()
{return atd::is_zero_bit<ADSC>::of(ADCSRA);}

inline void ADC::auto_trigger_mode()
{atd::write_bits<ADATE>::to<1>::in(ADCSRA);}

inline void ADC::single_conversion_mode()
{atd::write_bits<ADATE>::to<0>::in(ADCSRA);}


inline void ADC::interrupt_enable()
{atd::write_bits<ADIE>::to<1>::in(ADCSRA);}

inline void ADC::interrupt_disable()
{atd::write_bits<ADIE>::to<0>::in(ADCSRA);}


inline void ADC::clock_frequency_divide_by_2()
{// 000 ó 001
    atd::write_bits<ADPS2, ADPS1, ADPS0>::to<0,0,0>::in(ADCSRA);
}

inline void ADC::clock_frequency_divide_by_4()
{ atd::write_bits<ADPS2, ADPS1, ADPS0>::to<0,1,0>::in(ADCSRA); }

inline void ADC::clock_frequency_divide_by_8()
{ atd::write_bits<ADPS2, ADPS1, ADPS0>::to<0,1,1>::in(ADCSRA); }

inline void ADC::clock_frequency_divide_by_16()
{ atd::write_bits<ADPS2, ADPS1, ADPS0>::to<1,0,0>::in(ADCSRA); }

inline void ADC::clock_frequency_divide_by_32()
{ atd::write_bits<ADPS2, ADPS1, ADPS0>::to<1,0,1>::in(ADCSRA); }

inline void ADC::clock_frequency_divide_by_64()
{ atd::write_bits<ADPS2, ADPS1, ADPS0>::to<1,1,0>::in(ADCSRA); }

inline void ADC::clock_frequency_divide_by_128()
{ atd::write_bits<ADPS2, ADPS1, ADPS0>::to<1,1,1>::in(ADCSRA); }

// ADCH-ADCL registers
// -------------------
inline uint16_t ADC::ADC_in_arefs()
{return impl_of::ADC_macro();}

inline uint8_t ADC::ADCH_in_arefs()
{return ADCH;}


// ADCSRB register
// ---------------
inline void ADC::auto_trigger_source_free_running_mode()
{// 000
    atd::write_bits<ADTS2, ADTS1, ADTS0>::to<0,0,0>::in(ADCSRB);
}

inline void ADC::auto_trigger_source_analog_comparator()
{// 001
    atd::write_bits<ADTS2, ADTS1, ADTS0>::to<0,0,1>::in(ADCSRB);
}

inline void ADC::auto_trigger_source_external_interrup_request_0()
{// 010
    atd::write_bits<ADTS2, ADTS1, ADTS0>::to<0,1,0>::in(ADCSRB);
}

inline void ADC::auto_trigger_source_timer0_compare_match_A()
{// 011
    atd::write_bits<ADTS2, ADTS1, ADTS0>::to<0,1,1>::in(ADCSRB);
}

inline void ADC::auto_trigger_source_timer0_overflow()
{// 100
    atd::write_bits<ADTS2, ADTS1, ADTS0>::to<1,0,0>::in(ADCSRB);
}

inline void ADC::auto_trigger_source_timer1_compare_match_B()
{// 101
    atd::write_bits<ADTS2, ADTS1, ADTS0>::to<1,0,1>::in(ADCSRB);
}

inline void ADC::auto_trigger_source_timer1_overflow()
{// 110
    atd::write_bits<ADTS2, ADTS1, ADTS0>::to<1,1,0>::in(ADCSRB);
}

inline void ADC::auto_trigger_source_timer1_capture_event()
{// 111
    atd::write_bits<ADTS2, ADTS1, ADTS0>::to<1,1,1>::in(ADCSRB);
}




}// namespace hwd
}// namespace

#endif


