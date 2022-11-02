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

#ifndef __MAIN_H__
#define __MAIN_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Programa para averiguar el protocolo usado por un mando.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    02/11/2022 v0.0
 *
 ****************************************************************************/
#include "dev.h"

class Main{
public:
    Main();
    void run();
    
    static constexpr int8_t max_num_data = 100;

private:
// Hardware: 
    Timer timer;
    Sensor sensor;


// Data
    // Última lectura realizada
    Timer::counter_type data[max_num_data];
    int8_t num_data = 0; // número de datos válidos en el array data
		

    void init_uart();

    void read_data();
    void print_data();
    void print_nec_protocol();
    void print_raw_data();

};

// Devuelve true si a = b +- 20%
// Esto es, si b - 20% <= a <= b + 20%
// (cojo el 20% ya que:
//	1. El clock del avr tiene un 10% de incertidumbre
//	2. Doy un 10% más de margen de error
// )
template <typename Int1, typename Int2>
inline bool is_equal(const Int1& a, const Int2& b)
{ return (b*0.8 <= a and a <= b*1.2); }




#endif


