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

#include <algorithm>

#include "dev.h"
#include "cfg.h"
#include "pulse.h"

/***************************************************************************
 *			    FUNCIONES AUXILIARES
 ***************************************************************************/
// src = tiempo de duración del pulso
// is_one  = indica si el tiempo mide un pulso high or low
// El array que devolvemos es un array de pulsos: low-high (y no high-low).
static void copy(volatile uint16_t* src, volatile bool* is_one, int16_t n
		, atd::CArray_view<Pulse>& dst)
{
    if (n <= 0){
	dst.size = 0;
	return;
    }

    size_t N = static_cast<size_t>(n);
    auto p = dst.ptr;
    size_t i = 0;
    if (is_one[0] == true){ // esto no es de esperar, el primer pulso debería ser low
	p->time_low  =  0;
	p->time_high = src[0];
	++i;
	++p;
    }

    dst.size = N / 2;
    for (; i  + 1 < N; i += 2){
	if (is_one[i] == false and is_one[i+1] == true){
	    p->time_low  = src[i];
	    p->time_high = src[i+1];
	}
	else{
	    p->time_low  = 0;
	    p->time_high = 0;
	}
	++p;
    }

    // Caso en que el último pulso leido sea LOW 
    if (i < N){
	++dst.size;

	p->time_low = src[i];
	p->time_high = 0; // <-- 0 para indicar que está sin leer
    }
}


/***************************************************************************
 *			    IMPLEMENTACIÓN
 ***************************************************************************/
// Dependencias
// ------------
//  1. De hardware: (en dev.h)
//	+ ir_receiver_pin
//	+ avr	!
//		 => esto es el hardware necesario para programar esto.
//	+ Timer /
//
//  2. De software: (en cfg.h)
//	+ num_max_pulses


// static volatile Timer::counter_type buffer[Main::max_num_data];
static constexpr int16_t buffer_size = num_max_pulses * 2;
static volatile uint16_t buffer[buffer_size];
static volatile bool is_high[buffer_size];
static volatile int16_t i = -1;

static void reset_data()
{
    i = -1;
}



void receive_pulses(atd::CArray_view<Pulse>& pulse)
{
    reset_data();

    avr::Interrupt::enable_pin<ir_receiver_pin>();

    Timer::on<1>();	    // leemos microsegundos
    
    avr::enable_all_interrupts();

    while (i < 0) { ; }	// esperamos a recibir algo

    // while (Timer::safe_value() < Timer::max_top() <-- esto lo ralentiza (???)
    while (Timer::safe_value() < Timer::counter_type{60000}
		and i < buffer_size)
    { ; }


    avr::disable_all_interrupts();

    Timer::off(); 

    avr::Interrupt::disable_pin<ir_receiver_pin>();

    copy(buffer, is_high, i, pulse);

}

// (RRR) ¿por qué is_high[i]?
//	 Al principio no llevaba control de si es HIGH o no, dando por
//	 supuesto que siempre empezaba en LOW. Pero al probarlo resultó no ser
//	 así: a veces empieza en HIGH, otras en LOW, no funcionando bien.
//	 Intenté garantizar que el pin del sensor fuera HIGH, pero no me
//	 funcionó (¿cometí algún error? probable). Así que al final anoto el
//	 tipo de pulso que es.
//
//	 Si IR_receiver::is_one() quiere decir que el pin estaba en zero, luego NO
//	 era HIGH ==> is_high = false.
ISR_RECEIVER_PIN {
    if (i >= 0){
	buffer[i]  = Timer::unsafe_value();
	is_high[i] = !IR_receiver::is_one();
    }

    Timer::unsafe_reset();
    i = i + 1;
}





