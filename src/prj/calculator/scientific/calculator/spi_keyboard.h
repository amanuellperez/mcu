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

#ifndef __SPI_KEYBOARD_H__
#define __SPI_KEYBOARD_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Driver para un keyboard que se conecta por SPI.
 *	Aunque en principio solo lo voy a usar en la calculadora, lo voy
 *	escribiendo de forma genérica para poder reutilizarlo en el futuro en
 *	otras aplicaciones.
 *
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    19/12/2021 v0.0
 *
 ****************************************************************************/
#include <avr_atmega.h>

template <typename SPI_select0, uint16_t period_in_us0>
class SPI_Keyboard{
public:
    using SPI_select = SPI_select0;

    void init_SPI();
    uint8_t getchar();

private:
    using SPI = atmega::SPI_master;
    static constexpr uint16_t SPI_period_in_us = period_in_us0;
};


template <typename S, uint16_t T>
void SPI_Keyboard<S, T>::init_SPI()
{
    SPI::on<SPI_period_in_us>();
    SPI::spi_mode(0,0);
    SPI::data_order_LSB();
}

template <typename S, uint16_t T>
uint8_t SPI_Keyboard<S, T>::getchar()
{
    SPI_select select;
//    no_SS.write_zero();
    std::byte res = SPI::write(std::byte{0});
//   no_SS.write_one();

    return std::to_integer<uint8_t>(res);
}


#endif
