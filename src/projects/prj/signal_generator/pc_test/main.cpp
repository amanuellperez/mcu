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

#include "../metronome.h"

#include <alp_test.h>
#include <iostream>

#include <atd_static.h>
#include <atd_eng_magnitude.h>

using namespace test;


void test_metronome()
{
    using D1 = atd::static_array<uint8_t, 1, 2>;
    using D0 = atd::static_array<uint8_t, 3, 1>;
    using Metronome_ints =  Metronome<int, D1, D0>;
    using Metronome_freq =  Metronome<atd::ENG_frequency<uint16_t>, D1, D0>;

{
    const std::string res = "---1"
                            "---1"
                            "-1"
                            "-1"
                            "-1"
                            "---10"
                            "---10"
                            "-10"
                            "-10"
                            "-10"
                            "---100"
                            "---100"
                            "-100"
                            "-100"
                            "-100"
                            "100"
                            "100";

    Metronome_ints m;
    std::stringstream out;
    for (int i = 0; i < 44; ++i){
	if (m.trigger())
	    out << m.incr();
	else
	    out << '-';
	m.tick();
    }
    CHECK_TRUE(out.str() == res, "Metronome_ints");
}
{
    const std::string res = "---1"
                            "---1"
                            "-1"
                            "-1"
                            "-1"
                            "---10"
                            "---10"
                            "-10"
                            "-10"
                            "-10"
                            "---100"
                            "---100"
                            "-100"
                            "-100"
                            "-100"
                            "100"
                            "100";

    Metronome_freq m;
    std::stringstream out;
    for (int i = 0; i < 44; ++i){
	if (m.trigger())
	    out << m.incr().value();
	else
	    out << '-';
	m.tick();
    }
    CHECK_TRUE(out.str() == res, "Metronome_freq");
}
}


int main()
{
try{
    test::header("metronome");
    
    test_metronome();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}

