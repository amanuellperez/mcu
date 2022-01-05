// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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



struct Instructions_code
{
    static constexpr uint8_t null     = 0;
    static constexpr uint8_t unknown  = 255;

// Command edition
    static constexpr uint8_t first_cmd = 128;
    static constexpr uint8_t ac     = 128;
    static constexpr uint8_t del    = 130;
    static constexpr uint8_t up     = 131;
    static constexpr uint8_t down   = 132;
    static constexpr uint8_t right  = 133;
    static constexpr uint8_t left   = 134;
    static constexpr uint8_t last_cmd = 134;


// Abbrevations
    static constexpr uint8_t first_abb = 200;
    static constexpr uint8_t ans       = 200;
    static constexpr uint8_t last_abb = 200;

// symbols
    static constexpr uint8_t first_symbol = 210;
    static constexpr uint8_t sqrt	  = 210;
    static constexpr uint8_t last_symbol  = 210;
};

constexpr const char* abb2str[] = {"ANS"};


