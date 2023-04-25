// Copyright (C) 2023 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu

#include <iostream>
#include "../dev_sector.h"


#include <alp_test.h>

using namespace test;

using Cfg0    = dev::Sector_cfg<12, 6>;
using Sector0 = dev::Sector<Cfg0>;

using Cfg1    = dev::Sector_cfg<12, 5>;
using Sector1 = dev::Sector<Cfg1>;

void test_sector()
{
    test::interface("Sector");

{ // con la última línea completa
    Sector0 sector;

    uint8_t data[Sector0::sector_size] =
    {'a', 'b', 'c', 'd', 'e', 'f'
    , 'g', 'h', 'i', 'j', 'k', 'l'};

    std::copy(std::begin(data), std::end(data), sector.begin());

    std::cout << "Input: ";
    for (auto x: sector)
	std::cout << x << ' ';
    std::cout << '\n';


    std::cout << "\nOutput (sector):\n"
		   "------------\n";
    sector.print(std::cout);
    std::cout << "------------\n\n";

    std::cout << "\nOutput (one line):\n"
		   "------------\n";
    sector.print(std::cout, 3, 6);
    std::cout << "------------\n\n";

    std::cout << "\nOutput (two line):\n"
		   "------------\n";
    sector.print(std::cout, 3, 8);
    std::cout << "------------\n\n";
}

{ // con la última línea incompleta
    Sector1 sector;

    uint8_t data[Sector1::sector_size] =
    {'a', 'b', 'c', 'd', 'e', 
     'f', 'g', 'h', 'i', 'j',
     'k', 'l'};

    std::copy(std::begin(data), std::end(data), sector.begin());

    std::cout << "Input: ";
    for (auto x: sector)
	std::cout << x << ' ';
    std::cout << '\n';


    std::cout << "\nOutput:\n"
		   "------------\n";
    sector.print(std::cout);
    std::cout << "------------\n";
}

{ // con caracteres no ascii
    Sector0 sector;

    uint8_t data[Sector1::sector_size] =
    {'a', '\n', 'c', '\t', 'e', 
     3, 'g', 'h', 4, 'j',
     'k', 'l'};

    std::copy(std::begin(data), std::end(data), sector.begin());

    std::cout << "Input: ";
    for (auto x: sector)
	std::cout << x << ' ';
    std::cout << '\n';


    std::cout << "\nOutput:\n"
		   "------------\n";
    sector.print(std::cout);
    std::cout << "------------\n";
}
}

int main()
{
try{
    test::header("sdinspector");

    std::cerr << "PRUEBAS NO AUTOMÁTICAS!!!\n";
    test_sector();

}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
    return 1;
}

}
