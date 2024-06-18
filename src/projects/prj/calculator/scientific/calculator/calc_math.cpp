// Copyright (C) 2021 Manuel Perez 
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


#include <math.h>
#include <errno.h>

#include "error.h"

extern int errno;

static double errcheck(double d, const char* function_name)
{
    if (errno == EDOM){
	errno = 0;
	msg_error(function_name, "argument out of domain");

    } else if (errno == ERANGE){
	errno = 0;
	msg_error(function_name, "result out of range");
    }

    return d;
}


double Log(double x)
{ return errcheck(log(x), "log"); }

double Log10(double x)
{ return errcheck(log10(x), "log10"); }

double Exp(double x)
{ return errcheck(exp(x), "exp"); }

double Sqrt(double x)
{ return errcheck(sqrt(x), "sqrt"); }

double Pow(double x, double y)
{ return errcheck(pow(x, y), "exponentiation"); }

double integer(double x)
{ return (double)(long)x;}


