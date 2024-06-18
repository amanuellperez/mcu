// Copyright (C) 2021 Manuel Perez 
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
#ifndef __SOUND_GENERATOR_CFG_H__
#define __SOUND_GENERATOR_CFG_H__

#include <stdint.h>
#include <array>

#include <avr_types.h>
#include <atd_cast.h>

using namespace avr::literals;

constexpr uint8_t time_wait_release_key = 100; // in ms
constexpr uint8_t time_scan_keyboard    = 100; // in ms

constexpr uint8_t CANCEL_KEY  = 'a'; // para evitar conflicto con los 
				     // Keyboard_codes los defino como ascii

// El 00 indica que la primera pulsación genera trigger.
using metronome_d1 = atd::static_array<uint8_t, 0, 9, 9>;
using metronome_d0 = atd::static_array<uint8_t, 0, 1, 0>;

constexpr avr::Frequency sound_generator_freq0 = avr::frequency_in_Hz(440);

#endif


