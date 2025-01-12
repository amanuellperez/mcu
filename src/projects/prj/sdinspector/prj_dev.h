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

#ifndef __PRJ_DEV_H__
#define __PRJ_DEV_H__

#include <mega.h>
#include <mcu_SPI.h>
#include <dev_sdcard.h>

#include "dev_sector.h"

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;
			


// pines que usamos
// ----------------


// dispositivos que conectamos
// ---------------------------
// Dispositivos SPI

struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = myu::hwd::Pin<n>;

//    static constexpr uint32_t frequency_in_hz = 500'000; // máx. 10MHz
};

using SPI = myu::SPI_master<SPI_master_cfg>;

struct SDCard_cfg{
    using Micro = myu::Micro;
    using SPI = ::SPI;
    using SPI_select = mcu::SPI_pin_selector<Micro, SPI::SS_pin>;
    static constexpr uint32_t SPI_frequency      = 500'000;

    // using log = Log<UART_iostream>;
};

using SDCard = dev::hwd::SDCard<SDCard_cfg>;

using Sector_cfg = dev::Sector_cfg<SDCard::block_size>;
using Sector     = dev::Sector<Sector_cfg>;

#endif
