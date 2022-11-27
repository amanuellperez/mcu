// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __IR_NEC_PROTOCOL_H__
#define __IR_NEC_PROTOCOL_H__

#include "IR_NEC_message.h"
#include "dev_train_of_pulses.h"

// Observar la asimetría al transmitir y recibir:
//  transmito directamente el mensaje NEC (= ptrain codificado) mientras que
//  para recibir recibo un ptrain y luego lo imprimo.
//  TODO: 
//	    print(NEC_message{ptrain});
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
    // void transmit (typename Clock_us::counter_type time_first_burst_in_us, 
    //			uint8_t address, uint8_t command);
    // void transmit (typename Clock_us::counter_type time_first_burst_in_us, 
    //			uint8_t address, uint8_t inv_address, uint8_t command);
    template <typename Clock_us, typename SWG>
    static 
    void transmit (typename Clock_us::counter_type time_first_burst_in_us, 
			    const NEC_message& msg);

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
// Funciones comunes
    // Un mensaje es un array de pulsos
    static bool is_start_pulse(const dev::Cycle& pulse);
    static char pulse_to_bit(const uint16_t T);

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



#include "IR_NEC_convert.tcc"
#include "IR_NEC_print.tcc"
#include "IR_NEC_transmit.tcc"



#endif
