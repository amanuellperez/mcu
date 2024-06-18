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

#ifndef __MCU_STD_STDEXCEPT_H__
#define __MCU_STD_STDEXCEPT_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Equivalente a <stdexcept>
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
#include "std_exception.h"

namespace STD{

// logic_error
// -----------
class logic_error: public exception{
public:
//    explicit logic_error(const string& what_arg);
    explicit logic_error(const char* what_arg);
};

// domain_error
// ------------
class domain_error: public logic_error{
public:
    // explicit domain_error(const string& what_arg);
    explicit domain_error(const char* what_arg);
};

// invalid_argument
// ----------------
class invalid_argument: public logic_error{
public:
    //explicit invalid_argument(const string& what_arg);
    explicit invalid_argument(const char* what_arg);
};

// length_error
// ------------
class length_error : public logic_error{
public:
    //explicit length_error(const string& what_arg);
    explicit length_error(const char* what_arg);
};

// out_of_range
// ------------
class out_of_range : public logic_error{
public:
    //explicit out_of_range(const string& what_arg);
    explicit out_of_range(const char* what_arg);
};

// runtime_error
// ------------
class runtime_error : public exception{
public:
    //explicit runtime_error(const string& what_arg);
    explicit runtime_error(const char* what_arg);
};

// range_error
// ------------
class range_error : public runtime_error{
public:
    //explicit range_error(const string& what_arg);
    explicit range_error(const char* what_arg);
};


// overflow_error
// ------------
class overflow_error : public runtime_error{
public:
    //explicit overflow_error(const string& what_arg);
    explicit overflow_error(const char* what_arg);
};

// underflow_error
// ------------
class underflow_error : public runtime_error{
public:
    //explicit underflow_error(const string& what_arg);
    explicit underflow_error(const char* what_arg);
};


}// namespace STD
#endif



