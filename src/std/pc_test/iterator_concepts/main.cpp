// Copyright (C) 2023-2024 Manuel Perez 
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

#include "../../std_array.h"
#include "../../std_string_view.h"

#include <alp_test.h>
#include <alp_string.h>

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <iterator>

// CUIDADO!!!
// Estos iteradores son iteradores de std y no de mtd.
// Uno de los criterios que se usa para ver que se garantiza el concept es que
// esté generado por la primary template, que en el caso de std y mtd son
// DISTINTAS!!! Luego no se puede usar el mismo método usado para verificar el
// resto de programas. No puedo comparar el comportamiento de mtd con std para
// ver si está bien.

using std_it1 = std::istream_iterator<int>;
constexpr const char*  std_str_it1 = "istream_iterator";

using std_it2 = std::ostream_iterator<int>;
constexpr const char*  std_str_it2 = "ostream_iterator";

using std_it3 = std::vector<int>::iterator;
constexpr const char*  std_str_it3 = "vector::iterator";

using std_it4 = std::list<int>::iterator;
constexpr const char*  std_str_it4 = "list::iterator";

using std_it5 = std::map<int, int>::iterator;
constexpr const char*  std_str_it5 = "map::iterator";


// Estos iteradores son los que funcionan en mtd
using mtd_it0 = char*;
constexpr const char*  mtd_str_it0 = "char*";

using mtd_it1 = const char*;
constexpr const char*  mtd_str_it1 = "const char*";

using mtd_it2 = mtd::array<int,2>::iterator;
constexpr const char*  mtd_str_it2 = "mtd::array";

using mtd_it3 = mtd::string_view::const_iterator;
constexpr const char*  mtd_str_it3 = "mtd::string_view";

using namespace test;

void print_truefalse(bool x, const std::string& msg)
{
    std::cout << msg << "? ";

    if (x)
	std::cout << "true\n";
    else
	std::cout << "false\n";
}

//template <typename I>
//void test_input_or_output_iterator(const std::string& name_type)
//{
//    CHECK_TRUE(mtd::input_or_output_iterator<I> == 
//	       std::input_or_output_iterator<I>, name_type);
//}

void test_input_or_output_iterator()
{
    test::interface("input_or_output_iterator");

    CHECK_TRUE(mtd::input_or_output_iterator<mtd_it0>, mtd_str_it0);
    CHECK_TRUE(mtd::input_or_output_iterator<mtd_it1>, mtd_str_it1);
    CHECK_TRUE(mtd::input_or_output_iterator<mtd_it2>, mtd_str_it2);
    CHECK_TRUE(mtd::input_or_output_iterator<mtd_it3>, mtd_str_it3);
}

//template <typename I>
//void test_input_iterator(const std::string& name_type)
//{
//    CHECK_TRUE(mtd::input_iterator<I> == 
//	       std::input_iterator<I>, name_type);
//}

void test_input_iterator()
{
    test::interface("input_iterator");

    CHECK_TRUE(mtd::input_iterator<mtd_it0>, mtd_str_it0);
    CHECK_TRUE(mtd::input_iterator<mtd_it1>, mtd_str_it1);
    CHECK_TRUE(mtd::input_iterator<mtd_it2>, mtd_str_it2);
    CHECK_TRUE(mtd::input_iterator<mtd_it3>, mtd_str_it3);
}

void test_bidirectional_iterator()
{
    test::interface("bidirectional_iterator");

    CHECK_TRUE(mtd::bidirectional_iterator<mtd_it0>, mtd_str_it0);
    CHECK_TRUE(mtd::bidirectional_iterator<mtd_it1>, mtd_str_it1);
    CHECK_TRUE(mtd::bidirectional_iterator<mtd_it2>, mtd_str_it2);
    CHECK_TRUE(mtd::bidirectional_iterator<mtd_it3>, mtd_str_it3);
}


void test_forward_iterator()
{
    test::interface("forward_iterator");

    CHECK_TRUE(mtd::forward_iterator<mtd_it0>, mtd_str_it0);
    CHECK_TRUE(mtd::forward_iterator<mtd_it1>, mtd_str_it1);
    CHECK_TRUE(mtd::forward_iterator<mtd_it2>, mtd_str_it2);
    CHECK_TRUE(mtd::forward_iterator<mtd_it3>, mtd_str_it3);
}

void test_random_access_iterator()
{
    test::interface("random_access_iterator");

    CHECK_TRUE(mtd::random_access_iterator<mtd_it0>, mtd_str_it0);
    CHECK_TRUE(mtd::random_access_iterator<mtd_it1>, mtd_str_it1);
    CHECK_TRUE(mtd::random_access_iterator<mtd_it2>, mtd_str_it2);
    CHECK_TRUE(mtd::random_access_iterator<mtd_it3>, mtd_str_it3);
}

template <typename I>
concept prueba =
requires(const I& i){
	    { mtd::to_address(i) } -> 
			mtd::same_as<mtd::add_pointer_t<mtd::iter_reference_t<I>>>;
	};
void test_contiguous_iterator()
{
    test::interface("contiguous_iterator");

    CHECK_TRUE(mtd::contiguous_iterator<mtd_it0>, mtd_str_it0);
    CHECK_TRUE(mtd::contiguous_iterator<mtd_it1>, mtd_str_it1);
    CHECK_TRUE(mtd::contiguous_iterator<mtd_it2>, mtd_str_it2);
    CHECK_TRUE(mtd::contiguous_iterator<mtd_it3>, mtd_str_it3);
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

    test_incrementable<mtd_it0>(mtd_str_it0);
    test_incrementable<std_it1>(std_str_it1);
    test_incrementable<std_it2>(std_str_it2);
    test_incrementable<std_it3>(std_str_it3);
    test_incrementable<std_it4>(std_str_it4);
    test_incrementable<std_it5>(std_str_it5);
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

    test_indirectly_readable<mtd_it0>(mtd_str_it0);
    test_indirectly_readable<std_it1>(std_str_it1);
    test_indirectly_readable<std_it2>(std_str_it2);
    test_indirectly_readable<std_it3>(std_str_it3);
    test_indirectly_readable<std_it4>(std_str_it4);
    test_indirectly_readable<std_it5>(std_str_it5);
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
    test_input_iterator();
    test_forward_iterator();
    test_bidirectional_iterator();
    test_random_access_iterator();
    test_contiguous_iterator();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}





