//// Copyright (C) 2021 Manuel Perez 
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

#include <iostream> // TODO: orden
#include "../../atd_double.h"

#include <alp_test.h>
#include <alp_string.h>
#include <string>   
#include <math.h>

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




//void test_mantissa_to_str(const double& x, const std::string& res)
//{
//    std::array<char, 5> str;
//    auto [p0, pe] = atd::mantissa_to_str(x, str.begin(), str.end());
//    CHECK_TRUE((std::string{p0, pe} == res), "mantissa_to_str");
//}
//
//void test_mantissa_to_str()
//{
//    test::interface("mantissa_to_str");
//
//    test_mantissa_to_str(0, "0");
//    test_mantissa_to_str(0.1234, "1234");
//    test_mantissa_to_str(0.0001, "0001");
//    test_mantissa_to_str(0.0102, "0102");
//    test_mantissa_to_str(0.30045, "30045");
//
//}



void test_modf(double x, double i1, double f1)
{
    auto [i, f] = atd::modf(x);
std::cout << x << "->" << i << '@' << f << '\n';
    CHECK_TRUE(equal(i, i1) and equal(f, f1), "modf");
}

void test_modf()
{
    test::interface("modf");


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
    std::cout << " =? 12.45\n\tValor real = [" << std::setprecision(20) << x << "]\n";

    atd::print(std::cout, 3.8);
    std::cout << " =? 3.8\n\tValor real = [" << std::setprecision(20) << 3.8 << "]\n";

    atd::print(std::cout, 8);
    std::cout << " =? 8\n\tValor real = [" << std::setprecision(20) << 8 << "]\n";

    {// bug: esto funciona bien en g++, pero falla en avr-g++!!!
	volatile double a = 2;
	volatile double b = 8;
	double c = pow(a, b);
	atd::print(std::cout, c);
        std::cout << " =? 256\n\tValor real = [" << std::setprecision(20) << c
                  << "]\n";
    }
}

//void test_remove_trailing_zeros(char x0, char x1, char x2, char x3,
//                                const std::string& res)
//{
//    std::array<char, 4> str{x0, x1, x2, x3};
//    auto [p0, pe] = atd::__remove_trailing_zeros(str.begin(), str.end());
//    CHECK_TRUE((std::string{p0, pe} == res), "__remove_trailing_zeros");
//}
//
//void test_remove_trailing_zeros()
//{
//    test::interface("__remove_trailing_zeros");
//
//    test_remove_trailing_zeros('1', '2', '3', '4', "1234"); 
//    test_remove_trailing_zeros('0', '0', '0', '0', "0");
//
//    test_remove_trailing_zeros('1', '0', '0', '0', "1");
//    test_remove_trailing_zeros('0', '1', '2', '0', "012");
//    test_remove_trailing_zeros('1', '2', '0', '0', "12");
//    test_remove_trailing_zeros('1', '0', '2', '0', "102");
//}

void test_to_string(double x, const std::string& res)
{
    std::array<char, 20> str;

    auto [p0, pe] = atd::to_string<10>(x, str.begin(), str.end());
    CHECK_TRUE((std::string{p0, pe} == res), "to_string");
}

template <size_t ndecimals>
void test_to_string_with_zeros(double x, const std::string& res)
{// double_to_string
    std::array<char, 20> str;

    auto p0 = atd::to_string_with_zeros<ndecimals>(x, str.begin(), str.end());
    CHECK_TRUE((std::string{p0, str.end()} == res), "to_string_with_zeros");

}

template <size_t ndecimals>
void test_to_string_without_zeros(double x, const std::string& res)
{// double_to_string
    std::array<char, 20> str;

    auto p0 = atd::to_string_without_zeros<ndecimals>(x, str.begin(), str.end());
//std::cout << std::string{p0, str.end()} << " =? " << res << '\n';
    CHECK_TRUE((std::string{p0, str.end()} == res), "to_string_without_zeros");

}

void test_to_string()
{
    test::interface("to_string");

    test_to_string(1.23, "1.23");
    test_to_string(-1.03, "-1.03");
    test_to_string(0.23, "0.23");
    test_to_string(0.023, "0.023");
    test_to_string(-0.023, "-0.023");
    test_to_string(12.0, "12");
    test_to_string(23, "23");
    test_to_string(0, "0");

// bug
    test_to_string(7.99999999999, "8");
}


void test_to_string_with_zeros()
{
    test::interface("to_string_with_zeros");

    test_to_string_with_zeros<2>(234.1999, "234.20");
    test_to_string_with_zeros<2>(234, "234.00");
    test_to_string_with_zeros<2>(234.019, "234.02");
}

void test_to_string_without_zeros()
{
    test::interface("to_string_without_zeros");
    test_to_string_without_zeros<2>(234.1999, "234.2");
    test_to_string_without_zeros<2>(234, "234");


}


int main()
{
try{
    test::header("double");
    
//    test_remove_trailing_zeros();
//    test_mantissa_to_str();
    test_modf();

    test_to_string_with_zeros();
    test_to_string_without_zeros();
    test_to_string();

    test_print();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





