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

// ptrain: train of pulses
//	   Recibimos un tren de pulsos
//
// Este módulo está parametrizado por:
//		Timer e ir_receiver_pin
#include <algorithm>

#include "dev.h"
#include "cfg.h"
#include "pulse.h"

/***************************************************************************
 *			    FUNCIONES AUXILIARES
 ***************************************************************************/
// Comprueba que la sucesión de niveles es alternada: 0,1,0,1,...
static bool check(uint8_t polarity, volatile bool* level, int16_t n)
{ 
    if (n <= 0)
	return true;

    bool last_level = polarity;

    for (int16_t i = 0; i < n; ++i){
	if (level[i] == last_level)
	    return false;

	last_level = level[i];
    }

    return true;
}


// precondition: n > 0
// Observar que en caso de que se haya recibido mal el último pulso, no lo
// anotamos.
static uint16_t copy_polarity1(volatile uint16_t* src, int16_t n
		, Pulse* dst, uint16_t N)
{
    uint16_t size = std::min(static_cast<size_t>(n) / 2u, N);

    for (uint16_t i = 0; i < size; ++i){
	dst[i].time_low  = src[2*i];
	dst[i].time_high = src[2*i + 1];
    }

    return size;
}


// precondition: n > 0
// Observar que en caso de que se haya recibido mal el último pulso, no lo
// anotamos.
static uint16_t copy_polarity0(volatile uint16_t* src, int16_t n
		, Pulse* dst, uint16_t N)
{
    uint16_t size = std::min(static_cast<size_t>(n) / 2u, N);

    for (uint16_t i = 0; i < size; ++i){
	dst[i].time_high = src[2*i];
	dst[i].time_low  = src[2*i + 1];
    }

    return size;
}


static uint16_t copy( uint8_t polarity
		, volatile uint16_t* src, int16_t n
		, Pulse* dst, uint16_t N)
{
    if (n <= 0)
	return 0;

    if (polarity)
	return copy_polarity1(src, n, dst, N);

    else
	return copy_polarity0(src, n, dst, N);
}



/***************************************************************************
 *			    IMPLEMENTACIÓN
 ***************************************************************************/
static constexpr int16_t buffer_size = num_max_pulses * 2;
static volatile uint16_t buffer_[buffer_size];
static volatile bool level_[buffer_size];
static volatile int16_t nsemipulse_ = -1;


static void receive_semipulses()
{
    static constexpr Timer::counter_type time_out{60000};

    nsemipulse_ = -1;

    avr::Interrupt::enable_pin<ir_receiver_pin>();

    Timer::on<1>();	    // leemos microsegundos
    
    avr::enable_all_interrupts();

    while (nsemipulse_ < 0) { ; }	// esperamos a recibir algo

    // while (Timer::safe_value() < Timer::max_top() <-- esto lo ralentiza (???)
    while (Timer::safe_value() < time_out
		and nsemipulse_ < buffer_size)
    { ; }


    avr::disable_all_interrupts();

    Timer::off(); 

    avr::Interrupt::disable_pin<ir_receiver_pin>();

// anotamos el último semiperiodo
    buffer_[nsemipulse_] = time_out;
    level_ [nsemipulse_] = IR_receiver::is_one();
    nsemipulse_ = nsemipulse_ + 1;


}

// Generalicemos:
//	Inicialmente la señal puede estar en 0 ó en 1. En el caso del IR
//	estará en 1. A este valor lo llamo como en SPI la polarity.
uint16_t receive_train_of_pulses(Pulse* pulse, uint16_t N, uint8_t& polarity)
{
    polarity = IR_receiver::is_one();

    receive_semipulses();

    if (!check(polarity, level_, nsemipulse_))
	return 0;

    return copy(polarity, buffer_, nsemipulse_, pulse, N);
}


// (RRR) ¿por qué level_[nsemipulse_]?
//	 Al principio no llevaba control de si es HIGH o no, dando por
//	 supuesto que siempre empezaba en LOW. Pero al probarlo resultó no ser
//	 así: a veces empieza en HIGH, otras en LOW, no funcionando bien.
//	 Intenté garantizar que el pin del sensor fuera HIGH, pero no me
//	 funcionó (¿cometí algún error? probable). Así que al final anoto el
//	 tipo de pulso que es.
//
//	 Si IR_receiver::is_one() quiere decir que el pin estaba en zero, luego NO
//	 era HIGH ==> level_ = false.
ISR_RECEIVER_PIN {
    if (nsemipulse_ >= 0){
	buffer_[nsemipulse_] = Timer::unsafe_value();
	level_ [nsemipulse_] = !IR_receiver::is_one();
    }

    Timer::unsafe_reset();
    nsemipulse_ = nsemipulse_ + 1;
}





