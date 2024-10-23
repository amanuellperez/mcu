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

#include <avr_atmega.h>

#include "dev_sector.h"

// microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;
			
#include <dev_sdcard.h>


// pines que usamos
// ----------------


// dispositivos que conectamos
// ---------------------------
// Dispositivos SPI
using Chip_select = 
    dev::SDCard_select<my_mcu::Output_pin<my_mcu::SPI::noCS_pin_number>, my_mcu::SPI_master>;

using SDCard_cfg = dev::SDCard_cfg<my_mcu::Micro, my_mcu::SPI_master, Chip_select>;

using SDCard = dev::SDCard_basic<SDCard_cfg>;

using Sector_cfg = dev::Sector_cfg<SDCard::block_size>;
using Sector     = dev::Sector<Sector_cfg>;

#endif
