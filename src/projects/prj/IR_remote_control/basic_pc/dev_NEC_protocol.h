// Copyright (C) 2022 Manuel Perez 
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

#ifndef __DEV_NEC_PROTOCOL_H__
#define __DEV_NEC_PROTOCOL_H__

#include <cstdint>
#include <ostream>

#include "dev_train_of_pulses.h"



namespace dev{

// Devuelve true si a = b +- 20%
// Esto es, si b - 20% <= a <= b + 20%
// (cojo el 20% ya que:
//	1. El clock del avr tiene un 10% de incertidumbre
//	2. Doy un 10% más de margen de error
// )
template <typename Int1, typename Int2>
inline bool is_equal(const Int1& a, const Int2& b)
{ return (b*0.8 <= a and a <= b*1.2); }

/***************************************************************************
 *			    NEC_message
 ***************************************************************************/
struct NEC_message{
// Header
    uint16_t time_first_pulse; // in us
		
// Payload
    uint8_t address;
    uint8_t inv_address;
    uint8_t command;
    uint8_t inv_command;

// Tail (???)
};



std::ostream& operator<<(std::ostream& out, const NEC_message& msg);


/***************************************************************************
 *			    NEC_protocol
 ***************************************************************************/
// Observar la asimetría al transmitir y recibir:
//  transmito directamente el mensaje NEC (= ptrain codificado) mientras que
//  para recibir recibo un ptrain y luego lo imprimo.
//  TODO: 
//	    print(NEC_message{ptrain});
//
// (RRR) ¿Por qué configuro esta clase con los tiempos que dura un 0 y un 1 si
//       en NEC estos tiempos están definidos?
//
//       NEC envia los datos usando pulse distance encoding lo cual es algo
//       genérico. Voy preparando esta clase para generalizarla.
//
class NEC_protocol{
public:
    // TODO: 
    // En principio el inv_address y el inv_command se calculan a partir de
    // address y command así que no son necesarios pasarlos. Pero no parece
    // que se cumpla siempre por eso hay que dar diferentes versiones. 
    // DUDA: otra opcion sería:
    //	    transmit(compon_NEC_message{addr, cmd});
    //	    transmit(compon_NEC_message{addr, inv_addr, cmd});
    //	y de esa forma solo damos una versión de transmit.
    // void transmit (typename Miniclock_us::counter_type time_first_burst_in_us, 
    //			uint8_t address, uint8_t command);
    // void transmit (typename Miniclock_us::counter_type time_first_burst_in_us, 
    //			uint8_t address, uint8_t inv_address, uint8_t command);
    template <typename Miniclock_us, typename SWG>
    static 
    void transmit (const NEC_message& msg);

    /// Convierte el tren de pulsos en el msg NEC. 
    /// NO llamar a esta función directamente. Forma de uso:
    ///		    atd::convert(pulse).into(msg);
    template <size_t N>
    static bool convert_x_into_y(const dev::Train_of_pulses<N>& pulse, 
						    NEC_message& msg);

    /// Si el ptrain codifica un mensaje NEC lo imprime en out, devolviendo
    /// true. En caso contrario no hace nada devolviendo false.
    /// Versión verbose: detalla todos los campos.
    template <size_t N>
    static bool print_verbose(std::ostream& out, 
					const dev::Train_of_pulses<N>& pulse);

    /// Idem que print_verbose pero más abreviado.
    template <size_t N>
    static bool print(std::ostream& out
				, const dev::Train_of_pulses<N>& pulse);
    
private:
// Cfg
    static constexpr uint16_t t_burst   = 562;

    // Duración de un 'logical 0'
    static constexpr uint16_t t_burst_0 = t_burst;
    static constexpr uint16_t t_space_0 = 562;
    static constexpr uint16_t t_period_0= t_burst + t_space_0;

    // Duración de un 'logical 1'
    static constexpr uint16_t t_burst_1 = t_burst;
    static constexpr uint16_t t_space_1 = 1688;
    static constexpr uint16_t t_period_1= t_burst + t_space_1;


// Funciones comunes
    // Un mensaje es un array de pulsos
    static bool is_start_pulse(const dev::Cycle& pulse);
    static char decode_period_as_char(const uint16_t T);

    template <typename Message_it>
    static Message_it look_for_start(Message_it p, Message_it pe);
    

// convert implementation
    template <typename Message_it>
    static Message_it convert_byte(uint8_t& y, Message_it p, Message_it pe);


// print implementation
    template <typename Message_it>
    static Message_it print_byte_in_binary(std::ostream& out, 
					    Message_it p, Message_it pe);

    template <typename Message_it>
    static Message_it print_byte_in_hex( std::ostream& out
				    , Message_it p, Message_it pe);

    template <typename Message_it>
    static Message_it print_byte(std::ostream& out,
						Message_it p0, Message_it pe);
};



}// namespace

#include "dev_NEC_convert.tcc"
#include "dev_NEC_print.tcc"
#include "dev_NEC_transmit.tcc"



#endif
