#include "../../include/sdl.h"

void	initGpio() {

    if (gpioInitialise() < 0)
        throw std::runtime_error("ERROR! Failed initialization of GPIO. Aborting...");

    if (gpioSetMode(PIN_GPIO, PI_INPUT) < 0) {
        gpioTerminate();
        throw std::runtime_error("ERROR! Failed to set GPIO pin mode. Aborting...");
    }

    if (gpioSetPullUpDown(PIN_GPIO, PI_PUD_UP) < 0) {
        gpioTerminate();
        throw std::runtime_error("ERROR! Failed to set GPIO pull-up. Aborting...");
    }

    if (gpioSetAlertFunc(PIN_GPIO, pulse_callback) < 0) {
        gpioTerminate();
        throw std::runtime_error("ERROR! Failed to set GPIO alert callback. Aborting...");
    }
}
