#include "../../include/sdl.h"

void	cleanExit() {

	if (g_joystick) {
		SDL_JoystickClose(g_joystick);
		g_joystick = nullptr;
	}

    I2c::stop_all();

    SDL_Quit();

	gpioSetAlertFunc(PIN_GPIO, NULL);
    gpioTerminate();

	exit(EXIT_SUCCESS);
}

void	exitSDL() {

	if (g_joystick) {
		std::cout << "hoi\n";
		SDL_JoystickClose(g_joystick);
		g_joystick = nullptr;
	}
	exit(EXIT_FAILURE);
}

void	exitCar() {

	if (g_joystick) {
		SDL_JoystickClose(g_joystick);
		g_joystick = nullptr;
	}
	I2c::stop_all();
    SDL_Quit();
	exit(EXIT_FAILURE);
}