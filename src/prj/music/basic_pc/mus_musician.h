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

#ifndef __MUS_MUSICIAN_H__
#define __MUS_MUSICIAN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Clases responsables de tocar un instrumento
 *
 * HISTORIA
 *    A.Manuel L.Perez
 *    18/12/2022 Escrito
 *
 ****************************************************************************/
#include "mus_scale.h"
#include <array>

namespace music{


struct Note 
{
    // Pitch = {octave, step}
    Octave octave;
    Step step;
    uint8_t ticks;  // 1, 2, 4, 8, ..., 64 ticks. Dependiendo del reloj
		    // que usemos la duración de la nota será mayor o menor
};


Note progmem_read(const Note& x);


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
template <typename Song_notes>
class Song{
public:
    Song(uint16_t period_clock_in_ms0, const Song_notes& song0)
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
    static void play(Octave, Step);
    static void silence();

    /// Toca la nota indicada por la duración indicada.
    /// Bloquea: no devuelve control hasta que no pasan duration_in_ms
    static void play(Octave, Step, uint16_t duration_in_ms);

    /// Toquemos una partitura
    template <typename A>
    static void play(const Song<A>&);

private:
    static void play(uint32_t freq, uint16_t duration_in_ms);
    static void play(uint32_t freq);
    
};

template <typename I>
inline void Musician<I>::play(Octave octave, Step step, uint16_t duration_in_ms)
{ 
    play(step_to_frequency(octave, step), duration_in_ms); 
}

template <typename I>
inline void Musician<I>::play(Octave octave, Step step)
{ 
    play(step_to_frequency(octave, step)); 
}


template <typename I>
inline void Musician<I>::play(uint32_t freq, uint16_t duration_in_ms)
{
    Instrument::burst(freq, duration_in_ms);
}

template <typename I>
inline void Musician<I>::play(uint32_t freq)
{
    Instrument::generate(freq);
}

template <typename I>
inline void Musician<I>::silence()
{
    Instrument::stop();
}

template <typename I>
inline void Musician<I>::cfg()
{ 
    Instrument::cfg();
}


template <typename I>
template <typename AV>
void Musician<I>::play(const Song<AV>& song)
{
    for (size_t i = 0; i < song.size(); ++i){
	auto note = song.step(i); 
	play(note.octave, note.step, song.duration_of(note));
    }
}

} // namespace music
#endif
