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
constexpr const char* menu_str[] =
{"a first option","a second option","a third option","b1 - one","b2 - two","c1 ; 1","c2 ; 2","c3 ; 3","d1","d2"};


constexpr size_t menu_a0= 0;
constexpr size_t menu_a1= 1;
constexpr size_t menu_a2= 2;
constexpr size_t menu_b0= 3;
constexpr size_t menu_b1= 4;
constexpr size_t menu_c0= 5;
constexpr size_t menu_c1= 6;
constexpr size_t menu_c2= 7;
constexpr size_t menu_d0= 8;
constexpr size_t menu_d1= 9;



using Option = dev::Option<Main>;

using Options = atd::static_array<Option
	, Option{0, menu_a0, nullptr, 1}
	, Option{0, menu_a1, &Main::a2, 0}
	, Option{0, menu_a2, nullptr, 3}
	, Option{1, menu_b0, &Main::b1, 0}
	, Option{1, menu_b1, nullptr, 2}
	, Option{2, menu_c0, &Main::c1, 0}
	, Option{2, menu_c1, &Main::c2, 0}
	, Option{2, menu_c2, &Main::c3, 0}
	, Option{3, menu_d0, &Main::d1, 0}
	, Option{3, menu_d1, &Main::d2, 0}
	>;
#endif
