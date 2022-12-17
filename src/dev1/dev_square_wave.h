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
/****************************************************************************
 *
 * DESCRIPCION
 *	Generadores de ondas cuadradas.
 *
 * HISTORIA
 *    A.Manuel L.Perez
 *    13/12/2022 Square_wave_generator
 *
 ****************************************************************************/
#include <type_traits>
#include <cstdint>

namespace dev{

// Generamos ondas cuadradas en el pin npin del timer Timer_t.
// Controlamos la duración de los bursts usando Miniclock_t.
// Basamos el Square_wave_generator en dos dispositivos genéricos:
//	(1) Un Square_wave_generator_g: este suministra la función
//		    generate(frequency)
//	    sin controlar la duración. Solo se puede generar o no generar la
//	    señal, sin controlar el tiempo.
//
//	(2) Un Miniclock: que nos permite controlar el tiempo que dura el
//	    burst
//
//  Básicamente lo que hace es ampliar la capacidad del SWG0 de 
//	generate(frequency) a generate(frequency, time)
//
//  DUDA: nombres? Estoy duplicando el nombre de Square_wave_generator aquí y
//  el que suministra el atmega. ¿Llamar a este Square_burst_generator?
//  ¿o al del avr Basic_square_wave_generator? @_@
template < typename SWG0	// Square_wave_generator_g
	 , typename Output_pin0	// pin donde generar la señal
	 , typename Miniclock0	
	 , bool call_cfg0 = true> // ver comentarios en burst()
class Square_wave_generator{
public:
// Internal devices
    using SWG				= SWG0;
    using Pin				= Output_pin0;
    using Miniclock			= Miniclock0;
    using counter_type			= typename Miniclock::counter_type;

// Preconditions
    static_assert(SWG::is_pin(Pin::number), "Incorrect pin. "
				     "Doesn't belong to Square_wave_g");

    static_assert(!std::is_same_v<typename SWG0::Timer, typename Miniclock::Timer>,
		  "Timers for SWG and Miniclock have to be differents");

// Operations
    /// Configuramos el device para poder usarlo.
    static void cfg();

    // void generate(freq); <-- esta es fácil de añadir y tiene sentido
    // tenerla aquí. Añadirla si se necesita. (ampliaría SWG0)

    /// Genera un burst de una onda cuadrada de frecuencia `freq` que dura
    /// `ticks`. Si es Miniclock_us, ticks serán microsegundos.
    /// Si es Miniclock_ms, ticks serán milisegundos.
    static void burst(uint32_t freq, const counter_type& ticks);

private:
// Cfg
    static constexpr bool call_cfg = call_cfg0;
};


template <typename S, typename OP, typename MC, bool ci>
void Square_wave_generator<S, OP, MC, ci>::cfg()
{
    Miniclock::cfg();
    // SWG no tiene cfg
}


// (RRR) ¿llamar o no llamar a `cfg()` aquí?
//       Pros: Muy sencillo de usar. Si se quiere generar un burst llamas a
//       esta función y todo funciona.
//
//       Cons: 
//	    (1) Es un pelín más ineficiente. Si quiero generar una onda IR
//	        para un mando de TV tengo que generar un montón de burst de 38
//	        kHz seguidos. Llamar a `cfg` ralentizará la generación del
//	        tren de pulsos pudiendo no funcionar.
//
//	    (2) Suministro 2 formas de manejar esta clase:
//		[2.1] Si solo quieres llamar ocasionalmente a esta función, 
//		      llamas a `burst`.
//
//		[2.2] Para generar un tren de pulsos haré:
//		      cfg();
//		      for (...)
//			burst(...)
//
//	Al redactar esto y observar que necesito los 2 interfaces optó por
//	parametrizarlo con `call_cfg` y así que el cliente elija qué interfaz
//	usar.

template <typename S, typename OP, typename MC, bool ci>
void Square_wave_generator<S, OP, MC, ci>::
    burst(uint32_t freq, const counter_type& ticks)
{
    if constexpr (call_cfg)
	cfg(); 
	    
    // Pin::write_zero(); <-- ¿necesario?
    SWG::generate(freq, Pin::number);
    Miniclock::wait(ticks);
    SWG::stop();
    Pin::write_zero(); // lo dejamos en cero
}


}// namespace


#endif


