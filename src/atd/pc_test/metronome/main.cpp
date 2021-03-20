// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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



int main()
{
try{
    test::header("atd_metronome");

    test_basic();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





