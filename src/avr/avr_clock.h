// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __AVR_CLOCK_H__
#define __AVR_CLOCK_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para acceder al reloj (cristal) 
 *	    del microcontrolador.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 01/08/2017 Escrito
 *
 ****************************************************************************/
#include "avr_cfg.h"
#include <alp_avr.h>	// Integral_type


namespace avr{


/***************************************************************************
 *			Algunos tipos necesarios
 *
 *  TODO: mover esto. Esto lo necesito en la función square_wave pero
 *  realmente esta función no se debe de usar, ya que para eso tenemos el
 *  timer. No borrarlo ya que es un ejemplo de .asm pero no usarlo.
 ***************************************************************************/
struct Num_ciclos8_n{};
using Num_ciclos8 = alp::Integral_type<Num_ciclos8_n, uint8_t>;

struct Num_ciclos16_n{};
using Num_ciclos16 = alp::Integral_type<Num_ciclos16_n, uint16_t>;

struct Num_ciclos32_n{};
using Num_ciclos32 = alp::Integral_type<Num_ciclos32_n, uint32_t>;

struct Num_pulsos_n{};
using Num_pulsos = alp::Integral_type<Num_pulsos_n, uint8_t>;


/// Tipos de edges que puede tener una transición
enum class Edge {low2high, high2low};

/*!
 *  \brief  Reloj del microcontrolador que estamos usando
 *
 *  TODO: Observar que todo quedaría más claro y menos error-prone
 *	  si definiera: Frecuencia, Num_ciclos y Tiempo.
 *
 *  CUIDADO: AVR no funciona bien con doubles, y los tiempos habría
 *  que devolverlos como doubles en lugar de long. Esto puede generar
 *  confusión. Si es así, eliminar este interfaz!!!
 */
class Clock{
public:
    /// Divisores entre los que podemos preescalar el reloj
    enum class Divisor
    {no_prescaling, entre_8, entre_64, entre_256, entre_1024};

    /// Convertimos el divisor a número
    static uint16_t to_int(Divisor d);

    // ---------------------------------------------------
    // Frecuencia de reloj expresada en distintas unidades
    // ---------------------------------------------------
    /// Frecuencia a la que opera el microcontrolador.
    /// Esta frecuencia será la usada para calcular el resto de relojes.
    /// Observar que es la frecuencia en Hz, no en MHz ni kHz.
    /// Otro nombre podría ser: ciclos_por_segundo().
    static constexpr uint32_t frecuencia_en_Hz() 
    {return AVR_CLOCK_FREQUENCY_IN_HZ;}
    // {return F_CPU;}

    /// Número de ciclos de reloj que se dan por segundo.
    /// Sinónimo: frecuencia_en_Hz()
    static constexpr uint32_t ciclos_por_segundo() 
    {return frecuencia_en_Hz();}

    /// Número de ciclos de reloj que se dan por milisegundo
    static constexpr uint32_t ciclos_por_milisegundo()
    {return ciclos_por_segundo()/1000UL;}

    /// Número de ciclos de reloj que se dan por microsegundo
    static constexpr uint32_t ciclos_por_microsegundo()
    {return ciclos_por_segundo()/1000000UL;}


    // ----------------------------------------------------
    // Funciones de conversión de número de ciclos a tiempo
    // ----------------------------------------------------
    /// Devuelve el tiempo, en microsegundos, que tarda en ejecutarse
    /// 'num_ciclos'.
    static constexpr 
    uint32_t num_ciclos_to_microsegundos(uint32_t num_ciclos) 
    {return num_ciclos/ciclos_por_microsegundo();}

    /// Devuelve el número de ciclos que se ejecutan en 'us' microsegundos.
    static constexpr 
    uint32_t microsegundos_to_num_ciclos(uint32_t us) 
    {return us*ciclos_por_microsegundo();}

//    /// Devuelve el tiempo, en milisegundos, que tarda en ejecutarse
//    /// 'num_ciclos'.
//    /// CUIDADO: Como devuelve un long, no devuelve  la parte decimal 
//    /// de los milisegundos, luego si num_ciclos no llega a 1 ms 
//    /// devuelve 0. <--- como esto puede ser confuso, elimino esta función.
//    static constexpr 
//    uint32_t num_ciclos_to_milisegundos(uint32_t num_ciclos) 
//    {return num_ciclos/ciclos_por_milisegundo();}

    /// Devuelve el número de ciclos que se ejecutan en 'ms' milisegundos.
    static constexpr uint32_t milisegundos_to_num_ciclos(uint32_t ms) 
    {return ms*ciclos_por_milisegundo();}

    /// Sinónimo de milisegundos_to_num_ciclos
    static constexpr uint32_t ms2ciclos(uint32_t ms) 
    {return milisegundos_to_num_ciclos(ms);}

};

/// Convertimos el divisor a número
inline uint16_t Clock::to_int(Divisor d)
{
    switch(d){
	case Divisor::no_prescaling : return 1;
	case Divisor::entre_8	    : return 8;
	case Divisor::entre_64	    : return 64;
	case Divisor::entre_256	    : return 256;
	case Divisor::entre_1024    : return 1024;
    }

    // Aqui nunca llega. El return es para evitar el 
    // warning del compilador.
    return 1;
}



}// namespace

#endif

