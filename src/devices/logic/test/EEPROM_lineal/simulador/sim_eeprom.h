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

#pragma once

#ifndef __SIM_EEPROM_H__
#define __SIM_EEPROM_H__

#include <cstdint>
#include <cstring>
#include <array>
#include <iostream>

// Simulador de una EEPROM particular
namespace sim{

template <int num_pages_, int page_size_>
class EEPROM{
public:
    using Address   = std::size_t;
    using size_type = std::size_t;

    EEPROM() { clear();}

    void cfg_SPI()  const {}
    
    void clear() {mem_.fill(0);}
    
    /// Lee n bytes a partir de la dirección de memoria addr, guardando
    /// el resultado en buf. Si al leer se alcanza la última dirección de
    /// memoria, continua leyendo desde la dirección 0.
    void read(Address address, uint8_t* buf, size_type n);

    /// Escribe n bytes en la dirección de memoria indicada. 
    /// Como cada página tiene 64 bytes, en caso de que address + n se pase
    /// del fin de la página continua escribiendo en el principio de la
    /// página. Tarda máximo 5 ms.
    void write(Address address, uint8_t* buf, size_type n);

    /// Set the write enable latch.
    void write_enable() {}

    /// Reset the write enable latch.
    void write_disable() {}

    uint8_t read_status_register() {return 0;}
//  void write_status_register();  <--- TODO: solo se pueden escribir 2 bits.

    // Funciones de estado
    // -------------------
    /// Devuelve el valor del bit Write-In-Process del status register.
    /// Indica si el chip está ocupado con una operación de escritura.
    static bool write_in_process(uint8_t status_reg) { return false;}

    /// Devuelve el valor del bit Write Enable Latch del status register.
    /// Indica el chip está habilitado para escribir o no. Este bit es el 
    /// que activa la instrucción write_enable() y lo desactiva
    /// write_disable().
    static bool write_enable_latch(uint8_t status_reg) { return true;}

    /// Número de bytes que tiene cada página.
    constexpr size_type page_size() const
    {return page_size_;}

    /// Dirección de la última posición de memoria.
    constexpr size_type max_address() const
    {return num_pages_ * page_size_ - 1;}


    // Funciones de ayuda
    void print(std::ostream& out)
    {
	for (int i = 0; i < num_pages_; ++i){
	    for (int j = 0; j < page_size_; ++j)
		out << (int) mem_[page_size_*i + j] << ", ";

	    out << '\n';
	}
    }
    
private:
    std::array<uint8_t, num_pages_ * page_size_> mem_;

    constexpr size_type ie() const
    {return num_pages_ * page_size_;}
};

// Lee n bytes a partir de la dirección de memoria addr, guardando
// el resultado en buf. Si al leer se alcanza la última dirección de
// memoria, continua leyendo desde la dirección 0.

template <int np, int ps>
void EEPROM<np, ps>::read(Address i, uint8_t* buf, size_type N)
{
    size_type n = std::min(ie() - i, N);
    std::memcpy(buf, mem_.data() + i, n);
    N -= n;

    if (N > 0){
	buf += n;
	std::memcpy(buf, mem_.data(), N);
    }
}


// Escribe n bytes en la dirección de memoria indicada. 
// Como cada página tiene 64 bytes, en caso de que address + n se pase
// del fin de la página continua escribiendo en el principio de la
// página. Tarda máximo 5 ms.
template <int np, int ps>
void EEPROM<np, ps>::write(Address address, uint8_t* buf, size_type N)
{
    // (i, j) = (num_page, pos_page)
    size_type i = address/page_size();
    size_type page_addr0 = page_size()*i;   // dirección inicial de la página
    size_type j = address - page_addr0;

    size_type n = std::min(N, page_size() - j);

    std::memcpy(mem_.data() + address, buf, n);
    N -= n;

    if (N > 0){
	buf += n; // No valido si se pasa. Es un simulador para probar.
	std::memcpy(mem_.data() + page_addr0, buf, N);
    }
}


}// namespace


#endif
