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

#ifndef __SDC_PRINT_H__
#define __SDC_PRINT_H__

// Funciones que solo dependen de la SD Card y de los mensajes.
#include "prj_dev.h"	// OBLIGATORIO PONERLO.
			// Tal como está hecho ahora, las `strings` usan 
			// my_mcu::Progmem <-- necesitan saber el micro `mcu` que
			// se usa!!! ¿Cómo parametrizarlo? ¿Se pueden hacer
			// estos mensajes genéricos de tal manera que si no se
			// llaman no se guarde nada en PROGMEM?
#include "sdc_strings.h"
		     
// Convenio (v0.0):
//  + Funciones `print` se limitan a imprimir un mensaje.
//  + Funciones `ask` imprimen un mensaje y devuelven una respuesta (sería el
//    análogo a `scanf` de C)
void print(std::ostream& out, const SDCard::R1& r1);
void print_if_error_r1(std::ostream& out, const SDCard::R1& r1);
void print_raw_R3(std::ostream& out, const SDCard::R3& r3);
void print_type_card(std::ostream& out, const SDCard::R3& r3);
void print(std::ostream& out, const SDCard::R3& r3);
void print(std::ostream& out, const SDCard::R7& r7);
void print(std::ostream& out, const SDCard::Read_return& r);
void print(std::ostream& out, const SDCard::R2& r);
void print(std::ostream& out, const SDCard::Write_return& r);
void print(std::ostream& out, const SDCard::Data_response_token& r);



#endif


