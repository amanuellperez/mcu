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

#ifndef __DEV_CYCLE_H__
#define __DEV_CYCLE_H__
#include <stdint.h>

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
namespace dev{

struct Cycle{
    uint16_t time_low;
    uint16_t time_high;

    uint16_t period() const {return time_low + time_high;}
};

}// namespace
 
 
#endif
