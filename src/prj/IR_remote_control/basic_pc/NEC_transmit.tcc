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

#ifndef __NEC_TRANSMIT_TCC__
#define __NEC_TRANSMIT_TCC__

// OK: I have a problem with my english. Which one is better: transmit or send
// an IR message? Now it sounds better `send`, but I don't know.

#include "NEC_protocol.h"

namespace dev{

template <typename Clock_us0,
	  typename SWG0,
	  uint16_t t_burst_00, uint16_t t_space_00,
	  uint16_t t_burst_10, uint16_t t_space_10>
struct NEC_transmitter_cfg_{
    using Clock_us = Clock_us0;
    using SWG      = SWG0;

    static constexpr uint16_t t_burst_0 = t_burst_00;
    static constexpr uint16_t t_space_0 = t_space_00;

    // Duración de un 'logical 1'
    static constexpr uint16_t t_burst_1 = t_burst_10;
    static constexpr uint16_t t_space_1 = t_space_10;
};


template <typename Cfg>
class NEC_transmitter_{
public:
    using Clock_us = Cfg::Clock_us;
    using SWG      = Cfg::SWG;

    static void transmit(const NEC_message& msg);

private:
// Cfg
    // Duración de un 'logical 0'
    static constexpr uint16_t t_burst_0 = Cfg::t_burst_0;
    static constexpr uint16_t t_space_0 = Cfg::t_space_0;

    // Duración de un 'logical 1'
    static constexpr uint16_t t_burst_1 = Cfg::t_burst_1;
    static constexpr uint16_t t_space_1 = Cfg::t_space_1;

// Functions
    static void transmit_byte(uint8_t b);
    static void transmit_one(const NEC_message& msg);
};


// inline porque no se si es suficientemente eficiente
template <typename Cfg>
inline void NEC_transmitter_<Cfg>::transmit_byte(uint8_t b)
{
    for (uint8_t i = 0; i < 8; ++i){
	if (b & 0x01){
	    SWG::burst_38kHz_of(t_burst_1);
	    SWG::wait_us(t_space_1);
	}
	else{
	    SWG::burst_38kHz_of(t_burst_0);
	    Clock_us::wait_us(t_space_0);
	}

	b >>= 1;
    }
}


template <typename Cfg>
void NEC_transmitter_<Cfg>::transmit_one(const NEC_message& msg)
{
// Header
    SWG::burst_38kHz_of(msg.time_first_pulse);
    Clock_us::wait_us(4500);

// Payload
    transmit_byte(msg.address);
    transmit_byte(msg.inv_address);
    transmit_byte(msg.command);
    transmit_byte(msg.inv_command);

// Tail
    SWG::burst_38kHz_of(562);

}


template <typename Cfg>
void NEC_transmitter_<Cfg>::transmit(const NEC_message& msg)
{
    Clock_us::on();
//    for (uint8_t i = 0; i < 3; ++i){
	transmit_one(msg);
//	timer_wait_us(30000);
//    }
    Clock_us::off();
}


template <typename Clock_us, typename SWG>
void NEC_protocol::transmit(const NEC_message& msg)
{
    using Cfg = NEC_transmitter_cfg_<Clock_us, SWG, 
				     t_burst_0, t_space_0, 
				     t_burst_1, t_space_1>;

    NEC_transmitter_<Cfg>::transmit(msg);
}


}// namespace


#endif


