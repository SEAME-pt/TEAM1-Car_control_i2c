#include "../../include/sdl.h"

void	initGpio() {

	if (gpioInitialise() < 0)
		throw("ERROR! Failed inicialization of gpio. Aborting...\n");

	int PIN = 24; // GPI24 = pino físico 24
    gpioSetMode(PIN, PI_INPUT);
    gpioSetPullUpDown(PIN, PI_PUD_UP);
    gpioSetAlertFunc(PIN, pulse_callback);
}