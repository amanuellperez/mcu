// Copyright (C) 2023-2025 Manuel Perez 
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

#include <mcu_SPI.h>
#include <dev_sdcard.h>

#include "dev_sector_driver.h"

#include "atd_fat.h"

// Niveles de traza usados (borrador)
// 1: Error crítico: fallo de comunicación, error de hardware.
// 2: Error: no hay espacio en disco duro, ...
// 3: Error: no se ha podido hacer algo pero se puede continuar aunque quedará
// todo inconsistente.
// 5: warnings
// 7: precondiciones
// 9: traza para depurar, borrarlas al final.



/***************************************************************************
 *			    MICROCONTROLLER
 ***************************************************************************/
// atmega328p
// ----------
#ifdef IF_atmega328p
#error "Don't use atmega32! You need more RAM!"
//#include <mega.h>
//
//namespace myu = atmega;
//
//inline void init_mcu()
//{}
//
//using UART = myu::UART_8bits;
//
//
//struct SPI_master_cfg{
//    template <uint8_t n>
//    using Pin = myu::hwd::Pin<n>;
//
////    static constexpr uint32_t frequency_in_hz = 500'000; // máx. 10MHz
//};
//
//static constexpr uint32_t SPI_init_frequency = 250'000; // < 400kHz
//static constexpr uint32_t SPI_frequency      = 500'000; // 500kHz < 25MHz

// atmega4809
// ----------
#elifdef IF_atmega4809

#include <mega0.h>

namespace myu = atmega4809;

inline void init_mcu()
{
    myu::init();
//    myu::Clock_controller::clk_main_divided_by_16(); // a 1 MHz
}

using UART = myu::UART1_8bits;


struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = myu::hwd::Pin<n>;

//    static constexpr uint8_t prescaler = 16;
//    static constexpr uint32_t frequency_in_hz = myu::clk_per() / prescaler; // máx. 10MHz
    static constexpr auto mode = myu::SPI_master_cfg::normal_mode;
};

							
// clk_per == 20MHz / 6
static_assert(myu::clk_per() == 20'000'000 / 6);
static constexpr uint32_t SPI_init_frequency = myu::clk_per() / 16; // < 400kHz
static constexpr uint32_t SPI_frequency = myu::clk_per() / 2; // máx.  25 MHz
#endif


/***************************************************************************
 *				DEVICES
 ***************************************************************************/
// Microcontroller
// ---------------
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<UART>;

// Flujo de entrada/salida por defecto
// (DUDA) ¿cómo definir cout/cin siendo el mismo flujo uart?
extern UART_iostream uart;


// pines que usamos
// ----------------


// dispositivos que conectamos
// ---------------------------
using SPI = myu::SPI_master<SPI_master_cfg>;

// Dispositivos SPI

struct SDCard_cfg{
    using Micro = myu::Micro;
    using SPI = ::SPI;
    using SPI_select = mcu::SPI_pin_selector<Micro, SPI::SS_pin>;
    static constexpr uint32_t SPI_frequency      = ::SPI_frequency;

    // using log = Log<UART_iostream>;
};

// Tipos usados
using SDCard = dev::hwd::SDCard<SDCard_cfg>;
using Sector_driver = dev::Sector_driver<SDCard>;

using Volume = atd::FAT32::Volume<Sector_driver>;
using Directory = atd::FAT32::Directory<Sector_driver>;
using Entry      = Directory::Entry;
using Entry_info = Directory::Entry_info;
using Attribute = Directory::Attribute;
using File = atd::FAT32::File<Sector_driver>;


#endif
