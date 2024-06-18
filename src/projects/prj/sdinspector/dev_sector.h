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

#pragma once

#ifndef __DEV_SECTOR_H__
#define __DEV_SECTOR_H__

#include <array>
#include <ostream>
#include <atd_ostream.h>
#include <atd_math.h>
#include <limits>

namespace dev{

template <size_t sector_size_in_bytes,
	  uint8_t nbytes_per_line0 = 16
	 >
struct Sector_cfg{
    static constexpr size_t sector_size = sector_size_in_bytes;
    static constexpr uint8_t nbytes_per_line = nbytes_per_line0;
};

template <typename Cfg>
struct Sector: std::array<uint8_t, Cfg::sector_size>{
// Cfg del sector
    // using Address = SDCard::Address; TODO: descomentar
    using Address = uint32_t;
    static constexpr size_t sector_size = Cfg::sector_size;

// Cfg del print
    static constexpr uint8_t nbytes_per_line = Cfg::nbytes_per_line;

// Data
// uint8_t data[sector_size]; <-- implícito al heredar de std::array
    // me suena mejor 'number' que 'address' (???)
    // Si address == max ==> el sector consideramos que no está cargado en
    // memoria.
    Address address = std::numeric_limits<Address>::max(); 

    bool is_valid() const { return !is_invalid(); }
    bool is_invalid() const 
		{return address == std::numeric_limits<Address>::max();}

// printing
// --------
    // TODO: comprobar que el tamaño del sector >= SDCard::block_size
    // ???: de momento cargo el sector directamente en el programa. Si se
    // quiere generalizar esto habría que definir esta función.
    // Carga en memoria el sector indicado (número Address)
//    static error read_from(SDCard, SDCard::Address);

    // Muestra en el flujo indicado el sector cargado en memoria.
    // De momento es responsabilidad del usuario garantizar que se haya
    // cargado el sector correspondiente.
    void print(std::ostream& out) const { print(out, 0, sector_size); }

    // Muestra en el flujo indicado el sector cargado en memoria, los bytes
    // [i0, i0 + N).
    void print(std::ostream&, size_t i0, size_t N) const;

private:

    // Helpers
    void print_line(std::ostream& out, size_t i, size_t N) const;

};


template <typename Cfg>
void Sector<Cfg>::print(std::ostream& out, size_t i0, size_t sz) const
{
    out << "Sector: " << address << '\n';

    size_t ie = i0 + sz; // [i0, ie) intervalo a imprimir
    for (; ie - i0 > nbytes_per_line; i0 += nbytes_per_line){
	print_line(out, i0, nbytes_per_line);
    }

    print_line(out, i0, ie - i0);

}

template <typename Cfg>
void Sector<Cfg>::print_line(std::ostream& out, size_t i, size_t N) const
{
    auto p = this->data();

// header:
    out.fill('0');
    out.width(3);
    out << i << ':';

// in hexadecimal:
    for (uint8_t k = 0; k < N; ++k){
	if (atd::is_even(k)) out << ' ';
	atd::print_int_as_hex(out, p[i + k], false);
    }

    // ajustamos al final de línea en caso de que N < nbytes_per_line
    for (uint8_t k = 0; k < nbytes_per_line - N; ++k){
	if (atd::is_even(k)) out << ' ';
	out << "  "; 
    }


// as chararacters:
    out << ' ';

    for (uint8_t k = 0; k < N; ++k){
//	if (atd::is_even(k)) out << ' ';

	if (isprint(p[i + k]))
	    out << p[i + k];
	else 
	    out << '.';
    }

    out << '\n';
}

}// namespace
 
#endif


