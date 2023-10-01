// Copyright (C) 2022 Manuel Perez 
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

#include "prj_main.h"

// ¿Dónde poner este warning? Si lo pongo en dev.h se genera el warning al
// compilar todos los ficheros, lo cual ocultaría warnings reales. De momento
// lo dejo aquí.
#if F_CPU==8000000UL
#pragma GCC warning "Micro in 8MHz: remember to execute `make set_fast_fuse`"
#endif

void Main::init_uart()
{
    UART::init();
}


Main::Main()
{
    init_uart();
    Miniclock_us::cfg();
}


int main()
{
    Main app;
    app.run();
}



