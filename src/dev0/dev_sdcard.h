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

// Clase por defecto para seleccionar la SDCard_basic
// El `Selector_SPI` lo define en general el cliente. Es el que diseña el
// hardware de la aplicación el que sabe cómo va a conectar los diferentes
// dispositivos SPI. `SDCard_select` es el selector más básico: usa un pin
// (que por defecto será el que SPI::CS_pin_number).
// TODO: Generalizar esta clase. Se puede meter en mcu_SPI.h ya que no depende
// del micro concreto a usar ni de los dispositivos. Se basa en un pin.
template <typename Output_pin, typename SPI>
class SDCard_select{
public:
    static void init() {Output_pin::as_output();}

    static void select()
    { 
//	SPI::write(0xFF);   // hacemos MOSI = 1
	Output_pin::write_zero(); 
//	SPI::write(0xFF);   // hacemos MOSI = 1
    }

    static void deselect()
    {
//	SPI::write(0xFF);   // hacemos MOSI = 1
	Output_pin::write_one();
//	SPI::write(0xFF);   // hacemos MOSI = 1
    }

    SDCard_select() {select();}
    ~SDCard_select() {deselect();}
};


// Response R1
// -----------
// Traductor del mapa de bits R1 en funciones
struct SDCard_basic_R1_response{
    uint8_t r1;

// State
    bool is_ready() const
    { return r1 == 0x00;} 

    bool is_in_idle_state()  const
    { return r1 == 0x01;}

    // r1 contiene un valor válido? mejor is_ok()?
    bool is_valid() const
    { return atd::is_zero_bit<7>::of(r1);}

    bool is_an_error() const
    { return r1 > 0x01; }

// Flags
//  DUDA: quedaría mejor llamarla in_idle_state(), pero sería muy facil
//  confundirla con is_in_idle_state(). Por eso opto por dejar el _flag.
//  ¿Nombre?
    bool in_idle_state_flag() const
    { return atd::is_one_bit<0>::of(r1);}

    bool erase_reset_flag() const
    { return atd::is_one_bit<1>::of(r1);}

    bool illegal_command_flag() const
    { return atd::is_one_bit<2>::of(r1);}

    bool communication_CRC_error_flag() const
    { return atd::is_one_bit<3>::of(r1);}

    bool erase_sequence_error_flag() const
    { return atd::is_one_bit<4>::of(r1);}

    bool address_error_flag() const
    { return atd::is_one_bit<5>::of(r1);}

    bool parameter_error_flag() const
    { return atd::is_one_bit<6>::of(r1);}

// Helpers
    static uint8_t invalid_value()
    { return 0xFF; }
};

// Referencias
//	5.1@physical_layer
//	7.3.2.4@physical_layer
//
// TODO: de momento no implemento los bits UHS... 
struct SDCard_basic_R3_response{ 
    enum class Type{ SDSC, SDHC_or_SDXC };

    SDCard_basic_R1_response r1;

    uint8_t OCR[4];
    template <typename SPI>
    void read(){
	OCR[0] = SPI::read();	// MSByte
	OCR[1] = SPI::read();	
	OCR[2] = SPI::read();
	OCR[3] = SPI::read();
    }


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
struct SDCard_basic_R7_response{ 
    SDCard_basic_R1_response r1;

    uint8_t data[4];

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

    // TODO: poner esto como atd::nibble...???
    uint8_t version() const {return ((data[0] & 0xF0) >> 4);}
    uint8_t pattern() const {return data[3];}

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



// Documentos que uso:
//	"Part 1 - physical layer simplified specivication ver9.0"
//
// Este es el documento que voy a traducir aquí en código.
//
template <typename Cfg>
class SDCard_basic{
public:
// TODO: parametrizar todo esto
    using Micro	      = mcu::Micro;
    using SPI	      = mcu::SPI_master;
    using Select_chip = SDCard_select<mcu::Output_pin<SPI::CS_pin_number>, SPI>;
    
// Types
    using R1 = SDCard_basic_R1_response;
    using R3 = SDCard_basic_R3_response;
    using R7 = SDCard_basic_R7_response;

// Constructor
    // Interfaz static. Solo hay una SDCard de este tipo porque el Select_chip
    // es unico. (probemos así, el uso marcará si esto es útil)
    SDCard_basic() = delete;

// Commands
    // Devuelve response R1. Si todo va bien r1_is_in_idle_state(R1) == true
    static R1 go_idle_state()
    {
	SPI_cfg_init();
	initialization_sequence();

	Select_chip select{};

	send_cmd0();
	return R1{read_R1()};
    }

    // return: r7
    static void send_if_cond(R7& r7)
    {
	// TODO: estoy repitiendo la configuracion del SPI!!! Ordenar esto
	SPI_cfg_init();

	Select_chip select{};

	send_cmd8(cmd8_supply_voltage, cmd8_pattern, cmd8_crc);

	read_R7(cmd8_supply_voltage, cmd8_pattern, r7);
    }


    // return: r3
    static void read_ocr(R3& r3)
    {
	// TODO: estoy repitiendo la configuracion del SPI!!! Ordenar esto
	SPI_cfg_init();

	Select_chip select{};

	send(58u);

	read_R3(r3);
    }


    // 7.2.1@physical_layer
    // HCS: High capacity support?
    //	    por defecto considero que todas las tarjetas son mayores de > 2GB
    static R1 sd_send_op_cond(bool HCS = true)
    {
	// Devolverá in_idle_state mientras no haya inicializado. La datasheet
	// indica que hay que reintentar ACMD41 hasta que finalice la
	// inicialización.
	uint8_t r1{0};

	for (uint8_t i = 0; i < max_retries; ++i){
	    transfer_app_cmd__();

	    if (r1 > 1)
		return R1{r1};

	    r1 = transfer_sd_send_op_cond__(HCS);

	    if (r1 != 0x01) // 0x01 == in_idle_state sin error
		return R1{r1};

	}
    
	return R1{r1};

    }



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
    static constexpr uint32_t init_frequency = 250'000; // 250 kHz
							

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
    static void SPI_cfg__()
    {
	SPI::cfg_pins();
	SPI::spi_mode(0,0);
	SPI::data_order_MSB();
	SPI::default_transfer_value(0xFF); // 0xFF sería un error de R1
					   // al tener el bit[7] == '1'
					   // ¿Servirá para detectar errores
					   // si la SD card no responde?
    }

    template<uint32_t frequency>
    static void SPI_cfg()
    {
	SPI_cfg__();
	SPI::clock_frequency_in_hz<frequency>();
	SPI::on();
    }

    static void SPI_cfg_init()
    { SPI_cfg<init_frequency>(); }
    
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
    static void send_cmd8(uint8_t supply_voltage, uint8_t pattern, uint8_t crc)
    {
	// 7.3.1.3@physical_layer
	// formato cmd8 
	//	[31:12] reserved|[11:8]supply_voltage|[7:0]check_pattern
	// arg=      00 00 0    |        1           |       AA
	uint32_t arg 
	    = atd::concat_bytes<uint32_t>
				(0x00, 0x00, supply_voltage, pattern);
	// cmd8_crc = CRC7(8, arg);
	send(8u, arg, crc);
    }

    // Comando interno: los comandos son o de 1 byte, o de 2 bytes empezando
    // por {cmd55, ...}. CMD55 lo llaman app_cmd
    // return: R1 response
    static uint8_t transfer_app_cmd__()
    {
	// TODO: estoy repitiendo la configuracion del SPI!!! Ordenar esto
	SPI_cfg_init();

	Select_chip select{};

	send(55u);

	return read_R1();
    }

    // Esta es la segunda parte del comando {CMD55, ACMD41}
    // Solo hay que enviar HCS en el 
    // return: R1 response
    static uint8_t transfer_sd_send_op_cond__(bool HCS = false)
    {
	// TODO: estoy repitiendo la configuracion del SPI!!! Ordenar esto
	SPI_cfg_init();

	Select_chip select{};
	uint32_t arg{0};
	if (HCS) 
	    atd::write_bit<30>::to<1>::in(arg);

	send(41u, arg);

	return read_R1();
    }

    // Envia un comando con argumentos
    static void send(uint8_t cmd, uint32_t arg, uint8_t crc = 0x00);

    // Envia un comando que no tiene argumentos
    static void send(uint8_t cmd);


    //	Return: R1 si todo va bien (bit<7>::of(R1) == 0)
    //		0xFF en caso de error.
    static uint8_t read_R1();
    static void read_R3(R3& r3);


    // Args: (supply_voltage, pattern0) son los pasados a CMD8
    // return: {R7, byte que genera el error}
    //
    // ¿por qué devuelve el byte que genera el error? En principio no es
    // necesario. R7 traduce la respuesta. Lo devuelvo para depurar.
    // En principio no tengo claro que haya leido bien la datasheet ni tampoco
    // que vaya a funcionar correctamente. Necesito poder depurar esta
    // clase. 
    // return: R7
    static void read_R7(uint8_t supply_voltage, uint8_t pattern0, R7& r7);

};

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
    SPI::write(cmd | 0x40);	

    // TODO: generalizar. Esto es SPI::write<uint32_t>(arg) escribiendo
    // primero el MSByte first (otra forma de hacerlo será con el LSByte
    // first) (endianness, ¿por qué lo llaman Big & little endian cuando
    // realmente es most/least significant byte??? @_@)
    SPI::write(static_cast<uint8_t>((arg & 0xFF000000) >> 24));
    SPI::write(static_cast<uint8_t>((arg & 0x00FF0000) >> 16));
    SPI::write(static_cast<uint8_t>((arg & 0x0000FF00) >> 8));
    SPI::write(static_cast<uint8_t>((arg & 0x000000FF)));

    SPI::write(crc | 0x01);
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

}// namespace

#endif


