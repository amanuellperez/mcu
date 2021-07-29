// Copyright (C) 2021 A.Manuel L.Perez 
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

#include "../../atd_double.h"

#include <alp_test.h>
#include <alp_string.h>
#include <iostream>
#include <string>   


using namespace test;

inline bool equal(double a, double b)
{
    if (a > b)
	return (a-b < 0.01);

    return (b-a < 0.01);
}


template <typename It>
void print(It p, It pe)
{
    for(; p != pe; ++p)
	std::cout << *p;
}




void test_mantissa_to_str(const double& x, const std::string& res)
{
    std::array<char, 5> str;
    auto [p0, pe] = atd::mantissa_to_str(x, str.begin(), str.end());
    CHECK_TRUE((std::string{p0, pe} == res), "mantissa_to_str");
}

void test_mantissa_to_str()
{
    test::interfaz("mantissa_to_str");

    test_mantissa_to_str(0, "0");
    test_mantissa_to_str(0.1234, "1234");
    test_mantissa_to_str(0.0001, "0001");
    test_mantissa_to_str(0.0102, "0102");
    test_mantissa_to_str(0.30045, "30045");

}



void test_modf(double x, double i1, double f1)
{
    auto [i, f] = atd::modf(x);
std::cout << x << "->" << i << '@' << f << '\n';
    CHECK_TRUE(equal(i, i1) and equal(f, f1), "modf");
}

void test_modf()
{
    test::interfaz("modf");


// normal
    test_modf(2.354, 2, 0.35);
    test_modf(2.354, 2, 0.354);
    test_modf(2.354, 2, 0.354);
    test_modf(2.354, 2, 0.354);

    test_modf(2.354, 2, 0.35);
    test_modf(2.354, 2, 0.354);
    test_modf(2.354, 2, 0.354);
    test_modf(2.354, 2, 0.354);

    test_modf(-2.354, -2, -0.354); // CUIDADO con los menos!!!
}



void test_print()
{
    double x = 12.45;
    atd::print(std::cout, x);
    std::cout << " =? " << std::setprecision(20) << x << '\n';
}

void test_remove_trailing_zeros(char x0, char x1, char x2, char x3,
                                const std::string& res)
{
    std::array<char, 4> str{x0, x1, x2, x3};
    auto [p0, pe] = atd::__remove_trailing_zeros(str.begin(), str.end());
    CHECK_TRUE((std::string{p0, pe} == res), "__remove_trailing_zeros");
}

void test_remove_trailing_zeros()
{
    test::interfaz("__remove_trailing_zeros");

    test_remove_trailing_zeros('1', '2', '3', '4', "1234"); 
    test_remove_trailing_zeros('0', '0', '0', '0', "0");

    test_remove_trailing_zeros('1', '0', '0', '0', "1");
    test_remove_trailing_zeros('0', '1', '2', '0', "012");
    test_remove_trailing_zeros('1', '2', '0', '0', "12");
    test_remove_trailing_zeros('1', '0', '2', '0', "102");
}

void test_to_string(double x, const std::string& res)
{
    std::array<char, 20> str;

    auto [p0, pe] = atd::to_string(x, str.begin(), str.end());
// std::cout << x << " --> [" << std::string{p0, pe} << "]\n";
    CHECK_TRUE((std::string{p0, pe} == res), "to_string");
}


void test_to_string()
{
    test::interfaz("to_string");

    test_to_string(1.23, "1.23");
    test_to_string(-1.03, "-1.03");
    test_to_string(0.23, "0.23");
    test_to_string(12.0, "12");
    test_to_string(23, "23");
    test_to_string(0, "0");

}


int main()
{
try{
    test::header("double");
    
    test_remove_trailing_zeros();
    test_mantissa_to_str();
    test_modf();
    test_print();
    test_to_string();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





