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

#ifndef __DEV_DHT22_H__
#define __DEV_DHT22_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Driver para acceder al sensor de temperatura y humedad
 *	    DHT22 y para el DHT11.
 *
 *	    Los dos sensores funcionan igual, la diferencia es que el DHT22
 *	    tiene más resolución. De todas formas de los 2 sensores DHT11 que
 *	    compré los 2 miden fatal el RH, y uno da la temperatura correcta
 *	    pero el otro no (no parece que sean muy buenos sensores).
 *
 *	    Conecto una pull-up resistor tal como dice la datasheet de estos
 *	    sensores.
 *
 *   - COMENTARIOS: Usa el timer0, con lo que la aplicación no puede usarlo.
 *
 *   - HISTORIA:
 *           Manuel Perez - 19/04/2018 Escrito
 *
 ****************************************************************************/
#include "avr_pin.h"



namespace dev{
/*!
 *  \brief  Driver del DHT22
 *
 *  Conoce el protocolo de comunicación con este sensor.
 *  Al no poder controlar el tiempo, lo único que deja al cliente gestionar
 *  es que llame cada 2 segundos a la función read para leer la temperatura
 *  y la humedad.
 *
 */
class DHT22{
public:
    enum class Error
    {
      NONE = 0,
      BUS_HUNG,
      NOT_PRESENT,
      ACK_TOO_LONG,
      SYNC_TIMEOUT,
      DATA_TIMEOUT,
      CHECKSUM
    };

    /// Conectamos el sensor al pin indicado
    explicit DHT22(uint8_t num_pin);

    /// Actualiza la la temperatura y el % de humedad
    /// Esta función hay que llamarla cada 2 segundos. 
    /// No voy a llevar control del tiempo aquí para no bloquear ningún timer.
    /// Será el cliente el responsable de saber cuándo llamar a esta función.
    /// Devuelve si la lectura fue realizada con éxito (true) o no (false)
    /// Usa internamente el Timer0!!!
    Error read();

    // Última lectura realizada con éxito.
    int8_t T_parte_entera;
    uint8_t T_parte_decimal;
    uint8_t RH_parte_entera;
    uint8_t RH_parte_decimal;

private:
    // pin de datos al que conectamos el sensor
    // datos del pin (las funciones de Pin son a dia de hoy costosas, no
    // funcionando si uso Pin)
    uint8_t num_pin_;

    constexpr static uint8_t DATA_BIT_COUNT= 40u;

    // Funciones de ayuda
    Error read_data(uint8_t* bit_high, uint8_t* bit_low);
};

}// namespace

#endif

