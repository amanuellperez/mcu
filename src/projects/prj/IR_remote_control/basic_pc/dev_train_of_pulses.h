// TODO: esto YA está migrado a `dev`. Usar el genérico. Hay que retocar
// alguna cosa para integrarlo con el genérico.
//
//
// Copyright (C) 2022 Manuel Perez 
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

#ifndef __DEV_TRAIN_OF_PULSES_H__
#define __DEV_TRAIN_OF_PULSES_H__

#include "dev_cycle.h"

#include <atd_array.h>
#include <avr_atmega.h>	    // TODO: esto depende del avr. Generic_pin???

// TODO: estoy duplicando la memoria. Al implementar Train_of_pulses_receiver
//       puedo usar la memoria alojada en él para Train_of_pulses.

/***************************************************************************
 *			    TRAIN OF PULSES
 ***************************************************************************/
namespace dev{

template <size_t N>
class Train_of_pulses{
public:
    Train_of_pulses();

// pulses
    Cycle& operator[](size_t i) { return cycle_[i];} 
    const Cycle& operator[](size_t i) const { return cycle_[i];} 

    size_t size() const  { return cycle_.size(); }
    size_t capacity() const { return cycle_.capacity(); }
    
    bool empty() const {return cycle_.empty(); }
    bool full() const { return cycle_.full(); }

    Cycle* begin() { return cycle_.begin(); }
    Cycle* end() { return cycle_.end(); }

    const Cycle* begin() const { return cycle_.begin(); }
    const Cycle* end() const { return cycle_.end(); }

// polarity
    uint8_t polarity() const { return polarity_; }

// operations
    // Devuelve el número de ciclos recibidos
    template <typename Cfg>
    uint8_t receive(volatile bool& abort);

private:
// Data
    uint8_t polarity_; // 0 or 1
    atd::Array<Cycle, N> cycle_;
};


template <size_t N>
inline Train_of_pulses<N>::Train_of_pulses()
    : polarity_{0xFF}
{}



/***************************************************************************
 *			    Train_of_pulses_receiver
 ***************************************************************************/
template <typename Miniclock_us0, uint8_t ir_receiver_pin0, int16_t num_max_pulses>
struct Train_of_pulses_receiver_cfg{
    static constexpr uint8_t ir_receiver_pin = ir_receiver_pin0;
    static constexpr int16_t buffer_size = 2 * num_max_pulses;
    using Miniclock_us = Miniclock_us0;

};

// Por culpa de la callback tengo que definirlo todo como static.
template <typename Cfg>
class Train_of_pulses_receiver{
public:
    Train_of_pulses_receiver() = delete;

    static uint16_t receive( Cycle* pulse, uint16_t N
			    , uint8_t& polarity
			    , volatile bool& abort); 
    
    static void interrupt_callback();

private:
// Data
    inline static constexpr int16_t buffer_size = Cfg::buffer_size;
    inline static volatile uint16_t buffer_[buffer_size];
    inline static volatile bool level_[buffer_size];
    inline static volatile int16_t nsemipulse_;

// Cfg
    static constexpr uint8_t ir_receiver_pin = Cfg::ir_receiver_pin;
    using Receiver_pin = atmega::Pin<ir_receiver_pin>;
    using Miniclock_us = Cfg::Miniclock_us;

// Functions
    static bool check(uint8_t polarity, volatile bool* level, int16_t n);

    static uint16_t copy_polarity1(volatile uint16_t* src, int16_t n
		, Cycle* dst, uint16_t N);

    static uint16_t copy_polarity0(volatile uint16_t* src, int16_t n
		    , Cycle* dst, uint16_t N);

    // Devuelve el número de ciclos copiados
    static uint16_t copy( uint8_t polarity
		, volatile uint16_t* src, int16_t n
		, Cycle* dst, uint16_t N);

    static void receive_semipulses(volatile bool& abort);
    
};


// Inicialmente la señal puede estar en 0 ó en 1. En el caso del IR
// estará en 1. A este valor lo llamo como en SPI la polarity.
template <size_t N>
template <typename Cfg>
uint8_t Train_of_pulses<N>::receive(volatile bool& abort)
{
    uint8_t n = Train_of_pulses_receiver<Cfg>::
		    receive(  cycle_.data(), cycle_.capacity()
			    , polarity_
			    , abort);

    cycle_.size(n);

    return n;

}


// Comprueba que la sucesión de niveles es alternada: 0,1,0,1,...
template <typename C>
bool Train_of_pulses_receiver<C>::check(uint8_t polarity, volatile bool* level, int16_t n)
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
template <typename C>
uint16_t Train_of_pulses_receiver<C>::
	copy_polarity1(volatile uint16_t* src, int16_t n
		, Cycle* dst, uint16_t N)
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
template <typename C>
uint16_t Train_of_pulses_receiver<C>::
	copy_polarity0(volatile uint16_t* src, int16_t n
		, Cycle* dst, uint16_t N)
{
    uint16_t size = std::min(static_cast<size_t>(n) / 2u, N);

    for (uint16_t i = 0; i < size; ++i){
	dst[i].time_high = src[2*i];
	dst[i].time_low  = src[2*i + 1];
    }

    return size;
}


template <typename C>
uint16_t Train_of_pulses_receiver<C>::copy( uint8_t polarity
		, volatile uint16_t* src, int16_t n
		, Cycle* dst, uint16_t N)
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
template <typename C>
void Train_of_pulses_receiver<C>::receive_semipulses(volatile bool& abort)
{
    using counter_type = typename Miniclock_us::counter_type;
    static constexpr counter_type time_overflow{60000};

    nsemipulse_ = -1;
    Receiver_pin::as_input_without_pullup();

    // atmega::Interrupt::enable_pin<ir_receiver_pin>();
    atmega::Pin<ir_receiver_pin>::enable_change_level_interrupt();

    Miniclock_us::start();
    
    {// TODO: meter esto en una función. nombre?
	atmega::Micro::Enable_interrupts lock; // NO GENERIC!!

	while (nsemipulse_ < 0 and !abort) { ; }	// esperamos a recibir algo

	if (!abort){
	    // while (Timer::safe_value() < Timer::max_top() <-- esto lo ralentiza (???)
	    while (Miniclock_us::time() < time_overflow
			and nsemipulse_ < buffer_size)
	    { ; }
	}

    }// ~Enable_interrupt

    Miniclock_us::stop(); 

    //atmega::Interrupt::disable_pin<ir_receiver_pin>();
    atmega::Pin<ir_receiver_pin>::disable_change_level_interrupt();

// anotamos el último semiperiodo
    buffer_[nsemipulse_] = time_overflow;
    level_ [nsemipulse_] = Receiver_pin::is_one();
    nsemipulse_ = nsemipulse_ + 1;


}

// Generalicemos:
//	Inicialmente la señal puede estar en 0 ó en 1. En el caso del IR
//	estará en 1. A este valor lo llamo como en SPI la polarity.
template <typename C>
uint16_t Train_of_pulses_receiver<C>::
	    receive( Cycle* pulse, uint16_t N
		    , uint8_t& polarity
		    , volatile bool& abort)
{
    polarity = Receiver_pin::is_one();

    receive_semipulses(abort);

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
template <typename C>
inline  // inline ya que quiero que sea lo más eficiente posible la ISR.
void Train_of_pulses_receiver<C>::interrupt_callback()
{
    if (nsemipulse_ >= 0){
	buffer_[nsemipulse_] = Miniclock_us::unsafe_time();
	level_ [nsemipulse_] = !Receiver_pin::is_one();
    }

    Miniclock_us::unsafe_reset();
    nsemipulse_ = nsemipulse_ + 1;
}


/***************************************************************************
 *			Transmitimos un tren de pulsos
 ***************************************************************************/
template <typename SWG_us, typename Miniclock_us, size_t N>
bool transmit(const Train_of_pulses<N>& pulse)
{
    constexpr uint16_t frequency = 38000; // 38 kHz
					  
    if(!pulse.polarity())
	return false;

    Miniclock_us::start();
    
    SWG_us swg{frequency};

    for (auto p = pulse.begin(); p != pulse.end(); ++p){
	swg.generate_burst(p->time_low);	
	Miniclock_us::wait(p->time_high);
    }

    Miniclock_us::stop();

    return true;
}



}// namespace
 
#endif
