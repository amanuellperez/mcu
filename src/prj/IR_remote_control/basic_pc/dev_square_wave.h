// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __DEV_SQUARE_WAVE_H__
#define __DEV_SQUARE_WAVE_H__

#include <avr_pin.h>	// TODO: particular del avr ¿Generic_pin?
	
#include "dev_train_of_pulses.h"

namespace dev{

// El objetivo es generar trenes de burst de 38 kHz.
// Varias formas de hacerlo:
//  1. Usando 2 timers y una puerta lógica AND.
//     Un timer genera 38kHz y el otro el envelope haciéndose el AND de las 2
//     señales. Muy eficiente pero necesitamos la puerta AND.
//
//  2. Usar 2 timers: uno sirve para generar la señal de 38 kHz mientras que
//     el otro mide el tiempo. (esta es la opción que implemento ahora)
//
//  3. Usar 1 timer: para saber el tiempo que dura el burst tiene que ir
//     contando los pulsos generados (generamos interrupción y con contador)
//
//
//  Precondición: El Clock_us está configurado correctamente. Es un clock (el
//  cliente habrá llamado a Clock_us::init()).
//
//  (RRR) ¿por qué definir wait_us si aquí no lo necesito? 
//        Para facilitar la vida al cliente al enviar trenes de burst.
//        Sin wait_us se escribe:
//		SWG::burst_38kHz_of(562);
//		Clock_us::wait_us(562);
//
//	  Con el wait_us:
//		SWG::burst_38kHz_of(562);
//		SWG::wait_us(562);
//	    
template < typename Timer0, uint8_t npin
	 , typename Clock_us0>
class Square_wave_generator{
public:
// Types
    using Timer		= Timer0;
    using Clock_us	= Clock_us0;
    using counter_type	= typename Timer::counter_type;

// Preconditions
    static_assert(npin == Timer::pin_channel1);

// Operations
    static void generate_38kHz_on();
    static void generate_38kHz_off();

    static void burst_38kHz_of(Clock_us::counter_type time_in_us);
    static void wait_us(Clock_us::counter_type time_in_us);

    // TODO: esta función no genera una onda cuadrada sino un tren de
    // bursts!!! ¿por qué está dentro de esta clase?
    template <size_t N>
    static bool transmit(const Train_of_pulses<N>& pulse);


private:
    // TODO: que admita Frequency directamente.
    static constexpr counter_type freq_in_kHz_to_top(const counter_type& freq);

    template <size_t N>
    static void transmit1(const Train_of_pulses<N>& pulse);
};


template <typename T, uint8_t N, typename C>
inline constexpr typename Square_wave_generator<T,N, C>::counter_type 
    Square_wave_generator<T,N,C>::freq_in_kHz_to_top(const counter_type& freq)
{ return 1000 / (freq * 2); }


template <typename Timer0, uint8_t N, typename C>
void Square_wave_generator<Timer0, N, C>::generate_38kHz_on()
{
    // constexpr Timer::counter_type T = 1000 / (38 * 2); // freq = 38kHz
    constexpr counter_type T = freq_in_kHz_to_top(38);

    Timer::mode_square_wave();
    Timer::square_wave_top(T); 
    Timer::square_wave_connect_ch1(); 
    Timer::template on<1>();		
}

template <typename T, uint8_t N, typename C>
void Square_wave_generator<T,N,C>::generate_38kHz_off()
{
    Timer::off();
    Timer::square_wave_disconnect_ch1();

    // Garantizamos que acabe en cero
    avr::Pin<Timer::pin_channel1>::as_output();
    avr::Pin<Timer::pin_channel1>::write_zero();

}


template <typename T, uint8_t N, typename C>
void Square_wave_generator<T,N,C>::
		    burst_38kHz_of(Clock_us::counter_type time_in_us)
{
    generate_38kHz_on();
    Clock_us::wait_us(time_in_us);
    generate_38kHz_off();
}


template <typename T, uint8_t N, typename C>
inline void Square_wave_generator<T,N,C>::
		    wait_us(Clock_us::counter_type time_in_us)
{
    Clock_us::wait_us(time_in_us);
}


template <typename T, uint8_t npin, typename C>
template <size_t N>
bool Square_wave_generator<T,npin,C>::transmit(const Train_of_pulses<N>& pulse)
{
    if(pulse.polarity()){
	transmit1(pulse);
	return true;
    }

    return false;

}


template <typename T, uint8_t npin, typename C>
template <size_t N>
void Square_wave_generator<T,npin,C>::transmit1(const Train_of_pulses<N>& pulse)
{
    Clock_us::on();

    for (auto p = pulse.begin(); p != pulse.end(); ++p){
	burst_38kHz_of(p->time_low);	
	Clock_us::wait_us(p->time_high);
    }

    Clock_us::off();
}


} // namespace



#endif

