// Copyright (C) 2019-2020 Manuel Perez 
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

// #include "../../std_streambuf.h"

#include "../alp_streambuf.h"

#include <alp_test.h>
#include <iostream>
#include <vector>
#include <cstring>

#include <string.h> // memcpy
using namespace test;


void test_streambuf()
{
    test::interfaz("streambuf");
 
    LCD lcd;
    mtd::my_streambuf buf{lcd};
    const char s[] = "Esto es una prueba";
    buf.sputn(s, strlen(s));
    buf.sputc('\n');
}


void test_fix_streambuf()
{
    test::interfaz("fix_streambuf (put area)");
 
    UART uart;
    mtd::fix_streambuf buf{uart};
    const char s[] = "1234567";
    std::cout << "out(sputn): [] =? [";
    auto n = buf.sputn(s, strlen(s));
    std::cout << "]\n";
    CHECK_TRUE(static_cast<unsigned long>(n) == strlen(s), "return sputn");
    std::cout << "out(flush): [" << s << "] =? [";
    buf.pubsync();
    std::cout << "]\n";
    const char s2[] = "1234567890ABCDE";
    std::cout << "Excedemos el buffer, out: [1234567890] =? [";
    buf.sputn(s2, strlen(s2));
    std::cout << "]\n";

    const char s3[] = "6789";
    std::cout << "out: [] =? [";
    buf.sputn(s3, strlen(s3));
    std::cout << "]\n";
    std::cout << "out: [] =? [";
    buf.sputc('0');
    std::cout << "]\n";
    std::cout << "out: [ABCDE67890] =? [";
    buf.sputc('F');
    std::cout << "]\n";
    std::cout << "out : [F] =? [";
    buf.pubsync();
    std::cout << "]\n";

    std::cout << "out: [] =? [";
    buf.pubsync();
    std::cout << "]\n";


    test::interfaz("fix_streambuf (get area)");
    buf.as_input();
    for (int i = 0; buf.sgetc() != mtd::char_traits<char>::eof(); ++i){
	int r = buf.sbumpc();
	CHECK_TRUE(char_traits<char>::to_char_type(r) == UART::msg[i], "sbumpc");
    }

    buf.reset();
    int r = buf.sbumpc();
    buf.sputbackc(char_traits<char>::to_char_type(r));
    int c = buf.sbumpc();
    CHECK_TRUE(c == r, "sputbackc");

}



int main()
{
try{
    test::header("streambuf");

    std::cerr << "NO SON AUTOMÁTICAS ESTAS PRUEBAS!!!\n";
    test_streambuf();
    test_fix_streambuf();

}catch(alp::Excepcion& e){
    cerr << e.what() << '\n';
    return 1;
}

}





