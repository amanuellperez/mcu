// Copyright (C) 2021 A.Manuel L.Perez 
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

#ifndef __GENERIC_DEVICES_H__
#define __GENERIC_DEVICES_H__

// ¿En qué namespace meter a todo lo genérico?
//   1. Podrían meterse en 'mcu', de microcontrolador. Pero luego tengo el
//      Generic_LCD que no está dentro del microcontrolador.
//   2. Además, podría pensar que tengo un Timer independiente del micro, en
//      cuyo caso no tendría mucho sentido que estuviera dentro de mcu, sino
//      en dev. 
//
//   Probemos con 'dev': serán todos disposivitos genéricos (ya estén dentro
//   del microcontrolador o fuera).
namespace dev{

// TODO: eliminar Generic_timer a favor de 
//	    Generic_counter
//	    Generic_timer_counter
//	    Generic_square_wave_generator
//	    Generic_PWM_generator
template <typename Timer>
class Generic_timer;

template <typename Timer>
class Generic_timer_counter;

}// namespace


#endif


