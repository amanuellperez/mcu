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

#ifndef __DEV_OV7670_H__
#define __DEV_OV7670_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor para la cámara OV7670
 *
 * HISTORIA
 *    Manuel Perez
 *    01/12/2023 Primeros experimentos
 *
 ****************************************************************************/
#include <stdint.h>
#include <iostream>

namespace dev{

// Result
namespace impl_of{

enum class OV7670_state{
    ok,
    twi_fail_connection,
    write_register_index_fail,
    write_register_data_fail,
    read_register_index_fail,
    read_register_data_fail
};

enum class OV7670_size{
    VGA, QVGA, QQVGA, CIF, QCIF
};



struct OV7670_register{
    uint8_t address;
    uint8_t value;
};

}// impl_of

// Camera Inteface (CAMIF)
// -----------------------
template <uint8_t pin_VSYNC>
struct OV7670_VSYNC{
    static constexpr uint8_t VSYNC = pin_VSYNC;
};

template <uint8_t pin_HREF>
struct OV7670_HREF{
    static constexpr uint8_t HREF = pin_HREF;
};

template <uint8_t pin_PCLK>
struct OV7670_PCLK{
    static constexpr uint8_t PCLK = pin_PCLK;
};

// (RRR) ¿Por qué DPort?
//       Es fundamental acceder lo más eficientemente posible al puerto de la
//       cam D[0..7]. Para ello lo mejor es vincular un PORT del avr a esos
//       pines y leerlos directamente como byte. Pero no es práctico asociar
//       un PORT completo del avr a D[0..7] sino que es mejor usar dos nibbles
//       (por ejemplo, avr::PORTD_HIGH, avr::PORTC_LOW). ¿Cómo independizar la
//       clase OV7670 de la forma de acceder a estos pines? De eso es
//       responsable la clase DPort (= puerto de pines D[0..7] de la cam)
template <typename pin_VSYNC,
	  typename pin_HREF,
	  typename pin_PCLK,
	  typename DPort0>
struct OV7670_pins{
    static constexpr uint8_t VSYNC = pin_VSYNC::VSYNC;
    static constexpr uint8_t HREF  = pin_HREF::HREF;
    static constexpr uint8_t PCLK  = pin_PCLK::PCLK;
    using DPort = DPort0;   // Maneja los 8 pins D[0..7] como byte
};


template <typename Micro0, typename TWI_master_ioxtream0,
	  typename Pins0>
struct OV7670_cfg{
    using Micro        = Micro0;
    using TWI          = TWI_master_ioxtream0;
    using TWI_master   = typename TWI::TWI_master;
    using Pin          = Pins0;
};

// Como el slave_address es único, solo es posible tener una cámara de este
// estilo conectada. Por eso todas las funciones son static.
template <typename Cfg>
class OV7670{
public:
    using Micro      = Cfg::Micro;
    using TWI	     = Cfg::TWI;
    using TWI_master = Cfg::TWI_master;
    using Address    = typename TWI::Address;
    using Result     = impl_of::OV7670_state;
    using Register   = impl_of::OV7670_register;

    static constexpr Address slave_address = 0x21; // es única
	
    // Miramos a ver si hay comunicación con el twi-device
    // Return: true (found device); false (no response)
    static bool probe();

// Construction
    static bool init();

// Commands
    static bool reset();

    // Guarda una imagen en out. TODO: name??? save_image??? take_picture???
    static void capture_image(std::ostream& out);

// Funciones de bajo nivel para acceder a los registros
    // register[i] = x;
    static bool write_register(uint8_t i, uint8_t x);

    // x = register[i];
    static uint8_t read_register(uint8_t i);

    // (RRR) ¿Por qué no pasarle un array directamente o un std::span?
    //	     ¿write(std::span<Register>)?
    //
    //	     Lo normal es que guardemos el array con la configuración dentro 
    //	     de la PROGMEM y no en RAM. std::span solo funciona con RAM.
    //
    //	     Al usar iteradores, el iterador encapsula el acceso a RAM o a
    //	     PROGMEM, desvinculando esta clase de PROGMEM.
    //
    // Escribe un array de registros en memoria
    template <std::forward_iterator It>
    static bool write(It p0, It pe);

// Gestión de errores
    // En caso de que la última operación haya fallado, aquí se pueden
    // encontrar los detalles.
    static Result result_last_operation() {return result_;}
    static bool last_operation_is_ok() {return result_ == Result::ok;}
    static bool last_operation_fail() {return !last_operation_is_ok();}

private:
    inline static Result result_;

// Connections
    template <uint8_t n>
    using Pin = typename Micro::Pin<n>;
    using VSYNC = Pin<Cfg::Pin::VSYNC>;
    using HREF  = Pin<Cfg::Pin::HREF>;
    using PCLK  = Pin<Cfg::Pin::PCLK>;
    using DPort = Cfg::Pin::DPort;
    

// Cfg
    static constexpr uint8_t null_uint8_t = 0xBA; // 0xBAD!!!
    static constexpr uint16_t null_uint16_t= 0xBAD0; 
};

template <typename Cfg>
bool OV7670<Cfg>::init()
{
    VSYNC::as_input_without_pullup();
    HREF::as_input_without_pullup();
    PCLK::as_input_without_pullup();
    DPort::as_input_without_pullup();

    return reset();
}


// TODO: escribir array de registros. Esperar 300 ms al final
template <typename Cfg>
bool OV7670<Cfg>::write_register(uint8_t i, uint8_t x)
{
    TWI twi{slave_address};
    if (twi.error()) {
	result_ = Result::twi_fail_connection;
	return false;
    }

    twi << i;
    if (twi.error()) {
	result_ = Result::write_register_index_fail;
	return false;
    }

    twi << x;
    if (twi.error()) {
	result_ = Result::write_register_data_fail;
	return false;
    }

    twi.close();

    Micro::wait_ms(1); //TODO: según la datasheet máximo 300 ms en cambiar un registro!!!

    result_ = Result::ok;
    return true;
}


template <typename Cfg>
uint8_t OV7670<Cfg>::read_register(uint8_t i)
{
    TWI twi{slave_address};
    if (twi.error()) {
	result_ = Result::twi_fail_connection;
	return null_uint8_t;
    }

    twi << i;
    if (twi.error()) {
	result_ = Result::read_register_index_fail;
	return null_uint8_t;
    }

    twi.close();
    Micro::wait_ms(1);  // FUNDAMENTAL!!!

    uint8_t res{};
    twi.open(slave_address);
    twi >> res;

    if (twi.error()) {
	result_ = Result::read_register_data_fail;
	return null_uint8_t;
    }

    twi.close();
    Micro::wait_ms(1); 
    
    result_ = Result::ok;
    return res;
}

template <typename Cfg>
    template <std::forward_iterator It>
bool OV7670<Cfg>::write(It p0, It pe)
{
    for (; p0 != pe; ++p0)
	write_register(p0->address, p0->value);
}



template <typename Cfg>
inline bool OV7670<Cfg>::probe()
{ return TWI_master::probe(slave_address); }

template <typename Cfg>
bool OV7670<Cfg>::reset()
{
    result_ = Result::ok;

    if (write_register(0x12, 0x80) == false)
	return false;

    Micro::wait_ms(100);    // TODO: segun la datasheet 1 ms máximo
			
    return true;
    
}


// TODO: esta función da por supuesto que la cámara tiene configurada una
// cierta polaridad (se puede cambiar en COM10). Generalizarla.
template <typename Cfg>
void OV7670<Cfg>::capture_image(std::ostream& out)
{
  Micro::Disable_interrupts(); // CUIDADO: ostream no debe usar interrupciones!!!

  DPort dport;

// Esperamos a que empiece el siguiente frame
    while (VSYNC::is_zero()) { ; }
    while (VSYNC::is_one()) { ; }
    
// Leemos lo que envia la cámara y lo escribimos en out
    while (VSYNC::is_zero()){
	while (HREF::is_zero()) { ; } // wail_start_next_row();
				 
	while (HREF::is_one()){
	    while (PCLK::is_zero()) { ; } // wait_read_next_pixel();
	    
	    out << dport;
	}
    }
}



}// dev
 

#endif
