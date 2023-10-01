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

#pragma once

#ifndef __SYMBOL_H__
#define __SYMBOL_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Gestión de la tabla de símbolos (funciones predefinidas,
 *  constantes y variables).
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    05/08/2021 v0.0
 *
 ****************************************************************************/
#include <array>
#include <string.h>
#include <ostream>

struct Constant {
    const char* name;
    double value;
};

struct Variable {
    const char* name;
    double value;
};

struct Built_in_function{ // built-ins
    const char* name;
    double (*func)(double);
};



/// symbol table entry
struct Symbol{
    short type;		    // VAR, BLTIN, UNDEF
    const char* name;
    union {
	double value;		// if VAR
	double (*ptr)(double);  // if BLTIN
    } u;

};


template <size_t Nb, size_t Nc, size_t Nv>
class Symbol_table{
public:
    // Los id los define 'bison', son externos a Symbol_table.
    Symbol_table(short id_builtin, short id_constant, short id_variable)
	:id_builtin_{id_builtin}, 
	 id_constant_{id_constant}, 
	 id_variable_{id_variable}
    {}

    void init (const std::array<Built_in_function, Nb>& builtins,
	    const std::array<Constant, Nc>& consts, 
	    const std::array<Variable, Nv>& variables);

    static constexpr size_t size() {return Nb + Nc + Nv;}

    /// Devuelve el símbolo correspondiente al nombre `name`. Si no lo
    /// encuentra devuelve nullptr.
    Symbol* operator[](const char* name);

    // para depurar
    void print_data(std::ostream& out);

private:
    std::array<Symbol, size()> symbol_;
    
    short id_builtin_;
    short id_constant_;
    short id_variable_;


};



template <size_t Nb, size_t Nc, size_t Nv>
void Symbol_table<Nb, Nc, Nv>::init(
    const std::array<Built_in_function, Nb>& builtins,
    const std::array<Constant, Nc>& consts,
    const std::array<Variable, Nv>& variables)
{
    size_t i = 0;
    for (; i < consts.size(); ++i){
	symbol_[i].name = consts[i].name;
	symbol_[i].type = id_constant_;
	symbol_[i].u.value = consts[i].value;
    }

    for (size_t j = 0; j < builtins.size(); ++i, ++j){
	symbol_[i].name = builtins[j].name;
	symbol_[i].type = id_builtin_;
	symbol_[i].u.ptr= builtins[j].func;
    }

    for (size_t j = 0; j < variables.size(); ++i, ++j){
	symbol_[i].name = variables[j].name;
	symbol_[i].type = id_variable_;
	symbol_[i].u.value = variables[j].value;
    }

}

 
template <size_t Nb, size_t Nc, size_t Nv>
Symbol* Symbol_table<Nb, Nc, Nv>::operator[](const char* name)
{
    for (size_t i = 0; i < symbol_.size(); ++i){
	if (strcmp(symbol_[i].name, name) == 0){
	    return &symbol_[i];
	}
    }

    return nullptr; // not found
}


template <size_t Nb, size_t Nc, size_t Nv>
void Symbol_table<Nb, Nc, Nv>::print_data(std::ostream& out)
{
    out << "------------\n";
    out << "Symbol table:\n";
    out << "size = [" << size() << "]\n";
    for (size_t i = 0; i < symbol_.size(); ++i){
	out << symbol_[i].name << '\n';
	}

    out << "------------\n";
}

#endif


