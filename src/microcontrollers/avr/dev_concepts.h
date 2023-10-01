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

#ifndef __DEV_CONCEPTS_H__
#define __DEV_CONCEPTS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Concepts devices.
 *	
 *	WARNING: nunca he usado los concepts y tampoco tengo claro las
 *	características genéricas de los dispositivos. Así que esto irá
 *	evolucionando a medida que gane experiencia. Conclusión: esto, al
 *	principio, será MUY INESTABLE.
 *
 *	¿Cuáles son las ideas claves que caracterizan los devices? Esas ideas
 *	deberían de traducirse en C++ concepts. Este es un primer intento de
 *	hacerlo.
 *
 *	Por otra parte es muy atrayente la idea de usar los concepts para
 *	definir las características genéricas de un device. ¿Qué es un Timer?
 *	Yo quiero que las clases Miniclock, Clock, ... vengan parametrizadas
 *	por el concept `Timer` y no por un `typename`. Creo (???) que este no 
 *	es el uso para el que se concibieron los concepts pero puede que sea
 *	muy útil. Seguramente acabe experimentando con ello.
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    11/12/2022 Unsafe_device
 *
 ****************************************************************************/
namespace dev{

// Timer1 tiene un contador de 16 bits que para acceder hay que bloquear las
// interrupciones. No hacerlo podría generar errores.
//
// Timer0 al tener un contador de 8 bits no padece de ese problema.
//
// Eso quiere decir que mientras el interfaz de Timer0 es `safe` en el sentido
// de que no es necesario bloquear las interrupciones, Timer1 suministra
// funciones unsafe_ para recordar que hay que hacer algo más si se quiere
// usar esa función sin problemas. 
//
// La implementación de Miniclock tiene que saber si el dispositivo es safe
// (para llamar funciones sin prefijo) o unsafe (para llamar las mismas
// funciones con prefijo safe_).
template <typename T>
concept Unsafe_device = requires(T a){
    {T::is_unsafe};	// prueba: usemos init como marcador
};

template <typename T>
concept Safe_device = (!Unsafe_device<T>);

}


#endif


