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

#ifndef __MCU_PWM_H__
#define __MCU_PWM_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones genéricas para generar PWM	
 *
 *      De momento pruebo a suministrar dos formas diferentes de generarlo: 
 *      una con interrupciones, y otra haciendo polling. Cotilleando el código
 *      en ensamblador, da la impresión de que la polling version es más
 *      eficiente (sobre todo para enviar arrays de uint8_t).
 *
 *      La frecuencia del micro limita la frecuencia máxima que se puede
 *      generar.
 *
 * HISTORIA
 *    Manuel Perez
 *    27/08/2024 Pulse_wave_01_pin
 *
 ****************************************************************************/
#include <ostream>

#include <atd_percentage.h>
#include <atd_names.h>	// ok/fail

namespace mcu{
/***************************************************************************
 *			    PULSE_WAVE_01_INTERRUPT
 ***************************************************************************/
// struct Pulse_wave_01_pin_cfg {
//
//    using PWM_pin = ...
//
//    constexpr uint32_t frequency_in_hz = 800'000;
//    constexpr uint8_t duty_cycle_bit0 = 30;
//    constexpr uint8_t duty_cycle_bit1 = 60;
// };
//
//
//
// Un sistema de codificación de ceros y unos es codificarlos como pulsos de
// diferente longitud, generando un "pulse wave" de ceros y unos (de ahi el
// nombre) (DUDA: ¿cómo se llama este tipo de codificación?)
//
// Ejemplo: la led strip WS2812B
//
// Pulse_wave_01_pin<800'000, 30, 60>: queremos generar, usando el PWM_pin,
// una señal de 800kHz donde los ceros se codifican en pulsos del 30% 
// de duty cycle, mientras que los unos se codifican en pulsos del 60%.
template <typename Cfg>
class Pulse_wave_01_pin_interrupt{
public:
// types
    using PWM_pin = Cfg::PWM_pin;

    static constexpr uint8_t number = PWM_pin::number;

// cfg
    static constexpr uint32_t frequency_in_hz = Cfg::frequency_in_hz;
    static constexpr auto duty_cycle_bit0 = Cfg::duty_cycle_bit0;
    static constexpr auto duty_cycle_bit1 = Cfg::duty_cycle_bit1;

// constructor
    static nm::Result init();

// function
    // Envia `nbits` del uint16_t x empezando por x[0]
    static void send(const uint16_t x, uint8_t nbits = 16);

// Interrupts
// Precondition: damos por supuesto que la interrupción que se captura es
// la finalización del pulso generado (que corresponderá con cuando se alcance
// OCR en los avr)
// Esto es: generamos un pulso
//	    +----+         +---
//          |    |         |
//          |    |         |
// ---------+    +---------+
//          1    2         3
//
//  En 1 empieza el pulso, en 2 acaba el pulso, y en 3 empieza el siguiente
//  pulso. Suponemos que es en 2 cuando se genera la interrupción
//  (independientemente de que el idle sea 0 ó 1)
//
//  Le añado el atributo always_inline para forzarla inline
//  (buscamos eficiencia y además esta función solo se llamará una vez dentro
//  de la interrupcion)
    static void handle_interrupt() __attribute__((always_inline));

// debug
    static void print_cfg(std::ostream& out);

private:
    using Prescaler = typename PWM_pin::Prescaler;
    using Hertz     = typename PWM_pin::Hertz;
    using Frequency = Hertz;

    using Data   = uint16_t;	// DUDA: pasarlo como template parameter?
    using counter_type = PWM_pin::counter_type;

    // De momento, en la primera implementación, copio los datos que quiero
    // enviar y los guardo. ¿Se podría usar un puntero? Probablemente que no
    // ya que al usar interrupciones se devuelve el control a la función
    // `send` inmediatamente, pudiendo "destruirse" la variable `x` pasada a
    // `send` no siendo válido el puntero.
    inline static volatile Data data_; 
    inline static volatile uint8_t i_;     // bit que estamos enviando
    inline static volatile uint8_t n_;	    // número de bits a enviar

    // TODO: ocr_bit0 y ocr_bit1 se pueden calcular en tiempo de compilación.
    // Para ello es necesario modificar las funciones PWM_pin que lo
    // configuran que ahora no son constexpr. Cambiarlas a constexpr.
    inline static counter_type ocr_bit0_;
    inline static counter_type ocr_bit1_;
    inline static Prescaler prescaler_;
};

template <typename C>
nm::Result Pulse_wave_01_pin_interrupt<C>::init()
{
    typename PWM_pin::Timer_cfg cfg;
    if (PWM_pin::cfg_to_generate(Hertz{frequency_in_hz}, cfg) == nm::fail)
	return nm::fail;

    PWM_pin::cfg(cfg);
    prescaler_ = cfg.prescaler;

    ocr_bit0_ = PWM_pin::duty_cycle(cfg, atd::Percentage{duty_cycle_bit0});
    ocr_bit1_ = PWM_pin::duty_cycle(cfg, atd::Percentage{duty_cycle_bit1});

    return nm::ok;
}

// Precondicion: PWM_pin::is_turn_off();
template <typename C>
void Pulse_wave_01_pin_interrupt<C>::send(const uint16_t x, uint8_t nbits)
{
    data_ = x;
    n_    = nbits - 1; // uso nbits - 1 para poner el ++i_ en el handle al
		       // final
    i_    = 0;

    if (data_ & (1 << i_))
	PWM_pin::duty_cycle(ocr_bit1_); 
    else
	PWM_pin::duty_cycle(ocr_bit0_); 

    i_ = i_ + 1; // al entrar en el handle entramos con i_ = 1

    PWM_pin::connect();
    PWM_pin::enable_interrupt();

    PWM_pin::turn_on(prescaler_);
}

// Lo primero que pongo es la actualización del OCR para que el Timer sepa a
// cuál es el valor del siguiente 
template <typename C>
inline void Pulse_wave_01_pin_interrupt<C>::handle_interrupt()
{
    if (data_ & (1 << i_))
	PWM_pin::unsafe_duty_cycle(ocr_bit1_);

    else
	PWM_pin::unsafe_duty_cycle(ocr_bit0_);

    i_ = i_ + 1;

    if (i_ == n_){
	PWM_pin::turn_off();
	PWM_pin::disable_interrupt();
    }

}

template <typename C>
void Pulse_wave_01_pin_interrupt<C>::print_cfg(std::ostream& out)
{
    out <<   "prescaler = " << prescaler_ << 
	   "\nocr_bit0  = " << ocr_bit0_ <<
	   "\nocr_bit1  = " << ocr_bit1_;
}


/***************************************************************************
 *			    PULSE_WAVE_01_POLLING
 ***************************************************************************/
// struct Pulse_wave_01_pin_cfg {
//
//    using PWM_pin = ...
//    using Micro   = ...
//
//    constexpr uint32_t frequency_in_hz = 800'000;
//    constexpr uint8_t duty_cycle_bit0 = 30;
//    constexpr uint8_t duty_cycle_bit1 = 60;
// };
//
//
//
template <typename Cfg>
class Pulse_wave_01_pin_polling{
public:
// types
    using PWM_pin = Cfg::PWM_pin;
    using Disable_interrupts = typename Cfg::Micro::Disable_interrupts;

    static constexpr uint8_t number = PWM_pin::number;

// cfg
    static constexpr uint32_t frequency_in_hz = Cfg::frequency_in_hz;
    static constexpr auto duty_cycle_bit0 = Cfg::duty_cycle_bit0;
    static constexpr auto duty_cycle_bit1 = Cfg::duty_cycle_bit1;

// constructor
    static nm::Result init();

// function
    // Enviamos x[0...n). Observar que, de momento, enviamos bytes enteros.
    // Esta función bloquea el micro. No devuelve el control hasta no acabar
    static void send(const uint8_t* x, uint8_t n);


// debug
    static void print_cfg(std::ostream& out);

private:
    using Prescaler = typename PWM_pin::Prescaler;
    using Hertz     = typename PWM_pin::Hertz;
    using Frequency = Hertz;

    using counter_type = PWM_pin::counter_type;

    // TODO: ocr_bit0 y ocr_bit1 se pueden calcular en tiempo de compilación.
    // Para ello es necesario modificar las funciones PWM_pin que lo
    // configuran que ahora no son constexpr. Cambiarlas a constexpr.
    inline static counter_type ocr_bit0_;
    inline static counter_type ocr_bit1_;
    inline static Prescaler prescaler_;

// Functions
    static void send_impl(const uint8_t* x, uint8_t n);

};

// Misma que Pulse_wave_01_pin_interrupt
template <typename C>
nm::Result Pulse_wave_01_pin_polling<C>::init()
{
    typename PWM_pin::Timer_cfg cfg;
    if (PWM_pin::cfg_to_generate(Hertz{frequency_in_hz}, cfg) == nm::fail)
	return nm::fail;

    PWM_pin::cfg(cfg);
    prescaler_ = cfg.prescaler;

    ocr_bit0_ = PWM_pin::duty_cycle(cfg, atd::Percentage{duty_cycle_bit0});
    ocr_bit1_ = PWM_pin::duty_cycle(cfg, atd::Percentage{duty_cycle_bit1});

    return nm::ok;
}

// Precondicion: PWM_pin::is_turn_off();
// Diferencias con Pulse_wave_01_pin_interrupt: 
//
// 1) como esta función va a bloquear el array x[0..n) va a ser válido 
//    durante toda su ejecución no siendo necesario copiarlo internamente.
//
// 2) Al no usar interrupciones las variables no son volatile. Además, al ser
//    locales confio (???) en que se guarden en registros no necesitando
//    perder tiempo en cargarlas desde memoria a los registros.
//
// 3) Al hacer polling ahorro la llamada a la interrupción, pero la sustituyo
//    por el polling. ¿es más eficiente?
//
template <typename C>
void Pulse_wave_01_pin_polling<C>::send(const uint8_t* x, uint8_t n)
{
    if (n == 0)
	return;

    Disable_interrupts disint;

    PWM_pin::reset();
    PWM_pin::connect();
    PWM_pin::enable_interrupt(); 

    send_impl(x, n);

    PWM_pin::turn_off();
    PWM_pin::disable_interrupt();
}

// Esta es la función que tiene que ser eficiente
template <typename C>
void Pulse_wave_01_pin_polling<C>::send_impl(const uint8_t* x, uint8_t n)
{
    PWM_pin::unsafe_duty_cycle(0); // generamos un pulso 0
    PWM_pin::turn_on(prescaler_);


    for (uint8_t i = 0; i < n; ++i){

	for (uint8_t j = 0; j < 8; ++j){
	    
	    PWM_pin::wait_for_interrupt();

	    if (x[i] & (1 << j))
		PWM_pin::unsafe_duty_cycle(ocr_bit1_);

	    else
		PWM_pin::unsafe_duty_cycle(ocr_bit0_);
	}
    }

    // Esperamos a generar el último pulso
    PWM_pin::wait_for_interrupt();
}


// TODO borrame: antigua, pierde el primer pulso
//template <typename C>
//void Pulse_wave_01_pin_polling<C>::send_impl(const uint8_t* x, uint8_t n)
//{
//    // El tener que configurar el primer pulso antes de empezar, rompe todo el
//    // flujo (queda muy feo, alguna forma más sencilla?)
//    uint8_t j = 0;
//
//    if (x[0] & (1 << j))
//	PWM_pin::unsafe_duty_cycle(ocr_bit1_);
//
//    else
//	PWM_pin::unsafe_duty_cycle(ocr_bit0_);
//    
//    ++j;
//
//    PWM_pin::turn_on(prescaler_);
//
//
//    for (uint8_t i = 0; i < n; ++i){
//
//	for (; j < 8; ++j){
//	    
//	    PWM_pin::wait_for_interrupt();
//
//	    if (x[i] & (1 << j))
//		PWM_pin::unsafe_duty_cycle(ocr_bit1_);
//
//	    else
//		PWM_pin::unsafe_duty_cycle(ocr_bit0_);
//	}
//	
//	j = 0;
//
//    }
//
//    // Esperamos a generar el último pulso
//    PWM_pin::wait_for_interrupt();
//}


template <typename C>
void Pulse_wave_01_pin_polling<C>::print_cfg(std::ostream& out)
{
    out <<   "prescaler = " << prescaler_ << 
	   "\nocr_bit0  = " << ocr_bit0_ <<
	   "\nocr_bit1  = " << ocr_bit1_;
}


}// mcu


#endif


