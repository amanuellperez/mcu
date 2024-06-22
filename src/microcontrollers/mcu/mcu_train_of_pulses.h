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

#ifndef __MCU_TRAIN_OF_PULSES_H__
#define __MCU_TRAIN_OF_PULSES_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Manejo de trenes de pulsos
 *
 * HISTORIA
 *    Manuel Perez
 *    10/10/2023 Traído de IR remote control y generalizado.
 *
 ****************************************************************************/
#include "mcu_cycle.h"
#include <atd_array.h>

namespace mcu{

/***************************************************************************
 *			    TRAIN OF PULSES
 ***************************************************************************/
// Train_of_pulses es un vulgar array de ciclos + polaridad.
// Uso el término polarity con el mismo significado que se da en SPI.
template <size_t N>
class Train_of_pulses{
public:
    Train_of_pulses();

// pulses
    Cycle& operator[](size_t i) { return cycle_[i];} 
    const Cycle& operator[](size_t i) const { return cycle_[i];} 

    void size(size_t n)  {cycle_.size(n); }
    size_t size() const  { return cycle_.size(); }

    size_t capacity() const { return cycle_.capacity(); }
    
    bool empty() const {return cycle_.empty(); }
    bool full() const { return cycle_.full(); }

    Cycle* begin() { return cycle_.begin(); }
    Cycle* end() { return cycle_.end(); }

    const Cycle* begin() const { return cycle_.begin(); }
    const Cycle* end() const { return cycle_.end(); }

    Cycle* data()		{return cycle_.data();}
    const Cycle* data() const   {return cycle_.data();}

// polarity
    uint8_t polarity() const { return polarity_; }
    void polarity(uint8_t pol) { polarity_ = pol; }

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
 *			Train_of_pulses_isr_receiver
 *
 *  Recibimos el tren de pulsos usando un miniclock_us y capturando la
 *  interrupción correspondiente.
 *
 *  Ventajas
 *	Al usar el miniclock y las interrupciones es de esperar que 
 *      mida bastante bien la duración de los pulsos.
 *
 *  Desventajas
 *	La interrupt_callback que ahora está implementada en ensamblador 
 *	ejecuta unas 50 instrucciones. Eso (creo) es el motivo por el que es
 *	incapaz de capturar los pulsos generados por el DHT11 ya que esté
 *	sensor genera semipulsos de 20 us, y muchos menores de 30 us. Las
 *	interrupciones no funcionan en este caso.
 ***************************************************************************/
template <typename Micro0,
	  typename Miniclock_us0, 
	  uint8_t receiver_pin0, 
	  int16_t num_max_pulses,
	  typename Miniclock_us0::counter_type time_overflow0 = 60000>
struct Train_of_pulses_isr_receiver_cfg{
    using Micro        = Micro0;
    using Miniclock_us = Miniclock_us0;
    using counter_type = typename Miniclock_us::counter_type;

    static constexpr uint8_t receiver_pin    = receiver_pin0;
    static constexpr int16_t nmax_semicycles = 2 * num_max_pulses;
    static constexpr counter_type time_overflow = time_overflow0;

};

// Por culpa de la callback tengo que definirlo todo como static.
template <typename Cfg>
class Train_of_pulses_isr_receiver{
public:
    Train_of_pulses_isr_receiver() = delete;

    // Devuelve el número de pulsos recibidos.
    // Precondicion: el cliente llama a Pin::as_input_without_pullup
    template <size_t N>
    static size_t receive(Train_of_pulses<N>& pulse, volatile bool& abort);

    // FUNDAMENTAL: recordar llamar a esta función en la ISR correspondiente
    // al pin donde se van a leer los pulsos.
    // Si se olvida capturar la interrupción correspondiente el micro parece
    // que reinicia.
    static void interrupt_callback();

private:
// Data
    inline static constexpr int16_t nmax_semicycles = Cfg::nmax_semicycles;
    inline static volatile Semicycle semicycle[nmax_semicycles];
    inline static volatile int16_t nsemipulse_;

// Devices
    using Micro        = Cfg::Micro;
    using Pin          = typename Micro::Pin<Cfg::receiver_pin>;
    using Miniclock_us = Cfg::Miniclock_us;
    using Enable_interrupts = typename Micro::Enable_interrupts;

// Cfg
    static constexpr auto time_overflow = Cfg::time_overflow;


// Functions
    template <size_t N>
    static uint16_t copy_polarity1( volatile Semicycle* semicycle
				  , int16_t n
				  , Train_of_pulses<N>& pulse);

    template <size_t N>
    static uint16_t copy_polarity0( volatile Semicycle* semicycle
				  , int16_t n
				  , Train_of_pulses<N>& pulse);

    // Devuelve el número de ciclos copiados
    template <size_t N>
    static uint16_t copy( uint8_t polarity
			, volatile Semicycle* semicycle
			, int16_t n
			, Train_of_pulses<N>& pulse);

    static void receive_semicycles(volatile bool& abort);
    
};


// Inicialmente la señal puede estar en 0 ó en 1. En el caso del IR
// estará en 1. A este valor lo llamo como en SPI la polarity.
template <typename Cfg>
template <size_t N>
size_t Train_of_pulses_isr_receiver<Cfg>::
    receive(Train_of_pulses<N>& pulse, volatile bool& abort)
{
    uint8_t polarity = Pin::is_one();

    receive_semicycles(abort);

    size_t n = copy(polarity, semicycle, nsemipulse_, pulse);

    pulse.size(n);
    pulse.polarity(polarity);

    return n;
}


// precondition: n > 0
// Observar que en caso de que se haya recibido mal el último pulso, no lo
// anotamos.
template <typename C>
template <size_t N>
uint16_t Train_of_pulses_isr_receiver<C>::
	copy_polarity1(
		volatile Semicycle* semicycle
		, int16_t n
		, Train_of_pulses<N>& pulse)
{
    uint16_t size = std::min(static_cast<size_t>(n) / 2u, N);

    for (uint16_t i = 0; i < size; ++i){
	if (semicycle[2*i].level == 0 and semicycle[2*i + 1].level == 1){
	    pulse[i].time_low  = semicycle[2*i].time;
	    pulse[i].time_high = semicycle[2*i + 1].time;
	}

	else
	    return i + 1;
    }

    return size;
}


// precondition: n > 0
// Observar que en caso de que se haya recibido mal el último pulso, no lo
// anotamos.
template <typename C>
template <size_t N>
uint16_t Train_of_pulses_isr_receiver<C>::
	copy_polarity0(
		volatile Semicycle* semicycle
		, int16_t n
		, Train_of_pulses<N>& pulse)
{
    uint16_t size = std::min(static_cast<size_t>(n) / 2u, N);

    for (uint16_t i = 0; i < size; ++i){
	if (semicycle[2*i].level == 1 and semicycle[2*i + 1].level == 0){
	    pulse[i].time_high = semicycle[2*i].time;
	    pulse[i].time_low  = semicycle[2*i + 1].time;
	}

	else
	    return i + 1;
    }

    return size;
}


template <typename C>
template <size_t N>
uint16_t Train_of_pulses_isr_receiver<C>::copy( uint8_t polarity
		, volatile Semicycle* semicycle
		, int16_t n
		, Train_of_pulses<N>& pulse)
{
    if (n <= 0)
	return 0;

    if (polarity)
	return copy_polarity1(semicycle, n, pulse);

    else
	return copy_polarity0(semicycle, n, pulse);
}


/***************************************************************************
 *			    IMPLEMENTACIÓN
 ***************************************************************************/
template <typename C>
void Train_of_pulses_isr_receiver<C>::receive_semicycles(volatile bool& abort)
{
    nsemipulse_ = -1;

    Pin::enable_change_level_interrupt();

    Miniclock_us::start();
    
    {// TODO: meter esto en una función? nombre?
	Enable_interrupts lock; 

	while (nsemipulse_ < 0 and !abort) { ; } // esperamos a recibir algo

	if (!abort){
	    while (Miniclock_us::time() < time_overflow
			and nsemipulse_ < nmax_semicycles)
	    { ; }
	}

    }// ~Enable_interrupt

    Miniclock_us::stop(); 

    Pin::disable_change_level_interrupt();

// anotamos el último semiperiodo
    semicycle[nsemipulse_].time = time_overflow;
    semicycle[nsemipulse_].level = Pin::is_one();

    nsemipulse_ = nsemipulse_ + 1;

}

// (RRR) ¿por qué guardar el level?
//       A fin de cuentas, si empiezas en 1, luego va 0, luego 1, 0, 1,...
//       Resulta que si se están enviando pulsos demasiado rápidos este método
//       no lo va a detectar. El level sirve para detectar si se han omitido
//       algunas interrupciones (por estar dentro de la ISR que llama 
//       a esta función interrupt_callback).
//
// Mirando el código .asm esta interrupción lleva unas 50 instrucciones.
// A 1MHz es posible que no pueda leer pulsos de menor longitud de 50 us.
// De hecho a 1 MHz no me está funcionando leer los pulsos emitidos por el
// DHT11 (que son algunos semipulsos de 20-30us). Sin embargo a 8MHz sí que es
// capaz de leerlos.
//
template <typename C>
inline  // inline ya que quiero que sea lo más eficiente posible la ISR.
void Train_of_pulses_isr_receiver<C>::interrupt_callback()
{
    if (nsemipulse_ >= 0){
	semicycle[nsemipulse_].time = Miniclock_us::unsafe_time();
	semicycle[nsemipulse_].level= !Pin::is_one();
    }

    Miniclock_us::unsafe_reset();
    nsemipulse_ = nsemipulse_ + 1;
}

/***************************************************************************
 *			Train_of_pulses_poll_receiver
 *
 *  En lugar de usar un miniclock y las interrupciones hacemos continuamente
 *  polling del pin para ver cuándo cambia.
 *
 ***************************************************************************/
template <typename Micro0,
	  uint8_t receiver_pin0, 
	  uint16_t num_max_pulses,
	  uint16_t time_overflow0 = 600>
struct Train_of_pulses_poll_receiver_cfg{

    using Micro        = Micro0;

    static constexpr uint8_t receiver_pin = receiver_pin0;
    static constexpr uint16_t nmax_semicycles = 2 * num_max_pulses;
    static constexpr uint16_t time_overflow = time_overflow0;

};

template <typename Cfg>
class Train_of_pulses_poll_receiver{
public:
    // Devuelve el número de pulsos recibidos.
    // (RRR) ¿por qué no pasarle un flag `abort` para cancelar la operación?
    //       Al hacer pruebas el estar pendiente de si abort == true ralentiza
    //       mucho el contar los ticks, no funcionando para medir pulsos
    //       rápidos (como los que envía el DHT11 de 20-30 us). Por eso opto
    //       por eliminar el abort. Se mantiene el time_out del overflow.
    // Precondicion: el cliente llama a Pin::as_input_without_pullup
    template <size_t N>
    static void receive(Train_of_pulses<N>& pulse);

private:
// Devices
    using Micro        = Cfg::Micro;
    using Pin          = typename Micro::Pin<Cfg::receiver_pin>;
    
// Cfg
    static constexpr uint8_t receiver_pin     = Cfg::receiver_pin;
    static constexpr uint16_t nmax_semicycles = Cfg::nmax_semicycles;
    static constexpr uint16_t time_overflow   = Cfg::time_overflow;
    
// Helpers
    static 
    std::pair<size_t, bool> receive(uint16_t* semicycle_ticks, size_t n);

    template <size_t N>
    static void copy(bool polarity, uint16_t* semicycle, size_t n
			, Train_of_pulses<N>& pulse);

    template <size_t N>
    static 
    void copy_polarity1( uint16_t* semicycle, size_t n
		       , Train_of_pulses<N>& pulse);

    template <size_t N>
    static 
    void copy_polarity0( uint16_t* semicycle, size_t n
		       , Train_of_pulses<N>& pulse);
};

template <typename C>
template <size_t N>
void Train_of_pulses_poll_receiver<C>::receive(Train_of_pulses<N>& pulse)
{
    uint16_t semicycle_ticks[nmax_semicycles];
    auto [nsemicycles, polarity] = receive(semicycle_ticks, nmax_semicycles);

    copy(polarity, semicycle_ticks, nsemicycles, pulse);

    pulse.polarity(polarity);
}

template <typename C>
template <size_t N>
void Train_of_pulses_poll_receiver<C>::copy(bool polarity
		, uint16_t* semicycle, size_t n
		, Train_of_pulses<N>& pulse)
{
    if (n == 0)
	return;

    if (polarity)
	return copy_polarity1(semicycle, n, pulse);

    else
	return copy_polarity0(semicycle, n, pulse);
}


template <typename C>
template <size_t N>
void Train_of_pulses_poll_receiver<C>::
	copy_polarity1( uint16_t* semicycle, size_t n
		      , Train_of_pulses<N>& pulse)
{
    size_t size = std::min(n / 2u, N);

    pulse.size(size);

    for (size_t i = 0; i < size; ++i){
	pulse[i].time_low  = semicycle[2*i];
	pulse[i].time_high = semicycle[2*i + 1];
    }
}


template <typename C>
template <size_t N>
void Train_of_pulses_poll_receiver<C>::
	copy_polarity0( uint16_t* semicycle, size_t n
		      , Train_of_pulses<N>& pulse)
{
    size_t size = std::min(n / 2u, N);

    pulse.size(size);

    for (size_t i = 0; i < size; ++i){
	pulse[i].time_high = semicycle[2*i];
	pulse[i].time_low  = semicycle[2*i + 1];
    }
}

// Va un poco lento. ¿Se puede optimizar algo?
// 1. Probé a cambiar size_t por uint8_t: no mejora.
// 2. Probé a eliminar counter y usar directamente semicycle_ticks[i]: va más
//    lento.
// 3. Usar un puntero p = semicycle_ticks, y acceder directamente via este
//    puntero: similar a 2.
//
// Por ello dejo la implementación inicial: es la más sencilla de entender y
// no parece que haya mejoras sencillas.
// A 8 MHz funciona bastante bien, a 1 MHz da la impresión de que también
// (aunque genera muchos ticks = 0, cuando son menores de 25us +-)
//
// He eliminado `abort` de esta función ya que la ralentizaba bastante.
template <typename C>
std::pair<size_t, bool> Train_of_pulses_poll_receiver<C>::
			    receive(uint16_t* semicycle_ticks, size_t n)
{
    bool polarity = Pin::is_one();
    bool last_bit = polarity; // mejor es 'bit_actual', pero eso en ingles,
				   // ¿cómo es? ¿actual_bit? Suena a otra
				   // cosa.

    // wait_for_first_bit
    size_t counter = 0;
    while ( Pin::is_one() == last_bit and 
	    counter < time_overflow){
	++counter;
    }
    last_bit = !last_bit;


    // read_all_bits
    size_t i = 0;
    for (; i < n; ++i){

	counter = 0;
	while (Pin::is_one() == last_bit){
	    ++counter;

	    if (counter == time_overflow){
		semicycle_ticks[i] = time_overflow;
		return {i + 1, polarity};
	    }
	}

	last_bit = !last_bit;
	semicycle_ticks[i] = counter;
    }


    return {i, polarity};
}


}// namespace
 
#endif
