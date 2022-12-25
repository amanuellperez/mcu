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
 *    A.Manuel L.Perez
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

namespace priv_{

// TODO: meterlo en PROGMEM?
//	 Si se mete al tocar una canción cargarlo en RAM (???)
static constexpr uint8_t note2freq_size = 5 * 12;

// Frecuencias en Hz de cada una de las notas de la escala musical
static constexpr uint16_t note2freq[note2freq_size] = {
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

using Note_type = uint8_t;
enum class Note : Note_type{
    Do = 0, DoS,
    Re, ReS,
    Mi,
    Fa, FaS,
    Sol, SolS,
    La, LaS,
    Si
};

// Aunque el orden de las octavas es:
//	great, small, one line, two line and three line
// empiezo a contar one_line = 1, para que coincidan 1, 2 y 3 con one, two and
// three. Eso hace que great = 4 y small = 5.
using Octave_type = uint8_t;
enum class Octave: Octave_type{
    one_line = 1, two_line, three_line,
    great, small
};


// Mantener oculta la forma de guardar note2freq para poder elegir en el
// futuro si meterla en PROGMEM o no.
inline uint32_t note_to_frequency(uint16_t n)
{ return priv_::note2freq[n];}


// Esta función es para facilitar la vida del programandor, aunque es posible
// que no sea necesaria (puede que en el organ_toy). Con ella podemos
// escribir:  
//
//	    note_to_frequency(Octave::one_line, Note::Mi) 
//
// para obtener la frecuencia de la nota Mi en la clave de Sol.
inline uint32_t note_to_frequency(Octave octave, Note note)
{ return note_to_frequency(static_cast<uint8_t>(octave)*12u + static_cast<uint8_t>(note));}


// Para depurar
inline std::ostream& operator<<(std::ostream& out, Note note)
{
    out << "note(" << static_cast<uint16_t>(note) << "):";
    switch(note){
	break; case Note::Do: out << "Do";
	break; case Note::DoS: out << "DoS";
	break; case Note::Re: out << "Re";
	break; case Note::ReS: out << "ReS";
	break; case Note::Mi: out << "Mi";
	break; case Note::Fa: out << "Fa";
	break; case Note::FaS: out << "FaS";
	break; case Note::Sol: out << "Sol";
	break; case Note::SolS: out << "SolS";
	break; case Note::La: out << "La";
	break; case Note::LaS: out << "LaS";
	break; case Note::Si: out << "Si";
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


