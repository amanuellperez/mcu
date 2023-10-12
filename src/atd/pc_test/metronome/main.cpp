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

#include "../../atd_metronome.h"
#include "../../atd_static.h"

#include <alp_test.h>
#include <iostream>



using namespace test;


template <typename d1_max, typename d0_max>
void test_basic(const std::string& res)
{
    atd::Metronome<d1_max, d0_max> met;

    std::stringstream out;
    for (size_t i = 0; i < res.size(); ++i){
	if(met.end())
	    out << 'E';
	else if (met.trigger())
	    out << 'T';
	else
	    out << '-';

	met.tick();
    }

    CHECK_TRUE(out.str() == res, "Metronome");
}


void test_basic()
{    
{
    using d1_max = atd::static_array<uint8_t, 2, 4>;
    using d0_max = atd::static_array<uint8_t, 3, 6>;

    const std::string res = "---T"
                            "---T"
                            "---T"
                            "------T"
                            "------T"
                            "------T"
                            "------T"
                            "------E"
                            "EEE";
    test_basic<d1_max, d0_max>(res);
}
{
    using d1_max = atd::static_array<uint8_t, 2, 2, 3>;
    using d0_max = atd::static_array<uint8_t, 3, 1, 0>;

    const std::string res = "---T"
                            "---T"
                            "---T"
                            "-T"
                            "-T"
                            "-T"
                            "T"
                            "T"
                            "T"
                            "EEE";
    test_basic<d1_max, d0_max>(res);
}

}

void test_sequence_of_steps()
{
    test::interface("static_Sequence_of_steps");

    using array_x = atd::static_array<uint8_t, 0, 2, 5>;
    using array_y = atd::static_array<uint8_t, 1, 3, 7>;

    using Seq = atd::static_Sequence_of_steps<array_x, array_y>;

    std::array<uint8_t, 20> res = {1,1,3,3,3,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
    Seq seq;
    for (size_t i = 0; i < 20; ++i){
	CHECK_TRUE(res[i] == seq.value(), "Sequence_of_steps");
	seq.next();
    }

// ------------
{
    test::interface("Sequence_of_steps");

    std::array<uint8_t, 3> x{0, 2, 5};
    std::array<uint8_t, 3> y{1, 3, 7};

    atd::Sequence_of_steps seq(x, y);
    std::array<uint8_t, 20> res = {1,1,3,3,3,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
    for (size_t i = 0; i < 20; ++i){
	CHECK_TRUE(res[i] == seq.value(), "Sequence_of_steps");
	seq.next();
    }
}
}

int main()
{
try{
    test::header("atd_metronome");

    test_basic();
    test_sequence_of_steps();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





