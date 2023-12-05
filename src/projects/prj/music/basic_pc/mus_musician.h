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

#ifndef __MUS_MUSICIAN_H__
#define __MUS_MUSICIAN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Clases responsables de tocar un instrumento
 *
 * HISTORIA
 *    Manuel Perez
 *    18/12/2022 Escrito
 *
 ****************************************************************************/
#include "mus_scale.h"
#include <array>


namespace music{


struct Note 
{
    uint8_t midi_number; // codifica el pitch = {octave, step}
    uint8_t ticks;  // 1, 2, 4, 8, ..., 64 ticks. Dependiendo del reloj
		    // que usemos la duración de la nota será mayor o menor

    constexpr Note(uint8_t midi_number0, uint8_t ticks0)
	    : midi_number{ midi_number0}, ticks{ticks0} { }

    constexpr Note(Octave octave, Step step, uint8_t ticks0)
	    : midi_number{octave_step2midi_number(octave, step)}
	    , ticks{ticks0} { }
};


Note progmem_read(const Note& x);

struct ROM_read_Note{
    Note operator()(const Note& x) { return progmem_read(x);}
};


// PARTITURAS (notación en español)
//
//	Una partitura es la forma de escribir una canción. Cada símbolo de 
//  la partitura indica la nota a tocar (octava y nota) y la duración. La
//  duración viene codificada en el dibujo:
//		nombre		= duración relativa
//		redonda		= 1
//		blanca		= 1/2	= 4/8
//		negra con punto	= 3/8
//		negra		= 1/4	= 2/8
//		corchea		= 1/8
//		semicorchea	= 1/16
//		fusa		= 1/32
//		semifusa	= 1/64
//
//  Creo (???) que la semifusa es la nota más corta pudiéndola tomar como base
//  para nuestro reloj. Midiendo el tiempo en semifusas en lugar de en blancas
//  obtenemos:
//
//		nombre		= duración relativa
//		redonda		= 64
//		blanca		= 32
//		negra con punto	= 24
//		negra		= 16
//		corchea		= 8
//		semicorchea	= 4
//		fusa		= 2
//		semifusa	= 1
//
// Song_notes = array (o mejor view) de las notas que forman una canción.
// La canción puede estar en RAM, en PROGMEM, en disco duro, ... No suponer
// cómo se almacena.
//
// (RRR) ¿Por qué llamarla Mononote_song?
//	 En una partitura normal se encuentra con que el pianista tiene que
//	 tocar varias notas a la vez. De momento empiezo con la restricción de
//	 que solo se puede tocar una nota a la vez. De esta forma si en el
//	 futuro esto evoluciona la clase `Song` representará una canción real,
//	 no como Mononote_song.
//
//	 Otra forma de concebirlo es imaginando a un pianista con un solo
//	 dedo. Las canciones que puede tocar son las Mononote_song.
template <typename Song_notes>
class Mononote_song{
public:
    Mononote_song(uint16_t period_clock_in_ms0, const Song_notes& song0)
	: period_clock_in_ms_{period_clock_in_ms0}
	, note_{song0}
    { }
	

    // DUDA: devolver por valor o por referencia?
    // Si la memoria está en PROGMEM seguramente sea haya que devolver por
    // valor.
    Note step(size_t i) const {return note_[i];}

    size_t size() const {return note_.size(); }

    uint16_t duration_of(const Note& note) const
    { return period_clock_in_ms_ * note.ticks; }
	

private:
    uint16_t period_clock_in_ms_;    // = tiempo de duración de la semifusa
    const Song_notes& note_;	// Notas a tocar
};


// Un músico es el responsable de tocar un instrumento musical
// El instrumento musical realmente será un SWG (square wave generator) o algo
// parecido (sinus wave generator???)
//
// DUDA: para reutilizar la clase voy a dejar publica la función play_freq, de
// esta forma el músico podrá tocar CUALQUIER frecuencia. Observar que esto es
// una generalización de un músico. Un músico está limitado a las frecuencias
// musicales (teclas del piano).
template <typename Instrument0>
class Musician{
public:
    using Instrument = Instrument0;

    // static interface
    Musician() = delete;

    /// configuramos el musician
    static void cfg();

    /// Toca la nota indicada. Devuelve el control inmediatamente, sin
    /// bloquear. Para dejar de tocar esa nota, tocar otra nota o llamar a
    /// `silence`
    static void silence();
    static void play_freq(uint16_t freq);
    static void play(Octave, Step);

    /// Toca la nota indicada por la duración indicada.
    /// Bloquea: no devuelve control hasta que no pasan duration_in_ms
    // La versión Octave:step, es más útil para programar un órgano: el
    // usuario selecciona la octava y el step.
    static void silence(uint16_t duration_in_ms);
    static void play(Octave, Step, uint16_t duration_in_ms);

    // Observar que al no usar Midi_number and Frequency tengo que
    // 'sobrecargar' las funciones con el nombre.
    static void play_midi(uint8_t midi_number, uint16_t duration_in_ms);
    static void play_freq(uint16_t freq, uint16_t duration_in_ms);

    /// Toquemos una partitura
    template <typename A>
    static void play(const Mononote_song<A>&);

    // TODO: la función play bloquea, lo cual no es buena idea. Se necesita
    // usar un flag que marque el cliente para en caso de que se quiera
    // abortar devolviendo el control.
//    template <typename A>
//    static void play(const Mononote_song<A>&, volatile& user_abort);
private:
    
};

template <typename I>
inline void Musician<I>::play(Octave octave, Step step, uint16_t duration_in_ms)
{ 
    play_freq(step_to_frequency(octave, step), duration_in_ms); 
}

template <typename I>
inline void Musician<I>::play_midi(uint8_t midi_number, uint16_t duration_in_ms)
{ 
    play_freq(step_to_frequency(midi_number), duration_in_ms); 
}

template <typename I>
inline void Musician<I>::play(Octave octave, Step step)
{ 
    play_freq(step_to_frequency(octave, step)); 
}


template <typename I>
inline void Musician<I>::play_freq(uint16_t freq, uint16_t duration_in_ms)
{
    Instrument::generate(freq, duration_in_ms);
}

template <typename I>
inline void Musician<I>::play_freq(uint16_t freq)
{
    Instrument::generate(freq);
}

template <typename I>
inline void Musician<I>::silence()
{
    Instrument::stop();
}

template <typename I>
inline void Musician<I>::silence(uint16_t duration_in_ms)
{
    silence();
    Instrument::wait(duration_in_ms);
}


template <typename I>
inline void Musician<I>::cfg()
{ 
    Instrument::cfg();
}


template <typename I>
template <typename AV>
void Musician<I>::play(const Mononote_song<AV>& song)
{
    for (size_t i = 0; i < song.size(); ++i){
	auto note = song.step(i); 
	play_midi(note.midi_number, song.duration_of(note));
    }
}

} // namespace music
#endif
