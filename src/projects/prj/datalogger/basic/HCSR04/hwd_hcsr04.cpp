#include "../prj_main.h"
#include "hwd_hcsr04.h"

void Main::hello()
{
    uart << "\n\nDistance datalogger\n"
	        "-------------------\n"
	    "Connect HCSR04 echo pin to pin " << (int) echo_pin
	    << " and trigger pin to pin " << (int) trigger_pin
	    << "\nand a cristal of 32kHz in pins TOSC1 and TOSC2\n\n";
}

void  print_result(std::ostream& out, Sensor::Result result)
{
    using Result = Sensor::Result;

    switch(result){
	break; case Result::ok: 
			out << "OK\n"; 

	break; case Result::time_out:
			out << "TIMEOUT\n";

    }
}

void Main::print_data(std::ostream& out)
{
    atd::Meter<atd::uFloat16> m = sensor_.read();
    if (sensor_.last_operation_fail()){
	print_result(out, sensor_.result_last_operation());
	return;
    }

    print_time(out, Clock::now());
    atd::Centimeter<atd::uFloat16> cm = m;
    out << ": " << m << " = " << cm << '\n';
}


// No tiene configuración
void Main::print_sensor_options()
{ return;}

