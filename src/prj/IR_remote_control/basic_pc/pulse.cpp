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
// src = array de 'n' elementos conteniendo alternativamente la duracion de
// los pulsos low-high-low-high...
static void copy(volatile uint16_t* src, int8_t n, atd::CArray_view<Pulse>& dst)
{
    if (n < 0){
	dst.size = 0;
	return;
    }

    size_t N = static_cast<size_t>(n);
    dst.size = N / 2;
    for (size_t i = 0; i < dst.size * 2; i += 2){
	dst[i / 2].time_low  = src[i];
	dst[i / 2].time_high = src[i+1];
    }

    // Caso en que el último pulso leido sea LOW 
    if (dst.size * 2 == N - 1){
	++dst.size;

	dst [dst.size - 1].time_low = src[N - 1];
	dst [dst.size - 1].time_high = 0; // <-- 0 para indicar que está sin leer
    }
}


/***************************************************************************
 *			    IMPLEMENTACIÓN
 ***************************************************************************/
// Dependencias
// ------------
//  1. De hardware: (en dev.h)
//	+ sensor_pin
//	+ avr	!
//		 => esto es el hardware necesario para programar esto.
//	+ Timer /
//
//  2. De software: (en cfg.h)
//	+ num_max_pulses

// static volatile Timer::counter_type buffer[Main::max_num_data];
static volatile uint16_t buffer[num_max_pulses*2];
static volatile int8_t i = -1;
static volatile bool end_of_reading = false;

static void reset_data()
{
    i = -1;
    end_of_reading = false;
}



void read_pulses(atd::CArray_view<Pulse>& pulse)
{
    reset_data();

    avr::enable_all_interrupts();
    Timer::enable_overflow_interrupt();
    avr::Interrupt::enable_pin<sensor_pin>();

    Timer::on<1>();	    // leemos microsegundos
    
    while (end_of_reading == false and i < 2 * num_max_pulses)
    { ; }

    avr::disable_all_interrupts();

    Timer::off(); 

    copy(buffer, i, pulse);

}


ISR_SENSOR_PIN {
    if (i >= 0)
	buffer[i] = Timer::value();

    Timer::reset();
    i = i + 1;
}



ISR_TIMER_OVF {
    if (i != -1){
	end_of_reading = true;
	buffer[i] = Timer::top();   // acabo leyendo el pulso HIGH
	i = i + 1;
    }
}


