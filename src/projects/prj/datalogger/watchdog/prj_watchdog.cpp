#include "prj_main.h"

ISR_WDT{
    Main::nseconds = Main::nseconds + 1;
}
