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

ESTA SIN PROBAR!!!
#include "../../../std_iostream.h"
#include "../alp_streambuf.h"

#include <alp_test.h>
#include <iostream>
#include <vector>
#include <cstring>

using namespace test;

namespace mtd{

class prueba_iostream : public mtd::iostream{
public:
    explicit prueba_iostream(UART& uart):
	iostream{&sb_}, sb_{uart} 
    {
	sb_.as_input();
    }

private:
    fix_streambuf sb_;
};


} // namespace


void test_istream()
{
    test::interfaz("istream");

    {
    constexpr char msg[] = "hola FIN";
    UART uart{msg};
    mtd::prueba_istream in{uart};
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
    mtd::prueba_istream in{uart};
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
    std::cout << "out(getline): [] =? [" << res << "]\n";

    in.getline(res, res_size);
    CHECK_TRUE(!in.good(), "good");
    CHECK_TRUE(in.eof(), "eof");
    CHECK_TRUE(in.gcount() == strlen("fin"), "gcount");
    std::cout << "out(getline): [fin] =? [" << res << "]\n";
    }


    {
    constexpr char msg[] = "      123 negativo -45 esto no lo leo Ahora si\n"
                           "Una linea entera\nEspacios    \t -54 ";
    UART uart{msg};
    mtd::prueba_istream in{uart};
    int i;

    if (in >> i)
	std::cout << "out(int): [123] =? [" << i << "]\n";
    else
	CHECK_TRUE(1, "ERROR al leer 123");

    char c;
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
    mtd::prueba_istream in{uart};
    char c;
    in.get(c);
    CHECK_TRUE(bool(in), "in");
    CHECK_TRUE(in.gcount() == 1, "gcount");
    }

}


int main()
{
try{
    test::header("istream");

    std::cerr << "NO SON AUTOMÁTICAS ESTAS PRUEBAS!!!\n";
    test_istream();

}catch(alp::Excepcion& e){
    cerr << e.what() << '\n';
    return 1;
}

}





