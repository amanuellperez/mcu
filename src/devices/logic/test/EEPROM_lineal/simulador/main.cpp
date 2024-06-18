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

#include <algorithm>	    // EEPROM usa std::min.
		    // Problema: que al compilar en gcc mi std_algorithm
		    // mete min en mtd::min. Por eso pongo este algorithm 
		    // aqui delante de dev_EEPROM.
#include "../../dev_EEPROM_buffer.h"

#include "sim_eeprom.h"

#include <iostream>
#include <vector>
#include <alp_test.h>


//using namespace avr;
using namespace test;

// TODO: hacerlo automático
void test_sim_eeprom()
{
    test::interfaz("sim_eeprom");

    constexpr int num_pages = 3;
    constexpr int page_size = 5;
    sim::EEPROM<num_pages, page_size> eeprom;

    std::array<uint8_t, page_size> a = {10, 20, 30, 40, 50};

    eeprom.write(0, a.data(), page_size);
    eeprom.print(std::cout);
    std::cout << "\n";

    eeprom.clear();
    eeprom.print(std::cout);
    std::cout << "\n";


    eeprom.write(1, a.data(), page_size);
    eeprom.print(std::cout);
    std::cout << "\n";

    eeprom.write(7, a.data(), page_size);
    eeprom.print(std::cout);
    std::cout << "\n";

    eeprom.write(12, a.data(), page_size);
    eeprom.print(std::cout);
    std::cout << "\n";

    uint8_t buf[page_size * num_pages];
    eeprom.read(0, buf, page_size*num_pages);
    for (int i = 0; i < page_size * num_pages; ++i)
	std::cout << (int) buf[i] << ", ";
    std::cout << '\n';

    std::fill(buf, buf + page_size*num_pages, 0);
    eeprom.read(page_size*num_pages - 1, buf, page_size*num_pages);
    for (int i = 0; i < page_size * num_pages; ++i)
	std::cout << (int) buf[i] << ", ";
    std::cout << '\n';

    std::fill(buf, buf + page_size*num_pages, 0);
    eeprom.read(page_size*num_pages - 2, buf, 4);
    for (int i = 0; i < page_size * num_pages; ++i)
	std::cout << (int) buf[i] << ", ";
    std::cout << '\n';
}


void test_eeprom()
{
    constexpr int num_pages = 3;
    constexpr int page_size = 2;

    avr::EEPROM_buffer<sim::EEPROM<num_pages,page_size>> eeprom;


    std::array<uint8_t, num_pages*page_size> a;
    for (uint8_t i = 0; i < a.size(); ++i)
	a[i] = i;

    std::cout << "array a = ";
    for (auto x: a)
	std::cout << (int) x << ' ';
    std::cout << '\n';

    {
    uint8_t res = eeprom.write(0, a.data(), a.size());
    CHECK_TRUE(res == a.size(), "write");
    }

    {// leemos todo
    uint8_t buf[a.size()];
    uint8_t ret = eeprom.read(0, buf, a.size());
    CHECK_TRUE(ret == a.size(), "read ret");
    std::vector<uint8_t> y = {0,1,2,3,4,5};
    CHECK_EQUAL_CONTAINERS(buf, buf + a.size(), y.begin(), y.end(), "read");
    }

    {// leemos solo el último elemento
    uint8_t buf[a.size()];
    uint8_t ret = eeprom.read(a.size() - 1, buf, a.size());
    CHECK_TRUE(ret == 1, "read 1 ret");
    std::vector<uint8_t> y = {5};
    CHECK_EQUAL_CONTAINERS(buf, buf + 1, y.begin(), y.end(), "read");
    }

    {// leemos 0 elementos
    uint8_t buf[a.size()];
    uint8_t ret = eeprom.read(a.size(), buf, a.size());
    CHECK_TRUE(ret == 0, "read 0 ret");
    }

    {// write: intentamos escribir de más
     // 0 1 2 3 4 5 --> 0 1 2 3 0 1
    uint8_t res = eeprom.write(4, a.data(), a.size());
    CHECK_TRUE(res == 2, "write");

    uint8_t buf[a.size()];
    uint8_t ret = eeprom.read(0, buf, a.size());
    CHECK_TRUE(ret == a.size(), "read ret");
    std::vector<uint8_t> y = {0,1,2,3,0,1};
    CHECK_EQUAL_CONTAINERS(buf, buf + a.size(), y.begin(), y.end(), "write");
    }

    {// write: caso extremo. Intentamos escribir 1 elemento
     // 0 1 2 3 0 1 --> 0 1 2 3 0 0
    uint8_t res = eeprom.write(5, a.data(), a.size());
    CHECK_TRUE(res == 1, "write 1");

    uint8_t buf[a.size()];
    uint8_t ret = eeprom.read(0, buf, a.size());
    CHECK_TRUE(ret == a.size(), "read ret");
    std::vector<uint8_t> y = {0,1,2,3,0,0};
    CHECK_EQUAL_CONTAINERS(buf, buf + a.size(), y.begin(), y.end(), "write");
    }

    {// write: caso extremo. Intentamos escribir fuera
    uint8_t res = eeprom.write(6, a.data(), a.size());
    CHECK_TRUE(res == 0, "write 0");
    }

    {// write: caso de escritura habitual: entre paginas
     // 0 1 2 3 0 0 --> 0 10 20 30 40 0 
    std::array<uint8_t, 4> b = {10, 20, 30, 40};

    uint8_t res = eeprom.write(1, b.data(), b.size());
    CHECK_TRUE(res == 4, "write 4");

    uint8_t buf[a.size()];
    uint8_t ret = eeprom.read(0, buf, a.size());
    CHECK_TRUE(ret == a.size(), "read ret");
    std::vector<uint8_t> y = {0,10, 20, 30, 40, 0};
//    std::cout << "y = ";
//    for (uint8_t i = 0; i < a.size(); ++i)
//	std::cout << (int) buf[i] << ' ';
//    std::cout << '\n';
    CHECK_EQUAL_CONTAINERS(buf, buf + a.size(), y.begin(), y.end(), "write");
    }

    {// write: caso de escritura habitual: una pagina completa
     // 0 10 20 30 40 0  --> 0 10 3 4 40 0
    std::array<uint8_t, 2> b = {3, 4};

    uint8_t res = eeprom.write(2, b.data(), b.size());
    CHECK_TRUE(res == 2, "write 2");

    uint8_t buf[a.size()];
    uint8_t ret = eeprom.read(0, buf, a.size());
    CHECK_TRUE(ret == a.size(), "read ret");
    std::vector<uint8_t> y = {0,10, 3, 4, 40, 0};
    CHECK_EQUAL_CONTAINERS(buf, buf + a.size(), y.begin(), y.end(), "write");
    }

    {// write: caso de escritura habitual: solo dentro de una pagina
     // 0 10 3 4 40 0 --> 0 10 3 7 40 0
    std::array<uint8_t, 1> b = {7};

    uint8_t res = eeprom.write(3, b.data(), b.size());
    CHECK_TRUE(res == 1, "write 1");

    uint8_t buf[a.size()];
    uint8_t ret = eeprom.read(0, buf, a.size());
    CHECK_TRUE(ret == a.size(), "read ret");
    std::vector<uint8_t> y = {0,10, 3, 7, 40, 0};
    CHECK_EQUAL_CONTAINERS(buf, buf + a.size(), y.begin(), y.end(), "write");
    }
}


int main(int argc, char* argv[])
{
try{
    test::header("dev_EEPROM_buffer");
//    test_sim_eeprom();
    test_eeprom();

}catch(std::exception& e){
    std::cerr << argv[0] << ": " << e.what() << '\n';
    return 1;
}

}
