// Copyright (C) 2019-2021 Manuel Perez 
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

#ifndef __DEV_POT_MCP4231_H__
#define __DEV_POT_MCP4231_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Driver del potenciómetro MCP4231
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	19/10/2019 v0.0: Escrito.
 *	17/04/2021       Reestructurado.
 *
 ****************************************************************************/
#include <avr_pin.h>
#include <avr_SPI_basic.h>
#include "not_generic.h"

#include <atd_bit.h>
#include <atd_static.h>


namespace dev{

// ---------------------------------------------------------------------------
// datasheet 4.2.1.2
// mem <--> struct
struct __MCP4231_TCON{
//    uint8_t _resistor0_shutdown : 1;
//    uint8_t _resistor0_terminalA_connect : 1;
//    uint8_t _resistor0_terminalW_connect : 1;
//    uint8_t _resistor0_terminalB_connect : 1;
//
//    uint8_t _resistor1_shutdown : 1;
//    uint8_t _resistor1_terminalA_connect : 1;
//    uint8_t _resistor1_terminalW_connect : 1;
//    uint8_t _resistor1_terminalB_connect : 1;
//
//    __MCP4231_TCON() = default;
//    __MCP4231_TCON(std::byte tcon) {to_struct(std::to_integer<uint8_t>(tcon));}
//    std::byte to_mem() const;


// resistor 0
//  CUIDADO: esta shutdown cuando este bit es 0!!!
    static void resistor0_shutdown(std::byte& tcon, bool on)
    { mask_R0HW(tcon) = std::byte{!on};}

    static bool resistor0_shutdown(std::byte& tcon)
    { return !atd::to_bool(mask_R0HW(tcon)); }

    static void resistor0_terminalA_connect(std::byte& tcon, bool on)
    { mask_R0A(tcon) = std::byte{on}; }

    static bool resistor0_terminalA_connect(std::byte& tcon)
    { return atd::to_bool(mask_R0A(tcon));}

    static void resistor0_terminalW_connect(std::byte& tcon, bool on)
    { mask_R0W(tcon) = std::byte{on}; }

    static bool resistor0_terminalW_connect(std::byte& tcon)
    { return atd::to_bool(mask_R0W(tcon));}

    static void resistor0_terminalB_connect(std::byte& tcon, bool on)
    { mask_R0B(tcon) = std::byte{on}; }

    static bool resistor0_terminalB_connect(std::byte& tcon)
    { return atd::to_bool(mask_R0B(tcon));}


// resistor 1
//  CUIDADO: esta shutdown cuando este bit es 0!!!
    static void resistor1_shutdown(std::byte& tcon, bool on)
    { mask_R1HW(tcon) = std::byte{!on};}

    static bool resistor1_shutdown(std::byte& tcon)
    { return !atd::to_bool(mask_R1HW(tcon)); }

    static void resistor1_terminalA_connect(std::byte& tcon, bool on)
    { mask_R1A(tcon) = std::byte{on}; }

    static bool resistor1_terminalA_connect(std::byte& tcon)
    { return atd::to_bool(mask_R1A(tcon));}

    static void resistor1_terminalW_connect(std::byte& tcon, bool on)
    { mask_R1W(tcon) = std::byte{on}; }

    static bool resistor1_terminalW_connect(std::byte& tcon)
    { return atd::to_bool(mask_R1W(tcon));}

    static void resistor1_terminalB_connect(std::byte& tcon, bool on)
    { mask_R1B(tcon) = std::byte{on}; }

    static bool resistor1_terminalB_connect(std::byte& tcon)
    { return atd::to_bool(mask_R1B(tcon));}


private:
    static constexpr atd::Range_bitmask<7, 7, std::byte> mask_R1HW{};
    static constexpr atd::Range_bitmask<6, 6, std::byte> mask_R1A{};
    static constexpr atd::Range_bitmask<5, 5, std::byte> mask_R1W{};
    static constexpr atd::Range_bitmask<4, 4, std::byte> mask_R1B{};

    static constexpr atd::Range_bitmask<3, 3, std::byte> mask_R0HW{};
    static constexpr atd::Range_bitmask<2, 2, std::byte> mask_R0A{};
    static constexpr atd::Range_bitmask<1, 1, std::byte> mask_R0W{};
    static constexpr atd::Range_bitmask<0, 0, std::byte> mask_R0B{};


//    void to_struct(uint8_t tcon);
};

// DUDA: merece la pena hacer esto? Ocupa mucho espacio? Facil de usar?
//void __MCP4231_TCON::to_struct(uint8_t tcon)
//{
//    _resistor0_shutdown  = !atd::to_bool(mask_R0HW(tcon));
//    _resistor0_terminalA_connect = atd::to_bool(mask_R0A(tcon));
//    _resistor0_terminalW_connect = atd::to_bool(mask_R0B(tcon)); 
//    _resistor0_terminalB_connect = atd::to_bool(mask_R0W(tcon)); 
//
//    _resistor1_shutdown  = !atd::to_bool(mask_R1HW(tcon));
//    _resistor1_terminalA_connect = atd::to_bool(mask_R1A(tcon));
//    _resistor1_terminalW_connect = atd::to_bool(mask_R1B(tcon)); 
//    _resistor1_terminalB_connect = atd::to_bool(mask_R1W(tcon)); 
//}
//
//
//std::byte __MCP4231_TCON::to_mem() const
//{
//    std::byte tcon{};
//
//    mask_R0HW(tcon) = std::byte{(_resistor0_shutdown? 0: 1)};
//    mask_R0A(tcon) = std::byte{_resistor0_terminalA_connect};
//    mask_R0W(tcon) = std::byte{_resistor0_terminalW_connect};
//    mask_R0B(tcon) = std::byte{_resistor0_terminalB_connect};
//
//    mask_R1HW(tcon) = std::byte{(_resistor1_shutdown? 0: 1)};
//    mask_R1A(tcon) = std::byte{_resistor1_terminalA_connect};
//    mask_R1W(tcon) = std::byte{_resistor1_terminalW_connect};
//    mask_R1B(tcon) = std::byte{_resistor1_terminalB_connect};
//
//
//    return std::byte{tcon};
//}
//

/*!
 *  \brief  Traductor del MCP4231
 *
 *  Esta clase se limita a traducir la datasheet a código.
 *
 *  Acceso típico al dispositivo:
 *
 *  Pot_MCP4231_basic::SPI_cfg();
 *  select();
 *  Pot_MCP4231_basic::cmd1
 *  Pot_MCP4231_basic::cmd2
 *  ...
 *  deselect();
 *
 */
// TODO: añadir pin shutdown. Despues, sacar todo el código fuente de dentro
// de la clase (ordenarlo un poco todo).
class Pot_MCP4231_basic{
public:
// Types
    using SPI = not_generic::SPI_master;
    using TCON = __MCP4231_TCON;

// Configuration
    // TODO: comprar otros potenciómetros de 8 bits y usar esta misma clase
    // para tanto los de 7 como los de 8.
    constexpr static uint8_t nbits = 7; // Pueden ser 7 ó 8 bits

    constexpr static uint16_t wiper_max_value()
    {return 128u * (nbits - 7u) + 128u;} // valor[7] = 128, valor[8] = 256


    // Antes que acceder al potenciometro es necesario configurar el SPI.
    /// Configuramos el SPI para que se comunique con el potenciómetro.
    /// Recordar haber inicializado correctamente el SPI antes.
    /// Es responsabilidad del usuario el inicializar el SPI y configurarlo
    /// antes de acceder al potenciómetro.
    static void SPI_cfg();


// Wiper Registers
// ---------------
    /// Escribe el valor x en el potenciometro npot ( = 0/1)
    /// El valor de la resistencia será: R = x*Rab/wiper_max_value()
    /// siendo Rab el valor máximo.
    template <uint8_t npot>
    static void wiper_write_data(uint16_t x)
    {
        if (x > wiper_max_value())
            x = wiper_max_value();

        write_cmd(cmd_wiper_write_data::at<npot>, x);
    }

    template <uint8_t npot>
    static uint16_t wiper_read_data()
    { return read_cmd16(cmd_wiper_read_data::at<npot>); }

    template <uint8_t npot>
    static void wiper_increment()
    { write_cmd(cmd_wiper_increment::at<npot>); }

    template <uint8_t npot>
    static void wiper_decrement()
    { write_cmd(cmd_wiper_decrement::at<npot>); }


// Status Register
// ---------------
    static std::byte status_register()
    { return read_cmd8(status_register_read_data); }

    // status_register::SHDN = hardware shutdown bit
    // static bool is_hardware_shutdown() <-- de alto nivel
    static bool status_register_hardware_shutdown_pin()
    { return atd::is_one_bit<status_register_no_shdn_bit>::of(status_register()); }

// TCON Register
// -------------
    static void TCON_register(std::byte x)
    { write_cmd(tcon_register_write_data, std::to_integer<uint16_t>(x)); }

    static std::byte TCON_register()
    { return read_cmd8(tcon_register_read_data); }



private:
// Instruction set (table 7-2 datasheet)
    // Wipers Registers
    using cmd_wiper_write_data = atd::static_array<uint16_t, 0x0000, 0x1000>;
    using cmd_wiper_read_data  = atd::static_array<uint8_t, 0x0C, 0x1C>;
    using cmd_wiper_increment  = atd::static_array<uint8_t, 0x04, 0x14>;
    using cmd_wiper_decrement  = atd::static_array<uint8_t, 0x08, 0x18>;

    // TCON Register
    constexpr static uint16_t tcon_register_write_data = 0x4000;
    constexpr static uint8_t tcon_register_read_data   = 0x4C;

    // Status Register
    constexpr static uint8_t status_register_read_data   = 0x5C;

    constexpr static uint8_t status_register_no_shdn_bit = 1;

    // Bit de error
    constexpr static uint8_t CMDERR_BIT = 7;    // bit que ocupa CMDERR





// Funciones de ayuda
// ------------------
    // return true = OK, false = ERROR.
    static bool SPI_write(uint16_t x) 
    { 
	std::byte res = SPI::write(static_cast<std::byte>(x >> 8)); 

	if (atd::is_zero_bit<CMDERR_BIT>::of(res))
	    return false;
	
	// si ha fallado ¿puedo ahorrar esta llamada?
	SPI::write(static_cast<std::byte>(x)); 

	return true;
    }

    // Operaciones básicas 
    // -------------------
    // Lee un comando del potenciometro
    static std::byte read_cmd8(uint8_t cmd)
    {
	std::byte res = SPI::write(std::byte{cmd});

	if (atd::is_zero_bit<CMDERR_BIT>::of(res))
	    return std::byte{0};

	return SPI::read();
    }


    static uint16_t read_cmd16(uint8_t cmd)
    {
	std::byte res1 = SPI::write(std::byte{cmd});

	if (atd::is_zero_bit<CMDERR_BIT>::of(res1))
	    return uint16_t{0};

	std::byte res2 = SPI::read();

	return atd::concat_bytes<uint16_t>(res1 & std::byte{0x01}, res2);
    }

    static void write_cmd(uint8_t cmd)
    {
	SPI::write(std::byte{cmd});
    }

    // precondition: los bits 9-15 de data == 0.
    // El comando lo definimos de tal manera que sus 9 primeros bits sean 0.
    static void write_cmd(uint16_t cmd, uint16_t data)
    {
	SPI_write(cmd | data);
    }

};


void Pot_MCP4231_basic::SPI_cfg()
{
    SPI::spi_mode(0, 0);
    SPI::data_order_MSB();
}


/*!
 *  \brief  Añadimos la gestión automática de SPI_cfg
 *
 *  Esta clase simplifica la interacción con el dispositivo. 
 *
 */
// ¿Cómo se selecciona el dispositivo? ¿Directamente mediante un pin?
// ¿Mediante un registro? ¿...? Eso lo decide el cliente, por eso la clase
// está parametrizada por Select.
template <typename Select>
class Pot_MCP4231 : public Pot_MCP4231_basic{
public:
    using Base = Pot_MCP4231_basic;

// Wiper Registers
// ---------------
    /// Escribe el valor x en el potenciometro npot ( = 0/1)
    /// El valor de la resistencia será: R = x*Rab/wiper_max_value()
    /// siendo Rab el valor máximo.
    template <uint8_t npot>
    static void wiper_write_data(uint16_t x)
    {
	SPI_cfg();
	Select select;
	Base::wiper_write_data<npot>(x);
    }

    template <uint8_t npot>
    static uint16_t wiper_read_data()
    {
	SPI_cfg();
	Select select;
	return Base::wiper_read_data<npot>();
    }

    template <uint8_t npot>
    static void wiper_increment()
    {
	SPI_cfg();
	Select select;
	Base::wiper_increment<npot>();
    }

    template <uint8_t npot>
    static void wiper_decrement()
    {
	SPI_cfg();
	Select select;
	Base::wiper_decrement<npot>();
    }


// Status Register
    static std::byte status_register()
    {
	SPI_cfg();
	Select select;
	return Base::status_register();
    }


// TCON register
    static void TCON_register(std::byte x)
    {
	SPI_cfg();
	Select select;
	Base::TCON_register(x);
    }

    static std::byte TCON_register()
    {
	SPI_cfg();
	Select select;
	return Base::TCON_register();
    }


// State
// -----
//
//    /// Forces (or not) resistor 0 into the "shutdown" configuration
//    static void resistor0_shutdown(bool on) { resistor_shutdown<0>(on);}
//
//    /// Is resistor 0 into the "shutdown" configuration?
//    static bool resistor0_shutdown() { return resistor_shutdown<0>();}

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


//    /// Forces (or not) resistor 1 into the "shutdown" configuration
//    static void resistor1_shutdown(bool on) { resistor_shutdown<1>(on);}
//
//    /// Is resistor 1 into the "shutdown" configuration?
//    static bool resistor1_shutdown() { return resistor_shutdown<1>();}

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


//    template <uint8_t i>
//    static void resistor_shutdown(bool on)
//    { TCON_register_write_bit<TCON_REGISTER_RHW_BIT[i]>(!on); }
//
//    template <uint8_t i>
//    static bool resistor_shutdown() 
//    { return !TCON_register_bit<TCON_REGISTER_RHW_BIT[i]>(); }
//
//    template <uint8_t i>
//    static void resistor_connect_terminal_A(bool on)
//    { TCON_register_write_bit<TCON_REGISTER_RA_BIT[i]>(on); }
//
//    template <uint8_t i>
//    static bool resistor_connect_terminal_A() 
//    { return TCON_register_bit<TCON_REGISTER_RA_BIT[i]>(); }
//
//    template <uint8_t i>
//    static void resistor_connect_wiper(bool on)
//    { TCON_register_write_bit<TCON_REGISTER_RW_BIT[i]>(on); }
//
//    template <uint8_t i>
//    static bool resistor_connect_wiper() 
//    { return TCON_register_bit<TCON_REGISTER_RW_BIT[i]>(); }
//
//    template <uint8_t i>
//    static void resistor_connect_terminal_B(bool on)
//    { TCON_register_write_bit<TCON_REGISTER_RB_BIT[i]>(on); }
//
//    template <uint8_t i>
//    static bool resistor_connect_terminal_B() 
//    { return TCON_register_bit<TCON_REGISTER_RB_BIT[i]>(); }
};
}// namespace

#endif

