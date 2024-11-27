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

#include "mega_SPI.h"
#include "mega_pin_hwd.h"

// Observar que todas son funciones de configuración que se ejecutan una sola
// vez. No es imprescindible que sean las más eficientes.

namespace mega_{
using namespace hwd;
namespace cfg = hwd::cfg;

// En la appnote "AVR151: Setup And use of the SPI", indican que:
// The MISO, MOSI, and SCK lines of all the other slaves are tristated 
// (configured as input pins of a high impedance with no pullup resistors
// enabled). 
// ==> Conclusión: no tienen pull-up resistor.
// Pero en el momento en el que haces el chip_select conectas el registro del
// SPI del master con el registro del SPI del slave formando un circuito
// cerrado. Da la impresión de no necesitar pull-up resistors (???)
void SPI_master::cfg_pins()
{

    // Configuración de los pins
    Pin<cfg::spi::SCK_pin_number>::as_output();
    Pin<cfg::spi::MOSI_pin_number>::as_output();
//    Pin<SPI_num_pin_MISO>::as_input_without_pullup();
    Pin<cfg::spi::SS_pin_number>::as_output(); // fundamental para que no sea slave:
				     // punto 23.3.2: leer este punto. Indica
				     // que si se define como entrada tiene
				     // que mantenerse high, si cambia a low
				     // cambia el modo del SPI a slave!!!
    
}


void SPI_slave::init()
{
    namespace cfg = hwd::cfg;

    // Configuración de los pins (table 23-1). 
//    Pin<SPI_num_pin_SCK>::as_input_without_pullup();
//    Pin<SPI_num_pin_MOSI>::as_input_without_pullup();
    hwd::Pin<cfg::spi::MISO_pin_number>::as_output();
//    Pin<SPI_num_pin_SS>::as_input_without_pullup(); 
}



} // namespace
