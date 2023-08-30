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

#include "dev_DS18B20.h"

namespace dev{

_Scratchpad::Celsius _Scratchpad::temperature(uint8_t T0, uint8_t T1,
								Resolution res)
{
    int32_t T = ((T0 & 0xF0) >> 4) | ((T1 & 0x0F) << 4);
    if (T1 & 0xF0)
	T = -T;
    
    int32_t T_dec = decimal_part_of_T(T0, res);

    auto rep = Celsius::Rep{T, T_dec};
    return Celsius{rep};
}



int32_t _Scratchpad::decimal_part_of_T(uint8_t T0, Resolution res)
{
    switch (res){
	break; case Resolution::bits_9 : return ((T0 & 0x0F) >> 3) * 5;
	break; case Resolution::bits_10: return ((T0 & 0x0F) >> 2) * 25; 
	break; case Resolution::bits_11: return ((T0 & 0x0F) >> 1) * 125;
	break; case Resolution::bits_12: return ((T0 & 0x0F) >> 0) * 625;
    }

    // El compilador da un warning si no pongo este return (???)
    return 1;
}

_Scratchpad::Resolution _Scratchpad::resolution() const
{
    switch (mask_cfg(data[4])){
	break; case 0: return Resolution::bits_9;
	break; case 1: return Resolution::bits_10;
	break; case 2: return Resolution::bits_11;
	break; case 3: return Resolution::bits_12;
    }

    // Aqui nunca puede llegar
    return Resolution::bits_12;
}


uint8_t _Scratchpad::to_cfg_register(Resolution res)
{
    uint8_t cfg = 0;

    switch(res){
	break; case Resolution::bits_9 : mask_cfg(cfg) = 0;
	break; case Resolution::bits_10: mask_cfg(cfg) = 1;
	break; case Resolution::bits_11: mask_cfg(cfg) = 2;
	break; case Resolution::bits_12: mask_cfg(cfg) = 3;
    }

    return cfg;

}


}// namespace
 
