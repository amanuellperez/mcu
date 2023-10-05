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

#include "../../std_iterator_concepts.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <vector>
#include <list>
#include <map>

// CUIDADO!!!
// Estos iteradores son iteradores de std y no de mtd.
// Uno de los criterios que se usa para ver que se garantiza el concept es que
// esté generado por la primary template, que en el caso de std y mtd son
// DISTINTAS!!! Luego no se puede usar el mismo método usado para verificar el
// resto de programas. No puedo comparar el comportamiento de mtd con std para
// ver si está bien.
using It0 = int;
constexpr const char*  it0 = "int";

using It1 = std::istream_iterator<int>;
constexpr const char*  it1 = "istream_iterator";

using It2 = std::ostream_iterator<int>;
constexpr const char*  it2 = "ostream_iterator";

using It3 = std::vector<int>::iterator;
constexpr const char*  it3 = "vector::iterator";

using It4 = std::list<int>::iterator;
constexpr const char*  it4 = "list::iterator";

using It5 = std::map<int, int>::iterator;
constexpr const char*  it5 = "map::iterator";


using namespace test;

void print_truefalse(bool x, const std::string& msg)
{
    std::cout << msg << "? ";

    if (x)
	std::cout << "true\n";
    else
	std::cout << "false\n";
}

template <typename I>
void test_input_or_output_iterator(const std::string& name_type)
{
    CHECK_TRUE(mtd::input_or_output_iterator<I> == 
	       std::input_or_output_iterator<I>, name_type);
}

void test_input_or_output_iterator()
{
    test::interface("input_or_output_iterator");

    test_input_or_output_iterator<It0>(it0);
    test_input_or_output_iterator<It1>(it1);
    test_input_or_output_iterator<It2>(it2);
    test_input_or_output_iterator<It3>(it3);
    test_input_or_output_iterator<It4>(it4);
    test_input_or_output_iterator<It5>(it5);
}

// Esto FIJO va a dar error. No usarlo!!!
//template <typename I>
//void test_input_iterator(const std::string& name_type)
//{
//    CHECK_TRUE(mtd::input_iterator<I> == 
//	       std::input_iterator<I>, name_type);
//}

void test_input_iterator()
{
    test::interface("input__iterator");
}

template <typename I>
void test_incrementable(const std::string& name_type)
{
    CHECK_TRUE(mtd::incrementable<I> == 
	       std::incrementable<I>, name_type);
}

void test_incrementable()
{
    test::interface("incrementable");

    test_incrementable<It0>(it0);
    test_incrementable<It1>(it1);
    test_incrementable<It2>(it2);
    test_incrementable<It3>(it3);
    test_incrementable<It4>(it4);
    test_incrementable<It5>(it5);
}


template <typename I>
void test_indirectly_readable(const std::string& name_type)
{
    CHECK_TRUE(mtd::indirectly_readable<I> == 
	       std::indirectly_readable<I>, name_type);
}

void test_indirectly_readable()
{
    test::interface("indirectly_readable");

    test_indirectly_readable<It0>(it0);
    test_indirectly_readable<It1>(it1);
    test_indirectly_readable<It2>(it2);
    test_indirectly_readable<It3>(it3);
    test_indirectly_readable<It4>(it4);
    test_indirectly_readable<It5>(it5);
}




int main()
{
try{
    test::header("std_iterator_concepts.h");

    // ???
    // ---
    test_indirectly_readable();
    test_incrementable();
    // test_sentinel_for();
    // test_sized_sentinel_for();

    // iterators
    // ---------
    test_input_or_output_iterator();
    // test_input_iterator();
    // ...

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





