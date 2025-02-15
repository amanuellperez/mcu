// Copyright (C) 2019-2023 Manuel Perez 
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

#ifndef __MEGA_SPI_HWD_H__
#define __MEGA_SPI_HWD_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Traductor del SPI.
 *
 *  - COMENTARIOS:
 *	¿Cómo gestionar los errores en el SPI?
 *	En principio da la impresión de que (por lo menos por software) no
 *	podemos. Si el SPI funciona como master, SS, SCK y MOSI son pins de
 *	salida. El SPI escribe ahí. El MISO es el único pin de entrada: nos
 *	limitamos a leerlo, nada más. El potencial que haya en ese pin es el
 *	que consideramos que es el byte entrante. ¡Imposible detectar que el
 *	dispositivo no está conectado!
 *
 *	La gestión de errores hay que hacerla en el driver del dispositivo.
 *	El dispositivo tendrá que tener algún tipo de protocolo con el avr: te
 *	pido tal cosa, tu me respondes con otra. Si podemos comprobar que la
 *	respuesta es o no la adecuada sabremos si está conectado o no.
 *  
 *  - HISTORIA:
 *    Manuel Perez
 *	10/04/2018 v0.0
 *	10/07/2019 Reescrito. Comienzo con traductor.
 *	19/10/2019 Lo dejo como un traductor puro.
 *	10/04/2021 SPI, SPI_master, SPI_slave
 *	12/02/2023 Elimino std::byte a favor de uint8_t. Usar std::byte obliga
 *		   a tener que estar haciendo castings todo el rato. Puede que
 *		   no sepa usarlo. De momento me quedo con uint8_t para
 *		   representar los bytes.
 *
 ****************************************************************************/
#include <stdint.h> // uint8_t
#include <avr/io.h> // registros: DDRB... PORT...
#include <atd_bit.h>
#include "mega_pin_hwd.h"
#include "mega_interrupt.h"
#include "mega_import_avr.h"	// clock_frequency_in_hz

namespace mega_{
namespace hwd{

class SPI{
public:
    using Reg = cfg::spi;

// CARACTERÍSTICAS DEL SPI
    // Chip select: el atmega tiene la característica de que cuando se usa
    // como master este pin obligatoriamente tiene que estar como output:
    // usémoslo como chip select!!! (esto será genérico? qué hacen otros
    // micros?)
    // De acuerdo a la datasheet, 23.3.2, en caso de configurarlo como input
    // pin, si su valor es LOW el SPI consideraría que hay otro SPI-Master que
    // quiere seleccionarlo y el micro cambiaría la configuración a
    // SPI-slave!!!
    // Si no se trabaja en un entorno con varios SPI-master, definirlo como
    // output es la configuración más sencilla.

// pines a los que está conectado el SPI
    static constexpr uint8_t MOSI_pin = Reg::MOSI_pin;
    static constexpr uint8_t MISO_pin = Reg::MISO_pin;
    static constexpr uint8_t SCK_pin  = Reg::SCK_pin;
    static constexpr uint8_t SS_pin   = Reg::SS_pin;

// CONSTRUCTOR
    // No permitimos que se construya este objeto
    SPI() = delete;

    // ---------
    // Traductor
    // ---------
    /// Causes the SPI interrupt to be executed if a serial transmission is
    /// completed and interrupts are enable.
    // CUIDADO: recordar llamar enable_interrupts!!! 
    static void interrupt_enable() 
    {atd::write_bit<SPIE>::to<1>::in(SPCR);}

    /// Disable interrupt.
    static void interrupt_disable() 
    {atd::write_bit<SPIE>::to<0>::in(SPCR);}

    /// Enable SPI as master. This must be set to enable SPI operations.
    static void enable_as_a_master() 
    {atd::write_bits<MSTR, SPE>::to<1,1>::in(SPCR);}


    /// Enable SPI as a slave. This must be set to enable SPI operations.
    static void enable_as_a_slave() 
    { atd::write_bits<MSTR, SPE>::to<0,1>::in(SPCR); }

    /// Disable SPI.
    static void disable() 
    {atd::write_bit<SPE>::to<0>::in(SPCR);}
 

    /// El SPI envia primero el LSB (least significant bit = unidades)
    static void data_order_LSB() 
    {atd::write_bit<DORD>::to<1>::in(SPCR);}


    /// El SPI envia primero el MSB (most significant bit)
    static void data_order_MSB() 
    {atd::write_bit<DORD>::to<0>::in(SPCR);}


    /// Configuramos el modo de operación: la polaridad cpol y la fase cpha
    /// cpol y cpha son números: 0 ó 1
    static void mode(uint8_t cpol, uint8_t cpha);


    // Selección de la velocidad del reloj.
    // Solo funciona oara el master. No tiene efecto en el slave (ver
    // datasheet)
    static void clock_frequency_divide_by_2();
    static void clock_frequency_divide_by_4();
    static void clock_frequency_divide_by_8();
    static void clock_frequency_divide_by_16();
    static void clock_frequency_divide_by_32();
    static void clock_frequency_divide_by_64();
    static void clock_frequency_divide_by_128();


    /// Is a serial transfer complete?
    static bool is_transmission_complete()
    {return atd::is_one_bit<SPIF>::of(SPSR);}

    /// The SPI data register was written during data transfer?
    // To clear this bit first read WCOL, then read SPI data register.
    static bool is_a_write_collision()
    {return atd::is_one_bit<WCOL>::of(SPSR);}


    /// Write x in the data register and initiates data transmissioin (si SS
    /// está seleccionado).
    static void data_register(uint8_t x) {SPDR = x;}  

    /// Devuelve el último uint8_t recibido en el último trade.
    static uint8_t data_register() {return SPDR;}
};



}// namespace 
}// namespace 


#endif 

