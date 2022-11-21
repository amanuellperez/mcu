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

#ifndef __PULSE_H__
#define __PULSE_H__

#include <atd_array.h>

//  NOTACIÓN
//  --------
//
//  * cycle:   _____
//	       |
//	    ___|
//
//  * pulse:        ___                  __________  ____________
//                  | |                            | |
//          ________| |___________  or             |_|
//
//  * burst of 38kHz: resultado de un pulso AND 38kHz
//  * Tren de pulsos: es una señal PWM. La diferencia con PWM es que en PWM
//    tenemos definido un duty cycle constante, mientras que en el tren de
//    pulsos no:
//           ___    _______         _____
//           | |    |     |         |   |
//	_____| |____|     |_________|   |______
//
//    Un tren de pulsos lo podemos describir como una sucesión de cycles. 
//   
//   * Tren de burst de 38kHz: resultado de tren de pulsos AND 38kHz.
//
/***************************************************************************
 *				    CYCLE
 ***************************************************************************/
struct Cycle{
    uint16_t time_low;
    uint16_t time_high;

    uint16_t period() const {return time_low + time_high;}
};


/***************************************************************************
 *			    TRAIN OF PULSES
 ***************************************************************************/
// Problema: El Train_of_pulses como su nombre indica es un tren de pulsos, no
// es un Train_of_pulses_view: contiene los pulsos. Pero contenerolos supone
// que necesito conocer su tamaño N. Por ello lo tengo que definir como
// template (y definirlo aquí). 
// Sin embargo la recepción la hago al estilo de C. Por ello necesito esta
// función receive_train_of_pulses que es la que implementa la recepción.
// DUDA: ¿cómo mejorar esto? ¿Merece la pena generalizarlo?
uint16_t receive_train_of_pulses(Cycle* pulse, uint16_t N, uint8_t& polarity);

namespace dev{

template <size_t N>
class Train_of_pulses{
public:
    Train_of_pulses();
//    Train_of_pulses(Cycle* cycle_container, size_t max_sz);

// pulses
    Cycle& operator[](size_t i) { return cycle_[i];} 
    const Cycle& operator[](size_t i) const { return cycle_[i];} 

    size_t size() const  { return cycle_.size(); }
    size_t capacity() const { return cycle_.capacity(); }
    
    bool empty() const {return cycle_.empty(); }
    bool full() const { return cycle_.full(); }

    Cycle* begin() { return cycle_.begin(); }
    Cycle* end() { return cycle_.end(); }

    const Cycle* begin() const { return cycle_.begin(); }
    const Cycle* end() const { return cycle_.end(); }

// polarity
    uint8_t polarity() const { return polarity_; }

// operations
    void receive();

private:
// Data
    uint8_t polarity_; // 0 or 1
    atd::CArray<Cycle, N> cycle_;
};


//inline Train_of_pulses::Train_of_pulses(Cycle* cycle_container, size_t max_sz)
template <size_t N>
inline Train_of_pulses<N>::Train_of_pulses()
    : polarity_{0xFF}
{}


// Inicialmente la señal puede estar en 0 ó en 1. En el caso del IR
// estará en 1. A este valor lo llamo como en SPI la polarity.
template <size_t N>
void Train_of_pulses<N>::receive()
{
    uint8_t n = receive_train_of_pulses(cycle_.data(), cycle_.capacity()
	    , polarity_);

    cycle_.size(n);

}


}// namespace
 
#endif
