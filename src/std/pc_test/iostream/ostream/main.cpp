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

#include "../../../std_ostream.h"
#include "../../../std_iomanip.h"

#include "../alp_streambuf.h"

#include <alp_test.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <limits>

using namespace test;

namespace mtd{

class my_ostream : public mtd::ostream{
public:
    explicit my_ostream(LCD& lcd):
	ostream{&sb_}, 
	sb_{lcd} {}

    virtual ~my_ostream() { }

private:
    my_streambuf sb_;
    
};
//

class fix_ostream : public mtd::ostream{
public:
    explicit fix_ostream(UART& uart):
	ostream{&sb_}, 
	sb_{uart} {}


private:
    fix_streambuf sb_;
    
};



} // namespace

template <typename Int>
void test_int(mtd::my_ostream& out, const std::string& msg)
{
    Int m = std::numeric_limits<Int>::min();
    if (m < 0)
	++m;

    std::cout << msg << "(min): [" << m<< "] ?= ["; out << m; std::cout << "]\n";
    Int M = std::numeric_limits<Int>::max();
    std::cout << msg << "(max): [" << M << "] ?= ["; out << M; std::cout << "]\n";
}

void test_ostream()
{
    test::interfaz("ostream");

    LCD lcd;
    mtd::my_ostream out{lcd};
    short s = -10;
    unsigned short us = 40;

    int i = -25;
    unsigned int ui = 100;
 
    long l = -1234567890123456789;
    unsigned long ul = 1234567890123456789;


    out << "Prueba en el flujo\n";
    out << "out: [-10] =? [" << s << "]\n";
    out << "out:  [40] =? [" << us << "]\n";
    out << "out: [-25] =? [" << i <<  "]\n";
    out << "out: [100] =? [" << ui << "]\n";
    std::cout << "out: [" << l << "] ?= ["; out << l; std::cout << "]\n";
    std::cout << "out: [" << ul << "] ?= ["; out << ul; std::cout << "]\n";
    std::cout << "out: [a] ?= ["; out << 'a'; std::cout << "]\n";

    // tipos
    test_int<uint8_t>(out, "uint8_t");
    test_int<int8_t>(out, "int8_t");

    test_int<uint16_t>(out, "uint16_t");
    test_int<int16_t>(out, "int16_t");

    test_int<uint32_t>(out, "uint32_t");
    test_int<int32_t>(out, "int32_t");

    test_int<uint64_t>(out, "uint64_t");
    test_int<int64_t>(out, "int64_t");
}

void test_fix_ostream()
{
    test::interfaz("fix_ostream");

    UART uart;
    mtd::fix_ostream out{uart};

    std::cout << "Sin buffer (unitbuf = true)\n";

    CHECK_TRUE((bool(out.flags() & mtd::ios_base::unitbuf)) == false, "unitbuf por defecto = 0");
    out << mtd::unitbuf;
    CHECK_TRUE((bool(out.flags() & mtd::ios_base::unitbuf)), "out << unitbuf");

    constexpr char msg[] = "1234567890abcdefghijkl";
    for (size_t i = 0; i < strlen(msg); ++i){
	std::cout << "out: [" << msg[i] << "] =? [";
	out.put(msg[i]);
	std::cout << "]\n";
	if (!out.good())
	    CHECK_TRUE(0, "ERROR: !out.good()");
    }

    short s = -10;
    unsigned short us = 40;

    int i = -25;
    unsigned int ui = 100;
    
    std::cout << "out: [-10] =? ["; out << s; std::cout << "]\n";
    std::cout << "out:  [40] =? ["; out << us; std::cout << "]\n";
    std::cout << "out: [-25] =? ["; out << i; std::cout <<  "]\n";
    std::cout << "out: [100] =? ["; out << ui; std::cout << "]\n";


    std::cout << "Con buffer (unitbuf = false)\n";
    out << mtd::nounitbuf;
    CHECK_TRUE((bool(out.flags() & mtd::ios_base::unitbuf)) == false, "nounitbuf");

    for (size_t i = 0; i < strlen(msg); ++i){
	std::cout << "out: [" << msg[i] << "] =? [";
	out.put(msg[i]);
	std::cout << "]\n";
//	(static_cast<mtd::fix_streambuf*>(out.rdbuf()))->print_put_area();
	if (!out.good())
	    CHECK_TRUE(1, "!out.good()");
    }

    std::cout << "out: [-10] =? ["; out << s; std::cout << "]\n";
    std::cout << "out:  [40] =? ["; out << us; std::cout << "]\n";
    std::cout << "out: [-25] =? ["; out << i; std::cout <<  "]\n";
    std::cout << "out: [100] =? ["; out << ui; std::cout << "]\n";
    std::cout << "resto sin volcar = ["; out.flush(); std::cout << "]\n";
    

    std::cout << "write:\n";
    std::cout << "out: [" << msg << "] ?= [";
    out.write(msg, strlen(msg));
    out.flush();
    std::cout << "]\n";

    {
	char c = 'a';
	std::cout << "out: [a] =? ["; out << c; out.flush(); std::cout << "]\n";

	unsigned char uc = 'a';
	std::cout << "out: [a] =? ["; out << uc; out.flush(); std::cout << "]\n";

    }

    std::cout << "Saliendo, tiene que hacer flush de [adios] ?= ";
    out << "adios\n";

}


void test_fill()
{
    test::interfaz("fill");

    UART uart;
    mtd::fix_ostream out{uart};

    out.width(4);
    out.fill('*');
    left(out);
    std::cout << "left: [3***] ?= ["; out << 3; out.flush(); std::cout << "]\n";
    std::cout << "width no sticky: [3] ?= ["; out << 3; out.flush(); std::cout << "]\n";

    out << mtd::right;
    out.width(4);
    std::cout << "right: [***3] ?= ["; out << 3; out.flush(); std::cout << "]\n";

    std::cout << "no sticky: [3] ?= ["; out << 3; out.flush(); std::cout << "]\n";
    out.width(4);
    std::cout << "right: [***3] ?= ["; out << 3; out.flush(); std::cout << "]\n";


    out.width(4);
    std::cout << "char right: [***a] ?= ["; out << 'a'; out.flush(); std::cout << "]\n";

    std::cout << "char no sticky: [a] ?= ["; out << 'a'; out.flush(); std::cout << "]\n";

    out << mtd::left;
    out.width(4);
    std::cout << "char left: [a***] ?= ["; out << 'a'; out.flush(); std::cout << "]\n";

    std::cout << "manipulador: [b---] ?= [";
	out << mtd::setw(4) << mtd::setfill('-') << 'b' << mtd::flush;
    std::cout << "]\n";

    std::cout << "manipulador: [###b] ?= [";
	out << mtd::setw(4) << mtd::right << mtd::setfill('#') << 'b' << mtd::flush;
    std::cout << "]\n";
}

void test_hexadecimal()
{
    LCD lcd;
    mtd::my_ostream out{lcd};

    out << "my: HEX\n";
    for (uint8_t i = 0; i < 255; ++i)
	out.put(i);
    out << "HEX\n";

    out << "std: HEX\n";
    for (uint8_t i = 0; i < 255; ++i)
	std::cout.put(i);
    out << "HEX\n";
}

void test_bugs()
{
//    LCD lcd;
//    mtd::my_ostream out{lcd};
//    out << "bug[";
//    out.put('\0');
//    out << "] =? 0x00\n";
}

int main()
{
try{
    test::header("ostream");

    std::cerr << "THESE ARE NOT AUTOMATIC TEST!!!\n";
    test_ostream();
    test_fix_ostream();
    test_fill();
//  test_hexadecimal();
    test_bugs();

}catch(alp::Excepcion& e){
    cerr << e.what() << '\n';
    return 1;
}

}





