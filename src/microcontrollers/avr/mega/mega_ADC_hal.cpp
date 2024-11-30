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
#include "mega_ADC_hal.h"

namespace mega_{
namespace hal{

/// Hace una petición de conversión y espera hasta que haya finalizado
/// devolviendo el valor convertido en arefs (1 aref = AREF/1024 mV).
/// Esta función lee del pin ya seleccionado.
/// devolviendo el valor indicado en arefs (1 aref = AREF/1024 mV)
// TODO: Modificarla para que se pueda elegir un tiempo máximo de espera a
// leer (para ello necesitaría un reloj!!!).
// La función debería devolver el control cuando:
//		1.- ha leido del ADC
//		2.- El timer ha vencido (se puede dar la opción de esperar
//					 eternamente)
//		3.- Ha ocurrido un error (¿puede saberse esto?)
ADC::AREF_type ADC::read_in_arefs()
{
    hwd::ADC::start_conversion();
    wait_until_conversion_is_complete();

    return hwd::ADC::ADC_in_arefs();
}


nm::Result ADC::clock_frequency_in_kHz_1MHz(uint16_t adc_frequency)
{
    if (adc_frequency == 500u)
	clock_frequency_in_kHz_1MHz<500u>();

    else if (adc_frequency == 250u)
	clock_frequency_in_kHz_1MHz<250u>();
    
    else if (adc_frequency == 125u)
	clock_frequency_in_kHz_1MHz<125u>();

    else if (adc_frequency == 62u or adc_frequency == 63u)
	clock_frequency_in_kHz_1MHz<62u>();
    // El resto da frecuencias inferiores a 50kHz

    else
	return nm::fail;

    return nm::ok;
}

nm::Result ADC::clock_frequency_in_kHz_8MHz(uint16_t adc_frequency)
{
    if (adc_frequency == 500u)
	clock_frequency_in_kHz_8MHz<500u>();

    else if (adc_frequency == 250u)
	clock_frequency_in_kHz_8MHz<250u>();
    
    else if (adc_frequency == 125u)
	clock_frequency_in_kHz_8MHz<125u>();

    else if (adc_frequency == 62u or adc_frequency == 63u)
	clock_frequency_in_kHz_8MHz<62u>();
    // El resto da frecuencias inferiores a 50kHz

    else
	return nm::fail;

    return nm::ok;

}


nm::Result ADC::clock_frequency_in_kHz_12MHz(uint16_t adc_frequency)
{
    if (adc_frequency == 375u)
	clock_frequency_in_kHz_12MHz<375u>();

    else if (adc_frequency == 187u or adc_frequency == 188u)
	clock_frequency_in_kHz_12MHz<187u>();
    
    else if (adc_frequency == 94u)
	clock_frequency_in_kHz_12MHz<94u>();

    else
	return nm::fail;

    return nm::ok;

}


}// namespace
}// namespace
