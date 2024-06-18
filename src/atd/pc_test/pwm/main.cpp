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


#include "../../atd_pwm.h"
#include "../../atd_magnitude.h"

#include <alp_test.h>
#include <alp_string.h>


using namespace test;



void test_pwm_signal()
{
    test::interface("PWM_signal");

    using Hertz       = atd::Hertz<uint32_t>;
    using KiloHertz   = atd::KiloHertz<uint32_t>;
    using MegaHertz   = atd::MegaHertz<uint32_t>;
    using Microsecond = atd::Microsecond<uint32_t>;
    using Nanosecond  = atd::Nanosecond<uint32_t>;

    // Si le pongo nanoseconds puedo manejar calcular inversos de frecuencias
    // de hasta 1 MHz con varias cifras significativas.
    // No puedo usar Seconds como medida de Time ya que estoy usando uint32_t
    // como Rep. Si usaramos float sería más sencillo.
    using PWM_signal = atd::PWM_signal<Hertz, Nanosecond>;

    {// normal
	PWM_signal pwm{Hertz{1'000'000}, atd::Percentage{20}};
	std::cout << pwm.frequency() << ", " << pwm.duty_cycle() << '\n';
	std::cout << pwm.period() << '\n';

	CHECK_TRUE(pwm.frequency() == MegaHertz{1}, "frequency");
	CHECK_TRUE(pwm.period() == Microsecond{1}, "period");
	CHECK_TRUE(pwm.duty_cycle() == atd::Percentage{20}, "duty_cycle");

	pwm.frequency(KiloHertz{123});
	CHECK_TRUE(pwm.frequency() == Hertz{123'000}, "frequency");
	CHECK_TRUE(pwm.period() == Nanosecond{8'130}, "period");
	CHECK_TRUE(pwm.duty_cycle() == atd::Percentage{20}, "duty_cycle");

	pwm.period(Microsecond{123});
	CHECK_TRUE(pwm.frequency() == Hertz{8'130}, "frequency");
	
	// El periodo vale 123'001 ns. Al compararlo con 123
	// microsegundos lo que hace el operator== es convertir los 123 us en
	// 123'000 ns != 123'001 ns fallando el test. Por eso hago el casting.
	CHECK_TRUE(Microsecond{pwm.period()} == Microsecond{123}, "period");
	CHECK_TRUE(pwm.duty_cycle() == atd::Percentage{20}, "duty_cycle");

    }

}



int main()
{
try{
    test::header("PWM");

    test_pwm_signal();

}catch(std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
}





