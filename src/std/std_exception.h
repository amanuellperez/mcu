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

#pragma once

#ifndef __MCU_STD_EXCEPTION_H__
#define __MCU_STD_EXCEPTION_H__

/****************************************************************************
 *
 * DESCRIPCION
 *	Equivalente a <except>
 *
 * TODO: 
 *	Falta implementarlo todo. Como no voy a lanzar excepciones solo necesito
 *	los tipos correspondientes para que funcione mtd::throw_exception.
 *
 * HISTORIA
 *    Manuel Perez
 *    08/10/2023 Escrito
 *
 ****************************************************************************/
#include "std_config.h"

namespace STD{

class exception{
public:
    exception() noexcept;
    exception(const exception&) noexcept;
    exception& operator=(const exception&) noexcept;
    virtual ~exception();
    virtual const char* what() const noexcept;
};


}// namespace STD
#endif



