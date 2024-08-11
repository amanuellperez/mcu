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

#ifndef __DEV_SDD1306_H__
#define __DEV_SDD1306_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del SDD1306	
 *
 * TORNOT
 *	Los de Adafruit meten todos los comandos en progmem. Supongo (???) que
 *	merecerá la pena hacerlo, pero como esto es experimental y ese cambio
 *	no afecta al usuario, pospongámoslo para el futuro.
 *
 * HISTORIA
 *    Manuel Perez
 *    08/08/2024 Escrito
 *
 ****************************************************************************/
#include <cstdint>

#include <mcu_concepts.h>
#include <mcu_TWI_master_ioxtream.h>

namespace dev{


// (RRR) Al usar el concept TWI_master podemos sobrecargar SDD1306_basic y
//       usar este mismo nombre tanto si lo conectamos via SPI o TWI.
//
template <Type::TWI_master TWI_master0, 
	  typename TWI_master0::Address address0 = 0x3C>
class SDD1306_basic{
public:
// Types
    using TWI_master = TWI_master0;
    using TWI = mcu::TWI_master_ioxtream<TWI_master>;
    using Address = typename TWI::Address;

// Cfg
    static constexpr uint8_t TWI_buffer_size = 10; // TODO: ajustarlo al mínimo?
    static_assert(TWI_buffer_size <= TWI_master::buffer_size,
                  "Buffer too small!!! Choose a bigger one.");
						    
    static constexpr Address address = address0;
    static_assert(address == 0x3C or address == 0x3D, "Wrong SDD1306 address");

// COMMAND TABLE
// Convenio: Esto es un traductor con lo que (salvo alguna función de más alto
// nivel) toda la clase se limita a traducir la datasheet. El usuario de esta
// clase tiene que conocer el dispositivo. Para dar flexibilidad, en lugar de
// traducir envio comandos
//	set_contrast_control(tipo);
// y que esta clase abra sea responsable de abrir o no la conexión, dejo esa
// responsabilidad al usuario, dando más flexibilidad. El usuario, si usa el
// interfaz donde pasa TWI, será el responsable de abrir la conexión TWI.
// Todos los comandos que pasan TWI tienen como precondición haber llamado a
// `open_to_send_command`
// 0. Open TWI connection
    static void prepare_to_send_command(TWI& twi);

// 1. Fundamental Command Table
    static void set_contrast_control(TWI& twi, uint8_t contrast); 

    // TODO: nombre más corto?
    static void enable_display_on_follows_RAM_content(TWI& twi);
    static void enable_display_on_ignore_RAM_content(TWI& twi);

    static void display_normal_mode(TWI& twi);
    static void display_inverse_mode(TWI& twi);

    static void set_display_on(TWI& twi);
    static void set_display_off(TWI& twi);

    // Alias 
    static void turn_on(TWI& twi) {set_display_on(twi);}
    static void turn_off(TWI& twi){set_display_off(twi);}
    static void sleep(TWI& twi)   {set_display_off(twi);}

// 2. Scrolling Command Table
// TODO

// 3. Addressing Setting Command Table
    // horizontal or vertical addressing mode
    // --------------------------------------
    // hv_mode = horizontal or vertical mode
    static void set_horizontal_addressing_mode(TWI& twi);
    static void set_vertical_addressing_mode(TWI& twi);
    static void hv_mode_column_address(TWI& twi, 
				    uint8_t column_start, uint8_t column_end);

    static void hv_mode_page_address(TWI& twi,
				    uint8_t page_start, uint8_t page_end);
			

    // page addressing mode
    // --------------------
    static void set_page_addressing_mode(TWI& twi);

    // La datasheet tiene dos funciones para definir el column address start:
    // una para el lower nibble y otra para el higher. Lo fusiono en una
    static void page_mode_column_start_address(TWI& twi, uint8_t start_address);

    // npage = from 0 to 7
    static void page_mode_page_start_address(TWI& twi, uint8_t npage);

// 4. Hardware Configuration Command Table
    static void set_display_start_line(TWI& twi, uint8_t start_line);

    static void map_COL0_to_SEG0(TWI& twi);
    static void map_COL127_to_SEG0(TWI& twi);

    // mux_ratio = from 16 to 63
    static void set_multiplex_ratio(TWI& twi, uint8_t mux_ratio);

    // Scan from COM[0] to COM[N-1]
    static void scan_from_COM0_to_COMn_1(TWI& twi);

    static void scan_from_COMn_1_to_COM0(TWI& twi);

    static void set_display_offset(TWI& twi, uint8_t offset);

    // If alternative_cfg == true, alternative COM pin cfg
    // else			   sequential COM pin cfg
    static void set_COM_pins(TWI& twi, 
			    bool alternative_cfg, bool enable_left_right_remap);

    // Este comando son dos funciones mezcladas: define divide ratio and set
    // oscillatory frequency
    // Solo es válido el lower nibble de divide_ratio y de
    // oscillatory_frequency
    static void set_display_clock(TWI& twi, 
				uint8_t divide_ratio,
				uint8_t oscillatory_frequency);

    static void set_precharge_period(TWI& twi, uint8_t phase1, uint8_t phase2);
    
    // set V[comh] deselect level to 0.65*Vcc
    static void set_Vcomh_deselect_level_065(TWI& twi);

    // set V[comh] deselect level to 0.77*Vcc
    static void set_Vcomh_deselect_level_077(TWI& twi);

    // set V[comh] deselect level to 0.83*Vcc
    static void set_Vcomh_deselect_level_083(TWI& twi);

    // Command for no operation
    // DUDA: ¿para qué sirve esto? 
    static void nop(TWI& twi);

// 2.1 Command table for Charge Bump Setting
    static void enable_charge_pump_regulator(TWI& twi);
    static void disable_charge_pump_regulator(TWI& twi);

// GDDRAM
//    static void gddram_write()

// DEBUG
    // Prueba a ver si el SDD1306 responde
    static bool probe() { return TWI_master::probe(address); }

private:
// Posibles conversaciones con el device
    static constexpr uint8_t control_byte = 0x00;   // comando
    static constexpr uint8_t data_byte    = 0x40;   // rellenamos GDDRAM
    
};


template <Type::TWI_master T, typename T::Address A>
void SDD1306_basic<T, A>::prepare_to_send_command(TWI& twi)
{ twi << control_byte; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_contrast_control(TWI& twi, uint8_t contrast)
{ twi << uint8_t{0x81} << contrast; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::enable_display_on_follows_RAM_content(TWI& twi)
{ twi << uint8_t{0xA4}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::enable_display_on_ignore_RAM_content(TWI& twi)
{ twi << uint8_t{0xA5}; }
    
template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::display_normal_mode(TWI& twi)
{ twi << uint8_t{0xA6}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::display_inverse_mode(TWI& twi)
{ twi << uint8_t{0xA7}; }




template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_display_on(TWI& twi)
{ twi << uint8_t{0xAF}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_display_off(TWI& twi)
{ twi << uint8_t{0xAE}; }



template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_horizontal_addressing_mode(TWI& twi)
{ twi << uint8_t{0x20} << uint8_t{0x00}; }



template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_vertical_addressing_mode(TWI& twi)
{ twi << uint8_t{0x20} << uint8_t{0x01}; }


template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_page_addressing_mode(TWI& twi)
{ twi << uint8_t{0x20} << uint8_t{0x02}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::hv_mode_column_address(TWI& twi, 
				    uint8_t column_start, uint8_t column_end)
{ twi << uint8_t{0x21} << column_start << column_end; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::hv_mode_page_address(TWI& twi,
				    uint8_t page_start, uint8_t page_end)
{ twi << uint8_t{0x22} << page_start << page_end; }

template <Type::TWI_master T, typename T::Address A>
void SDD1306_basic<T, A>::
page_mode_column_start_address(TWI& twi, uint8_t start_address)
{
    uint8_t lower_nibble = (start_address & 0x0F);
    uint8_t higher_nibble = (start_address >> 4) | 0x10;
    twi << lower_nibble << higher_nibble;
}

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::page_mode_page_start_address(TWI& twi, uint8_t npage)
{ 
    uint8_t page = (0x07 & npage); // Garantizo que los 5 primeros bits sean 0
    uint8_t address = (0xB0 | page);
    twi << address;
}


template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_display_start_line(TWI& twi, uint8_t start_line)
{
    uint8_t sl = (0x3F & start_line); // Garantizo que los 2 primeros bits sean 0
    uint8_t line = (0x40 | sl);
    twi << line;
}


template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::map_COL0_to_SEG0(TWI& twi)
{ twi << uint8_t{0xA0}; }


template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::map_COL127_to_SEG0(TWI& twi)
{ twi << uint8_t{0xA1}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_multiplex_ratio(TWI& twi, uint8_t mux_ratio)
{ twi << uint8_t{0xA8} << mux_ratio; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::scan_from_COM0_to_COMn_1(TWI& twi)
{ twi << uint8_t{0xC0}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::scan_from_COMn_1_to_COM0(TWI& twi)
{ twi << uint8_t{0xC8}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_display_offset(TWI& twi, uint8_t offset)
{ twi << uint8_t{0xD3} << offset; }

template <Type::TWI_master T, typename T::Address A>
void SDD1306_basic<T, A>::set_COM_pins(TWI& twi, 
			    bool alternative_cfg, bool enable_left_right_remap)
{
    uint8_t cmd = 0x02;

    if (alternative_cfg) 
	cmd |= 0x10;

    if (enable_left_right_remap)
	cmd |= 0x20;


    twi << uint8_t{0xDA} << cmd;
}


template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_display_clock(TWI& twi, 
				uint8_t divide_ratio,
				uint8_t oscillatory_frequency)
{
    uint8_t data = ((oscillatory_frequency & 0x0F) << 4) |
		   (divide_ratio & 0x0F) ;
    twi << uint8_t{0xD5} << data;
}



template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::
	set_precharge_period(TWI& twi, uint8_t phase1, uint8_t phase2)
{
    uint8_t data = ((phase2 & 0x0F) << 4) |
		   (phase1 & 0x0F) ;

    twi << uint8_t{0xD9} << data;

}



template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_Vcomh_deselect_level_065(TWI& twi)
{ twi << uint8_t{0xDB} << uint8_t{0x00}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_Vcomh_deselect_level_077(TWI& twi)
{ twi << uint8_t{0xDB} << uint8_t{0x20}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::set_Vcomh_deselect_level_083(TWI& twi)
{ twi << uint8_t{0xDB} << uint8_t{0x30}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::nop(TWI& twi)
{ twi << uint8_t{0xE3};}

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::enable_charge_pump_regulator(TWI& twi)
{ twi << uint8_t{0x8D} << uint8_t{0x14}; }

template <Type::TWI_master T, typename T::Address A>
inline
void SDD1306_basic<T, A>::disable_charge_pump_regulator(TWI& twi)
{ twi << uint8_t{0x8D} << uint8_t{0x10}; }

}// namespace
 


#endif


