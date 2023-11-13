// Copyright (C) 2020 Manuel Perez 
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

#include "../../dev_BME280_basic.h"

#include <atd_cstddef.h>

#include <avr_atmega.h>

#include <dev_TWI_master.h>

// Microcontroller
// ---------------
namespace mcu = atmega;
using Micro   = mcu::Micro;


// TWI Protocol
// ------------
constexpr uint8_t TWI_buffer_size = 100;
using TWI_master_cfg = dev::TWI_master_cfg<Micro, 
                                           mcu::TWI_basic,
					   TWI_buffer_size>;

using TWI_master  = dev::TWI_master<TWI_master_cfg>;
using TWI = dev::TWI_master_ioxtream<TWI_master>;

// 50 kHz es la unica frecuencia de TWI que va a 1MHz.
// 100 kHz a 8 MHz
static constexpr int TWI_frecuency = 50; // kHz


// Devices
// -------
// En el breakout de adafruit la dirección la determina la conexión del pin
// SDO:
//	si SDO = GND	    ==> sensor_address = 0x76
//	si SDO = flotante   ==> sensor_address = 0x77
static constexpr TWI::Address sensor_address = 0x77;
using Sensor = dev::BME280_TWI<TWI_master, sensor_address>;


// Functions
// ---------
void init_UART()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.turn_on();
}

void init_TWI()
{
    // 50 kHz es la unica frecuencia de TWI que va a 1MHz.
    // 100 kHz a 8 MHz
    TWI_master::turn_on<TWI_frecuency>();
}

void twi_print_state(TWI::iostate st)
{
    mcu::UART_iostream uart;
    uart << "state = ";

    switch(st){
	case TWI::iostate::ok:
	uart << "ok\n";
	break;

	case TWI::iostate::read_or_write:
	uart << "read_or_write\n";
	break;

	case TWI::iostate::sla_w:
	uart << "sla_w\n";
	break;

	case TWI::iostate::sla_r:
	uart << "sla_r\n";
	break;

	case TWI::iostate::no_response:
	uart << "no_response\n";
	break;

	case TWI::iostate::transmitting:
	uart << "transmitting\n";
	break;

	case TWI::iostate::eow:
	uart << "eow\n";
	break;

	case TWI::iostate::eow_data_nack:
	uart << "eow_data_nack\n";
	break;

	case TWI::iostate::error_buffer_size:
	uart << "error_buffer_size\n";
	break;

	case TWI::iostate::receiving:
	uart << "receiving\n";
	break;

	case TWI::iostate::eor_bf:
	uart << "eor_bf\n";
	break;

	case TWI::iostate::eor:
	uart << "eor\n";
	break;

	case TWI::iostate::bus_error:
	uart << "bus_error\n";
	break;

	case TWI::iostate::unknown_error:
	uart << "unknown_error\n";
	break;

	case TWI::iostate::prog_error:
	uart << "prog_error\n";
	break;

	default:
	uart << "desconocido\n";
	break;
    }
}


void bme280_read_all_mem(uint8_t addr, uint8_t* mem, uint8_t n)
{
    TWI twi(sensor_address);
    
    twi << addr;
//    twi.prepare_to_read(n);
    twi.read(mem, n);

    if (twi.error()){
	mcu::UART_iostream uart;
	uart << "ERROR (bme280_read_all_mem): ";
	twi_print_state(TWI::state());
    }

}

void print_in_hex(std::ostream& uart, uint8_t* p, uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i){
	atd::print_int_as_hex(uart, p[i]);
	uart << ' ';
    }
    uart << '\n';
}

void mem_dump_(std::ostream& uart, 
	       uint8_t addr, uint8_t* mem, uint8_t mem_size)
{
    bme280_read_all_mem(addr, mem, mem_size);

    uart << "Memoria [";
    atd::print_int_as_hex(uart, addr);
    uart << "]:\n";

    print_in_hex(uart, mem, mem_size);
    uart << "\n";
}

void mem_dump(std::ostream& uart)
{
//    {
//    constexpr uint8_t mem_size = 24;
//    constexpr uint8_t addr{0x88};
//    uint8_t mem[mem_size];
//    uart << "Calibration params: ";
//
//    mem_dump_(uart, addr, mem, mem_size);
//    }

    // Hay que leerlo en dos partes. 'reset' es de solo escritura,
    // si se intenta leer falla la lectura del resto.
    {
    constexpr uint8_t mem_size = 1;
    constexpr uint8_t addr{0xD0};
    uint8_t mem[mem_size];

    mem_dump_(uart, addr, mem, mem_size);
    }
    {
    constexpr uint8_t mem_size = 3;
    constexpr uint8_t addr{0xF3};
    uint8_t mem[mem_size];
    mem_dump_(uart, addr, mem, mem_size);
    }
    {
    constexpr uint8_t mem_size = 6;
    constexpr uint8_t addr{0xF7};
    uint8_t mem[mem_size];
    mem_dump_(uart, addr, mem, mem_size);
    }


}



void print_mode(std::ostream& out, uint8_t mode)
{
    using Cfg = dev::impl_of::BME280_config;
    if (mode == Cfg::sleep_mode)
	out << "sleep_mode";
    else if (mode == Cfg::force_mode)
	out << "force_mode";
    else if (mode == Cfg::normal_mode)
	out << "normal_mode";
    else
	out << "ERROR: Unknown mode";
}

void print_oversampling(std::ostream& out, uint8_t osr)
{
    using Cfg = dev::impl_of::BME280_config;
    if (osr == Cfg::oversampling_none)
	out << "oversampling_none";

    else if (osr == Cfg::oversampling_x1)
	out << "oversampling_x1";

    else if (osr == Cfg::oversampling_x2)
	out << "oversampling_x2";

    else if (osr == Cfg::oversampling_x4)
	out << "oversampling_x4";

    else if (osr == Cfg::oversampling_x8)
	out << "oversampling_x8";

    else if (osr == Cfg::oversampling_x16)
	out << "oversampling_x16";

    else
	out << "ERROR: Unknown";

}

void print_t_sb(std::ostream& out, uint8_t t_sb)
{
    using Cfg = dev::impl_of::BME280_config;

    if (t_sb == Cfg::t_sb_0_5_ms)
	out << "t_sb_0_5_ms";

    else if (t_sb == Cfg::t_sb_62_5_ms)
	out << "t_sb_62_5_ms";

    else if (t_sb == Cfg::t_sb_125_ms)
	out << "t_sb_125_ms";

    else if (t_sb == Cfg::t_sb_250_ms)
	out << "t_sb_250_ms";

    else if (t_sb == Cfg::t_sb_500_ms)
	out << "t_sb_500_ms";

    else if (t_sb == Cfg::t_sb_1000_ms)
	out << "t_sb_1000_ms";

    else if (t_sb == Cfg::t_sb_2000_ms)
	out << "t_sb_2000_ms";

    else if (t_sb == Cfg::t_sb_4000_ms)
	out << "t_sb_4000_ms";

    else
	out << "ERROR: unknown";
}

void print_filter(std::ostream& out, uint8_t filter)
{
    using Cfg = dev::impl_of::BME280_config;

    if (filter == Cfg::filter_off)
	out << "filter_off";

    else if (filter == Cfg::filter_coeff_2)
	out << "filter_coeff_2";

    else if (filter == Cfg::filter_coeff_4)
	out << "filter_coeff_4";

    else if (filter == Cfg::filter_coeff_8)
	out << "filter_coeff_8";

    else if (filter == Cfg::filter_coeff_16)
	out << "filter_coeff_16";

    else
	out << "ERROR: unknown filter: [" << (int) filter << "]\n";
}

void print_spi3w_en(std::ostream& out, uint8_t spi3w_en)
{
    using Cfg = dev::impl_of::BME280_config;

    if (spi3w_en == Cfg::spi3w_disable)
	out << " spi3w_disable";

    else if (spi3w_en == Cfg::spi3w_enable)
	out << " spi3w_enable";


    else
	out << "ERROR: unknown";
}

void print_ok_wrong(std::ostream& out, bool x)
{
    if (x)  out << " OK\n";
    else    out << " WRONG\n";
}

void print_equal_sign(std::ostream& out, bool x)
{
    if (x)  out << " == ";
    else    out << " != ";
}

void print_cfg(std::ostream& out, 
	      dev::impl_of::BME280_config& cfg0,
	      dev::impl_of::BME280_config& cfg1)
{
    out << "\nWriting configuration\n"
	     "---------------------\n";
    out << "            :     WRITING\t\tREADING\n";
    out << "mode        : ";
    print_mode(out, cfg0.mode); out << '\t';
    print_equal_sign(out, cfg0.mode == cfg1.mode);
    print_mode(out, cfg1.mode);
    out << "\t\t";
    print_ok_wrong(out, cfg0.mode == cfg1.mode);


    out << "osrs_t      : ";
    print_oversampling(out, cfg0.osrs_t); out << '\t';
    print_equal_sign(out, cfg0.osrs_t == cfg1.osrs_t);
    print_oversampling(out, cfg1.osrs_t); 
    out << "\t";
    print_ok_wrong(out, cfg0.osrs_t == cfg1.osrs_t);

    out << "osrs_p      : ";
    print_oversampling(out, cfg0.osrs_p); out << '\t';
    print_equal_sign(out, cfg0.osrs_p == cfg1.osrs_p);
    print_oversampling(out, cfg1.osrs_p);
    out << "\t";
    print_ok_wrong(out, cfg0.osrs_p == cfg1.osrs_p);

    out << "t_sb        : ";
    print_t_sb(out, cfg0.t_sb); out << '\t';
    print_equal_sign(out, cfg0.t_sb == cfg1.t_sb); 
    print_t_sb(out, cfg1.t_sb); 
    out << "\t\t";
    print_ok_wrong(out, cfg0.t_sb == cfg1.t_sb); 

    out << "filter_coeff: ";
    print_filter(out, cfg0.filter); out << '\t';
    print_equal_sign(out, cfg0.filter == cfg1.filter);
    print_filter(out, cfg1.filter);
    out << "\t";
    print_ok_wrong(out, cfg0.filter == cfg1.filter);

    out << "spi3w_en    :";
    print_spi3w_en(out, cfg0.spi3w_en); out << '\t';
    print_equal_sign(out, cfg0.spi3w_en == cfg1.spi3w_en); 
    print_spi3w_en(out, cfg1.spi3w_en); 
    out << "\t";
    print_ok_wrong(out, cfg0.spi3w_en == cfg1.spi3w_en); 
    out << "-------------------------\n";
}
//
//
//void init(Sensor& sensor)
//{
//    mcu::UART_iostream uart;
//    uart << "init ... ";
//    sensor.init();
//    if (sensor.error()){
//	uart << "ERROR ... ";
//	twi_print_state(sensor.state());
//    }
//    else
//	uart << "OK\n";
//
//    // Modos por defecto que propone Bosch.
////    sensor.handheld_device_low_power();
//    // sensor.handheld_device_dynamic();
//    // sensor.weather_monitoring(); <-- este es forced mode
//    // sensor.elevator_floor_change_detector();
//    // sensor.drop_detection();
//    // sensor.indoor_navigation();
//
//// Configuración a bajo nivel:
//// Table 9: indoor navigation
//    using Cfg = Sensor::Config;
//    Cfg cfg;
//    cfg.mode   = normal_mode;
//    cfg.t_sb   = t_sb_0_5_ms; 
//    cfg.osrs_p = oversampling_x16;
//    cfg.osrs_t = oversampling_x2;
//    cfg.osrs_h = oversampling_x1;
//    cfg.filter = filter_coeff_16;
//    cfg.spi3w_en = Cfg::spi3w_disable; // es TWI
//
//    sensor.write(cfg); 
//
//
//    if (sensor.error()){
//	uart << "Error al configurar el sensor\n";
//	twi_print_state(sensor.state());
//    }
//    else
//	uart << "cfg OK\n";
//
//
//// depurar configuración:
//    Sensor::Config cfg2;
//    sensor.read(cfg2);
//    uart << "LEIDOS:\n";
//
//    print_cfg(uart, cfg2);
//}


void print_params(std::iostream& uart, Sensor& sensor)
{
    uart << "\tdig_T1 = [" << sensor.dig_T1() << "]\n";
    uart << "\tdig_T2 = [" << sensor.dig_T2() << "]\n";
    uart << "\tdig_T3 = [" << sensor.dig_T3() << "]\n";

    uart << "\tdig_P1 = [" << sensor.dig_P1() << "]\n";
    uart << "\tdig_P2 = [" << sensor.dig_P2() << "]\n";
    uart << "\tdig_P3 = [" << sensor.dig_P3() << "]\n";
    uart << "\tdig_P4 = [" << sensor.dig_P4() << "]\n";
    uart << "\tdig_P5 = [" << sensor.dig_P5() << "]\n";
    uart << "\tdig_P6 = [" << sensor.dig_P6() << "]\n";
    uart << "\tdig_P7 = [" << sensor.dig_P7() << "]\n";
    uart << "\tdig_P8 = [" << sensor.dig_P8() << "]\n";
    uart << "\tdig_P9 = [" << sensor.dig_P9() << "]\n";

    uart << "\tdig_H1 = [" << (int) sensor.dig_H1() << "]\n";
    uart << "\tdig_H2 = [" << sensor.dig_H2() << "]\n";
    uart << "\tdig_H3 = [" << (int) sensor.dig_H3() << "]\n";
    uart << "\tdig_H4 = [" << sensor.dig_H4() << "]\n";
    uart << "\tdig_H5 = [" << sensor.dig_H5() << "]\n";
    uart << "\tdig_H6 = [" << (int) sensor.dig_H6() << "]\n";
}




//void debug(std::ostream& out, Sensor& sensor)
//{
//    Sensor::Temp_and_press tp;
//    sensor.read(tp);
//
//    out << "\nsin comp: uT = " << tp.utemperature
//	<< "; uP = " << tp.upressure << '\n';
////	<< "comp: T = " << sensor.compensate_T(tp.utemperature) << " ºC; ";
////    int32_t press_q248 = sensor.compensate_P_(tp.upressure);
////    out << "P comp. = " << press_q248 
////			<< " (" << press_q248/25600 << " hPa)\n";
//
////    auto press = sensor.compensate_P(tp.upressure);
////    out << "P = " << press << " Pa (" << Sensor::Hectopascal{press}
////        << " hPa)\n";
//
//}

//void test_bme280()
//{
//// init_UART();
//    mcu::UART_iostream uart;
//    mcu::basic_cfg(uart);
//    uart.turn_on();
//
//    uart << "----------------------------------------\n"
//	 << "BME280\n"
//	 << "----------------------------------------\n\n";
//
//// init_TWI();
//    // 50 kHz es la unica frecuencia de TWI que va a 1MHz.
//    // 100 kHz a 8 MHz
//    TWI_master::on<50>();
//
//// init_sensor();
//    Sensor sensor;
//    init(sensor);
//
//    while (1){
////	debug(uart, sensor);
//
//	auto [T, P] = sensor.T_and_P();
//	uart << "T_and_P = " << T << " ºC; P = " << P << " Pa\n";
//
//	auto [T2, hP] = sensor.T_and_hP();
//	uart << "T_and_hP= " << T2 << " ºC; P = " << hP << " hPa\n";
//
//	uart << "T       = " << sensor.T() << " ºC\n";
//
//	// Probar a desconectar el sensor mientras está funcionando. Tiene que
//	// generar el error correspondiente.
//	if (sensor.no_response())
//	    uart << "Error: no response\n";
//
//	if (sensor.error())
//	    uart << "Error: error en el sensor!!!\n";
//
//        wait_ms(4000);
//    }
//}


void probe_sensor()
{
    mcu::UART_iostream uart;

    Sensor sensor;

    uart << '\n';

    while(1){
	uart << "Connecting to sensor ... ";

	Sensor::Id id;
	sensor.read(id);
	if (sensor.no_response())
	    uart << "Error: no response\n";

	else if (sensor.error()){
	    uart << "Error: ";
	    twi_print_state(sensor.state());
	    uart << '\n';
	}

	else if (!id.is_valid())
	    uart << "Invalid ID. Not a BME280 sensor!!!\n";

	else{
	    uart << "found BME280.\n";
	    return;
	}

	Micro::wait_ms(1000);

    }
}


void print_params()
{
    mcu::UART_iostream uart;

    uart << "Params: \n";

    while(1){
	Sensor sensor;
//	sensor.init();
	if (sensor.error()){
	    uart << "ERROR ... ";
	    twi_print_state(sensor.state());
	}
	else {
	    print_params(uart, sensor);
	    return;
	}

	Micro::wait_ms(5000);
    }
}


void test_config()
{
    mcu::UART_iostream uart;

    Sensor sensor;

    while(1){
	// Table 9: indoor navigation
	using Cfg = Sensor::Config;
	Cfg cfg0;
	cfg0.mode   = Cfg::normal_mode;
	cfg0.t_sb   = Cfg::t_sb_0_5_ms; 
	cfg0.osrs_p = Cfg::oversampling_x16;
	cfg0.osrs_t = Cfg::oversampling_x2;
	cfg0.osrs_h = Cfg::oversampling_x1;
	cfg0.filter = Cfg::filter_coeff_16;
	cfg0.spi3w_en = Cfg::spi3w_disable; // es TWI

	sensor.write(cfg0); 

	if (sensor.no_response())
	    uart << "Error: no response\n";

	else if (sensor.error()){
	    uart << "Error: ";
	    twi_print_state(sensor.state());
	    uart << '\n';
	}
	else {
	    Sensor::Config cfg1;
	    sensor.read(cfg1);

	    print_cfg(uart, cfg0, cfg1);

	    if (cfg0 == cfg1){
		uart << "OK\n";
		return;
	    }
	    else
		uart << "ERROR: can't write configuration\n";
	}

	Micro::wait_ms(5000);

    }
}


void test_uncompensated()
{
    mcu::UART_iostream uart;

    uart << "----------------------------------------\n"
	 << "BME280 (test uncompensated)\n"
	 << "----------------------------------------\n\n";


// init_sensor();
    Sensor sensor;

    // Table 9: indoor navigation
    using Cfg = Sensor::Config;
    Cfg cfg0;
    cfg0.mode   = Cfg::normal_mode;
    cfg0.t_sb   = Cfg::t_sb_0_5_ms; 
    cfg0.osrs_p = Cfg::oversampling_x16;
    cfg0.osrs_t = Cfg::oversampling_x2;
    cfg0.osrs_h = Cfg::oversampling_x1;
    cfg0.filter = Cfg::filter_coeff_16;
    cfg0.spi3w_en = Cfg::spi3w_disable; // es TWI

    sensor.write(cfg0); 

    if (sensor.no_response())
	uart << "Error: no response\n";

    else if (sensor.error()){
	uart << "Error: ";
	twi_print_state(sensor.state());
	uart << '\n';
    }
    else
	uart << "Configuracion: OK\n";


    while(1){
	Sensor::Temp_and_press_and_hum data;

	sensor.read(data);

	if (sensor.no_response())
	    uart << "Error: no response\n";

	else if (sensor.error()){
	    uart << "Error: ";
	    twi_print_state(sensor.state());
	    uart << '\n';
	}
	else{
	    uart << "\n\nupressure    = " << data.upressure << '\n'
		 << "utemperature = " << data.utemperature << '\n'
		 << "uhumidity    = " << data.uhumidity << '\n';
	}

	Micro::wait_ms(3000);

    }
}


void test_t_p_h()
{
    mcu::UART_iostream uart;

    uart << "----------------------------------------\n"
	 << "BME280 (test T, P, H)\n"
	 << "----------------------------------------\n\n";


// init_sensor();
    Sensor sensor;

//    sensor.init(); // FUNDAMENTAL!!!

    // Table 9: indoor navigation
//    using Cfg = Sensor::Config;
//    Cfg cfg0;
//    cfg0.mode   = Cfg::normal_mode;
//    cfg0.t_sb   = Cfg::t_sb_0_5_ms; 
//    cfg0.osrs_p = Cfg::oversampling_x16;
//    cfg0.osrs_t = Cfg::oversampling_x2;
//    cfg0.osrs_h = Cfg::oversampling_x1;
//    cfg0.filter = Cfg::filter_coeff_16;
//    cfg0.spi3w_en = Cfg::spi3w_disable; // es TWI
//    sensor.write(cfg0); 

    sensor.indoor_navigation();

    if (sensor.no_response())
	uart << "Error: no response\n";

    else if (sensor.error()){
	uart << "Error: ";
	twi_print_state(sensor.state());
	uart << '\n';
    }
    else
	uart << "Configuracion: OK\n";


    while(1){
//	using Hectopascal = Sensor::Hectopascal;
	Sensor::Celsius T;
	Sensor::Pascal P;
	Sensor::Relative_humidity H;
	sensor.T_and_P_and_H(T, P, H);

	uart  << "T_and_P = " << T << "; P = " << P
	      << "; H = " << H << "%\n";

	if (sensor.no_response())
	    uart << "Error: no response\n";

	else if (sensor.error()){
	    uart << "Error: ";
	    twi_print_state(sensor.state());
	    uart << '\n';
	}


	Micro::wait_ms(3000);

    }
}

void hello()
{
    mcu::UART_iostream uart;
    uart << "\n\nBME280 test\n"
	    "-----------\n"
	    "slave_address       =\t";
    atd::print_int_as_hex(uart, sensor_address);
    uart << "\nTWI SCL frequency =\t" << (int) TWI_frecuency << '\n';

}

int main()
{
    init_UART();
    init_TWI();

    hello();

    while (1){
    // TODO: convertir esto en menu y probar bien
    probe_sensor();
    print_params();
    test_config();
//    test_uncompensated();
    test_t_p_h();
    }
}




ISR(TWI_vect)
{
    TWI_master::handle_interrupt();
}


