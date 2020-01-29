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

#ifndef __DEV_POT_MCP4231_H__
#define __DEV_POT_MCP4231_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Driver del potenciómetro MCP4231
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	19/10/2019 v0.0: Escrito.
 *
 ****************************************************************************/
#include <avr_pin.h>
#include <avr_SPI.h>
#include <atd_bit.h>


namespace dev{


// TODO: añadir pin shutdown. Despues, sacar todo el código fuente de dentro
// de la clase (ordenarlo un poco todo).
template <uint8_t num_pin_no_CS>
class Pot_MCP4231{
public:
    // Construcción
    Pot_MCP4231() : good_{true} { deselect(); }

    // Antes que acceder al potenciometro es necesario configurar el SPI.
    /// Configuramos el SPI para que se comunique con el potenciómetro.
    /// Recordar haber inicializado correctamente el SPI antes.
    /// Es responsabilidad del usuario el inicializar el SPI y configurarlo
    /// antes de acceder al potenciómetro.
    // Observar: la hacemos static indicando de esta forma que esta
    // configuración es la misma para todos los potenciometros: basta con
    // llamarla una vez aunque tengamos 2 ó más.
    static void cfg_SPI();


    // Operaciones que admite el chip
    // -------------------------------
    /// Escribe el valor x en el potenciometro i (= 0 ó 1).
    /// Solo los primeros 8/9 bits de x son válidos: 0 <= x <= 128/256.
    /// x es el tanto por 128(ó 256) (= º/128 ó º/256) del valor total del 
    /// potenciometro.
    // x tiene que ser un uint16_t ya que el potenciometro va hasta 256.
    template <uint8_t i>
    void write_data(uint16_t x)
    {
	if (x > valor_max(num_bits)) x = valor_max(num_bits);	

	write_cmd(WRITE_DATA[i], x);
    }


    template <uint8_t i>
    uint16_t read_data()
    { return read_cmd16(READ_DATA[i]); }

    template <uint8_t i>
    void increment_wiper()
    { write_cmd(INCREMENT_WIPER[i]); }

    template <uint8_t i>
    void decrement_wiper()
    { write_cmd(DECREMENT_WIPER[i]); }


    // Estado
    // ------
    /// Indica si el potenciómetro está en buen estado y podemos escribir en
    /// él.
    // ¿Cuándo se puede producir un error? 
    //	1.- Por error de programación, se envía un comando inexistente.
    //	2.- Que haya ruido en la transmisión y el potenciómetro reciba mal los
    //	datos.
    bool good() const {return good_;}

    /// Deja el potenciómetro en buen estado.
    void clear() 
    {
	good_ = true;
	// deselect(); <-- no es necesario hacer esto ya que la clase Select
	// garantiza que siempre se deja el potenciometro desconectado del SPI
    }

    /// Indicates if the Hardware Shutdown pin (no_SHDN) is low.
    bool is_hardware_shutdown()
    { return atd::bit<STATUS_REGISTER_no_SHDN_BIT>::of(status_register()); }

    /// Forces (or not) resistor 0 into the "shutdown" configuration
    void resistor0_shutdown(bool on) { resistor_shutdown<0>(on);}

    /// Is resistor 0 into the "shutdown" configuration?
    bool resistor0_shutdown() { return resistor_shutdown<0>();}

    // TODO: estas funciones deberían de funcionar, pero están sin probar
//    bool resistor0_connect_terminal_A() 
//    { return resistor_connect_terminal_A<0>(); }
//
//    void resistor0_connect_terminal_A(bool on)
//    { resistor_connect_terminal_A<0>(on); }
//
//    bool resistor0_connect_wiper() 
//    { return resistor_connect_wiper<0>(); }
//
//    void resistor0_connect_wiper(bool on) 
//    { resistor_connect_wiper<0>(on); }
//
//    bool resistor0_connect_terminal_B() 
//    { return resistor_connect_terminal_B<0>(); }
//
//    void resistor0_connect_terminal_B(bool on)
//    { resistor_connect_terminal_B<0>(on); }


    /// Forces (or not) resistor 1 into the "shutdown" configuration
    void resistor1_shutdown(bool on) { resistor_shutdown<1>(on);}

    /// Is resistor 1 into the "shutdown" configuration?
    bool resistor1_shutdown() { return resistor_shutdown<1>();}

    // TODO: estas funciones deberían de funcionar, pero están sin probar
//    bool resistor1_connect_terminal_A() 
//    { return resistor_connect_terminal_A<1>(); }
//
//    void resistor1_connect_terminal_A(bool on)
//    { resistor_connect_terminal_A<1>(on); }
//
//    bool resistor1_connect_wiper() 
//    { return resistor_connect_wiper<1>(); }
//
//    void resistor1_connect_wiper(bool on) 
//    { return resistor_connect_wiper<1>(on); }
//
//    bool resistor1_connect_terminal_B() 
//    { return resistor_connect_terminal_B<1>(); }
//
//    void resistor1_connect_terminal_B(bool on)
//    { resistor_connect_terminal_B<1>(on); }


private:
    // Datos
    // -----
    // Pines del device
    avr::Output_pin<num_pin_no_CS> no_CS_;

    // Estado
    bool good_;	

    // Configuración
    // TODO: comprar otros potenciómetros de 8 bits y usar esta misma clase
    // para tanto los de 7 como los de 8.
    constexpr static uint8_t num_bits = 7; // Pueden ser 7 ó 8 bits

    // Devuelve el valor máximo que puede tomar el potenciómetro dependiendo
    // del número de bits.
    constexpr static uint8_t valor_max(uint8_t n)
    {return 128u*(n - 6u);}  // = 128*(x-7)+128; valor[7] = 128, valor[8] = 256


    // Instruction set (table 7-2 datasheet)
    // -------------------------------------
    constexpr static uint16_t WRITE_DATA[2] = {0x0000, 0x1000};

    constexpr static uint8_t READ_DATA[2]       = {0x0C, 0x1C};
    constexpr static uint8_t INCREMENT_WIPER[2] = {0x04, 0x14};
    constexpr static uint8_t DECREMENT_WIPER[2] = {0x08, 0x18};

    constexpr static uint16_t TCON_REGISTER_WRITE_DATA = 0x4000;
    constexpr static uint8_t TCON_REGISTER_READ_DATA   = 0x4C;

    constexpr static uint8_t STATUS_REGISTER_READ_DATA = 0x5C;
    
    // Bit de error
    constexpr static uint8_t CMDERR_BIT = 7;    // bit que ocupa CMDERR

    // status register bits
    constexpr static uint8_t STATUS_REGISTER_no_SHDN_BIT = 1;

    // TCON register bits
    constexpr static uint8_t TCON_REGISTER_RHW_BIT[2] = {3, 7};
    constexpr static uint8_t TCON_REGISTER_RA_BIT[2]  = {2, 6};
    constexpr static uint8_t TCON_REGISTER_RW_BIT[2]  = {1, 5};
    constexpr static uint8_t TCON_REGISTER_RB_BIT[2]  = {0, 4};

    // Funciones de ayuda
    // ------------------
    // Seleccionamos el chip
    void select() {no_CS_.write_zero();}

    // Lo deseleccionamos
    void deselect() {no_CS_.write_one();}

    class Select{
    public:
	explicit Select(Pot_MCP4231& pot) : pot_{&pot} 
	{ pot_->select(); }

	~Select() { pot_->deselect(); }

	Select(const Select&) = delete;
	Select& operator= (const Select&) = delete;

	explicit operator bool() const {return pot_->good();}

    private:
	Pot_MCP4231* pot_;
    };


    void SPI_write(uint16_t x) 
    { 
	std::byte res = avr::SPI::write(static_cast<std::byte>(x >> 8)); 

	if (!atd::bit<CMDERR_BIT>::of(res))
	    good_ = false;
	
	// si ha fallado ¿puedo ahorrar esta llamada?
	avr::SPI::write(static_cast<std::byte>(x)); 
    }

    // Lectura/escritura del status register
    // -------------------------------------
    // El status register solo tiene 1 bit de interes: 
    // el STATUS_REGISTER_no_SHDN
    std::byte status_register()
    {
	// DUDA: Los bits D5:D8 del status register siempre son 1. Se podria
	// comprobar esto para ver si hay errores.
	return read_cmd8(STATUS_REGISTER_READ_DATA);
    }

    // Lectura/escritura del TCON register
    // -----------------------------------
    void TCON_register_write(std::byte x)
    { write_cmd(TCON_REGISTER_WRITE_DATA, std::to_integer<uint16_t>(x)); }

    std::byte TCON_register_read()
    { return read_cmd8(TCON_REGISTER_READ_DATA); }


    template <uint8_t pos>
    void TCON_register_write_bit(bool on)
    {
	std::byte reg = TCON_register_read();
	atd::bit<pos>::of(reg) = on;
	TCON_register_write(reg);
    }

    template <uint8_t pos>
    bool TCON_register_read_bit() 
    { 
	std::byte reg = TCON_register_read();
	return atd::bit<pos>::of(reg); 
    }

    // Operaciones básicas 
    // -------------------
    // Lee un comando del potenciometro
    std::byte read_cmd8(uint8_t cmd)
    {
	Select select{*this};

	if (!select)
	    return std::byte{0};

	std::byte res1 = avr::SPI::write(std::byte{cmd});

	if (!atd::bit<CMDERR_BIT>::of(res1))  
	    good_ = false;

	return avr::SPI::read();
    }


    uint16_t read_cmd16(uint8_t cmd)
    {
	Select select{*this};

	if (!select)
	    return 0;

	std::byte res1 = avr::SPI::write(std::byte{cmd});

	if (!atd::bit<CMDERR_BIT>::of(res1))  
	    good_ = false;

	std::byte res2 = avr::SPI::read();

	return atd::concat_bytes<uint16_t>(res1 & std::byte{0x01}, res2);
    }

    void write_cmd(uint8_t cmd)
    {
	Select select{*this};

	if (!select)
	    return;

	avr::SPI::write(std::byte{cmd});
    }

    // data = es de 9 bits, por eso tiene que ser un uint16_t.
    // precondicion: los bits 9-15 de data == 0.
    // El comando lo definimos de tal manera que sus 9 primeros bits sean 0.
    void write_cmd(uint16_t cmd, uint16_t data)
    {
	Select select{*this};

	if (!select)
	    return;

	SPI_write(cmd | data);
    }




    template <uint8_t i>
    void resistor_shutdown(bool on)
    { TCON_register_write_bit<TCON_REGISTER_RHW_BIT[i]>(!on); }

    template <uint8_t i>
    bool resistor_shutdown() 
    { return !TCON_register_read_bit<TCON_REGISTER_RHW_BIT[i]>(); }

    template <uint8_t i>
    void resistor_connect_terminal_A(bool on)
    { TCON_register_write_bit<TCON_REGISTER_RA_BIT[i]>(on); }

    template <uint8_t i>
    bool resistor_connect_terminal_A() 
    { return TCON_register_read_bit<TCON_REGISTER_RA_BIT[i]>(); }

    template <uint8_t i>
    void resistor_connect_wiper(bool on)
    { TCON_register_write_bit<TCON_REGISTER_RW_BIT[i]>(on); }

    template <uint8_t i>
    bool resistor_connect_wiper() 
    { return TCON_register_read_bit<TCON_REGISTER_RW_BIT[i]>(); }

    template <uint8_t i>
    void resistor_connect_terminal_B(bool on)
    { TCON_register_write_bit<TCON_REGISTER_RB_BIT[i]>(on); }

    template <uint8_t i>
    bool resistor_connect_terminal_B() 
    { return TCON_register_read_bit<TCON_REGISTER_RB_BIT[i]>(); }

};


template <uint8_t no_CS>
void Pot_MCP4231<no_CS>::cfg_SPI()
{
    avr::SPI::spi_mode(0, 0);
    avr::SPI::data_order_MSB();
}


}// namespace

#endif

