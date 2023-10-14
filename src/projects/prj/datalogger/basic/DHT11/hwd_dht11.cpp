#include "../prj_main.h"


void Main::hello()
{
    uart << "\n\nTemperature and humidity datalogger\n"
	        "-----------------------------------\n"
	    "Connect DHT11 to pin " << (int) sensor_pin
					<< " with a 4.7k pull-up resistor\n"
	    "and a cristal of 32kHz in pins TOSC1 and TOSC2\n\n";
}

void print_result(std::ostream& out, Sensor::Result result)
{
    using Result = Sensor::Result;

    switch(result){
	break; case Result::ok:	out << "ok\n";
	break; case Result::train_of_pulses_bad_polarity:
			out << "Bad polarity\n";

	break; case Result::train_of_pulses_bad_size:
			out << "Bad size\n";
	break; case Result::bad_check_sum:
			out << "Wrong check sum\n";
    }	    

}

void Main::print_data(std::ostream& out)
{
    // auto [T, H] = sensor_.read(timeout_ms);
    auto [T, H] = sensor_.read();

    if (sensor_.last_operation_fail())
	print_result(out, sensor_.result_last_operation());

    else{
	print_time(out, Clock::now());
	out << ": " << T << " ºC\t" << H << " %\n";
    }


}

// No tiene configuración
void Main::print_sensor_options()
{ return;}



