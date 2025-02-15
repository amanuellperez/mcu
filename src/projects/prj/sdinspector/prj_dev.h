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


/***************************************************************************
 *			    MICROCONTROLLER
 ***************************************************************************/
// atmega328p
// ----------
#ifdef IF_atmega328p
#include <mega.h>

namespace myu = atmega;

inline void init_mcu()
{}

using UART = myu::UART_8bits;


struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = myu::hwd::Pin<n>;

//    static constexpr uint32_t frequency_in_hz = 500'000; // máx. 10MHz
};

static constexpr uint32_t SPI_init_frequency = 250'000; // < 400kHz
static constexpr uint32_t SPI_frequency      = 500'000; // 500kHz < 25MHz

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

using SDCard = dev::hwd::SDCard<SDCard_cfg>;


// Sector_driver es el driver que se usa para acceder a un dispositivo físico
// que almacena la memoria en sectores (en este caso la SDCard).
// (RRR) ¿Por qué uso Sector_span?
//       Al querer implementar Volume::read(Boot_sector) como Boot_sector es
//       una struct necesito convertirla a un array de bytes. Este array de
//       bytes es el que paso a read.
//       Por otra parte, si se quiere leer un sector usaremos Sector que es un
//       std::array. Para leerlo necesito pasarlo a read.
//       std::span es una view de los arrays de C como de std::array
//       fusionando los dos interfaces.
struct Sector_driver{
    static constexpr uint16_t sector_size = 512;
    //using Sector = atd::Sector<sector_size>;
    using Sector = std::array<uint8_t, sector_size>;
    using Sector_span = std::span<uint8_t, sector_size>;

    static_assert(sector_size == SDCard::block_size);
    
    //static bool read(const SDCard::Address& add, Sector& sector)
    static bool read(const SDCard::Address& add, Sector_span sector)
    {
	auto r = SDCard::read(add, sector);
	return r.ok();
    }

    //static bool write(const SDCard::Address& add, Sector& sector)
    static bool write(const SDCard::Address& add, Sector_span sector)
    {
	auto r = SDCard::write(add, sector);
	return r.ok();
    }
};



#endif
