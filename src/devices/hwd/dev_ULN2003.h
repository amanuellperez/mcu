// Copyright (C) 2024 Manuel Perez 
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

#ifndef __DEV_ULN2003_H__
#define __DEV_ULN2003_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del ULN2003	
 *
 * HISTORIA
 *    Manuel Perez
 *    20/06/2024 Empezando...
 *    TODO: sin acabar. Este driver bloquea por completo el micro, no parece
 *    muy práctico. ¿Estará pensado para ser controlado electrónicamente? De
 *    momento prefiero implementar el A4988
 *
 ****************************************************************************/
#include <cstdint>

namespace dev{

// DUDA: ¿cómo se podría definir la cfg con nombres y pasarla como parametro
// de template a ULN2003_basic?
//	Esto es:
//	    using Cfg = ULN2003_basic_cfg{IN1 = 11; IN2 = 12; ...}
//	    using ULN2003 = dev::ULN2003_basic<Cfg>; (???)
template <typename Micro0, 
	  uint8_t IN1_pin, uint8_t IN2_pin, 
	  uint8_t IN3_pin, uint8_t IN4_pin>
struct ULN2003_basic_cfg{
    using Micro = Micro0;
    static constexpr uint8_t IN1 = IN1_pin;
    static constexpr uint8_t IN2 = IN2_pin;
    static constexpr uint8_t IN3 = IN3_pin;
    static constexpr uint8_t IN4 = IN4_pin;
};

// TODO: (o duda)
//	Si tengo 2 motores y los conecto a un ULN2003_basic voy a generar
//	2 clases diferentes (!!!) por culpa de que Cfg es distinta: 
//	ULN2003_basic<Cfg1> y ULN2003_basic<Cfg2> serán clases distintas, lo
//	cual duplicará el código.
//	
//	Cuando es de esperar que solo haya un dispositivo en una aplicación no
//	generá más código, pero en casos como este puede que sí. 
//	Eso se puede resolver escribiendo ULN2003_basic como un mero wrapper
//	(o clase hija) de otra clase común a todas las ULN2003_basic<Cfg_n>.
//	Como de momento es experimental no lo implemento. Si se usa recordar
//	que tal como ahora está escrito seguramente generé más código del
//	debido.
template <typename Cfg>
class ULN2003_basic
{
public:
// Constructor
    ULN2003_basic() = delete; // static interface
    static void init();

// stepping
    static void step();

private:
// Types
    using Micro = Cfg::Micro;

    template <uint8_t n>
    using Pin = typename Micro::Pin<n>;

    using IN1 = Pin<Cfg::IN1>;
    using IN2 = Pin<Cfg::IN2>;
    using IN3 = Pin<Cfg::IN3>;
    using IN4 = Pin<Cfg::IN4>;


// Helpers
    static void write(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);

};

template <typename C>
void ULN2003_basic<C>::init()
{
    IN1::as_output();
    IN2::as_output();
    IN3::as_output();
    IN4::as_output();
}


template <typename C>
// inline   // inline or not inline? 
void ULN2003_basic<C>::write(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4)
{
    IN1::write(in1);
    IN2::write(in2);
    IN3::write(in3);
    IN4::write(in4);
    // Lo minimo para que gire son 800 us
    Micro::wait_ms(1);
}


template <typename C>
void ULN2003_basic<C>::step()
{
    write(0,0,0,1);
    write(0,0,1,1);
    write(0,0,1,0);
    write(0,1,1,0);
    write(0,1,0,0);
    write(1,1,0,0);
    write(1,0,0,0);
    write(1,0,0,1);
}

}// namespace dev

#endif


