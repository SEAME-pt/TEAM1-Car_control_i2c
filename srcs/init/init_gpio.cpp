#include "../../include/sdl.h"

void	initGpio() {

    if (gpioInitialise() < 0)
        throw GenException::InitException("Failed to initialize GPIO");

    if (gpioSetMode(PIN_GPIO, PI_INPUT) < 0) {
        gpioTerminate();
        throw GenException::InitException("Failed to set GPIO pin mode");
    }

    if (gpioSetPullUpDown(PIN_GPIO, PI_PUD_UP) < 0) {
        gpioTerminate();
        throw GenException::InitException("Failed to set GPIO pull-up");
    }

    if (gpioSetAlertFunc(PIN_GPIO, pulse_callback) < 0) {
        gpioTerminate();
        throw GenException::InitException("Failed to set GPIO alert callback");
    }
}
