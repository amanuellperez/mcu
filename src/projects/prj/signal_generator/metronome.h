// Copyright (C) 2021 Manuel Perez 
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

#ifndef __METRONOME_H__
#define __METRONOME_H__
/****************************************************************************
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    21/03/2021 v0.0
 *
 ****************************************************************************/

#include <array>
#include <atd_metronome.h>
#include <atd_types.h>

// TODO: revisar el Bounded!!! creo que tiene algun error. No funciona esta
// clase como debiera.
// Esta clase es completamente genérica. El problema es ¿cómo llamarla?
// Es un metronomo de incrementos (???) Metronome_incrs? 
template <typename Incr, typename D1, typename D0>
class Metronome : public atd::Metronome<D1, D0>{
public:
    Metronome() {reset();}

    void reset() 
    {
	atd::Metronome<D1,D0>::reset();
	i_  = 0;
	incr_ = incrs[i_];
    }

    void tick()
    {
	if (end() and ! i_.max()){
	    atd::Metronome<D1,D0>::reset();
	    ++i_;
	}
	else 
	    atd::Metronome<D1,D0>::tick();
    }


    Incr incr() const { return incrs[i_];}

private:
    Incr incr_ = incrs[0];
    static constexpr std::array<Incr, 3> incrs{Incr{1}, Incr{10}, Incr{100}};

    atd::Bounded<size_t, 0, incrs.size() - 1> i_;

// Helper
    bool end() const { return atd::Metronome<D1,D0>::end();}

};



#endif



