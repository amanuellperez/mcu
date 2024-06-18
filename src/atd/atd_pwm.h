// Copyright (C) 2024 Manuel Perez 
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

#ifndef __ATD_PWM_H__
#define __ATD_PWM_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones para manejar señales PWM
 *
 * HISTORIA
 *    Manuel Perez
 *    05/06/2024 Escrito: versión mínima
 *
 ****************************************************************************/
#include "atd_percentage.h"

namespace atd{

template <typename Frequency_t, typename Time_t>
class PWM_signal{
public:
// typs
    using Frequency = Frequency_t;
    using Time      = Time_t;

// constructors
    PWM_signal(const Frequency& f, const Percentage& duty_cycle)
	    : frequency_{f}, duty_cycle_{duty_cycle} { }

// caracteristicas
    Frequency frequency() const { return frequency_;}
    void frequency(const Frequency& f) { frequency_ = f;}

    Time period() const { return inverse(frequency_);}
    void period(const Time& T) { frequency_ = inverse(T);}

    void duty_cycle(const Percentage& d) {duty_cycle_ = d;}
    Percentage duty_cycle() const { return duty_cycle_;}


private:
    Frequency frequency_;
    Percentage duty_cycle_; 
};



}// namespace

#endif


