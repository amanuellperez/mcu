// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __AVR_ADC_H__
#define __AVR_ADC_H__

/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para manejar el ADC del avr.
 *	    Un ADC es un conversor de analógico a digital, por esto 
 *	el interfaz habla de "convertir".
 *	
 *   - CONEXIONES:
 *	    Recordar conectar siempre el AVCC.
 *
 *  - TODO: eliminar Register por las nuevas funciones.
 *
 *   - HISTORIA:
 *           alp  - 26/07/2017 Escrito v0.0
 *		    29/01/2018 v0.1
 *		    05/08/2019 Acabo el traductor.
 *			       TODO: falta probar todas las funciones.
 *			       ¿Cómo probarlas? De momento según vaya haciendo
 *			       proyectos, pero sería mejor hacer un test
 *			       detallado.
 *
 ****************************************************************************/
#include <atd_register.h>
#include "avr_cfg.h"

namespace avr{

// Al definir avr la macro ADC, no me deja crear la clase ADC, así que lo
// primero que hay que hacer es eliminar ADC. El problema es que necesito esta
// macro para usar el ADC, por eso la redefino como MACRO_ADC_DE_AVR (nombre
// feo para que no se use lo menos posible). De todas formas esta macro la
// elimino al final de este fichero.
namespace impl{
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
    // a 125 kHz funcionamos por defecto
    constexpr static uint16_t FRECUENCIA_POR_DEFECTO = 125;

    // Es un interfaz estático
    ADC() = delete;


    // ---------
    // Traductor
    // ---------
    // -------------------------------
    // ADC Voltage Reference Selection
    // -------------------------------
    /// Conectamos AREF externamente.
    /// Datasheet: AREF, Internal Vref turned off
    static void AREF_external()
    {// 00
      atd::Register(ADMUX).write_zero_bit<REFS1>();
      atd::Register(ADMUX).write_zero_bit<REFS0>();
    }

    /// Tomamos AREF = AVCC.
    /// Hay que conectar un condensador entre el pin AREF
    /// GND. 
    /// La datasheet lo llama "AVCC with external capacitor at AREF pin"
    static void AREF_internal_to_AVCC()
    {// 01
      atd::Register(ADMUX).write_zero_bit<REFS1>();
      atd::Register(ADMUX).write_one_bit<REFS0>();
    }

    /// Datasheet: Internal 1.1V Voltage Reference with external capacitor 
    /// at AREF pin
    static void AREF_internal_to_1_1V()
    {// 11
      atd::Register(ADMUX).write_one_bit<REFS1>();
      atd::Register(ADMUX).write_one_bit<REFS0>();
    }





    // ----------------------
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
    static void left_adjust_result()
    { atd::Register(ADMUX).write_one_bit<ADLAR>(); }

    /// Esta es la opción por defecto.
    /// Ajusta a la derecha ADCH-ADCL. Ver left_adjust_result.
    static void right_adjust_result()
    { atd::Register(ADMUX).write_zero_bit<ADLAR>();}

    /// Seleccionamos el pin en el que queremos que el ADC lea.
    /// El número de pin pasado es el pin del chip real. De esta forma
    /// es más sencillo depurar: conecto el ADC al pin 28, siendo 28 el pin
    /// real del IC. Además, en caso de intentar conectarlo a un pin que no
    /// sea del ADC daría un error en tiempo de compilación.
    template <uint8_t num_pin>
    static void select_pin()
    { selecciona_pin_ADC_a_leer<ADC_PIN<num_pin>::value>(); }


    /// Enable ADC. Esta sería la función on()
    static void enable() 
    { atd::Register(ADCSRA).write_one_bit<ADEN>(); }

    /// Disable ADC.
    static void disable() 
    { atd::Register(ADCSRA).write_zero_bit<ADEN>(); }


    /// Indicamos al ADC que proceda a leer del pin el valor.
    /// En single mode: inicia una conversión. 
    /// En free running mode: write this bit to one to start the first
    /// conversion.
    static void start_conversion() 
    { atd::Register(ADCSRA).write_one_bit<ADSC>(); }

    /// El ADC ¿ha acabado la conversión?
    // En single mode: ADCSRA.ADSC será 1 mientras la conversión esté en
    // progreso. Pasará a 1 cuando acabe.
    static bool is_the_conversion_complete()
    { return atd::Register(ADCSRA).is_zero_bit<ADSC>(); }

    /// Auto-trigger enable. 
    /// The ADC will start a conversion on a positive edge of the selected 
    /// trigger signal. Hay que seleccionar la trigger signal.
    static void auto_trigger_enable()
    { atd::Register(ADCSRA).write_one_bit<ADATE>(); }

    /// Auto trigger disable.
    static void auto_trigger_disable()
    { atd::Register(ADCSRA).write_zero_bit<ADATE>(); }

    /// Activamos the ADC Conversion Complete Interrupt.
    /// When this bit is written to one and the I-bit in SREG is set, the
    /// ADC Conversion Conversion Interrupt is activated.
    static void interrupt_enable()
    { atd::Register(ADCSRA).write_one_bit<ADIE>(); }

    /// Desactivamos the ADC Conversion Complete Interrupt.
    static void interrupt_disable()
    { atd::Register(ADCSRA).write_zero_bit<ADIE>(); }

    // TODO: analog_comparator_multiplexer_enable();
    

    /// Cada vez que se acaba una conversión empieza otra automáticamente
    static void auto_trigger_source_free_running_mode()
    {// 000
	atd::Register(ADCSRB).write_zero_bit<ADTS2, ADTS1, ADTS0>();
    }

    static void auto_trigger_source_analog_comparator()
    {// 001
	atd::Register(ADCSRB).write_zero_bit<ADTS2, ADTS1>();
	atd::Register(ADCSRB).write_one_bit<ADTS0>();
    }

    static void auto_trigger_source_external_interrup_request_0()
    {// 010
	atd::Register(ADCSRB).write_zero_bit<ADTS2, ADTS0>();
	atd::Register(ADCSRB).write_one_bit<ADTS1>();
    }

    static void auto_trigger_source_timer0_compare_match_A()
    {// 011
	atd::Register(ADCSRB).write_zero_bit<ADTS2>();
	atd::Register(ADCSRB).write_one_bit<ADTS1, ADTS0>();
    }

    static void auto_trigger_source_timer0_overflow()
    {// 100
	atd::Register(ADCSRB).write_one_bit<ADTS2>();
	atd::Register(ADCSRB).write_zero_bit<ADTS1, ADTS0>();
    }

    static void auto_trigger_source_timer1_compare_match_B()
    {// 101
	atd::Register(ADCSRB).write_one_bit<ADTS2, ADTS0>();
	atd::Register(ADCSRB).write_zero_bit<ADTS1>();
    }

    static void auto_trigger_source_timer1_overflow()
    {// 110
	atd::Register(ADCSRB).write_one_bit<ADTS2, ADTS1>();
	atd::Register(ADCSRB).write_zero_bit<ADTS0>();
    }

    static void auto_trigger_source_timer1_capture_event()
    {// 111
	atd::Register(ADCSRB).write_one_bit<ADTS2, ADTS1, ADTS0>();
    }

    /// Elegimos la frecuencia a la que queremos que funcione el ADC
    // clock_speed_in_kHz<125>(); <-- indica que quieres que el reloj del ADC
    // funcione a 125 kHz. Esta función tiene la ventaja de que si pasas una
    // frecuencia no válida no compila, y si cambias de frecuencia recalcula
    // automáticamente qué divisor de frecuencia usar.
    template<uint16_t frecuencia
	, uint32_t clock_frecuencia_en_hz = AVR_CLOCK_FREQUENCY_IN_HZ>
    static void clock_speed_in_kHz();


    /// Devuelve el valor leído.
    /// Precondición: el ADC tiene que haber acabado de leer.
    /// El ADC lee 10 bits que se devuelven en un uint16_t.
    /// El registro ADC guarda el valor leido en arefs 
    /// (1 aref = AREF/1024 mV)
    static uint16_t ADC_in_arefs()
    {return impl::ADC_macro();}


    /// Devuelve el byte más significativo del valor leído.
    /// Si se llama a left_adjust_result() en este byte estará el valor
    /// leído (como tanto por 255 = 8 bits)
    static uint8_t ADCH_in_arefs()
    {return ADCH;}



    // ---------------------------
    // Funciones de más alto nivel
    // ---------------------------
    // syntax sugar
    enum class AREF{
	selection_internal_to_AVCC
	, selection_external
	, selection_internal_to_1_1V};

    /// Seleccionamos cómo conectar AREF (internamente a AVCC, 
    /// externamente, o internamente a 1.1 V)
    static void AREF_selection(AREF aref_selection)
    {
	switch(aref_selection){
	    case AREF::selection_internal_to_AVCC:
		AREF_internal_to_AVCC();
		return;

	    case AREF::selection_external:
		AREF_external();
		return;

	    case AREF::selection_internal_to_1_1V:
		AREF_internal_to_1_1V();
		return;
	}
    }

    /// Indicamos que el ADC funciona en single-mode (solo lee cuando se 
    /// lo pedimos).
    /// \in frecuencia:	Frecuencia a la que queremos que funcione el ADC
    /// \in aref: nos dice si queremos conectar internamente AREF a AVCC.
    ///	Típicos valores: 62'5, 125 kHz
    template <uint16_t frecuencia = FRECUENCIA_POR_DEFECTO>
    static void single_mode(AREF aref)
    {
	AREF_selection(aref);
	clock_speed_in_kHz<frecuencia>();
	enable();
    }


    /// Indicamos que el ADC funciona en free running mode (continuamente
    /// lee y lee y lee del pin correspondiente)
    /// \in frecuencia:	Frecuencia a la que queremos que funcione el ADC
    /// \in Conectar_AREF_a_AVCC: nos dice si queremos conectar internamente
    ///			AREF a AVCC
    ///	Típicos valores: 62, 125 kHz
    template <uint16_t frecuencia = FRECUENCIA_POR_DEFECTO>
    static void free_running_mode(AREF aref)
    {
	AREF_selection(aref);
	clock_speed_in_kHz<frecuencia>();

	auto_trigger_source_free_running_mode();
	auto_trigger_enable();
	enable();
	
	start_conversion();    // la primera lectura hay que pedírsela
    }


    /// Esperamos hasta que el ADC haya finalizado con la lectura
    static void wait_until_conversion_is_complete()
    {while(!is_the_conversion_complete()) ;}




    /// Hace una petición de conversión y espera hasta que haya finalizado
    /// devolviendo el valor convertido en arefs (1 aref = AREF/1024 mV).
    /// Esta función lee del pin ya seleccionado.
    /// devolviendo el valor indicado en arefs (1 aref = AREF/1024 mV)
    // TODO: Modificarla para que se pueda elegir un tiempo máximo de espera a
    // leer (para ello necesitaría un reloj!!!).
    // La función debería devolver el control cuando:
    //		1.- ha leido del ADC
    //		2.- El timer ha vencido (se puede dar la opción de esperar
    //					 eternamente)
    //		3.- Ha ocurrido un error (¿puede saberse esto?)
    static uint16_t read_in_arefs()
    {
	start_conversion();
	wait_until_conversion_is_complete();

	return ADC_in_arefs();
    }

    /// Funcion de ayuda que permite cambiar de medida del ADC a milivoltios
    /// Ejemplol: ADC::tomV<4960>(100); // AREF = 4960 mV
    //
    /// Multiplicamos por el factor de conversión *AREF/1024.
    /// Versión SIN calibrar el ADC.
    // Por 1023 y no por 1024 ya que el valor más grande de 10 bits es
    // 2^10 - 1 = 1023!!! <-- Error (???): lo cambio y lo dejo tal como dice
    // la datasheet, punto 28.7. El valor más grande 0x03FF es Vref - 1 (en
    // binario). Esto es, 1024 corresponde a Vref (???)
    template<uint16_t AREF>
    static uint32_t tomV(uint32_t x)
    {return (x*AREF)/1024u;}

    /// Esta versión calcula los milivoltios a los que corresponde aref,
    /// pero habiendo calibrado el ADC. Para más información consultar
    /// la application note AVR120.
    /// Versión calibrada: arefs = m*V_pin + n, o equivalentemente
    /// V_pin = w*(arefs - n).
    /// Para evitar operar con floats, multiplicamos por 2^14, obteniendo:
    ///		factor = 2^14*w
    ///		correction = 2^14*(0.5 - w*n)
    /// donde el 0.5 es para que redonde en lugar de truncar.
    // DUDA: debería de poder pasarle un uint16_t, pero la correction se me
    // pasa de 66.000 (no entra en un uint16_t!!!)
    template<uint32_t factor, uint32_t correction>
    static uint16_t tomV(uint16_t aref)
    {
	return ((((uint32_t) factor*aref + correction) << 2) >> 16);
    }

    /// Hace una petición de conversión y espera hasta que haya finalizado
    /// devolviendo el valor convertido en milivoltios.
    /// Versión sin calibrar el ADC.
    /// AREF = valor que tiene el potencial AREF
    template<uint16_t AREF>
    static uint16_t read_in_mV()
    { return tomV<AREF>(read_in_arefs()); }

    /// Hace una petición de conversión y espera hasta que haya finalizado
    /// devolviendo el valor convertido en milivoltios.
    /// Versión calibrada: arefs = m*V_pin + n.
    template<uint32_t factor, uint32_t correction>
    static uint16_t read_in_mV()
    { return tomV<factor, correction>(read_in_arefs()); }


private:
    // Seleccionamos el pin en el que queremos que el ADC lea
    // Observar que el num_pin_ADC es el del ADC (ADC0, ... ADC5 ...)
    // no el numero de pin del chip.
    template <uint8_t num_pin_ADC>
    static void selecciona_pin_ADC_a_leer()
    {// los 4 bits menos significativos de ADMUX contienen el pin que se lee
	ADMUX = (0xf0 & ADMUX) | num_pin_ADC;
    }

    /// Definimos la velocidad del ADC (¿cómo preescalamos el reloj del AVR?)
    ///
    /// Recordar que el ADC para que funcione su reloj tiene que 
    /// estar entre 50 kHz y 200 kHz.
    /// Si F_CPU = 1MHz, al dividir el reloj entre 16 obtenemos una velocidad
    /// de muestreo de 16'2 kHz que es válida.
    static void clock_speed_entre_2()
    {
	/// 000 ó 001
	atd::Register(ADCSRA).write_zero_bit<ADPS2, ADPS1, ADPS0>();
    }


    static void clock_speed_entre_4()
    {
	// 010
	atd::Register(ADCSRA).write_zero_bit<ADPS2, ADPS0>   ();
	atd::Register(ADCSRA).write_one_bit <ADPS1>	    ();
    }

    static void clock_speed_entre_8()
    {
	// 011
	atd::Register(ADCSRA).write_zero_bit<ADPS2>	    ();
	atd::Register(ADCSRA).write_one_bit <ADPS1, ADPS0>   ();
    }

    static void clock_speed_entre_16()
    {
	// 100
	atd::Register(ADCSRA).write_one_bit <ADPS2>	    ();
	atd::Register(ADCSRA).write_zero_bit<ADPS1, ADPS0>   ();
	}

    static void clock_speed_entre_32()
    {
	// 101
	atd::Register(ADCSRA).write_one_bit <ADPS2, ADPS0>   ();
	atd::Register(ADCSRA).write_zero_bit<ADPS1>	    ();
    }

    static void clock_speed_entre_64()
    {
	// 110
	atd::Register(ADCSRA).write_one_bit <ADPS2, ADPS1>   ();
	atd::Register(ADCSRA).write_zero_bit<ADPS0>	    ();
    }

    static void clock_speed_entre_128()
    {
	// 111
	atd::Register(ADCSRA).write_one_bit <ADPS2, ADPS1, ADPS0>   ();
    }
};


// reloj del avr a 1MHz
// a 125 kHz
template<>
inline void ADC::clock_speed_in_kHz<125u, 1000000UL>()
{clock_speed_entre_8();}

// a 62'5 kHz
template<>
inline void ADC::clock_speed_in_kHz<62u, 1000000UL>()
{clock_speed_entre_16();}


}// namespace

#endif


