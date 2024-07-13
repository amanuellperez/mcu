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

#pragma once

#ifndef __ATD_TEST_H__
#define __ATD_TEST_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Utilidades para hacer pruebas automáticas.
 *
 *    Uno de los problemas que hay a la hora de programar es probar el código
 *    escrito. Es importante hacer test, test, y más test.
 *
 *    Pero ¿cómo hacerlo con avr?
 *
 *    Mientras lo que se quiera no sea probar un interfaz de usuario (un LCD,
 *    un teclado, ...) todas las pruebas se pueden hacer (casi-)automáticas
 *    similar a como las realizo en el PC: en lugar de mostrar el resultado
 *    por pantalla "std::cout" lo mostramos en un terminal serie en el
 *    ordenador. Basta con conectar el avr con el ordenador y mirar la salida
 *    en pantalla para ver si funciona  o no.
 *
 *    Lo baso en el 'test' de 'alp'. La diferencia con alp es que atd no lanza
 *    excepciones.
 *
 *    TODO: De momento voy a implementar lo mínimo para poder empezar a hacer
 *    pruebas automáticas. Pero se podría generar un cliente en el ordenador
 *    que fuese interaccione con el avr vía UART. De tal manera que este
 *    cliente, para cada programa que se quiere probar:
 *	    1. lo compila y lo carga en el avr.
 *	    2. se comunica por uart mostrando al usuario el resultado.
 *	    3. el programa en el uart marca cuando se han hecho todas las
 *	       pruebas automáticas de tal manera que el cliente pueda compilar
 *	       el siguiente programa y cargarlo en el avr (volvemos a 1)
 *	    4. en caso de error el cliente le envía un código de error al
 *	       cliente del PC de tal manera que el cliente muestra un mensaje
 *	       de error, parando la compilación. De esta forma se pueden
 *	       probar un montón de programas a la vez sin tener que estar
 *	       pendientes de la ejecución. Si falla, ya nos lo dirá el
 *	       cliente.
 *	
 *
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    23/02/2022 v0.0: Escrita.
 *
 ****************************************************************************/
#include <ostream>

namespace test{

class Test{
public:
    Test(std::ostream& out0) : out_ {&out0} { }

    static Test interface(std::ostream& out, const char* name);

    void interface(const char* name);

    void check_true( bool condition, 
		     const char* fname, int line, 
		     const char* test_name);

    void silent_mode(bool be_silent = true) {silent_mode_ = be_silent;}

private:
    std::ostream& out() {return *out_;}
    std::ostream* out_;
    bool silent_mode_ = false;

    void line();
};

inline void Test::line()
{ out() << "---------------------------------\n"; }

// TODO: no he conseguido encontrar la definición de
// __builtin_source_location(), no pudiendo implementar source_location.
// Una vez que se implemente, se puede eliminar esta macro y llamar
// directamente a:
//	test.check_true(condicion, prueba); 
// siendo check_true una función real de Test.
//
#define CHECK_TRUE(test, condicion, prueba) \
	    {test.check_true((condicion), (__FILE__), (__LINE__), (prueba));}

}// namespace test

#endif


