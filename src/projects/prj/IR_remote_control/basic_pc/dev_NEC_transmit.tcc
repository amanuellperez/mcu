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

#ifndef __NEC_TRANSMIT_TCC__
#define __NEC_TRANSMIT_TCC__

// OK: I have a problem with my english. Which one is better: transmit or send
// an IR message? Now it sounds better `send`, but I don't know.

#include "dev_NEC_protocol.h"

namespace dev{

template <typename Miniclock_us0,
	  typename SWG0,
	  uint16_t t_burst_00, uint16_t t_space_00,
	  uint16_t t_burst_10, uint16_t t_space_10>
struct NEC_transmitter_cfg_{
    using Miniclock_us = Miniclock_us0;
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
    using Miniclock_us = Cfg::Miniclock_us;
    using SWG          = Cfg::SWG;

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
    static void transmit_byte(SWG& swg, uint8_t b);
    static void transmit_one(const NEC_message& msg);

    static constexpr uint16_t frequency = 38000; // 38 kHz
};


// inline porque no se si es suficientemente eficiente
template <typename Cfg>
inline void NEC_transmitter_<Cfg>::transmit_byte(SWG& swg, uint8_t b)
{

    for (uint8_t i = 0; i < 8; ++i){
	if (b & 0x01){
	    swg.generate_burst(t_burst_1);
	    Miniclock_us::wait(t_space_1);
	}
	else{
	    swg.generate_burst(t_burst_0);
	    Miniclock_us::wait(t_space_0);
	}

	b >>= 1;
    }
}

// Transmite 1 solo mensaje. 
// En general puede que sea interesante retransmitir el mismo mensaje varias
// veces seguidas, de ahí la diferencia entre transmit y transmit_one
template <typename Cfg>
void NEC_transmitter_<Cfg>::transmit_one(const NEC_message& msg)
{
    SWG swg{frequency};

// Header
    swg.generate_burst(msg.time_first_pulse);
    Miniclock_us::wait(4500);

// Payload
    transmit_byte(swg, msg.address);
    transmit_byte(swg, msg.inv_address);
    transmit_byte(swg, msg.command);
    transmit_byte(swg, msg.inv_command);

// Tail
    swg.generate_burst(562);

}


template <typename Cfg>
void NEC_transmitter_<Cfg>::transmit(const NEC_message& msg)
{
    Miniclock_us::start();
//    for (uint8_t i = 0; i < 3; ++i){
	transmit_one(msg);
//	timer_wait_us(30000);
//    }
    Miniclock_us::stop();
}


template <typename Miniclock_us, typename SWG>
void NEC_protocol::transmit(const NEC_message& msg)
{
    using Cfg = NEC_transmitter_cfg_<Miniclock_us, SWG, 
				     t_burst_0, t_space_0, 
				     t_burst_1, t_space_1>;

    NEC_transmitter_<Cfg>::transmit(msg);
}


}// namespace


#endif


