// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __DEV_TMP36_H__
#define __DEV_TMP36_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para el termómetro TMP36.
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           Manuel Perez- 05/08/2019 v1.0.
 *
 ****************************************************************************/

#include <avr_ADC.h>

namespace dev{

/*!
 *  \brief  Interfaz para el termómetro TMP36.
 *
 *  Conectar el ADC en single_mode.
 *
 */
class Termometro_TMP36{
public:
    Termometro_TMP36() = delete;

    // Mirando la datasheet del TMP36 la relación entre T y V es:
    //	V_out = 750 + 10(T - 25) = 500 + 10T
    //  donde V_out está expresado en mV y T en ºC.
    //
    //  Para poder trabajar con una cifra decimal, calculo 10T en lugar de
    //  T.
    //  Uso la versión calibrada del ADC ya que a poco error que se cometa al
    //  medir el potencial, el error en la temperatura es muy grande.
    //  El ADC tiene que estar conectado a 1.1V para poder tener una
    //  resolución de +-1mV (que da lugar a un error de +-0.1ºC en la medida,
    //  más el error de 1ºC del termómetro cometemos un error de 1.1ºC en
    //  total).
    template<uint8_t num_pin, uint32_t factor, uint32_t correction>
    static uint16_t temperatura_en_decigrados()
    {
	ADC::select_pin<num_pin>();
	return ADC::read_in_mV<factor, correction>() - 500u;
    }
};

}// namespace


#endif


