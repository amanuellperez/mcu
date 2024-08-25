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

#ifndef __DEV_MAX7219_H__
#define __DEV_MAX7219_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del MAX7219
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    22/08/2024 Escrito como traductor. El primer intento es del 2018.
 *
 ****************************************************************************/
#include <cstdint>
namespace dev{

/***************************************************************************
 *				MAX7219
 ***************************************************************************/
// struct MAX7219_cfg {
//	using SPI_master   = myu::SPI_master;
//	using SPI_selector = mcu::SPI_selector<myu::Micro, 
//						    SPI_master::CS_pin_number>;
// };

// PRECONDICIONES
//	+ El cliente es el responsable de fijar la frecuencia del reloj.
//	  Voy a suponer una comunicación SPI donde la frecuecia del relojo es
//	  la misma para todos los dispositivos SPI
//	  
template <typename Cfg>
class  MAX7219_basic{
public:
// Types
    using SPI		= typename Cfg::SPI_master;
    using SPI_select	= typename Cfg::SPI_selector;

// Constructor
    MAX7219_basic() = delete;
    static void init() { SPI_select::init(); }

// digits values
    // ndigit = 1..8
    // Escribimos el valor del digit correspondiente
    static void digit(uint8_t ndigit, uint8_t x) {send_packet (ndigit, x); }

// comandos
    static void no_op() {send_packet(0x00, 0x00); }

    static void enable_decode_mode() {send_packet(0x09, 0xFF);}
    static void disable_decode_mode() {send_packet(0x09, 0x00);}
    // Ver table 4 de la datasheet para posibles valores
    static void decode_mode(uint8_t mode) {send_packet(0x09, mode);}

    // Ver Table 7 para posibles valores
    static void intensity(uint8_t I) {send_packet (0x0A, I); }

    // Ver Table 8, aunque n indica el número de digit a mostrar
    static void scan_limit(uint8_t n) {send_packet (0x0B, n); }
    static void scan_all_digits() {send_packet (0x0B, 0x07); }

    // turn_on/turn_off
    static void normal_mode() {send_packet(0x0C, 0x01); }
    static void shutdown()    {send_packet(0x0C, 0x00); }

    // Sinónimos más comunes
    static void turn_on () { normal_mode(); }
    static void turn_off () { shutdown(); }

    
    static void display_test_on() {send_packet(0x0F, 0x01); }
    static void display_test_off() {send_packet(0x0F, 0x00); }

private:
    static void SPI_cfg();

    static void send_packet(uint8_t address, uint8_t data);

};

template <typename C>
void MAX7219_basic<C>::SPI_cfg() 
{
    SPI::spi_mode(0, 0); // CPOL = 0, CPHA = 0
    SPI::data_order_MSB();
}

template <typename C>
void MAX7219_basic<C>::send_packet(uint8_t address, uint8_t data)
{
    SPI_cfg();

    SPI_select select;

    SPI::write(address);
    // TODO: no funciona wait_transmission_complete!!!
//    SPI::wait_transmission_complete();
    SPI::write(data);

}


/***************************************************************************
 *			    MAX7219_driver
 ***************************************************************************/
// A partir de una matriz de 8x8 leds podemos crear un display de leds:
//
//  +------+------+------+------+
//  |      |      |      |      |
//  |      |      |      |      |
//  +------+------+------+------+
//  |      |      |      |      |
//  |      |      |      |      |
//  +------+------+------+------+
//  |      |      |      |      |
//  |      |      |      |      |
//  +------+------+------+------+
//
//  Por ejemplo, ese sería un display tipo matricial de 24 x 32 leds (filas x
//  columnas).
//
//  Como en el SDD1306 hablan por páginas, podemos concebir la matriz anterior
//  como 3 páginas de 32 leds cada una.


}// namespace dev

#endif

