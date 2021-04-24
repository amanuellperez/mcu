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

// Calculamos las frecuencias que podemos generar con el Timer1

#include <alp_test.h>
#include <alp_string.h>

#include <array>
#include <iostream>


using namespace test;

constexpr std::array d = {1,8, 64, 256, 1024};
constexpr int n_max = 65535;

void run()
{
    int fc = 1000000;

    for (int fs = 1; fs < 5000; ++fs){
	std::cout << fs << ": ";
        int q = fc / fs;

	for (size_t i = 0; i < d.size(); ++i){
	    int res = q / d[i] - 1;
	    if (res > 0 and res < n_max)
		std::cout << res << "; ";
	    else
		std::cout << "---;";
	}


	std::cout << '\n';
    }
}


int main()
{
try{

    run();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





