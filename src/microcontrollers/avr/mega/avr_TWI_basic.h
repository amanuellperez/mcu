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

#ifndef __AVR_TWI_BASIC_H__
#define __AVR_TWI_BASIC_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Traductor del TWI (I2C)
 *
 *
 *   - HISTORIA:
 *    Manuel Perez
 *    24/08/2019 v0.0
 *    27/01/2020 Reescribo todo.
 *    01/11/2023 Elimino std::byte a favor de uint8_t.
 *               He intentado usar std::byte porque con lo que trabajamos son
 *               con bytes y no con números, pero usarlo obliga a hacer
 *               continuamente castings tanto aquí como en el cliente. 
 *               Más sencillo uint8_t.
 *
 ****************************************************************************/
#include <atd_bit.h>
#include <atd_type_traits.h>	// always_false_v
#include <avr/io.h> // registros: DDRB... PORT...
#include <avr_constants.h>
#include "mega_cfg.h"

namespace avr_{


/*!
 *  \brief  Traductor literal de la datasheet.
 *
 *  IMPORTANTE (para principiantes):
 *	clear TWINT = write 0
 *	set TWINT   = write 1
 *
 *	Sin embargo, parece ser que es habitual en micros que cuando se quiere
 *	hacer un clear en un bit se escriba un 1, por eso la datasheet dice
 *	que si se quiere hacer clear(TWINT) hay que escribir un 1 en TWINT.
 *	Esto lo que hace es que se escriba un 0. De hecho me he tirado un buen
 *	rato (demasiado para mi gusto) mirando precisamente esto: yo escribo
 *	TWINT = 1, pero al mirar su valor resulta ser 0!!! 
 *
 *	El flujo de TWI usando interrupciones es el siguiente:
 *	TWINT = 0 <-- a la espera de que suceda algo
 *	sucede algo y se necesita respuesta por parte del usuario:
 *	TWINT = 1 (= ey! usuario, tienes que hacer algo!!!)
 *	Si TWIE = 1 y activadas las interrupciones se lanza una interrupción,
 *	Dentro del handler de la ISR siempre tendremos TWINT = 1 (precisamente
 *	es lo que lanza la interrupción).
 *	Después de procesar la interrupción, para decirle a TWI que continue
 *	tenemos que hacer clear a TWINT, para ello:
 *	1. escribimos TWINT = 1
 *	2. el hardware traduce esto inmediatamente a TWINT= 0.
 *
 *	Recordar que mientras no hagamos clear(TWINT) SCL será igual a 0 (lo
 *	cual nos va a interesar si lleva tiempo procesar lo que estemos
 *	haciendo).
 *
 *	Recordar:
 *	    1.- Escribir un 0 en TWINT = NO HACE NADA!!!
 *	    2.- Escribir un 1 en TWINT = hace un clear.
 */
class TWI_basic{
public:
// Types
    using streamsize = uint8_t;
    using Address = uint8_t;
    using State   = cfg::TWI_basic_state;

// Functions
    /// Selects the division factor for the bit rate generator.
    /// The bit rate generatro is a frequency divider which generatres the
    /// SCL clock frequency in the Master modes.
    static void division_factor_for_bit_rate_generator(uint8_t d)
    { TWBR = d; }
    
    /// Devuelve el division factor for the bit rate generator.
    static uint8_t division_factor_for_bit_rate_generator()
    {return TWBR;}


    /// Devuelve el status bit.
    static uint8_t status()
    { return TWSR & cfg::TWI_MASK_STATUS_BITS;}


    // Selección de la velocidad del reloj
    // -----------------------------------
    static void bit_rate_prescaler_value_1()
    { atd::write_bits<TWPS1, TWPS0>::to<0,0>::in(TWSR); }

    static void bit_rate_prescaler_value_4()
    { atd::write_bits<TWPS1, TWPS0>::to<0,1>::in(TWSR); }

    static void bit_rate_prescaler_value_16()
    { atd::write_bits<TWPS1, TWPS0>::to<1,0>::in(TWSR); }

    static void bit_rate_prescaler_value_64()
    { atd::write_bits<TWPS1, TWPS0>::to<1,1>::in(TWSR); }

    /// Devuelve los bits con el rate prescaler seleccionado.
    static uint8_t bit_rate_prescaler()
    { return TWSR & cfg::TWI_MASK_BIT_RATE_PRESCALER;}


    /// Definimos la frecuencia del reloj SCL.
    /// f_scl = frecuencia en kHz de SCL (tipica: 100 kHz y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = clock_frequency_in_hz>
    static void SCL_frequency_in_kHz();
    



    /// ¿Queremos que en multimaster systems se responda a la general call
    /// address (0x00)?
    static void enable_general_call()
    { atd::write_bit<TWGCE>::to<1>::in(TWAR);}

    static void disable_general_call()
    { atd::write_bit<TWGCE>::to<0>::in(TWAR);}

    static bool is_general_call_enabled()
    { return atd::is_one_bit<TWGCE>::of(TWAR);}

    // Observar que TWDR es el buffer del hardware de TWI. Es un buffer
    // de 1 byte que funciona a la vez como put/get area. ¿Sería mejor
    // llamarlo buffer?
    /// Dato que queremos enviar a continuación.
    static void data(uint8_t x)
    { TWDR = x; }

    /// Dato que hemos recibido.
    static uint8_t data()
    { return TWDR;}


    /// Enables TWI as a master.
    static void enable()
    { atd::write_bit<TWEN>::to<1>::in(TWCR);}

    // En el ejemplo de la app.note reinicia el master de la siguiente forma:
    static void master_reset()
    {
	atd::write_bits<TWINT, TWEA, TWSTA, TWSTO, TWWC, TWEN, TWIE>::
	             to<  0  ,   0 ,   0  ,   0  ,   0 ,  1,    0>::in(TWCR);
    }

    /// Set up TWI slave to its initial standby state.
    /// Remember to enable interrupts after initializing the TWI.
    // TWI_on = indica si vamos a gestionar TWI usando interrupciones (=1) o
    // mediante polling (= 0). En principio no parece que tenga mucho sentido
    // usar polling para manejar el slave, pero lo dejo configurable ya que
    // carezco de experiencia.
    template <uint8_t TWI_slave_address, uint8_t TWIE_on = 1>
    static void slave_init()
    {
	atd::write_range_bits<cfg::TWI_SLAVE_ADDRESS_BIT0,
						cfg::TWI_SLAVE_ADDRESS_BITn>::
	    to<TWI_slave_address>::in(TWAR); // TWAR = TWI_slave_address;

	slave_reset<TWIE_on>();
    }

    template <uint8_t TWIE_on>
    static void slave_reset()
    {
	atd::write_bits<TWINT, TWEA, TWSTA, TWSTO, TWWC, TWEN, TWIE>::
	             to<  0  ,   1 ,   0  ,   0  ,   0 ,  1, TWIE_on>::in(TWCR);
    }

    /// Switched to the not addressed Slave mode (esto es, se queda a la espera
    /// de que algún master le llame).
    /// Own SLA will be recognized. GCA will be recognized if
    /// enable_general_call().
    static void not_addressed_slave_mode()
    { atd::write_bits<TWSTA, TWSTO, TWINT, TWEA>::to<0,0,1,1>::in(TWCR); }

    /// Switched to the not addressed Slave mode (esto es, se queda a la espera
    /// de que algún master le llame).
    /// no recognition of own SLA or GCA
    static void not_addressed_slave_no_recognition_mode()
    { atd::write_bits<TWSTA, TWSTO, TWINT, TWEA>::to<0,0,1,0>::in(TWCR); }

    /// Disables TWI interface.
    static void disable()
    { atd::write_bit<TWEN>::to<0>::in(TWCR);}


    /// The TWI interrupt request will be activated.
    // OJO: hay que definir globalmente las interrupciones para que esto
    // funcione: Interrupt::enable_interrupts();
    // Observar que hay que escribir un 0 en TWINT. Si no se escribe, no se
    // por qué pero se hace un clear de TWINT (ojo: tal como está ahora
    // implementado write_bit es normal ya que write_bits reescribe TWCR y 
    // al escribir un 1 en TWINT hacemos el clear. Sin embargo, lo he probado
    // a hacerlo directamente y sigue haciendo el clear a TWINT). Además, el
    // ejemplo de la app.note avr315 desactiva las interrupciones de esta
    // forma.
    static void interrupt_enable()
    { atd::write_bits<TWIE, TWINT>::to<1,0>::in(TWCR);}

    /// The TWI interrupt request will be disactivated.
    // Fundamental poner TWINT = 0 (no hace nada). Ver comentarios de
    // interrupt_enable
    static void interrupt_disable()
    { atd::write_bits<TWIE, TWINT>::to<0,0>::in(TWCR);}


    // Actions by Master Mode (see: tables 26-3/4)
    // --------------------------------------------
    /// A START condition will be transmitted when the bus becomes free.
    static void master_transmit_start()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<1,0,1>::in(TWCR); }

    /// Repeated START will be transmitted.
    static void master_transmit_repeated_start()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<1,0,1>::in(TWCR); }

    /// STOP condition will be transmitted.
    static void master_transmit_stop()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<0,1,1>::in(TWCR); }

    /// STOP condition followed by a START condition will be transmitted.
    static void master_transmit_stop_followed_by_start()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<1,1,1>::in(TWCR); }


    /// SLA+R/W or data byte will be transmitted.
    static void master_transmit_byte(uint8_t x)
    {
	TWDR = x;
	atd::write_bits<TWSTA, TWSTO, TWINT>::to<0,0,1>::in(TWCR);
    }


//    /// SLA+W will be transmitted to 'slave_address'.
//    template <uint8_t slave_address>
//    static void master_transmit_sla_w()
//    {
//        master_transmit_byte(SLA_W(slave_address));
//    }


    /// SLA+W will be transmitted to 'slave_address'.
    static void master_transmit_sla_w(uint8_t slave_address)
    {
        master_transmit_byte(SLA_W(slave_address));
    }

    /// SLA+R will be transmitted to 'slave_address'.
//    template <uint8_t slave_address>
//    static void master_transmit_sla_r()
    static void master_transmit_sla_r(uint8_t slave_address)
    {
        master_transmit_byte(SLA_R(slave_address));
    }



    /// Comenzamos a recibir el siguiente byte, devolviendo
    /// ACK cuando lo hayamos recibido.
    static void master_receive_data_with_ACK()
    { atd::write_bits<TWSTA, TWSTO, TWINT, TWEA>::to<0,0,1,1>::in(TWCR);}

    /// Comenzamos a recibir el siguiente byte, devolviendo
    /// NACK cuando lo hayamos recibido.
    static void master_receive_data_with_NACK()
    { atd::write_bits<TWSTA, TWSTO, TWINT, TWEA>::to<0,0,1,0>::in(TWCR);}


// Actions by Slave Mode (see: tables 26-5/6)
// ------------------------------------------
    /// Data byte will be transmitted and NACK should be received.
    static void slave_transmit_byte_received_NACK(uint8_t x)
    {
	TWDR = x;
	atd::write_bits<TWSTO, TWINT, TWEA>::to<0,1,0>::in(TWCR);
    }

    /// Data byte will be transmitted and NACK should be received.
    static void slave_transmit_byte_received_ACK(uint8_t x)
    {
	TWDR = x;
	atd::write_bits<TWSTO, TWINT, TWEA>::to<0,1,1>::in(TWCR);
    }

    /// Data byte will be received and NACK will be returned.
    static void slave_receive_data_with_NACK()
    { atd::write_bits<TWSTO, TWINT, TWEA>::to<0,1,0>::in(TWCR); }

    /// Data byte will be received and ACK will be returned.
    static void slave_receive_data_with_ACK()
    { atd::write_bits<TWSTO, TWINT, TWEA>::to<0,1,1>::in(TWCR); }

//    /// En caso de error, recibir datos no esperados, paramos la transmisión.
//    // TODO: revisar la implementación. No tengo claro. Es "casi" copia de la
//    // app-note "recover_from_bus_error". 
//    Puedo implementarlo usando reset_slave y parece que funciona.
//    static void slave_stop_transmission()
//    { atd::write_bit<TWSTA, TWSTO, TWINT>::to<0,1,1>::in(TWCR); }


    // Miscellaneous States (table 26-7)
    // ---------------------------------
    static void recover_from_bus_error()
    { atd::write_bits<TWSTA, TWSTO, TWINT>::to<0,1,1>::in(TWCR); }


    // Other
    // -----
    /// Espera hasta acabar la transmisión en curso. Una vez acabada,
    /// se puede proceder a enviar más datos.
    static void wait_until_finished_its_current_job()
    {
	while(!atd::is_one_bit<TWINT>::of(TWCR))
	{ }
    }

    /// Compone el SLA+W command
    static constexpr uint8_t SLA_W(uint8_t TWI_address)
    {
        return (TWI_address << 1);
    }

    /// Compone el SLA+R command
    static constexpr uint8_t SLA_R(uint8_t TWI_address)
    {
        return ((TWI_address << 1) | 0x01);
    }
};

 
// La fórmula para calcularlo viene en la datasheet (26.5.2):
//
//	f_scl = f_clock / (16 + 2*TWBR*prescaler)
//
// (1) Despejando obtenemos que
//	
//	TWBR * prescaler = f_clock / (2 * f_scl)  - 8
//
//	formula que podemos usar para calcular TWBR y prescaler.
//
// (2) Esa misma fórmula la podemos escribir como
//	
//	f_clock / f_scl = 16 * 2*TWBR*prescaler >= 16
//
//	==> f_clock >= 16*f_scl.
//
//	(de hecho la app-note AVR315 indica que la frecuencia del reloj tiene
//	del slave tiene que ser 16 veces mayor que f_scl)
//
namespace impl_of{

struct TWBR_prescaler{
    uint8_t twbr;
    uint8_t prescaler;
    constexpr TWBR_prescaler(uint8_t twbr0, uint8_t prescaler0)
	    : twbr{twbr0}, prescaler{prescaler0} {}
};

// Calculamos {TWBR, prescaler} para los valores (f_scl, f_clock)
template <uint16_t f_scl_in_kHz, uint32_t f_clock>
constexpr TWBR_prescaler TWI_TWBR_prescaler()
{
    constexpr uint32_t f_scl = uint32_t{f_scl_in_kHz} * 1000u;

    static_assert ((f_clock >=  16u * f_scl), 
		   "f_clock must be at least 16 times higher than the SCL frequency.");


    uint32_t T_p = f_clock / (2 * f_scl);

    T_p -= 8;

    if (T_p < 256) 
	return {static_cast<uint8_t>(T_p), 1};

    T_p /= 4;
    if (T_p  < 256)
	return {static_cast<uint8_t>(T_p), 4};

    T_p /= 4;
    if (T_p  < 256)
	return {static_cast<uint8_t>(T_p), 16};

    T_p /= 4;
    if (T_p  < 256)
	return {static_cast<uint8_t>(T_p), 64};

    return {0, 0}; // error
}   

}// impl_of
template <uint16_t f_scl, uint32_t f_clock>
inline void TWI_basic::SCL_frequency_in_kHz()
{
    constexpr
	auto res = impl_of::TWI_TWBR_prescaler<f_scl, f_clock>();

    if constexpr      (res.prescaler ==  1) bit_rate_prescaler_value_1();
    else if constexpr (res.prescaler ==  4) bit_rate_prescaler_value_4();
    else if constexpr (res.prescaler == 16) bit_rate_prescaler_value_16();
    else if constexpr (res.prescaler == 64) bit_rate_prescaler_value_64();
    else
	static_assert(atd::always_false_v<int>, "Program logic error.");

    TWBR = res.twbr;
}


}// namespace


#endif

