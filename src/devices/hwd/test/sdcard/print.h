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

#ifndef __PRINT_H__
#define __PRINT_H__

#include "dev.h"

void print(std::ostream& out, const SDCard::R1& r1);
void print_if_error_r1(std::ostream& out, const SDCard::R1& r1);
void print_raw_R3(std::ostream& out, const SDCard::R3& r3);
void print_type_card(std::ostream& out, const SDCard::R3& r3);
void print(std::ostream& out, const SDCard::R3& r3);
void print(std::ostream& out, const SDCard::R7& r7);
void print(std::ostream& out, const SDCard::Read_return& r);
void print(std::ostream& out, const SDCard::R2& r);
void print(std::ostream& out, const SDCard::Write_return& r);
void print(std::ostream& out, const SDCard::Data_response_token& r);


template <typename String>
void print_question(std::ostream& out, const String& str, bool with_tab = true)
{
    if (with_tab)
	out << '\t';

    atd::print(out, str);
    out << "? ";
}

#endif


