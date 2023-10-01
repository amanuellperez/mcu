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

#ifndef __MUS_SCALE_H__
#define __MUS_SCALE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Escalas músicales
 *
 * IMPORTANTE
 *	Aquí solo dejar las notas musicales, no poner nada de código.
 *	De tal forma que se pueda usar este archivo en un programa en el
 *	ordenador para generar automáticamente los ficheros con las canciones
 *	a partir de la partitura.
 *
 * HISTORIA
 *    Manuel Perez
 *    18/12/2022 Escrito
 *
 ****************************************************************************/
#include <cstdint>
 #include <ostream>

namespace music{

/***************************************************************************
 *			CHROMATIC FIXED-DO SYSTEM (= teclado de piano)
 ***************************************************************************/
// ADVERTENCIA
// 
//	Al no ser músico ni tocar ningún instrumento no tengo claro que lo que
//  aquí escribo sea cierto. Hay cosas de las que estoy seguro pero otras no
//  tanto. Por ello que nadie considere estos comentarios como un tutorial
//  serio. Puede que en el futuro modifique cosas.
//
//
// CÁLCULO DE LAS FRECUENCIAS
//
//	La escala usada en el piano es crómatica: cada octava consta de 12 notas.
// Una octava equivale a multiplicar por 2 la frecuencia (todas las notas Do
// forman una sucesión geométrica de razón 2). 
//
// Las notas en la escala músical forman una sucesión geométrica de razón 'r'.
// Para averiguar la razón usamos el hecho de que cada octava (12 notas)
// consiste en multiplicar por 2 la frecuencia. Llamando f[n] la frecuencia de
// la nota n tenemos que 
//
//		f[12] = 2*f[0]
//
// Por otra parte como es una sucesión geométrica f[12] = r^12 * f[0] de donde
// obtenemos que 
//	r^12 = 2 ==> r = sqrt[12](2) = 1.0594631
//
// Sabiendo que la nota La tiene una frecuencia de 440 Hz podemos calcular
// toda la escala musical.
//
//
// OCTAVAS EN UN PIANO
//  
//	Con un piano se pueden tocar 5 octavas, cuyos nombres en inglés son:
//	    Great octave: corresponde a la clave de Fa
//	    Small octave
//	    One line octave: corresponde a la clave de Sol
//	    Two line octave
//	    Three line octave
//
//	Voy a usar la notación usada en musicxml: una nota está formada por un
//	pitch (la frecuencia) y una duración. El pitch los músicos lo
//	codifican en la partitura como (Octava, step), donde el step es lo que
//	habitualmente los que no sabemos música llamamos nota. Tiene sentido
//	llamarlo step: imaginar 5 pentagramas, uno encima de otro. Cada
//	línea/hueco representa una frecuencia. Tu vas subiendo por cada línea
//	a pasitos (a stepitos? @_@) de ahi, supongo, el nombre.

namespace priv_{

// TODO: meterlo en PROGMEM?
//	 Si se mete al tocar una canción cargarlo en RAM (???)
static constexpr uint8_t step2freq_size = 5 * 12;

// Observar que los números midi que estoy implementando van del 36 al 96
// ambos inclusive.
static constexpr uint8_t step2freq_index_min = 36;
static constexpr uint8_t step2freq_index_max = 96;

// Frecuencias en Hz de cada una de las notas de la escala musical
static constexpr uint16_t step2freq[step2freq_size] = {
// Great Octave
// do, doS re reS mi fa faS sol solS la laS si
   65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123,

// Small Octave
// do, doS, re, reS, mi, fa, faS, sol, solS, la, laS, si
  131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,

// One line octave
// do, doS, re, reS, mi, fa, faS, sol, solS, la, laS, si
  262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,

// Two line octave
// do, doS, re, reS, mi, fa, faS, sol, solS, la, laS, si
  523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,

// Three line octave
// do, doS, re, reS, mi, fa, faS, sol, solS, la, laS, si
 1046, 1109, 1175, 1244, 1318, 1397, 1480, 1568, 1661, 1760, 1865, 1976
};


}// namespace priv_


using Step_type = uint8_t;
enum class Step : Step_type{
    Do = 0, DoS,
    Re, ReS,
    Mi,
    Fa, FaS,
    Sol, SolS,
    La, LaS,
    Si,

    C = Do,
    Db = DoS,	// No sé cómo se escribe C# por eso uso Db == C#
    D = Re,
    Eb = ReS,
    E = Mi,
    F = Fa,
    Gb = FaS,
    G = Sol,
    Ab = SolS,
    A = La,
    Bb = LaS,
    B = Si

};


// Ordeno las octavas como en la Scientific pitch notation
using Octave_type = uint8_t;
enum class Octave: Octave_type{
    great = 2, small = 3, 
    one_line = 4, two_line = 5, three_line = 6
};


// Dos formas de indexar las frecuencias:
//  1. mediante el MIDI note number: [0, 127]
//  2. mediante el par (octave, step)
//     octave: [-1, 10]
//     step  : [ 0, 11]
//
// Las siguientes funciones nos permiten convertir de una representación a la
// otra
inline constexpr uint8_t octave_step2midi_number(uint8_t octave, uint8_t step)
{
    return (octave + 1) * 12 + step;
}


inline constexpr uint8_t octave_step2midi_number(Octave octave,  Step step)
{
    return octave_step2midi_number( static_cast<uint8_t>(octave)
				  , static_cast<uint8_t>(step));
}



// Mantener oculta la forma de guardar step2freq para poder elegir en el
// futuro si meterla en PROGMEM o no.
// n = MIDI note number
inline constexpr uint16_t step_to_frequency(uint8_t n)
{ return priv_::step2freq[n - priv_::step2freq_index_min];}



inline constexpr uint16_t step_to_frequency(uint8_t octave, uint8_t step)
{
    return step_to_frequency(octave_step2midi_number(octave, step));
}


// Esta función es para facilitar la vida del programandor, aunque es posible
// que no sea necesaria (puede que en el organ_toy). Con ella podemos
// escribir:  
//
//	    step_to_frequency(Octave::one_line, Step::Mi) 
//
// para obtener la frecuencia de la nota Mi en la clave de Sol.
inline constexpr uint16_t step_to_frequency(Octave octave, Step step)
{ 
    return step_to_frequency(
		    static_cast<uint8_t>(octave), static_cast<uint8_t>(step));
}


// Para depurar
inline std::ostream& operator<<(std::ostream& out, Step step)
{
    out << "step(" << static_cast<uint16_t>(step) << "):";
    switch(step){
	break; case Step::Do: out << "Do";
	break; case Step::DoS: out << "DoS";
	break; case Step::Re: out << "Re";
	break; case Step::ReS: out << "ReS";
	break; case Step::Mi: out << "Mi";
	break; case Step::Fa: out << "Fa";
	break; case Step::FaS: out << "FaS";
	break; case Step::Sol: out << "Sol";
	break; case Step::SolS: out << "SolS";
	break; case Step::La: out << "La";
	break; case Step::LaS: out << "LaS";
	break; case Step::Si: out << "Si";
    }

    return out;
}

inline std::ostream& operator<<(std::ostream& out, Octave octave)
{
    out << "octave(" << static_cast<uint16_t>(octave) << "):";
    switch(octave){
	break; case Octave::one_line: out << "one_line";
	break; case Octave::two_line: out << "two_line";
	break; case Octave::three_line: out << "three_line";
	break; case Octave::great: out << "great";
	break; case Octave::small: out << "small";
    }

    return out;
}


}// namespace music
 
#endif


