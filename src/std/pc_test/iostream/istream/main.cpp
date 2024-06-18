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

#include "../../../std_istream.h"
#include "../alp_streambuf.h"

#include <alp_test.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>

using namespace test;


namespace mtd{

class test_istream : public mtd::istream{
public:
    explicit test_istream(UART& uart):
	istream{&sb_}, sb_{uart} 
    {
	sb_.as_input();
    }

protected:
//    virtual void traza(const char* s) override
//    { std::cout << s; }
//
//    virtual void traza(char c) override
//    {std::cout << c;}
//
private:
    fix_streambuf sb_;
};


} // namespace


template <typename Int>
void print(const std::string& s, Int x)
{
    std::cout << s << x << "]\n";
}

// El uint8_t falla ya que lee como caracteres y no como números!!!
void print(const std::string& s, uint8_t x)
{
    std::cout << s << (unsigned int) x << "]\n";
}

template <typename Int>
void test_number(const std::string& s)
{
    test::interface(s);

    constexpr char msg[] = "23   \t 145  +876";
    UART uart{msg};
    mtd::test_istream in{uart};
    Int x;
    in >> x;
    print("out: [23] ?= [", x);
    in >> x;
    print("out: [145] ?= [", x);
    in >> x;
    print("out: [876] ?= [", x);

    CHECK_TRUE(bool(in), "in");
}


void test_istream()
{
    test::interface("istream");

    {
    constexpr char msg[] = "hola FIN";
    UART uart{msg};
    mtd::test_istream in{uart};
    CHECK_TRUE(in.good(), "good");

    constexpr int res_size = 200;
    char res[res_size];

    in.get(res, res_size, 'F');
    CHECK_TRUE(in.good(), "good");
    CHECK_TRUE(in.gcount() == 5, "gcount");
    
    std::cout << "out(get): [hola ] =? [" << res << "]\n";

    in.get(res, res_size, 'a');
    CHECK_TRUE(!in.good(), "good");
    CHECK_TRUE(in.eof(), "eof");
    CHECK_TRUE(in.gcount() == 3, "gcount");
    std::cout << "out(get): [FIN] =? [" << res << "]\n";
    }

    {// getline
    constexpr char msg[] = "una linea\nSegunda\nUna tercera aqui\n\nfin";

    UART uart{msg};
    mtd::test_istream in{uart};
    CHECK_TRUE(in.good(), "good");

    constexpr int res_size = 200;
    char res[res_size];

    in.getline(res, res_size);
    CHECK_TRUE(in.good(), "good");
    CHECK_TRUE(in.gcount() == strlen("una linea") + 1, "gcount");
    std::cout << "out(getline): [una linea] =? [" << res << "]\n";

    in.getline(res, res_size);
    CHECK_TRUE(in.good(), "good");
    CHECK_TRUE(in.gcount() == strlen("Segunda") + 1, "gcount");
    std::cout << "out(getline): [Segunda] =? [" << res << "]\n";

    in.getline(res, res_size);
    CHECK_TRUE(in.good(), "good");
    CHECK_TRUE(in.gcount() == strlen("Una tercera aqui") + 1, "gcount");
    std::cout << "out(getline): [Una tercera aqui] =? [" << res << "]\n";

    in.getline(res, res_size);
    CHECK_TRUE(in.good(), "good");
    CHECK_TRUE(in.gcount() == 1, "gcount");
    // Si no lee nada, no escribe nada en res.
//    std::cout << "out(getline): [] =? [" << res << "]\n";

    in.getline(res, res_size);
    CHECK_TRUE(!in.good(), "good");
    CHECK_TRUE(in.eof(), "eof");
    CHECK_TRUE(in.gcount() == strlen("fin"), "gcount");
    std::cout << "out(getline): [fin] =? [" << res << "]\n";
    }

    {// read
    test::interface("read");
    constexpr char msg[] = "una linea\nSegunda\nUna tercera aqui\n\nfin";

    {
    UART uart{msg};
    mtd::test_istream in{uart};
    CHECK_TRUE(in.good(), "good");
    char res[255];
    in.read(res, 255);
    CHECK_TRUE(std::string{msg} == std::string{res}, "read");
    }
    {
    UART uart{msg};
    mtd::test_istream in{uart};
    CHECK_TRUE(in.good(), "good");
    char res[255];
    in.read(res, 2);
    res[2] = '\0';  // read lee 2 bytes, no añade '\0'
    CHECK_TRUE(std::string{"un"} == std::string{res}, "read");
    }
    }

    {
    constexpr char msg[] = "456d   123 negativo -45 esto no lo leo Ahora si\n"
                           "Una linea entera\nEspacios    \t -54 ";
    UART uart{msg};
    mtd::test_istream in{uart};
    int i;


    if (in >> i)
	std::cout << "out(int): [456] =? [" << i << "]\n";
    else
	{CHECK_TRUE(1, "ERROR reading 456");}

    char c;
    if (in.get(c))
	{CHECK_TRUE(c == 'd', "leemos el 'd' despues del 456d");}
    else
	{CHECK_TRUE(1, "ERROR reading 'd'");}

    while (in.get(c) and c == ' ') 
	;


    if (in >> i)
	std::cout << "out(int): [123] =? [" << i << "]\n";
    else
	CHECK_TRUE(1, "ERROR reading 123");

    while (in.get(c) and c == ' ') 
	;

    std::cout << '[' << c << "]\n";
    while (in.get(c) and c != ' ') 
	std::cout << '[' << c << "]\n";

    in >> i;
    CHECK_TRUE(in.good(), "good");
    std::cout << "out(int): [-45] =? [" << i << "]\n";

    constexpr int res_size = 200;
    char res[res_size];

    in.get(res, res_size, 'A');
    CHECK_TRUE(in.good(), "good");

    std::cout << "out(get): [...] =? [" << res << "]\n";
    }

    {
    constexpr char msg[] = "una";
    UART uart{msg};
    mtd::test_istream in{uart};
    char c;
    in.get(c);
    CHECK_TRUE(bool(in), "in");
    CHECK_TRUE(in.gcount() == 1, "gcount");
    }

//    test_number<short>("short");
//    test_number<unsigned short>("unsigned short");
    test_number<int>("int");
    test_number<unsigned int>("unsigned int");
    test_number<long>("long");
    test_number<unsigned long>("unsigned long");
    test_number<long long>("long long");
    test_number<unsigned long long>("unsigned long long");

//    test_number<int8_t>("int8_t");
    test_number<uint8_t>("uint8_t");
//    test_number<int16_t>("int16_t");
//    test_number<uint16_t>("uint16_t");
    test_number<int32_t>("int32_t");
    test_number<uint32_t>("uint32_t");
    test_number<int64_t>("int64_t");
    test_number<uint64_t>("uint64_t");

// Al llamar a std::cin >> x, se ignoran los retornos de carro.
// La única forma de salir es escribiendo algo.
// Si se escribe una letra el flujo queda en mal estado.
//    int x = 3;
//    std::cin >> x;
//    if (std::cin)
//	std::cout << "OK\n";
//    else
//	std::cout << "FAIL\n";

}

void test_bugs()
{
    test::interface("bugs");

}


int main()
{
try{
    test::header("istream");

    std::cerr << "TODO: These aren't automatic tests. `grep` =?\n";
    test_istream();
//    test_bugs();

}catch(alp::Excepcion& e){
    cerr << e.what() << '\n';
    return 1;
}

}





