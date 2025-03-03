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
/****************************************************************************
 *
 * DESCRIPCION
 *	Funciones de impresión que no pertenecen a la aplicación particular. 
 *
 * HISTORIA
 *    Manuel Perez
 *    24/04/2023 Escrito
 *
 ****************************************************************************/
#include "prj_dev.h"

#include "dev_print.h"
#include "dev_strings.h"

/***************************************************************************
 *			    MENSAJES GENÉRICOS
 ***************************************************************************/
void print_bool_as_yes_no(std::ostream& out, bool x)
{
    if (x)
        atd::print(out, msg_yes);
    else
        atd::print(out, msg_no);

    out << '\n';
}


void ask_press_key_to_continue(std::iostream& out)
{
    out << '\n';
    atd::print(out, msg_press_key_to_continue);
    char c{};
    out >> c;
}


