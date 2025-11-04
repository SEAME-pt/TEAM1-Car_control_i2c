#include "../../include/sdl.h"

void	initGpio() {

	if (gpioInitialise() < 0)
		throw("ERROR! Failed inicialization of gpio. Aborting...\n");

    gpioSetMode(PIN_GPIO, PI_INPUT);
    gpioSetPullUpDown(PIN_GPIO, PI_PUD_UP);
    gpioSetAlertFunc(PIN_GPIO, pulse_callback);
}