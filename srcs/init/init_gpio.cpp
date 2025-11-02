#include "../../include/sdl.h"

void pulse_callback(int gpio, int level, uint32_t tick) {
    if(level == 0) // FALLING edge
        g_pulses++;
}

void	initGpio() {

	if (gpioInitialise() < 0)
		throw("ERROR! Failed inicialization of pigpio. Aborting...\n");

	int PIN = 24; // GPI24 = pino físico 24
    gpioSetMode(PIN, PI_INPUT);
    gpioSetPullUpDown(PIN, PI_PUD_UP);
    gpioSetAlertFunc(PIN, pulse_callback);

}