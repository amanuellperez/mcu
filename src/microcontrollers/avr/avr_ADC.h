// Copyright (C) 2024 Manuel Perez 
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

#ifndef __AVR_ADC_H__
#define __AVR_ADC_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	¿Cómo implementar un driver?
 *	1) El traductor: avr_ADC_basic.h
 *	2) Un (o varios) drivers: avr_ADC_drivers.h
 *	3) Interfaz genérico: avr_ADC_generic.h
 *
 *  1) El traductor se limita a traducir la datasheet. NO PUEDE tener ninguna
 *     función ajena a la datasheet, como mucho puede simplificar alguna cosa.
 *
 *  2) El driver contiene funciones de más alto nivel. El usuario del driver
 *     tiene que conocer los detalles del hardware particular.
 *     Ejemplo: para configurar el modo de lectura, ...
 *
 *  3) El interfaz genérico facilita la vida del programador, no teniendo que
 *     saber realmente qué dispositivo real hay por debajo. El problema de
 *     implementar esto es que se necesita experiencia programando diferentes
 *     ADCs.
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    14/06/2024 Primer intento: v-100
 *
 ****************************************************************************/

#include "avr_ADC_basic.h"

#include <atd_names.h>	// nm::ok/fail

namespace avr_{
/***************************************************************************
 *				ADC
 ***************************************************************************/
class ADC : public ADC_basic {
public:
// Types
    using AREF_type = ADC_basic::AREF_type;
    using Potential = avr_::Potential;

// Cfg
    static constexpr uint16_t adc_default_frequency_in_kHz = 125; // kHz

// Constructor
    // Static interface only
    ADC() = delete;
    
    /// Elegimos la frequency a la que queremos que funcione el ADC
    // clock_frequency_in_kHz<125>(); <-- indica que quieres que el reloj del ADC
    // funcione a 125 kHz. Esta función tiene la ventaja de que si pasas una
    // frequency no válida no compila, y si cambias de frequency recalcula
    // automáticamente qué divisor de frequency usar.
    // Tiene el inconveniente de que no se puede cambiar dinámicamente la
    // frecuencia.
    template<uint16_t adc_frequency_in_kHz
	, uint32_t clk_freq_in_hz = clock_frequency_in_hz>
    static void clock_frequency_in_kHz();

    template<uint32_t clk_freq_in_hz = clock_frequency_in_hz>
    static nm::Result clock_frequency(const Frequency& freq);
    
    // El cómo se conecta el ADC se tiene que conocer en tiempo de
    // compilaciónn. Si por lo que fuera alguna aplicación modifica
    // la conexión dinámicamente que use directamente el traductor.
    static constexpr uint8_t AREF_connect_to_internal_AVCC = 0;
    static constexpr uint8_t AREF_connect_to_external	   = 1;
    static constexpr uint8_t AREF_connect_to_internal_1_1V = 2;


    /// Indicamos que el ADC funciona en single-mode (solo lee cuando se 
    /// lo pedimos).
    /// \in freq:	Frecuencia a la que queremos que funcione el ADC
    /// \in AREF_connect_to: nos dice si queremos conectar internamente AREF a AVCC.
    ///	Típicos valores: 62'5, 125 kHz
    template <uint8_t AREF_connect_to, 
		    uint16_t adc_frequency_in_kHz = adc_default_frequency_in_kHz>
    static void single_mode();

    // Misma función pero dinámica. Si se le pasa una frecuencia errónea el
    // error no se detecta en tiempo de compilación. Usar la versión estatica
    // mejor.
    template <uint8_t AREF_connect_to>
    static nm::Result single_mode(const Frequency& freq);

    // Esta función es para single_mode solo ==> separar las clases de ADC
    // atendiendo al modo de operación o dejarlas fusionadas como aquí?
    static AREF_type read_in_arefs();

    template<uint16_t AREF>
    static Potential read_in_volts();

//    template <uint16_t frequency = default_frequency>
//    static void free_running_mode(AREF aref);

    static void wait_until_conversion_is_complete();


// helpers
    /// Convierte aref (potencial medido en unidades arefs) a potencial.
    template<uint16_t AREF_in_millivolts>
    static constexpr Potential aref2potential(uint32_t aref);

    // Devuelve el valor medido en voltios.
    // Versión static: esta es la ideal para usar. Supongo (???) que AREF se
    // define por hardware 
    template<uint16_t AREF>
    static Potential ADC_in_volts();

//    // Versión dinámica: le pasamos el valor de AREF
//    // Esta NO creo que sea necesaria salvo en los tests. No la defino.
//    static Potential ADC_in_volts(Potencial AREF);
private:
    // NOTA: la frecuencia de 12MHz solo la he usado una vez para programar la
    // TODO: están sin probar las de 8MHz y 12MHz!!!
    static nm::Result clock_frequency_in_kHz_1MHz(uint16_t adc_frequency);
    static nm::Result clock_frequency_in_kHz_8MHz(uint16_t adc_frequency);
    static nm::Result clock_frequency_in_kHz_12MHz(uint16_t adc_frequency);

    template <uint16_t adc_frequency>
    static void clock_frequency_in_kHz_1MHz();
    
    template <uint16_t adc_frequency>
    static void clock_frequency_in_kHz_8MHz();

    template <uint16_t adc_frequency>
    static void clock_frequency_in_kHz_12MHz();

    template <uint8_t AREF_connect_to>
    static void AREF_selection();

};


template <uint8_t AREF_connect_to>
inline void ADC::AREF_selection()
{
    if constexpr (AREF_connect_to ==  AREF_connect_to_internal_AVCC)
	    ADC_basic::AREF_internal_to_AVCC();

    else if constexpr (AREF_connect_to == AREF_connect_to_external)
	    ADC_basic::AREF_external();

    else if constexpr (AREF_connect_to == AREF_connect_to_internal_1_1V)
	    ADC_basic::AREF_internal_to_1_1V();

    else 
	static_assert(atd::always_false_v<int>,
		    "Wrong AREF_connect_to value");

}

// ¿Cuándo puede devolver fail? 
//  En caso de un error por parte del programador, que intenta asignar
//  una frecuencia indebida. Es mejor la versión que se le pasa la frecuencia
//  en tiempo de compilación ya que en esa versión se obtiene el error al
//  compilar.
//
//  Por ello, evitar usar esta función si se puede.
template <uint8_t AREF_connect_to>
nm::Result ADC::single_mode(const Frequency& freq)
{
    if (clock_frequency(freq) == nm::fail)
	return nm::fail;

    AREF_selection<AREF_connect_to>();
    ADC_basic::single_conversion_mode();
    ADC_basic::enable();

    return nm::ok;
}

// Indicamos que el ADC funciona en single-mode (solo lee cuando se 
// lo pedimos).
// \in frequency:	Frecuencia a la que queremos que funcione el ADC
// \in aref: nos dice si queremos conectar internamente AREF a AVCC.
//	Típicos valores: 62'5, 125 kHz
template <uint8_t AREF_connect_to, uint16_t adc_frequency_in_kHz>
void ADC::single_mode()
{
    clock_frequency_in_kHz<adc_frequency_in_kHz>();

    AREF_selection<AREF_connect_to>();
    ADC_basic::single_conversion_mode();
    ADC_basic::enable();
}


//template <uint16_t frequency = default_frequency>
//static void free_running_mode(AREF aref)
//{
//    AREF_selection(aref);
//    clock_frequency_in_kHz<frequency>();
//
//    auto_trigger_source_free_running_mode();
//    auto_trigger_enable();
//    enable();
//    
//    start_conversion();    // la primera lectura hay que pedírsela
//}


/// Esperamos hasta que el ADC haya finalizado con la lectura
inline void ADC::wait_until_conversion_is_complete()
{while(!ADC_basic::is_the_conversion_complete()) ;}





template<uint16_t AREF>
inline Potential ADC::read_in_volts()
{return aref2potential<AREF>(read_in_arefs());}


template<uint16_t AREF>
constexpr Potential ADC::aref2potential(uint32_t x)
{
    Millivolt res{AREF};
    res *= x;
    res /= uint32_t{1024};

    return res;
}



///// Esta versión calcula los milivoltios a los que corresponde aref,
///// pero habiendo calibrado el ADC. Para más información consultar
///// la application note AVR120.
///// Versión calibrada: arefs = m*V_pin + n, o equivalentemente
///// V_pin = w*(arefs - n).
///// Para evitar operar con floats, multiplicamos por 2^14, obteniendo:
/////		factor = 2^14*w
/////		correction = 2^14*(0.5 - w*n)
///// donde el 0.5 es para que redonde en lugar de truncar.
//// DUDA: debería de poder pasarle un uint16_t, pero la correction se me
//// pasa de 66.000 (no entra en un uint16_t!!!)
//template<uint32_t factor, uint32_t correction>
//static uint16_t tomV(uint16_t aref)
//{
//    return ((((uint32_t) factor*aref + correction) << 2) >> 16);
//}



// clock speed
// -----------
// (RRR) ¿por qué no simplemente intentar calcular el prescaler
//        clock_frequency / adc_frequency y dar un error si no es válido?
//        Esa opción tiene la ventaja de ser genérica; la opción particular
//        que ahora implemento es mejor para los principiantes ya que muestra
//        un mensaje de error diciendo cómo corregir el problema.
template <uint16_t adc_frequency>
void ADC::clock_frequency_in_kHz_1MHz()
{
    // Voy a empezar en 500kHz aunque seguramente sea demasiado rápida (???)
    if constexpr (adc_frequency == 500u)
	ADC_basic::clock_frequency_divide_by_2();

    else if constexpr (adc_frequency == 250u)
	ADC_basic::clock_frequency_divide_by_4();
    
    else if constexpr (adc_frequency == 125u)
	ADC_basic::clock_frequency_divide_by_8();

    else if constexpr (adc_frequency == 62u or adc_frequency == 63u)
	ADC_basic::clock_frequency_divide_by_16();
    // El resto da frecuencias inferiores a 50kHz

    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect ADC frequency. Try another one. "
		    "Valid ones: 500kHz, 250kHz, 125kHz or 62kHz. "
		    "To 10 digits resolution only 125kHz or 62kHz."
		    );

}

template <uint16_t adc_frequency>
void ADC::clock_frequency_in_kHz_8MHz()
{
// ¿puede funcionar a 4MHz? Los límites para que funcione bien con 10 bits de
// resolución son de 50kHz a 200kHz.
// Voy a empezar en 500kHz aunque seguramente sea demasiado rápida (???)
    if constexpr (adc_frequency == 500u)
	ADC_basic::clock_frequency_divide_by_16();

    else if constexpr (adc_frequency == 250u)
	ADC_basic::clock_frequency_divide_by_32();
    
    else if constexpr (adc_frequency == 125u)
	ADC_basic::clock_frequency_divide_by_64();

    else if constexpr (adc_frequency == 62u or adc_frequency == 63u)
	ADC_basic::clock_frequency_divide_by_128();

    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect ADC frequency. Try another one. "
		    "Valid ones: 500kHz, 250kHz, 125kHz or 62kHz. "
		    "To 10 digits resolution only 125kHz or 62kHz."
		    );

}


template <uint16_t adc_frequency>
void ADC::clock_frequency_in_kHz_12MHz()
{
// ¿puede funcionar a 4MHz? Los límites para que funcione bien con 10 bits de
// resolución son de 50kHz a 200kHz.
// Voy a empezar en 500kHz aunque seguramente sea demasiado rápida (???)
    if constexpr (adc_frequency == 375u)
	ADC_basic::clock_frequency_divide_by_32();

    else if constexpr (adc_frequency == 187u or adc_frequency == 188u)
	ADC_basic::clock_frequency_divide_by_64();
    
    else if constexpr (adc_frequency == 94u)
	ADC_basic::clock_frequency_divide_by_128();


    else
	static_assert(atd::always_false_v<int>,
		    "Incorrect ADC frequency. Try another one. "
		    "Valid ones: 375kHz, 187kHz or 94kHz. "
		    "To 10 digits resolution only 187kHz or 94kHz."
		    );

}

template<uint16_t adc_frequency_in_kHz
	, uint32_t clock_frequency_in_hz>
inline void ADC::clock_frequency_in_kHz()
{
    if constexpr (clock_frequency_in_hz == 1'000'000UL)
	clock_frequency_in_kHz_1MHz<adc_frequency_in_kHz>();

    else if constexpr (clock_frequency_in_hz == 8'000'000UL)
	clock_frequency_in_kHz_8MHz<adc_frequency_in_kHz>();

    else if constexpr (clock_frequency_in_hz == 12'000'000UL)
	clock_frequency_in_kHz_12MHz<adc_frequency_in_kHz>();

    else
        static_assert(atd::always_false_v<int>,
                      "I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");
}


template<uint32_t clock_frequency_en_hz>
nm::Result ADC::clock_frequency(const Frequency& adc_freq)
{
    if constexpr (clock_frequency_in_hz == 1'000'000UL)
	return clock_frequency_in_kHz_1MHz(KiloHertz{adc_freq}.value());

    else if constexpr (clock_frequency_in_hz == 8'000'000UL)
	return clock_frequency_in_kHz_8MHz(KiloHertz{adc_freq}.value());

    else if constexpr (clock_frequency_in_hz == 12'000'000UL)
	return clock_frequency_in_kHz_12MHz(KiloHertz{adc_freq}.value());

    else
        static_assert(atd::always_false_v<int>,
                      "I'm lazy. I haven't implemented "
                      "that frequency. Please implement it.");

    return nm::fail;
}

 
/***************************************************************************
 *				ADC_pin
 ***************************************************************************/
// (RRR) ¿Por qué pasar como parámetro AREF?
//	 AREF solo se necesita en
// AREF = valor en milivoltios del potencial de referencia que usa el ADC
template <uint8_t npin, uint16_t AREF0_in_millivolt>
    requires requires {ADC::select_pin<npin>();}
class ADC_pin_single_mode{
public:
// Types
    using Potential = ADC::Potential;
    using AREF_type = ADC::AREF_type;

    // Observar que la resolution va a ser independiente de que usemos 8, 9 ó
    // 10 bits. Voy a devolver siempre las partes por 1024 aunque usemos solo
    // 8 bits. 
    static constexpr uint16_t resolution = 1024;
    using Parts_per = atd::Parts_per<resolution>;

// Cfg
    static constexpr uint16_t AREF  = AREF0_in_millivolt;
    static constexpr uint8_t number = npin;


// Constructor
    ADC_pin_single_mode() = delete; // static interface
				      
// Read
    // ¿Qué es lo que lee el ADC? Realmente lo que está leyendo es el tanto
    // por 1024 (10 bits de resolución) del potencial conectado al pin AREF.
    // Un interfaz genérico devolverá Percentage y no AREF_type ya que si en
    // lugar de 10 bits manejamos 8, mediremos el tanto por 256.
    //
    // TODO: de momento doy por supuesto que leemos 10 bits y que está
    // alineado a la derecha. Para leer 8 bits se alinea a la izda y las
    // partes serían por 1024 / 4: habría que devolver 
    //	    return atd::Parts_per<1024>{arefs * 4};
    //	en ese caso.
    //	¿Se podría parametrizar el pin indicando la resolución: 8, 9 ó 10
    //	bits? 
    //
    //	Aunque devuelve Parts_per<1024> dejo el nombre como read_percentage
    //	por ser más genérico (y a fin de cuentas Parts_per<1024> y percentage
    //	significan lo mismo).
    static AREF_type read_arefs()
    {
	ADC::wait_until_conversion_is_complete();
	ADC::select_pin<npin>();
	return ADC::read_in_arefs();
    }

    static Parts_per read_percentage()
    { return Parts_per{read_arefs()}; }

    
    // A priori, ignorando cómo se usa realmente el ADC, lo que se dice es que
    // el ADC mide "potencial". Si esto es cierto da la impresión de que el
    // programador lo que quiere leer es el potencial y no el tanto por cien
    // de AREF. Luego la siguiente función será la necesaria. 
    // Esta función es la que necesita AREF pasarlo como parámetro.
    static Potential read_potential()
    {
	return ADC::aref2potential<AREF>(read_arefs());
    }
    
    // DUDA: read_potential vs read_volts???
    static Potential read_volts()
    {return read_potential(); }
};


}// namespace avr_

#endif


