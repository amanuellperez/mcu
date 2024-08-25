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
 * COMENTARIOS IMPLEMENTACIÓN
 *	Observar que estoy usando muchos números mágicos. ¿Por qué?
 *	Estoy implementando un traductor: voy leyendo la datasheet y la
 *	traduzco en código. 
 *
 *	Ejemplo: al escrribir la función write() la datasheet pone que tengo
 *	que enviar el start token que es 0xFE. Usando números mágicos el
 *	código queda:
 *	    SPI::write(0xFE);
 *	que corresponde a lo que leo en la datasheet.
 *
 *	Si uso constantes quedará
 *	    SPI::write(write_start_token);
 *	y para comprobar que está bien tendré que ir a ver la definición de
 *	write_start_token lo cual es incómodo.
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    11/02/2023 Escrito.
 *
 ****************************************************************************/
#include <avr_SPI_basic.h>

#include <span>

/***************************************************************************
 *  SDCARD
 *
 *  * La memoria en las tarjetas modernas están paginadas en páginas de 512
 *    bloques.
 *  * La dirección que se le pasa a la tarjeta es una de esas páginas, siendo
 *    imposible (escribir) un byte de forma individual. Se escribe toda la
 *    página.
 *
 ***************************************************************************/
namespace dev{

// Clase por defecto para seleccionar la SDCard_basic
// El `Selector_SPI` lo define en general el cliente. Es el que diseña el
// hardware de la aplicación el que sabe cómo va a conectar los diferentes
// dispositivos SPI. `SDCard_select` es el selector más básico: usa un pin
// (que por defecto será el que SPI::noCS_pin_number).
// TODO: Generalizar esta clase. Se puede meter en mcu_SPI.h ya que no depende
// del micro concreto a usar ni de los dispositivos. Se basa en un pin.
template <typename Output_pin, typename SPI>
class SDCard_select{
public:
    static void init() 
    {
	Output_pin::as_output();
	deselect();
    }

    // En general el usuario no debería de llamar a esta función.
    // Sin embargo, SDCard::initialization_sequence necesita garantizar que
    // el chip no esté seleccionado (¿podría omitir eso?) Por ello dejo
    // público esta función.
    static void deselect()
    {
//	SPI::write(0xFF);   // hacemos MOSI = 1
	Output_pin::write_one();
//	SPI::write(0xFF);   // hacemos MOSI = 1
    }

    // TODO: revisar la lógica, esto está raro... Usar mcu::SPI_pin_selector?
    SDCard_select() 
    {
	selected = Output_pin::is_one();
	select();
    }

    ~SDCard_select() 
    {
	if (!selected)
	    deselect();
    }

private:
    // selected permite llamar de forma anidada SDCard_select{}
    // Esto evitará muchos errores a la hora de mantener el código. Si no se
    // hace así, hay que llevar un control total de cuándo se ha seleccionado
    // o no el Chip. No seleccionarlo o deseleccionarlo generaría un error que
    // seguro lleva bastante tiempo encontrar. 
    bool selected = false;

    static void select()
    { 
//	SPI::write(0xFF);   // hacemos MOSI = 1
	Output_pin::write_zero(); 
//	SPI::write(0xFF);   // hacemos MOSI = 1
    }
};


namespace SDCard_types{

enum class Init_return{ cmd0_fail, 
			cmd8_crc_error, voltage_mismatch, cmd8_echo_fail,
			acmd41_in_idle_state, // retry sd_send_op_cond()
			card_no_power_up, // retry read_ocr()
			init_SDSC_card_ok, 
			init_SDHC_or_SDXC_ok
};


// Response R1
// -----------
// Traductor del mapa de bits R1 en funciones
struct R1_response{
    uint8_t data;

// State
    bool is_ready() const
    { return data == 0x00;} 

    bool is_in_idle_state()  const
    { return data == 0x01;}

    // data contiene un valor válido? 
    bool is_valid() const
    { return atd::is_zero_bit<7>::of(data);}

    bool is_illegal_command() const // así habla figure 7-2
    { return !is_valid(); }

    bool is_an_error() const
    { return data > 0x01; }

    // ok? == todo ha ido bien? Se ha recibido R1 y recibimos ok?
    bool ok() const {return !is_an_error();}

// Flags
//  DUDA: quedaría mejor llamarla in_idle_state(), pero sería muy facil
//  confundirla con is_in_idle_state(). Por eso opto por dejar el _flag.
//  ¿Nombre?
    bool in_idle_state_flag() const
    { return atd::is_one_bit<0>::of(data);}

    bool erase_reset_flag() const
    { return atd::is_one_bit<1>::of(data);}

    bool illegal_command_flag() const
    { return atd::is_one_bit<2>::of(data);}

    bool communication_CRC_error_flag() const
    { return atd::is_one_bit<3>::of(data);}

    bool erase_sequence_error_flag() const
    { return atd::is_one_bit<4>::of(data);}

    bool address_error_flag() const
    { return atd::is_one_bit<5>::of(data);}

    bool parameter_error_flag() const
    { return atd::is_one_bit<6>::of(data);}

// Helpers
    static uint8_t invalid_value()
    { return 0xFF; }
};

// 7.3.2.3@physical_layer
// Traductor del mapa de bits R1 en funciones
struct R2_response{ 
// Data
    R1_response r1;
    uint8_t r2;

// Helper
    template <typename SPI>
    void read(){
	r2 = SPI::read();
    }

// Traducimos los flags
    bool ok() const 
    { return r2 == 0x00; }

    bool card_is_locked() const
    { return atd::is_one_bit<0>::of(r2); }

    bool write_protect_erase_skip() const
    { return atd::is_one_bit<1>::of(r2); }

    bool error() const
    { return atd::is_one_bit<2>::of(r2); }

    bool CC_error() const
    { return atd::is_one_bit<3>::of(r2); }

    bool card_ECC_failed() const
    { return atd::is_one_bit<4>::of(r2); }

    bool write_protect_violation() const
    { return atd::is_one_bit<5>::of(r2); }

    bool erase_param() const
    { return atd::is_one_bit<6>::of(r2); }

    bool out_of_range() const
    { return atd::is_one_bit<7>::of(r2); }

};

// Referencias
//	5.1@physical_layer
//	7.3.2.4@physical_layer
//
// TODO: de momento no implemento los bits UHS... 
struct R3_response{ 
    enum class Type{ SDSC, SDHC_or_SDXC };

// Data
    R1_response r1;
    uint8_t OCR[4];

// Helper
    template <typename SPI>
    void read(){
	OCR[0] = SPI::read();	// MSByte
	OCR[1] = SPI::read();	
	OCR[2] = SPI::read();
	OCR[3] = SPI::read();
    }


// Traducimos los flags
    bool card_has_finished_power_up() const
    { return atd::is_one_bit<7>::of(OCR[0]); }

    // Sólo válido si card_has_finished_power_up() == true
    bool card_capacity_status() const
    { return atd::is_one_bit<6>::of(OCR[0]); }

    // suministro este sinónimo de `card_capacity_status`, tiene más
    // significado. Este es el traductor: traduzco bits a significado.
    Type card_type() const
    { 
	if (card_capacity_status())
	    return Type::SDHC_or_SDXC;

	else
	    return Type::SDSC;
    }


    // supports 3.5-3.6V ???
    bool support_3_5_3_6V() const
    { return atd::is_one_bit<7>::of(OCR[1]); }

    bool support_3_4_3_5V() const
    { return atd::is_one_bit<6>::of(OCR[1]); }

    bool support_3_3_3_4V() const
    { return atd::is_one_bit<5>::of(OCR[1]); }

    bool support_3_2_3_3V() const
    { return atd::is_one_bit<4>::of(OCR[1]); }

    bool support_3_1_3_2V() const
    { return atd::is_one_bit<3>::of(OCR[1]); }

    bool support_3_0_3_1V() const
    { return atd::is_one_bit<2>::of(OCR[1]); }

    bool support_2_9_3_0V() const
    { return atd::is_one_bit<1>::of(OCR[1]); }

    bool support_2_8_2_9V() const
    { return atd::is_one_bit<0>::of(OCR[1]); }

    bool support_2_7_2_8V() const
    { return atd::is_one_bit<7>::of(OCR[2]); }
};


// Referencias:
//  7.3.2.6@physical_layer
//	command version : bits[28,31]
//	voltage accepted: bits[8, 11]
//	pattern         : bits[0, 7]
//
//  4.3.13@physical_layer
//	Table 4-18 muestra los valores posibles del `voltage accepted`
struct R7_response{ 
// Data
    R1_response r1;
    uint8_t data[4];

// Helper
    template <typename SPI>
    void read(){
	data[0] = SPI::read();		
	data[1] = SPI::read();	
	data[2] = SPI::read();
	data[3] = SPI::read();
    }

    enum class Voltage{
	not_defined, 
	v2_7_to_3_6_V, // de 2.7 a 3.6 V
	low_voltage, 
	reserved
    };

// Traducimos los flags
    uint8_t version() const {return ((data[0] & 0xF0) >> 4);}
    uint8_t pattern() const {return data[3];}
    uint8_t VCA()     const {return data[2] & 0x0F;}

    // 4.3.13/table 4-18
    Voltage voltage() const {
	switch ((data[2] & 0x0F)){
	    break; case 0x01: return Voltage::v2_7_to_3_6_V;
	    break; case 0x02: return Voltage::low_voltage;
	    break; case 0x04: return Voltage::reserved;
	    break; case 0x08: return Voltage::reserved;
	    break; default  : return Voltage::not_defined;
	}
    }

};


class Read_return{
public:
    using R1 = R1_response;

    bool ok() const { return (!r1_.is_an_error() and det_ == 0); }
    bool timeout() const {return atd::is_one_bit<7>::of(det_);}

    bool r1_fail() const { return r1_.is_an_error(); }
    R1 r1() const {return r1_;}

// Posibles errores del data error token(7.3.3.3@physical_layer)
    bool error() const {return atd::is_one_bit<0>::of(det_);}
    bool CC_error() const {return atd::is_one_bit<1>::of(det_);}
    bool card_ECC_failed() const {return atd::is_one_bit<2>::of(det_);}
    bool out_of_range() const {return atd::is_one_bit<3>::of(det_);}


// Constructors
    static Read_return r1_error(const R1& r1) 
    { 
	Read_return err;
	err.r1_ = r1; 
	return err;
    }

    static Read_return time_out_error() 
    { 
	Read_return err;
	atd::write_bit<7>::to<1>::in(err.det_); 
	return err;
    }

    static Read_return data_error_token(uint8_t det) 
    {
	Read_return err;
	err.det_ = det;
	return err;
    }

private:
// data
    R1 r1_ {0};
    uint8_t det_{0}; // data_error_token
		 // == 0x00 si no hay error
		 // uso el bit[7] para marcar timeout
		 // resto de bits mirar 7.3.3.3@physical_layer
		 
};

// 7.3.3.1@physical_layer
//	Valores posibles: xxx0___1 = válido
//	Valores que puede tomar:
//	    xxx00101 : data accepted
//	    xxx01011 : data rejected due to CRC error
//	    xxx01101 : data rejected due to write error
//
//	Internamente garantizo que los xxx sean 000. De tal manera que
//	    xxx00101 -> 0x05 (accepted)
//	    xxx01011 -> 0x0B (CRC error)
//	    xxx01101 -> 0x0D (write error)
//
//	y añado:
//	    0xFF -> no válido
//	    0xF0 -> timeout
//		
class Data_response_token{
public:
// Data
    // data contiene un valor válido? 
    bool is_valid() const { return data != 0xFF;}

    bool data_accepted() const { return data == 0x05; }
    bool data_rejected_CRC_error() const {return data == 0x0B; }
    bool data_rejected_write_error() const {return data == 0x0D; }
    bool timeout() const {return data == 0xF0; }

    bool ok() const {return data_accepted(); }
    bool is_an_error() const {return !ok(); }


// Constructors
    static Data_response_token valid(uint8_t x)
    {	
	x &= 0x0F;
	return Data_response_token{x}; 
    }

    static Data_response_token timeout_error() 
    { return Data_response_token{0xF0}; }

    static Data_response_token invalid()
    { return Data_response_token{0xFF};}

// Consulta
    static bool is_valid(uint8_t x)
    { return (atd::is_zero_bit<4>::of(x) and atd::is_one_bit<0>::of(x)); }


private:
    Data_response_token(uint8_t d) : data{d} { }

    uint8_t data;
};


class Write_return{
public:
    using R1 = R1_response;
    using R2 = R2_response;

    // todo ha ido bien?
    bool ok() const { return (r1_.ok() and dt_.ok() and r2_.ok()); }

    // si ha habido algun error consultamos lo ocurrido:
    R1 r1() const {return r1_;}
    Data_response_token data_response_token() const {return dt_;}
    R2 r2() const {return r2_;}


// Constructors (internos)
    static Write_return r1_error(const R1& r1)
    { 
	Write_return err;
	err.r1_ = r1; 
	return err;
    }

    static Write_return dt_error(const R1& r1, const Data_response_token& dt)
    { 
	Write_return err;
	err.r1_ = r1;  
	err.dt_ = dt;
	return err;
    }

    // este será el caso en el que todo vaya bien (o falle R2)
    static Write_return send_status(const R1& r1, 
				    const Data_response_token& dt,
				    const R2& r2)
    { 
	Write_return err;
	err.r1_ = r1;  
	err.dt_ = dt;
	err.r2_ = r2;
	
	return err;
    }


    
private:
// data
    R1 r1_ {0};
    Data_response_token dt_ {Data_response_token::invalid()};
    R2 r2_ {0};

};

} // namespace SDCard_types

// Configuración de SDCard_basic
template <typename Micro_t,	    // ¿a qué micro conectamos la SD card?
	  typename SPI_master,	    // driver de SPI que usamos
	  typename Chip_select_t,    // ¿cómo seleccionamos el chip?
	  uint32_t SPI_init_frequency0 = 250'000, // 250 kHz <= 400 kHz
	  uint32_t SPI_frequency0      = 500'000  // 500 kHz
	  >
struct SDCard_cfg{
    using Micro	      = Micro_t;
    using SPI	      = SPI_master;
    using Chip_select = Chip_select_t;

    static constexpr uint32_t SPI_init_frequency = SPI_init_frequency0;
    static_assert(SPI_init_frequency <= 400'000, 
		"SD card SPI init frequency must be at most 400'000 Hz");

    static constexpr uint32_t SPI_frequency      = SPI_frequency0;
};


// Documentos que uso:
//	"Part 1 - physical layer simplified specivication ver9.0"
//
// Este es el documento que voy a traducir aquí en código.
//
template <typename Cfg>
class SDCard_basic{
public:
    using Micro	      = typename Cfg::Micro;
    using SPI	      = typename Cfg::SPI;
    using Select_chip = typename Cfg::Chip_select;
    
// Types
    using Init_return = SDCard_types::Init_return;

    using R1 = SDCard_types::R1_response;
    // TODO: using R1b = SDCard_types::R1b_response;
    using R2 = SDCard_types::R2_response;
    using R3 = SDCard_types::R3_response;
    using R7 = SDCard_types::R7_response;

    using Read_return = SDCard_types::Read_return;
    using Write_return= SDCard_types::Write_return;
    using Data_response_token = SDCard_types::Data_response_token;

    using Address = uint32_t;	// para cards SDHC/SDXC
    static constexpr uint16_t block_size = 512;
    using Block   = std::span<uint8_t, block_size>; 


// Constructor
    // Interfaz static. Solo hay una SDCard de este tipo porque el Select_chip
    // es unico. (probemos así, el uso marcará si esto es útil)
    SDCard_basic() = delete;


// COMMANDS
// --------
    // Inicializa la SDCard en SPI mode, dejándola preparada para funcionar
    static Init_return init(uint8_t nretries = max_retries_acmd41_command, 
			    bool HCS = true);


    // Lee el bloque que empieza en la dirección indicada
    // Return: Block	    = array con los datos leidos
    //	       Read_return  = resultado de la operación
    static Read_return read(const Address&, Block);

    // TODO: de momento no implemento multiple_read ya que el micro que estoy
    // usando es el ATMEGA32 y solo tiene 2kB de RAM: ya va mal leyendo 1
    // block.
    // static Read_return multiple_read(const Address&, Block);

    // Escribe el bloque en la dirección indicada
    //	    Block	= array con los datos a escribir
    //
    // return:	
    //	    Write_return = resultado de la operación
    static Write_return write(const Address&, const Block);

    // TODO: mismo comentario que multiple_read
    // static Write_return multiple_write(const Address&, const Block);

    // TODO:
    // erase_and_write_protect_management: 7.2.5

    // Returns the status register of the card (= R2 response)
    static R2 send_status();


// BRICK COMMANDS (mejor no llamarlos directamente)
// --------------
// (RRR) ¿por qué dejo public los brick commands?
//	 (1) Son comandos de la datasheet. Esto es un traductor, tiene que ser
//	     visible todo lo que hay en la datasheet.
//
//	 (2) Sirven para depurar esta clase.
//
    static void SPI_cfg()
    { SPI_turn_on<SPI_frequency>(); }

// Implementation of init() function
    // Cfg SPI para ejecutar la inicialización de la tarjeta
    static void SPI_cfg_init()
    { SPI_turn_on<SPI_init_frequency>(); }

    // Devuelve response R1. Si todo va bien r1_is_in_idle_state(R1) == true
    static R1 go_idle_state();

    // return: r7
    static void send_if_cond(R7& r7);

    // return: r3
    static void read_ocr(R3& r3);



    // 7.2.1@physical_layer
    // HCS: High capacity support?
    //	    por defecto considero que todas las tarjetas son mayores de > 2GB
    // nretries: es el número de veces que reintentamos el comando ACMD41.
    //     La datasheet indica que hay que reintentarlo hasta que devuelva
    //     `in_idle_state == 0`. Solo si la tarjeta es SDUC, que no soporta
    //     SPI mode, no se inicializará.
    static R1 sd_send_op_cond(uint8_t nretries = max_retries_acmd41_command,
			    bool HCS = true);



private:
// NOTACIÓN
// + transfer_: seleccionan el chip y lo deseleccionan,
//              completando por completo la transferencia del comando.
//
// + send_    : no seleccionan el chip. Serán llamadas dentro de otra
//              función responsable de ello.
//
    
// CFG
    // DUDA: ¿qué valor para max_retries? Los de Arduino indican que hay
    // mínimo de de 1 a 8 bytes antes de que responda la SD card. No he
    // encontrado en la especificación del SDCard_basic los tiempos de respuesta
    // (???). Arduino usa 10, usemos 10 de momento.
    static constexpr uint8_t max_retries = 10;
    static constexpr uint8_t max_retries_acmd41_command = 100;
    static constexpr uint32_t SPI_init_frequency = Cfg::SPI_init_frequency;
    static constexpr uint32_t SPI_frequency      = Cfg::SPI_frequency;

    // ¿Cuánto tiempo debemos de esperar a que finalice de escribir el bloque
    // enviado? 4.6.2.2@physical_layer: máximo 250 ms
    static constexpr uint16_t timeout_write_busy = 250; // ms
    
    template <uint32_t time_in_ms>
    static constexpr uint16_t time_as_number_of_transfers()
	{ return (time_in_ms*SPI_frequency) / 8000u; }
    
							

    // CMD8 data:
    static constexpr uint32_t cmd8_supply_voltage = 0x01;
    static constexpr uint32_t cmd8_pattern = 0xAA;
    static constexpr uint8_t cmd8_crc = 0x86;//TODO: calcularlo dentro de send_cmd8
							

    // 7.3.1.1@physical_layer: 
    //   All the SD Memory Card commands are 6 bytes long. 
    //   The command transmission always starts with the left most bit 
    //   of the bit string corresponding to the command codeword.
    //   Esto es: MSB first en los comandos.
    //
    // 7.3.2@physical_layer: All responses are transmitted MSB first.
    static void SPI_cfg__();

    template<uint32_t frequency>
    static void SPI_turn_on();

    
    // 6.4.1@physical_layer
    // + A device shall be ready to accept the first command within 1ms 
    //   from detecting VDD min.  
    // + Device may use up to 74 clocks for preparation before receiving 
    //   the first command.
    // + In case of SPI mode, CS shall be held to high during 74 clock cycles.
    static void initialization_sequence();

// SEND COMMAND
    // 7.2.2@physical_layer
    // cmd0 es constante = 0x40 00 00 00 00 95
    static void send_cmd0() { send(0x40, 0x00, 0x95); }

    // 7.2.1@physical_layer
    //	    "The argument format of CMD8 is the same as defined in SD mode and
    //	    the response format is define in section 7.3.2.6"
    //
    //	4.3.13@physical_layer
    //	    La table 4-18 "format of CMD8" indica que para el supply_voltage
    //	    solo hay un único posible valor = 0001b (2.7-3.6V) luego en la
    //	    práctica no podemos elegirlo.
    //
    //	Respecto del check_pattern parece ser que en la versión 2.00 de la
    //	physical_layer indicaba que recomendaban usar 10101010b (=0xAA) como
    //	check_pattern aunque esto ha desaparecido. Como en principio da la
    //	impresión de que da lo mismo este valor elijamos ese valor
    //	recomendado, de forma que podemos calcular en tiempo de compilación 
    //	el CRC (CMD8 necesita un CRC)
    // static void send_cmd8(uint8_t supply_voltage, uint8_t pattern) <-- esta
    // función sería más genérica. Habría que calcular el CRC dinámicamente.
    // En la práctica lo conozco todo en tiempo de compilación. De momento
    // pruebo con la más sencilla:
    static void send_cmd8(uint8_t supply_voltage, uint8_t pattern, uint8_t crc);

    // Comando interno: los comandos son o de 1 byte, o de 2 bytes empezando
    // por {cmd55, ...}. CMD55 lo llaman app_cmd
    // return: R1 response
    static uint8_t transfer_app_cmd__();

    // Esta es la segunda parte del comando {CMD55, ACMD41}
    // Solo hay que enviar HCS en el 
    // return: R1 response
    static uint8_t transfer_sd_send_op_cond__(bool HCS = false);

    // Envia un comando con argumentos
    // TODO: si se calcula el CRC desdoblar esta función en 2:
    //	    send(cmd, arg);	<-- envía crc == 0
    //	    send_crc(cmd, arg); <-- esta calcula el crc
    //	o mejor send(cmd, arg, bool crc = false); <-- si true calcula el crc
    static void send(uint8_t cmd, uint32_t arg, uint8_t crc = 0x00);

    // Envia un comando que no tiene argumentos
    static void send(uint8_t cmd);


// READ RESPONSE
    //	Return: R1 si todo va bien (bit<7>::of(R1) == 0)
    //		0xFF en caso de error.
    static uint8_t read_R1();
    // TODO: static uint8_t read_R1b();
    static R2 read_R2();
    static void read_R3(R3& r3);

    // Args: (supply_voltage, pattern0) son los pasados a CMD8
    // return: R7
    static void read_R7(uint8_t supply_voltage, uint8_t pattern0, R7& r7);

    static Data_response_token read_data_response_token();

// Implementation of read function
    static uint8_t send_read_single_block(const Address& addr);
    static Read_return read_data_block(Block b);
    static uint8_t read_start_block_token();

// Implementation of write function
    static uint8_t send_write_single_block(const Address& addr);
    static void send_data_block(const Block b);
    static Data_response_token read_response_send_data_block();


// Helpers
    template <typename T>
    static T SPI_read();

    static uint16_t SPI_read_uint16_t();
    static uint32_t SPI_read_uint32_t();

    static void SPI_write_uint8_t(uint8_t x) { SPI::write(x); }
    static void SPI_write_uint16_t(uint16_t x);
    static void SPI_write_uint32_t(const uint32_t& x);

};

// TODO: ¿Podemos generalizar esta función?
// Sí, pero dentro de SPI hay que mirar si el SPI está configurando enviando
// el MSB primero o no.
// No puedo especializarla para cada T (ya que parece ser que 1º hay que
// especializar SDCard_basic<...>) por eso uso el if constexpr
template <typename Cfg>
template <typename T>
inline T SDCard_basic<Cfg>::SPI_read()
{
    if constexpr (std::is_same_v<T, uint16_t>)
	return SPI_read_uint16_t();

    else if constexpr (std::is_same_v<T, uint32_t>)
	return SPI_read_uint32_t();

    else 
	static_assert(atd::always_false_v<T>, "Not implemented");
}

template <typename Cfg>
inline uint16_t SDCard_basic<Cfg>::SPI_read_uint16_t()
{
    uint8_t b[2];

    b[1] = SPI::read();
    b[0] = SPI::read();

    return atd::concat_bytes<uint16_t>(b[1], b[0]);
}

template <typename Cfg>
inline uint32_t SDCard_basic<Cfg>::SPI_read_uint32_t()
{
    uint8_t b[4];

    b[3] = SPI::read();
    b[2] = SPI::read();
    b[1] = SPI::read();
    b[0] = SPI::read();

    return atd::concat_bytes<uint32_t>(b[3], b[2], b[1], b[0]);
}


// TODO: generalizar. Esto es SPI::write<uint32_t>(arg) escribiendo
// primero el MSByte first (otra forma de hacerlo será con el LSByte
// first) (endianness, ¿por qué lo llaman Big & little endian cuando
// realmente es most/least significant byte??? @_@)
template <typename Cfg>
inline void SDCard_basic<Cfg>::SPI_write_uint16_t(uint16_t x)
{
    SPI::write(atd::byte<1>(x));
    SPI::write(atd::byte<0>(x));
}

template <typename Cfg>
inline void SDCard_basic<Cfg>::SPI_write_uint32_t(const uint32_t& x)
{
    SPI::write(atd::byte<3>(x));
    SPI::write(atd::byte<2>(x));
    SPI::write(atd::byte<1>(x));
    SPI::write(atd::byte<0>(x));
}


// 7.3.1.1@physical_layer: 
//   All the SD Memory Card commands are 6 bytes long. 
//   The command transmission always starts with the left most bit 
//   of the bit string corresponding to the command codeword.
//   Esto es: MSB first en los comandos.
//
// 7.3.2@physical_layer: All responses are transmitted MSB first.
template <typename Cfg>
void SDCard_basic<Cfg>::SPI_cfg__()
{
    SPI::cfg_pins();
    SPI::spi_mode(0,0);
    SPI::data_order_MSB();
    SPI::default_transfer_value(0xFF); // 0xFF sería un error de R1
				       // al tener el bit[7] == '1'
				       // ¿Servirá para detectar errores
				       // si la SD card no responde?
}

template <typename Cfg>
    template<uint32_t frequency>
void SDCard_basic<Cfg>::SPI_turn_on()
{
    SPI_cfg__();
    SPI::template clock_frequency_in_hz<frequency>();
    SPI::turn_on();
}


// 6.4.1@physical_layer
// + A device shall be ready to accept the first command within 1ms 
//   from detecting VDD min.  
// + Device may use up to 74 clocks for preparation before receiving 
//   the first command.
// + In case of SPI mode, CS shall be held to high during 74 clock cycles.
template <typename Cfg>
void SDCard_basic<Cfg>::initialization_sequence()
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

// 7.2.1@physical_layer
//	    "The argument format of CMD8 is the same as defined in SD mode and
//	    the response format is define in section 7.3.2.6"
//
//	4.3.13@physical_layer
//	    La table 4-18 "format of CMD8" indica que para el supply_voltage
//	    solo hay un único posible valor = 0001b (2.7-3.6V) luego en la
//	    práctica no podemos elegirlo.
//
//	Respecto del check_pattern parece ser que en la versión 2.00 de la
//	physical_layer indicaba que recomendaban usar 10101010b (=0xAA) como
//	check_pattern aunque esto ha desaparecido. Como en principio da la
//	impresión de que da lo mismo este valor elijamos ese valor
//	recomendado, de forma que podemos calcular en tiempo de compilación 
//	el CRC (CMD8 necesita un CRC)
// static void send_cmd8(uint8_t supply_voltage, uint8_t pattern) <-- esta
// función sería más genérica. Habría que calcular el CRC dinámicamente.
// En la práctica lo conozco todo en tiempo de compilación. De momento
// pruebo con la más sencilla:
template <typename Cfg>
inline void SDCard_basic<Cfg>::
	send_cmd8(uint8_t supply_voltage, uint8_t pattern, uint8_t crc)
{
    // 7.3.1.3@physical_layer
    // formato cmd8 
    //	[31:12] reserved|[11:8]supply_voltage|[7:0]check_pattern
    // arg=      00 00 0    |        1           |       AA
    uint32_t arg 
	= atd::concat_bytes<uint32_t>
			    (0x00, 0x00, supply_voltage, pattern);
    // cmd8_crc = CRC7(8 | 0x40, arg); TODO: que sea send quien calcule el
    // crc
    send(8u, arg, crc);
}

// Comando interno: los comandos son o de 1 byte, o de 2 bytes empezando
// por {cmd55, ...}. CMD55 lo llaman app_cmd
// return: R1 response
template <typename Cfg>
uint8_t SDCard_basic<Cfg>::transfer_app_cmd__()
{
//	SPI_cfg_init();
    Select_chip select{};

    send(55u);

    return read_R1();
}

// Esta es la segunda parte del comando {CMD55, ACMD41}
// Solo hay que enviar HCS en el 
// return: R1 response
template <typename Cfg>
uint8_t SDCard_basic<Cfg>::transfer_sd_send_op_cond__(bool HCS)
{
//	SPI_cfg_init();
    Select_chip select{};
    uint32_t arg{0};
    if (HCS) 
	atd::write_bit<30>::to<1>::in(arg);

    send(41u, arg);

    return read_R1();
}


template <typename Cfg>
SDCard_basic<Cfg>::R1 
	    SDCard_basic<Cfg>::go_idle_state()
{
//	SPI_cfg_init(); <-- Lo llama init
    initialization_sequence();

    Select_chip select{};

    send_cmd0();
    return R1{read_R1()};
}

// return: r7
template <typename Cfg>
void SDCard_basic<Cfg>::send_if_cond(R7& r7)
{
//	SPI_cfg_init();
    Select_chip select{};

    // TODO: pasar como argumentos supply_voltage y pattern. Para ello
    // necesito escribir primero la funcion de CRC7.
    send_cmd8(cmd8_supply_voltage, cmd8_pattern, cmd8_crc);

    read_R7(cmd8_supply_voltage, cmd8_pattern, r7);
}


// return: r3
template <typename Cfg>
void SDCard_basic<Cfg>::read_ocr(R3& r3)
{
//	SPI_cfg_init();
    Select_chip select{};

    send(58u);

    read_R3(r3);
}


// return: r2
template <typename Cfg>
SDCard_basic<Cfg>::R2 SDCard_basic<Cfg>::send_status()
{
//	SPI_cfg();
    Select_chip select{};

    send(13u);

    return read_R2();
}



// 7.2.8@physical_layer
// + in the SPI mode the card will always respond to a command.
// + A command may be rejected in, entre otros casos, "it is sent while
//   the card is in Busy".
//
//   Luego si la SDCard_basic esta ocupada responderá con error.
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
template <typename Cfg>
uint8_t SDCard_basic<Cfg>::read_R1()
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

    return R1::invalid_value();
}


template <typename Cfg>
inline SDCard_basic<Cfg>::R2 SDCard_basic<Cfg>::read_R2()
{
    uint8_t r = read_R1(); 

    R2 r2;
    r2.r1 = R1{r};
    r2.read<SPI>();

    return r2;
}


template <typename Cfg>
void SDCard_basic<Cfg>::read_R3(R3& r3)
{
    // Leemos R1
    uint8_t r = read_R1(); 
    r3.r1 = R1{r};

    if (r > 1) // if (r != 0x00 and r != 0x01)
	return;

    // Leemos los 4 bytes restantes
    r3.read<SPI>();
}


//
// R7 es la respuesta del cmd8. No hay ningún otro comando que tenga esta
// respuesta.
//
// 7.2.1@physical_layer
//	+ "If the card indicates an illegal command, the card is legacy and does
//	   not support CMD8"
//
//	+ "If the card supports CMD8 and can operate on the supplied voltage,
//	   the response echoes back the supply voltage and the check pattern 
//	   that were set in the command argument."
//
//  + "If the card supports CMD8 and can operate on the supplied voltage,
//     the response echoes back the supply voltage and the check pattern 
//     that were set in the command argument."
//
//  + "If VCA in the response is set to 0, the card cannot operate on the
//     supplied voltage."
//
//  + "If check pattern is not matched, CMD8 communication is not valid.
//     In this case, it is recommended to retry CMD8 sequence."
//
// Y esto se detalla en 7.3.1.4@physical_layer:
//	+ "In SPI mode, the card always returns response"
//	+ La table 7-5 muestra la respuesta. Puede ser:
//	    * R1 == 0x09: el CRC es erróneo. La respuesta en este caso consta
//	                  solo de R1, es 1 byte y no 5 bytes.
//
//	    * R1 = 0x01 y pattern == check_pattern (el enviado). 
//	      Dos posibles respuestas:
//		1) Si VCA == 0 there is mismatch. Según 7.2.1 la tarjeta no
//		   puede operar en el supplied voltage.
//
//		2) Si VCA == check_pattern todo correcto.
//
//	    * R1 == 0b1xx...x ??? En 7.2.1. indica que si the card is legacy
//	      devuelve un illegal command. En este caso la respuesta será de 1
//	      byte como el caso R1 == 0x09.
//
// Args: (supply_voltage, pattern0) son los pasados a CMD8
// return: R7
//
template <typename Cfg>
void SDCard_basic<Cfg>::read_R7(uint8_t supply_voltage, uint8_t pattern0,
				    R7& r7)
{
    // Leemos R1
    uint8_t r = read_R1(); 
    r7.r1 = R1{r};

    if (r == 0x09)
	return;

    // Al principio puse: `if(r != 0x01)` pero fallaba ya que la SDcard
    // devolvía '0'!!!
    if (r > 1) // if (r != 0x00 and r != 0x01)
	return;

    // Leemos los 4 bytes restantes
    r7.read<SPI>();
}


// 7.2.4@physical_layer
// Da la impresión, y así lo indican en la página de elm-chan, que el primer
// byte después de enviar el Block a escribir tiene que ser el `data response
// token`. Sin embargo no lo indican explicitamente en la datasheet. 
// En otra página de internet comentaban que había que esperar 250 ms. El
// punto 4.6.2.2 indica que no tardará más de 250 ms la operación de write. 
// La biblioteca de arduino considera que el siguiente token a recibir después
// de enviar el Block tiene que ser el `data response`. Como no estorba el
// bucle for, ya que si la sd card responde inmediatamente saldrá del bucle en
// la primera iteración, lo dejo de momento. 
// Si en el futuro encuentro la referencia en la datasheet correspondiente
// modificaré el codigo de acuerdo.
template <typename Cfg>
SDCard_basic<Cfg>::Data_response_token 
		SDCard_basic<Cfg>::read_data_response_token()
{
    constexpr uint16_t n = time_as_number_of_transfers<timeout_write_busy>();

    for (uint16_t i = 0; i < n; ++i){
	uint8_t r = SPI::read();

	if (Data_response_token::is_valid(r))
	    return Data_response_token::valid(r);
    }

    return Data_response_token::timeout_error();
}



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
template<typename Cfg>
void SDCard_basic<Cfg>::send(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    SPI_write_uint8_t(cmd | 0x40u);	

    SPI_write_uint32_t(arg);

    SPI_write_uint8_t(crc | 0x01u);
}

// 7.3.1.3@physical_layer
//   If a command does not require an argument, the value of 
//   this field should be set to zero.
template<typename Cfg>
void SDCard_basic<Cfg>::send(uint8_t cmd)
{
    SPI::write(cmd | 0x40);	   

    SPI::write(0x00);
    SPI::write(0x00);
    SPI::write(0x00);
    SPI::write(0x00);

    SPI::write(0x01);
}

template<typename Cfg>
SDCard_basic<Cfg>::Init_return SDCard_basic<Cfg>::
					init(uint8_t nretries, bool HCS)
{
    SPI_cfg_init();

    R1 r1 = go_idle_state();
    if (r1.is_illegal_command())
	return Init_return::cmd0_fail;

    R7 r7;
    send_if_cond(r7);


    // Posibles respuestas con significado (ver 7.3.1.4)
    // Si devuelve is_illegal_command() el flujo sigue correctamente,
    // devolviendo al final SDSC card (???)
//    if (r7.r1.is_illegal_command())
//	res = Init_return::card_version1;
//
//    else {
	if (r7.r1.data == 0x09)
	    return Init_return::cmd8_crc_error;

	if (r7.VCA() == 0) 
	    return Init_return::voltage_mismatch;

	// Si llega aquí debería de ser falso el siguiente if
	// TODO: pasar como parámetro a init(supply_voltage, pattern)
	  // 1º necesito implementar CRC7
	if (!(r7.VCA() == cmd8_supply_voltage
		and r7.pattern() == cmd8_pattern))
	    return Init_return::cmd8_echo_fail;
	    
//    }


    // Enviamos CMD58 (opcional) para averiguar qué potenciales es a los que
    // trabaja la tarjeta. Si nuestro hardware no trabaja con esos potenciales
    // tendríamos que considerarla no válida y acabar.
    // En este caso la tarjeta devolverá que está `busy`, sin inicializar. Es
    // normal (ver 7.2.1@physical_layer)
    R3 r3;
    read_ocr(r3);

    r1 = sd_send_op_cond(nretries, HCS);
    if (r1.is_in_idle_state())
	return Init_return::acmd41_in_idle_state;

    read_ocr(r3);

    if (!r3.card_has_finished_power_up())
	return Init_return::card_no_power_up;

    if (r3.card_type() == R3::Type::SDSC)
	return Init_return::init_SDSC_card_ok;

    // else
    return Init_return::init_SDHC_or_SDXC_ok;
}



template<typename Cfg>
SDCard_basic<Cfg>::R1 
	SDCard_basic<Cfg>::sd_send_op_cond(uint8_t nretries, bool HCS)
{
    // Devolverá in_idle_state mientras no haya inicializado. La datasheet
    // indica que hay que reintentar ACMD41 hasta que finalice la
    // inicialización.
    uint8_t r1{0};

    for (uint8_t i = 0; i < nretries; ++i){
	transfer_app_cmd__();

	if (r1 > 1)
	    return R1{r1};

	r1 = transfer_sd_send_op_cond__(HCS);

	if (r1 != 0x01) // 0x01 == in_idle_state sin error
	    return R1{r1};

    }

    return R1{r1};

}

/***************************************************************************
 *				READ
 ***************************************************************************/
template<typename Cfg>
uint8_t SDCard_basic<Cfg>::send_read_single_block(const Address& addr)
{
    send(17u, addr);
    return read_R1();
}


// 7.3.3.2@physical_layer: si todo va bien devuelve 0xFE y luego los datos
// 7.3.3.3@physical_layer: en caso de error devuelve 0x0error
// Devuelve:
//	0x00: todo va bien. A continuación la tarjeta envía 512 bytes.
//	0xFF: timeout
//	otro valor: error (formato de 'data error token')
template<typename Cfg>
uint8_t SDCard_basic<Cfg>::read_start_block_token()
{
    // DUDA: No he encontrado el tiempo máximo que tiene que esperar el micro
    // para que la SDCard responda. En una página web ponía 100 ms.
    // Confirmar!!!
    constexpr uint16_t n = time_as_number_of_transfers<100>();
    for (uint16_t i = 0; i < n; ++i){
	uint8_t r = SPI::read();
	if (r == 0xFE) // ok
	    return 0x00;

	if (atd::nibble<1>(r) == 0) 
	    return r;
    }

    return 0xFF; // timeout
}

template<typename Cfg>
SDCard_basic<Cfg>::Read_return
	SDCard_basic<Cfg>::read_data_block(Block b)
{
    uint8_t r = read_start_block_token();
    if (r == 0xFF)
	return Read_return::time_out_error();

    if (r != 0x00)
	return Read_return::data_error_token(r);


    for (Block::size_type i = 0; i < block_size; ++i)
	b[i] = SPI::read();

    uint16_t crc = SPI_read_uint16_t();
    (void) crc;

    // TODO: validar que el CRC sea correcto!!!

    return Read_return{}; // OK
}


// 7.2.3@physical_layer
template<typename Cfg>
SDCard_basic<Cfg>::Read_return
	SDCard_basic<Cfg>::read(const Address& addr, Block b)
{
    SPI_cfg();

    Select_chip select{};

    R1 r1{send_read_single_block(addr)};

    if (r1.is_an_error())
	return Read_return::r1_error(r1); 


    return read_data_block(b);
}

/***************************************************************************
 *				WRITE
 ***************************************************************************/
template<typename Cfg>
uint8_t SDCard_basic<Cfg>::send_write_single_block(const Address& addr)
{
    send(24u, addr);
    return read_R1();
}

// 7.2.4@physical_layer
template<typename Cfg>
SDCard_basic<Cfg>::Write_return
	SDCard_basic<Cfg>::write(const Address& addr, const Block b)
{
    SPI_cfg();

    Select_chip select{};
    R1 r1{send_write_single_block(addr)};
    if (r1.is_an_error())
	return Write_return::r1_error(r1); 

    send_data_block(b);
    Data_response_token dt = read_response_send_data_block();
    if (dt.is_an_error())
	return Write_return::dt_error(r1, dt);

    R2 r2 = send_status();
    return Write_return::send_status(r1, dt, r2);

}


template<typename Cfg>
SDCard_basic<Cfg>::Data_response_token
    SDCard_basic<Cfg>::read_response_send_data_block()
{
    Data_response_token dt = read_data_response_token();
    if (!dt.data_accepted())
	return dt;

    constexpr uint16_t n = time_as_number_of_transfers<timeout_write_busy>();
    for (uint16_t i = 0; i < n; ++i) {
	if (SPI::read() != 0x00) // busy_token == 0x00!!!
	    return dt;
    }

    return Data_response_token::timeout_error();
}


// 7.3.3.2@physical_layer
template<typename Cfg>
void SDCard_basic<Cfg>::send_data_block(const Block b)
{
    SPI::write(0xFE); // start block token
		      
    for (Block::size_type i = 0; i < b.size(); ++i)
	SPI::write(b[i]);

    SPI_write_uint16_t(0xFFFF); // CRC 
				// Da lo mismo su valor, ya que está configurado 
				// para no enviarlo CRC. Los de arduino envian
				// 0xFFFF, hagamos lo mismo.
}



}// namespace

#endif


