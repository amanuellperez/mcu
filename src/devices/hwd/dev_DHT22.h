// Copyright (C) 2023 Manuel Perez 
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

#ifndef __DEV_DHT22_H__
#define __DEV_DHT22_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Driver del DHT22 (el traductor es DHT_protocol)
 *
 * HISTORIA
 *    Manuel Perez
 *    14/10/2023 Escrito
 *
 ****************************************************************************/
#include "dev_DHT11.h" // DHT_protocol
#include <atd_magnitude.h>
#include <utility>

namespace dev{

// Fundamental a tener en cuenta:
// + When power is supplied to sensor, don't send any instruction to the 
//   sensor within one second to pass unstable status
//
// + Collecting period should be : >2 second. (esperar 2 segundos entre
//   medidas).
// 
// + Todas las funciones son secuenciales, bloquean al micro hasta que 
//   acaban de ejecutarse.
//
template <typename Micro0, uint8_t dht_pin>
class DHT22{
public:
// Types
    using Micro  = Micro0;
    using Result = DHT_protocol::Result;

    using Celsius  = atd::Celsius<atd::Decimal<int32_t, 1>>;
    using Humidity = atd::Decimal<int32_t, 1>;

// Functions
    // El valor devuelto será válido si result_last_operation == ok
    static std::pair<Celsius, Humidity> read();


// Interfaz unificado para sensores
    static Celsius read_temperature();
    static Humidity read_humidity();


// Gestión de errores
    // En caso de que la última operación haya fallado, aquí se pueden
    // encontrar los detalles.
    static Result result_last_operation() {return result_;}
    static bool last_operation_is_ok() {return result_ == Result::ok;}
    static bool last_operation_fail() {return !last_operation_is_ok();}


private:
// Data
    inline static Result result_;

// Internal cfg
    static constexpr uint8_t start_time_ms = 1;

// Special values
    static constexpr Celsius null_temperature = Celsius{-1};
    static constexpr Humidity null_humidity   = Humidity{-1};
};


template <typename M, uint8_t npin>
std::pair<typename DHT22<M, npin>::Celsius, 
	  typename DHT22<M, npin>::Humidity> 
	DHT22<M, npin>::read()
{
    uint8_t data[5];

    result_ = DHT_protocol::basic_read<Micro, npin, start_time_ms>(data);
    if (result_ != Result::ok)
	return {null_temperature, null_humidity};

    using Dec = atd::Decimal<int32_t, 1>;
    int32_t h = atd::concat_bytes<int32_t>(data[0], data[1]);
    Dec dec_H = Dec::significand(h);

    int32_t t = atd::concat_bytes<int32_t>(data[2], data[3]);
    Dec dec_T = Dec::significand(t);

    Celsius T{dec_T};
    Humidity H{dec_H};

    return {T, H};
}

template <typename M, uint8_t npin>
inline DHT22<M, npin>::Celsius DHT22<M, npin>::read_temperature()
{
    auto [T, H] = read();
    
    return T;
}

template <typename M, uint8_t npin>
inline DHT22<M, npin>::Humidity DHT22<M, npin>::read_humidity()
{
    auto [T, H] = read();
    
    return H;
}

}// namespace dev
 

#endif

