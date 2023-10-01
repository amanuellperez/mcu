// Copyright (C) 2022-2023 Manuel Perez 
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

#ifndef __DEV_SQUARE_WAVE_H__
#define __DEV_SQUARE_WAVE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Generadores de ondas cuadradas.
 *
 * HISTORIA
 *    Manuel Perez
 *    13/12/2022 Square_wave_generator
 *		 PRO : Fácil de generar una señal de una frecuencia cualquiera.
 *		 CONS: Introduce un delay por culpa de tener que configurar el
 *		       Timer para generar esa frecuencia.
 *
 *		 Uso: generar música.
 *
 *    02/01/2023 Square_wave_burst_generator
 *		 PRO : No introduce delays ya que se configura el Timer en el
 *		       constructor de la clase.
 *		 CONS: No se puede cambiar la frecuencia fácilmente. Hay que 
 *		       construir un nuevo objeto.
 *
 *		 Uso: generar tren de pulsos de IR remote control.
 *
 ****************************************************************************/
#include <type_traits>
#include <cstdint>

namespace dev{

// Generamos ondas cuadradas en el pin npin del timer Timer_t.
// Controlamos la duración de los generates usando Miniclock_t.
// Basamos el Square_wave_generator en dos dispositivos genéricos:
//	(1) Un Square_wave_generator_g: este suministra la función
//		    generate(frequency)
//	    sin controlar la duración. Solo se puede generar o no generar la
//	    señal, sin controlar el tiempo.
//
//	(2) Un Miniclock: que nos permite controlar el tiempo que dura el
//	    generate
//
//  Básicamente lo que hace es ampliar la capacidad del SWG0 de 
//	generate(frequency) a generate(frequency, time)
//
//  DUDA: nombres? Estoy duplicando el nombre de Square_wave_generator aquí y
//  el que suministra el atmega. ¿Llamar a este Square_generate_generator?
//  ¿o al del avr Basic_square_wave_generator? @_@
template < typename SWG0	// Square_wave_generator_g
	 , typename Output_pin0	// pin donde generar la señal
	 , typename Miniclock0	
	 , bool call_cfg0 = true> // ver comentarios en generate()
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

// Interface: generate/stop
    /// Genera una onda de la frecuencia indicada. 
    /// No bloquea, devolviendo el control inmediatamente.
    static void generate(uint32_t freq); 

    /// Para el generador. Es la opuesta a `generate`.
    static void stop();

    /// Espera, bloqueando, a que pasen los ticks indicados.
    static void wait(const counter_type& ticks);
    
// Interface: generate
    /// Genera un generate de una onda cuadrada de frecuencia `freq` que dura
    /// `ticks`. Si es Miniclock_us, ticks serán microsegundos.
    /// Si es Miniclock_ms, ticks serán milisegundos.
    /// Bloquea: no devuelve el control hasta después de que pasen los ticks.
    static void generate(uint32_t freq, const counter_type& ticks);

private:
// Cfg
    static constexpr bool call_cfg = call_cfg0;
};


template <typename S, typename OP, typename MC, bool ci>
void Square_wave_generator<S, OP, MC, ci>::cfg()
{
    Miniclock::cfg();
    Miniclock::start();	// Es necesario encender el reloj!!!
    // SWG no tiene cfg
}


// (RRR) ¿llamar o no llamar a `cfg()` aquí?
//       Pros: Muy sencillo de usar. Si se quiere generar un generate llamas a
//       esta función y todo funciona.
//
//       Cons: 
//	    (1) Es un pelín más ineficiente. Si quiero generar una onda IR
//	        para un mando de TV tengo que generar un montón de generate de 38
//	        kHz seguidos. Llamar a `cfg` ralentizará la generación del
//	        tren de pulsos pudiendo no funcionar.
//
//	    (2) Suministro 2 formas de manejar esta clase:
//		[2.1] Si solo quieres llamar ocasionalmente a esta función, 
//		      llamas a `generate`.
//
//		[2.2] Para generar un tren de pulsos haré:
//		      cfg();
//		      for (...)
//			generate(...)
//
//	Al redactar esto y observar que necesito los 2 interfaces optó por
//	parametrizarlo con `call_cfg` y así que el cliente elija qué interfaz
//	usar.
template <typename S, typename OP, typename MC, bool ci>
void Square_wave_generator<S, OP, MC, ci>::
    generate(uint32_t freq, const counter_type& ticks)
{
    if constexpr (call_cfg)
	cfg(); 
	    
//  Pin::write_zero(); <-- ¿necesario?
    generate(freq);
    //Miniclock::wait(ticks);
    wait(ticks);
    stop();
//  Pin::write_zero(); // lo dejamos en cero
}

template <typename S, typename OP, typename MC, bool ci>
inline void Square_wave_generator<S, OP, MC, ci>::generate(uint32_t freq)
{
    // Pin::write_zero(); <-- ¿necesario?
    SWG::generate(freq, Pin::number);
}

template <typename S, typename OP, typename MC, bool ci>
inline void Square_wave_generator<S, OP, MC, ci>::stop()
{
    SWG::stop();

    // Garantizamos que acabe en cero
    Pin::as_output();	// OJO: el pin está conectado al SWG no al GPIO
    Pin::write_zero(); 
}




template <typename S, typename OP, typename MC, bool ci>
inline void Square_wave_generator<S, OP, MC, ci>::wait(const counter_type& ticks)
{
    Miniclock::wait(ticks);
}


/***************************************************************************
 *			Square_wave_burst_generator
 *
 *  Clase pensada para generar burst de una misma frecuencia pero donde sea
 *  muy importante la eficiencia.
 *
 *  Ejemplo: generar el tren de pulsos de un IR remote control.
 *
 ***************************************************************************/
template < typename SWG0	// Square_wave_burst_generator_g
	 , typename Output_pin0	// pin donde generar la señal
	 , typename Miniclock0>
class Square_wave_burst_generator{
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

// Constructor
    // Configura y enciende el miniclock para poder usarlo.
    Square_wave_burst_generator(uint32_t freq_in_Hz);

// Interface: generate/stop
    /// Genera una onda de la frecuencia indicada. 
    /// No bloquea, devolviendo el control inmediatamente.
    void generate_burst();

    /// Para el generador. Es la opuesta a `generate`.
    void stop();

    /// Espera, bloqueando, a que pasen los ticks indicados.
    void wait(const counter_type& ticks);
    
// Interface: generate_burst
    /// Genera un generate de una onda cuadrada de frecuencia `freq` que dura
    /// `ticks`. Si es Miniclock_us, ticks serán microsegundos.
    /// Si es Miniclock_ms, ticks serán milisegundos.
    /// Bloquea: no devuelve el control hasta después de que pasen los ticks.
   void generate_burst(const counter_type& ticks);

private:
    SWG swg_;

};


template <typename S, typename OP, typename MC>
inline Square_wave_burst_generator<S, OP, MC>::
    Square_wave_burst_generator(uint32_t freq_in_Hz) 
	: swg_{freq_in_Hz} 
{ 
    Miniclock::cfg();
    Miniclock::start();
}


template <typename S, typename OP, typename MC>
inline 
void Square_wave_burst_generator<S, OP, MC>::
				    generate_burst(const counter_type& ticks) 
{ 
    generate_burst();
    wait(ticks);
    stop();
}



template <typename S, typename OP, typename MC>
inline 
void Square_wave_burst_generator<S, OP, MC>::generate_burst() 
{ 
    swg_.template generate_burst<Pin::number>(); 
}


template <typename S, typename OP, typename MC>
inline void Square_wave_burst_generator<S, OP, MC>::stop()
{
    SWG::stop();

    // Garantizamos que acabe en cero
    Pin::as_output();	// OJO: el pin está conectado al SWG no al GPIO
    Pin::write_zero(); 
}

template <typename S, typename OP, typename MC>
inline 
void Square_wave_burst_generator<S, OP, MC>::wait(const counter_type& ticks)
{
    Miniclock::wait(ticks);
}


}// namespace


#endif


