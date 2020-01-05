// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __DEV_REGISTER_74HC595_H__
#define __DEV_REGISTER_74HC595_H__

/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz con el registro 74HC595. 
 *   
 *   - TODO: Falta añadirle el pin no_OE. No lo añado porque tengo mis dudas
 *   de cómo configurarlo. Por defecto ¿conectar el registro  o no conectarlo?
 *   Es la versión 0. Usarlo y ya veremos qué hacer.
 *
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez  - 09/06/2019 Escrito v0.1
 *		    10/06/2019 v0.2: conecto el 74HC595 al SPI del avr.
 *
 ****************************************************************************/
#include <avr_pin.h>
#include <avr_time.h>
#include <avr_SPI.h>

namespace dev{

// Notas a la implementación:
//  No es necesario definir los pines dentro de esta clase, porque basta con
//  llamar a las funciones Pin<num_pin_SER>::write_one() para que todo
//  funcione. De hecho este código seguramente sea totalmente eficiente.
//  Sin embargo es más natural concebir el registro como teniendo unos pines
//  de entrada y de salida. Por eso opto por definir los pines dentro de la
//  clase, para que la programación (y el mantenimiento) sea lo más natural y
//  sencillo posible. ¿Introduzco algo de ineficiencia al hacer esto? Puede, o
//  puede que el compilador lo optimice todo. No lo he mirado.
//  Regla: a la hora de elegir entre facil mantenimiento y posible mejora de
//  optimizar, mejor elegir la opción del mantenimiento. Solo si va lento y
//  hay que optimizar optimizar.
/// Concebimos el registro 74HC595 como un dispositivo SPI.
/// Conexiones: 
///	    + el MOSI del avr al pin 14 del registro
///	    + el SCK del avr al pin 11 del registro.
///	    + conectar como se quiera al RCLK (es el que vuelca losdatos)
///	    + opcional: conectar el NO_SRCLR (=clear). Si no se conecta,
///			pasarle un 0.
///
/// Observar que el 74HC595 no tiene posibilidad de ser seleccionado. Esto,
/// supongo, puede generar un problema si se quiere integrar con otros
/// dispositivos SPI (aunque añadiendo una puerta and y una nor al clock del
/// registro es fácil crear ese pin de select).
///
/// FUNDAMENTAL: para que esta clase funciones es fundamental que el cliente
/// configure el SPI como master. Tiene que haberse llamado a SPI_master::init
/// y estar como low el pin no_SS (esto ultimo no lo tengo claro si es
/// obligatorio ya que funciona aunque este high, pero en la datasheet lo ponen
/// como low).
template <uint8_t num_pin_RCLK, uint8_t num_pin_NO_SRCLR>
class Register_74HC595{
public:
    Register_74HC595() 
    { 
	avr::SPI_master::data_order_LSB();
	pin_no_srclr_.write_one(); 
    }

    /// Escribimos en el registro un byte. No devuelve el control
    /// hasta no acabar de escribir.
    // Observar que esta función se puede definir como const. Sin embargo,
    // el cliente supondrá que es no const. La dejo con el prototipo esperado.
    void buffer_write(uint8_t x) { avr::SPI_master::write(x); }

    /// Vuelca el buffer en el registro de salida.
    void buffer_vuelca(){ pin_rclk_.pulse_of_1us(); }

    void buffer_clear() { pin_no_srclr_.negative_pulse_of_1us(); }

    /// Escribimos en la salida el byte x
    void write(uint8_t x)
    {
	buffer_write(x);
	buffer_vuelca();
    }

    /// Vacía el buffer y la salida
    void clear()
    {
	buffer_clear();
	buffer_vuelca();
    }

private:
    avr::Output_pin<num_pin_RCLK> pin_rclk_;	// volcado del buffer a la salida
    avr::Output_pin<num_pin_NO_SRCLR> pin_no_srclr_;// Borra el buffer de entrada
};


// Idioma: forzamos a que el usuario construya la clase con el orden de los
// pines adecuados.
template<uint8_t n>
struct Pin_register_74HC595_RCLK	{};

template<uint8_t n>
struct Pin_register_74HC595_NO_SRCLR	{};


template <uint8_t num_pin_RCLK, uint8_t num_pin_NO_SRCLR>
Register_74HC595<num_pin_RCLK,num_pin_NO_SRCLR> 
register_74HC595(Pin_register_74HC595_RCLK<num_pin_RCLK>,
		 Pin_register_74HC595_NO_SRCLR<num_pin_NO_SRCLR>)
{ return Register_74HC595<num_pin_RCLK,num_pin_NO_SRCLR> {}; }

}// namespace

#endif

