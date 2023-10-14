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

#ifndef __DEV_EEPROM_25LC256_BASIC_H__
#define __DEV_EEPROM_25LC256_BASIC_H__
/****************************************************************************
 *
 * TODO
 *  Esto está muy anticuado. Reescribir:
 *		EEPROM<Micro, SPI>
 *	ó
 *		EEPROM<Micro, SPI, SPI_selector>
 *
 *
 *   - DESCRIPCION: Traductor de la EEPROM 25LC256
 *	
 *   - HISTORIA:
 *           Manuel Perez- 15/07/2019 Escrito
 *
 *  TODO: Falta la gestión de errores:
 *	1.- ¿qué pasa si el dispositivo no responde?
 *	2.- ¿otros errores?
 *
 ****************************************************************************/
#include <avr_atmega.h> // TODO: parametrizar SPI en la EEPROM
#include "not_generic.h"

#include <cstddef>    // byte
#include <atd_bit.h>

namespace dev{

template <uint8_t num_pin_no_chip_select,
	  uint8_t num_pin_no_write_protect = 0,
	  uint8_t num_pin_no_hold = 0>
class EEPROM_25LC256{
public:
    // Este Address puede manejar eeproms de hasta 64 kB.
    using Address   = uint16_t;
    using size_type = uint16_t; 

    // Construcción
    EEPROM_25LC256() { deselect();}

    // Configuración
    // -------------
    /// Configuramos el SPI_master_g para que se comunique con la EEPROM.
    /// Recordar haber inicializado correctamente el SPI_master_g antes.
    // Observar: la hacemos static indicando de esta forma que esta
    // configuración es la misma para las EEPROMs: basta con
    // llamarla una vez aunque tengamos 2 ó más.
    static void cfg_SPI();


    // Operaciones que admite el chip
    // ------------------------------
    /// Lee n bytes a partir de la dirección de memoria addr, guardando
    /// el resultado en buf. Si al leer se alcanza la última dirección de
    /// memoria, continua leyendo desde la dirección 0.
    /// CUIDADO: Antes de llamar a esta función hay que garantizar que 
    /// se haya finalizado de escribir, si no fallaría.
    size_type read(Address address, uint8_t* buf, size_type n);


    /// Escribe n bytes en la dirección de memoria indicada. 
    /// Como cada página tiene 64 bytes, en caso de que address + n se pase
    /// del fin de la página continua escribiendo en el principio de la
    /// página. Tarda máximo 5 ms.
    /// FUNDAMENTAL: para que write funcione es necesario haber llamado
    /// antes a write_enable(). En caso contrario, esta función no escribirá
    /// nada.
    uint8_t write(Address address, uint8_t* buf, uint8_t n);

    /// Set the write enable latch.
    void write_enable() { write_cmd(WREN); }

    /// Reset the write enable latch.
    void write_disable() { write_cmd(WRDI); }

    uint8_t read_status_register();
//  void write_status_register(uint8_t);<--- TODO: solo se pueden escribir 2 bits.

    // Funciones de estado
    // -------------------
    /// Devuelve el valor del bit Write-In-Process del status register.
    /// Indica si el chip está ocupado con una operación de escritura.
    static bool write_in_process(uint8_t status_reg) 
    //{return atd::bit<WRITE_IN_PROCESS>::of(status_reg);}
    {return atd::is_one_bit<WRITE_IN_PROCESS>::of(status_reg);}

    /// Devuelve el valor del bit Write Enable Latch del status register.
    /// Indica el chip está habilitado para escribir o no. Este bit es el 
    /// que activa la instrucción write_enable() y lo desactiva
    /// write_disable().
    static bool write_enable_latch(uint8_t status_reg)
    //{return atd::bit<WRITE_ENABLE_LATCH>::of(status_reg);}
    {return atd::is_one_bit<WRITE_ENABLE_LATCH>::of(status_reg);}


    // bits Block Protection
    // uint8_t block_protection_bits(uint8_t status_reg) const
    // Los dos bits van a la par, asi que no tiene sentido devolverlos por
    // separado. Será mejor devolver un uint8_t que contenga los 2 bits.
    // Cuando lo use, implementarlo. TODO.

    // Datos de la memoria
    // -------------------
    /// Número de bytes que tiene cada página.
    constexpr static size_type page_size()
    {return NUM_BYTES_POR_PAGINA;}

    /// Dirección de la última posición de memoria.
    constexpr static size_type max_address() 
    {return MAX_ADDRESS;}

private:
    // Pines del device
    // ----------------
    // Usamos los mismos nombres que la datasheet
    not_generic::Output_pin<num_pin_no_chip_select> no_CS_;
    not_generic::Output_pin<num_pin_no_write_protect> no_WP_;
    not_generic::Output_pin<num_pin_no_hold> no_HOLD_;


    // Instruction Set
    // ---------------
    constexpr static uint8_t READ  {0b00000011}; /* read memory */
    constexpr static uint8_t WRITE {0b00000010}; /* write to memory */

    constexpr static uint8_t WRDI {0b00000100}; /* write disable */
    constexpr static uint8_t WREN {0b00000110}; /* write enable */

    constexpr static uint8_t RDSR {0b00000101}; /* read status register */
    constexpr static uint8_t WRSR {0b00000001}; /* write status register */


    // EEPROM Status Register Bits
    // ---------------------------
    constexpr static uint8_t WRITE_IN_PROCESS   = 1;
    constexpr static uint8_t WRITE_ENABLE_LATCH = 2;
    constexpr static uint8_t BLOCK_PROTECTION_0 = 4;
    constexpr static uint8_t BLOCK_PROTECTION_1 = 8;

    // Información
    constexpr static size_type NUM_BYTES_POR_PAGINA = 64;

    // Direcciones de memoria disponibles: [0, 0x7FFF]
    // Esta EEPROM es de 32 KB = 2^15 bytes, las direcciones van de la 0 a la
    // 0x8000 - 1 = 0x7FFFF.
    // static constexpr uint16_t MIN_ADDRESS     = 0u;
    constexpr static size_type MAX_ADDRESS     = 0x7FFF;


    // Funciones de ayuda
    // ------------------
    // Seleccionamos el chip
    void select() {no_CS_.write_zero();}

    // Lo deseleccionamos
    void deselect() {no_CS_.write_one();}

    class Select{
    public:
	explicit Select(EEPROM_25LC256& eeprom) : eeprom_{&eeprom} 
	{ eeprom_->select(); }

	~Select() { eeprom_->deselect(); }

	Select(const Select&) = delete;
	Select& operator= (const Select&) = delete;

//	explicit operator bool() const {return eeprom_->good();}

    private:
	EEPROM_25LC256* eeprom_;
    };


    void write_address(Address address)
    {	// Data order: MSB first, LSB last
	not_generic::SPI_master_g::write(static_cast<uint8_t>(address >> 8));
	not_generic::SPI_master_g::write(static_cast<uint8_t>(address));
    }

    void write_cmd(uint8_t cmd);

};

template <uint8_t no_CS, uint8_t no_WP, uint8_t no_HOLD>
void EEPROM_25LC256<no_CS, no_WP, no_HOLD>::cfg_SPI()
{
    not_generic::SPI_master_g::spi_mode(0, 0);
    not_generic::SPI_master_g::data_order_MSB();
}



// TODO: ¿cómo gestionar errores?
template <uint8_t no_CS, uint8_t no_WP, uint8_t no_HOLD>
typename EEPROM_25LC256<no_CS, no_WP, no_HOLD>::size_type
EEPROM_25LC256<no_CS, no_WP, no_HOLD>::read(Address address,
                                            uint8_t* buf,
                                            size_type n)
{
    Select select{*this};

    not_generic::SPI_master_g::write(READ);
    write_address(address);
    for (size_type i = 0; i < n; ++i){
	*buf = not_generic::SPI_master_g::read();
	++buf;
    }

    return n;
}


// TODO: ¿cómo gestionar errores?
//  El problema es que no es posible saber si el SPI_master_g funciona o no.
//  1.- Una forma sería que después de escribir todo se comprobase que se ha
//  escrito leyendo la memoria.
template <uint8_t no_CS, uint8_t no_WP, uint8_t no_HOLD>
uint8_t EEPROM_25LC256<no_CS, no_WP, no_HOLD>::
			    write(Address address, uint8_t* buf, uint8_t n)
{
    Select select{*this};

    not_generic::SPI_master_g::write(WRITE);
    write_address(address);
    
    uint8_t i = 0;
    for (; i < n; ++i){
	not_generic::SPI_master_g::write(*buf);
	++buf;
    }

    return i;
}


template <uint8_t no_CS, uint8_t no_WP, uint8_t no_HOLD>
void EEPROM_25LC256<no_CS, no_WP, no_HOLD>::write_cmd(uint8_t cmd)
{
    Select select{*this};
    not_generic::SPI_master_g::write(cmd);
}


template <uint8_t no_CS, uint8_t no_WP, uint8_t no_HOLD>
uint8_t EEPROM_25LC256<no_CS, no_WP, no_HOLD>::
					read_status_register()
{
    Select select{*this};
    not_generic::SPI_master_g::write(RDSR);
    uint8_t status = not_generic::SPI_master_g::read();

    return status;
}



}// namespace

#endif


