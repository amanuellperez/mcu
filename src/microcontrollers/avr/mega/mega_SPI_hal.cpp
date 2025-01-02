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

#include "mega_SPI_hal.h"
#include "mega_pin_hwd.h"

// Observar que todas son funciones de configuración que se ejecutan una sola
// vez. No es imprescindible que sean las más eficientes.

namespace mega_{
namespace hal{

using namespace hwd;
namespace cfg = hwd::cfg;


void SPI_slave::init()
{
    namespace cfg = hwd::cfg;

    // Configuración de los pins (table 23-1). 
//    Pin<SPI::SCK_pin>::as_input_without_pullup();
//    Pin<SPI::MOSI_pin>::as_input_without_pullup();
    hwd::Pin<SPI::MISO_pin>::as_output();
//    Pin<SPI::SS_pin>::as_input_without_pullup(); 
}



} // namespace
} // namespace
