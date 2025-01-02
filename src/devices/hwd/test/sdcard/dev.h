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

#ifndef __DEV_H__
#define __DEV_H__

#include <mega.h>

// microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;
			
#include "../../dev_sdcard.h"


// pines que usamos
// ----------------


// dispositivos que conectamos
// ---------------------------
struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = myu::hwd::Pin<n>;

// TODO:
//    static constexpr uint32_t frequency_in_hz = 500'000; // máx. 10MHz
};

using SPI_master = myu::SPI_master<SPI_master_cfg>;

// Dispositivos SPI
using Chip_select = 
    dev::SDCard_select<myu::hwd::Output_pin<myu::hwd::SPI::SS_pin>, 
						SPI_master>;

using SDCard_cfg = dev::SDCard_cfg<myu::Micro, SPI_master, Chip_select>;

using SDCard = dev::SDCard_basic<SDCard_cfg>;


#endif
