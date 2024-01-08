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

#ifndef __DEV_OV7670_ROM_H__
#define __DEV_OV7670_ROM_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	ROM usada por la cam OV7670
 *
 * HISTORIA
 *    Manuel Perez
 *    06/12/2023 Experimentando: Un problema con la ROM es que no sé cómo 
 *		 definirla de forma genérica. 
 *		 Se puede:
 *		    (1) Usar Micro::ROM_xxx
 *		    (2) Usar mcu::ROM_xxx
 *		    (3) Escribir un fichero con la información y un script
 *		        que lo convierta en código para cada micro.
 *		    
 *		En las opciones 1 y 2 ¿cómo pasar Micro  o mcu?
 *		Para la opción 2 se me ha ocurrido usar la variable de entorno
 *		MCU que definiremos en el makefile. El uso dirá si es
 *		práctico.
 *
 *		Esto es temporal hasta que se me ocurra alguna forma de
 *		implementarlo en C++. ¿Alguna idea?
 *
 ****************************************************************************/
 #include "dev_OV7670.h"

#ifndef MCU
    static_assert(false, "\nThis file needs MCU macro to compile.\n"
			  "Add to your makefile: 'USER_CXXFLAGS=-DMCU=atmega'.");
#endif

namespace dev{

namespace OV7670_register_cfg{

// Types (sencillez de escritura)
using Register = OV7670_register;
using REG = OV7670_register_name;

Register rom_read(const Register& x)
{
    Register reg;
    reg.address = MCU::rom_read(x.address);
    reg.value   = MCU::rom_read(x.value);
    return reg;
}

struct ROM_read_Register{
    Register operator()(const Register& x)
    { return rom_read(x); }
};

template <size_t N>
using ROM_register_array = 
    MCU::ROM_array<Register, N, ROM_read_Register>;



// TODO: cómo generalizar PROGMEM?? Esto es del avr!!! ¿o es de gcc?
constexpr ROM_register_array<3> color_bar_test PROGMEM = {
// frames per second
    Register{REG::DBLV		,0x0A}, // default = bypass PLL
    Register{REG::CLKRC		,0x07},

// QVGA, RGB, enable color bar
    Register{REG::COM7		, 0x16},    // QVGA, RGB, color bar enable
//    Register{REG::COM3		, 0x04},    // DCW enable
//
//    Register{REG::SCALING_DCWCTR, 0x22},    // horizontal & vertical sample by 4
//    Register{REG::COM14		, 0x1A},    // DCW and PCLK enable
//					    // Manual scalling.
//					    // PCLK divided by 4
//    Register{REG::SCALING_PCLK_DIV, 0xF2}  // divide by 4

};


// >>> TODO: borrar
constexpr ROM_register_array<7> vga PROGMEM = {
    Register{REG::HREF,0xF6},	// was B6  <-- TODO???
    Register{REG::HSTART,0x13},
    Register{REG::HSTOP,0x01},	

    Register{REG::VSTART,0x02},
    Register{REG::VSTOP,0x7a},
    Register{REG::VREF,0x0a}
};


constexpr ROM_register_array<9> qvga PROGMEM = {
    Register{REG::COM14, 0x19},
    Register{REG::SCALING_DCWCTR, 0x11},
    Register{REG::SCALING_PCLK_DIV, 0xf1},

    Register{REG::HSTART,0x16},
    Register{REG::HSTOP,0x04},
    Register{REG::HREF,0x24},

    Register{REG::VSTART,0x02},
    Register{REG::VSTOP,0x7a},
    Register{REG::VREF,0x0a},
};

constexpr ROM_register_array<9> qqvga PROGMEM = {
    Register{REG::COM14, 0x1a},	// divide by 4
    Register{REG::SCALING_DCWCTR, 0x22},		// downsample by 4
    Register{REG::SCALING_PCLK_DIV, 0xf2},		// divide by 4
							
    Register{REG::HSTART,0x16},
    Register{REG::HSTOP,0x04},
    Register{REG::HREF,0xa4},		   

    Register{REG::VSTART,0x02},
    Register{REG::VSTOP,0x7a},
    Register{REG::VREF,0x0a},
};

// <<< TODO: borrar

}// namespace OV7670_register_cfg
}// namespace dev
 
#endif



