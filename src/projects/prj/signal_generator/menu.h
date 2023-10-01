// Copyright (C) Manuel Perez 
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


// Automatically generated file. To change this menu change .mnu

#pragma once

#ifndef __MENU_H__
#define __MENU_H__

#include <user_menu.h>
#include "main.h"
constexpr const char* menu_str[] =
{"SW generator","Sound generator","cfg"};


constexpr size_t menu_main0= 0;
constexpr size_t menu_main1= 1;
constexpr size_t menu_main2= 2;



using Option = dev::Option<Main>;

using Options = atd::static_array<Option
	, Option{0, menu_main0, &Main::window_sw_generator, 0}
	, Option{0, menu_main1, &Main::sound_main, 0}
	, Option{0, menu_main2, &Main::window_cfg, 0}
	>;
#endif
