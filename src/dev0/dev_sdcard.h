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

#ifndef __DEV_SDCARD_H__
#define __DEV_SDCARD_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del interfaz SPI de la SD card.
 *
 * HISTORIA
 *    Manuel Perez
 *    11/02/2023 Escrito.
 *
 ****************************************************************************/
#include <avr_SPI_basic.h>

namespace dev{

// Clase por defecto para seleccionar la SDCard
// El `Selector_SPI` lo define en general el cliente. Es el que diseña el
// hardware de la aplicación el que sabe cómo va a conectar los diferentes
// dispositivos SPI. `SDCard_select` es el selector más básico: usa un pin
// (que por defecto será el que SPI::CS_pin_number).
// TODO: Generalizar esta clase. Se puede meter en mcu_SPI.h ya que no depende
// del micro concreto a usar ni de los dispositivos. Se basa en un pin.
template <typename Output_pin>
class SDCard_select{
public:
    static void init() {Output_pin::as_output();}

    static void select()
    { Output_pin::write_zero(); }

    static void deselect()
    {Output_pin::write_one();}

    SDCard_select() {select();}
    ~SDCard_select() {deselect();}
};

// Documentos que uso:
//	"Part 1 - physical layer simplified specivication ver9.0"
//
// TODO: en la respuesta del cmd0 hace un bucle if (res == 0xFF )
//  y sale transcurrido un tiempo. Según el foro de arduino algunas SD cards
//  tiene el MISO en open collector during the init sequence. Por eso es
//  fundamental que se salga del bucle y se siga. Con todo CONFIRMAR ESTO!!!
//
//  El interfaz es static ya que solo vamos a tener una única SDCard que se
//  selecciona con el `Select_chip`. Esta clase es única para cada SDCard que
//  conectemos.
class SDCard{
public:
// TODO: parametrizar todo esto
    using Micro	      = mcu::Micro;
    using SPI	      = mcu::SPI_master;
    using Select_chip = SDCard_select<mcu::Output_pin<SPI::CS_pin_number>>;
    
// Commands
    // Devuelve response R1. Si todo va bien r1_is_in_idle_state(R1) == true
    static uint8_t go_idle_state()
    {
	SPI_cfg_init();
	initialization_sequence();

	Select_chip select{};

	send_cmd0();
	return read_R1();
    }


    // Significado de los bits de R1
    // Estas funciones no son más que meros traductores. ¿Merece la pena
    // implementarlo como struct? ¿cómo enum? 
    // Notación: los _flag miran el bit correspondiente. 
    static bool r1_is_in_idle_state(uint8_t r1) 
    { return r1 == 0x01;}

    static bool r1_in_idle_state_flag(uint8_t r1) 
    { return atd::is_one_bit<0>::of(r1);}

    static bool r1_erase_reset_flag(uint8_t r1)   
    { return atd::is_one_bit<1>::of(r1);}

    static bool r1_ilegal_command_flag(uint8_t r1)
    { return atd::is_one_bit<2>::of(r1);}

    static bool r1_communication_CRC_error_flag(uint8_t r1)
    { return atd::is_one_bit<3>::of(r1);}

    static bool r1_erase_sequence_error_flag(uint8_t r1)
    { return atd::is_one_bit<4>::of(r1);}

    static bool r1_address_error_flag(uint8_t r1)  
    { return atd::is_one_bit<5>::of(r1);}

    static bool r1_parameter_error_flag(uint8_t r1)
    { return atd::is_one_bit<6>::of(r1);}

private:
// CFG
    // DUDA: ¿qué valor para max_retries? Los de Arduino indican que hay
    // mínimo de de 1 a 8 bytes antes de que responda la SD card. No he
    // encontrado en la especificación del SDCard los tiempos de respuesta
    // (???). Arduino usa 10, usemos 10 de momento.
    static constexpr uint8_t max_retries = 10;
    static constexpr uint32_t init_frequency = 250'000; // 250 kHz
							

    // 7.3.1.1@physical_layer: 
    //   All the SD Memory Card commands are 6 bytes long. 
    //   The command transmission always starts with the left most bit 
    //   of the bit string corresponding to the command codeword.
    //   Esto es: MSB first en los comandos.
    //
    // 7.3.2@physical_layer: All responses are transmitted MSB first.
    static void SPI_cfg__()
    {
	SPI::cfg_pins();
	SPI::spi_mode(0,0);
	SPI::data_order_MSB();
    }

    static void SPI_cfg_init()
    {
	SPI_cfg__();
	SPI::clock_frequency_in_hz<init_frequency>();
	SPI::on();
    }


    
    // 6.4.1@physical_layer
    // + A device shall be ready to accept the first command within 1ms 
    //   from detecting VDD min.  
    // + Device may use up to 74 clocks for preparation before receiving 
    //   the first command.
    // + In case of SPI mode, CS shall be held to high during 74 clock cycles.
    static void initialization_sequence()
    {
	Select_chip::deselect();    // CS shall be held high!!!

	Micro::wait_ms(1); // power up time

	// send 80 (> 74) clock cycles to synchronize 
	for (uint8_t i = 0; i < 10; ++i)
	    SPI::write(0xFF);


	// ¿por qué hay que enviar un byte cuando se ha deseleccionado 
	// la SD?
	// TODO: si no funciona descomentar esto
	// SPI::write(0xFF);
    }

    // 7.2.2@physical_layer
    // cmd0 es constante = 0x40 00 00 00 00 95
    static void send_cmd0() { send(0x40, 0x00, 0x95); }

    // 7.3.1@physical_layer
    // Todos los comandos son de 6 bytes. 
    // bits [47:46] = '10'
    // bits [45:40] = cmd (6 bits)
    // bits [39:8]  = arg (32 bits)
    // bits [7:1]   = CRC7
    // bit  [0]     = '1'
    //
    // Básicamente: '10cmd'- arg - 'crc1'
    //
    // CUIDADO: uso el (arg & 0x00FF0000) >> 16 ... para no tener que pensar
    // en la endianess.
    //
    // 7.2.2@physical_layer:
    // The SPI interface is initialized in the CRC OFF mode in default. El
    // único comando que requiere CRC es el CMD0, los demás no es necesario
    // (salvo que configuremos CRC ON). Salvo en el CMD0 el valor del CRC
    // puede ser cualquiera (por eso por defecto 0x00).
    static void send(uint8_t cmd, uint32_t arg, uint8_t crc = 0x00)
    {
	SPI::write(cmd | 0x40);	

	SPI::write(static_cast<uint8_t>((arg & 0xFF000000) >> 24));
	SPI::write(static_cast<uint8_t>((arg & 0x00FF0000) >> 16));
	SPI::write(static_cast<uint8_t>((arg & 0x0000FF00) >> 8));
	SPI::write(static_cast<uint8_t>((arg & 0x000000FF)));

	SPI::write(crc | 0x01);
    }

    // 7.3.1.3@physical_layer
    //   If a command does not require an argument, the value of 
    //   this field should be set to zero.
    static void send(uint8_t cmd)
    {
	SPI::write(cmd | 0x40);	   

	SPI::write(0x00);
	SPI::write(0x00);
	SPI::write(0x00);
	SPI::write(0x00);

	SPI::write(0x01);
    }

    // 7.2.8@physical_layer
    // + in the SPI mode the card will always respond to a command.
    // + A command may be rejected in, entre otros casos, "it is sent while
    //   the card is in Busy".
    //
    //   Luego si la SDCard esta ocupada responderá con error.
    //
    // 7.3.2.1@physical_layer
    //   +  El bit '7' tiene que ser igual a '0'.
    //
    //   Pero no indica que si es igual a '1' sea un error (aunque sería lo de
    //   esperar). Esto puede que lo aclare el punto 5.2.3.1@sdcard-scandisk, 
    //   indica:
    //
    //   "the MSB is always set to zero and the other bits are error 
    //    indications. A ‘1’ signals error." 
    //    
    //    El problema es que no queda claro si cuando el bit 7 de R1 es 1 
    //    quiere decir que hay un error o si se refiere a los demás flags de
    //    R1.
    //   
    // ¿Cómo implementar un `time_out`? 
    //	Intentandolo `max_retries` como mucho.
    //
    //	Return: R1 si todo va bien (bit<7>::of(R1) == 0)
    //		0xFF en caso de error.
    static uint8_t read_R1()
    {
	// SPI::read(); //TODO: descomentar si falla. Los de Arduino descartan
	// el primer byte "to avoid MISO pull-up problem" (???) Aunque tal
	// como está conectado SPI al tener la SD card seleccionada no tendría
	// que existir ningún problema (???)
	for (uint8_t i = 0; i < max_retries; ++i){

	    uint8_t r1 = SPI::read();

	    if (atd::is_zero_bit<7>::of(r1))
		return r1;
	}

	return 0xFF;
    }




};


}// namespace

#endif


