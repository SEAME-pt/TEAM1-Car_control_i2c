#include "../../include/sdl.h"

void	cleanExit() {

	if (g_joystick) {
		SDL_JoystickClose(g_joystick);
		g_joystick = nullptr;
	}

	I2c::All_close();

    SDL_Quit();

	gpioSetAlertFunc(PIN_GPIO, NULL);
    gpioTerminate();

	std::cout << "Program has reached the end with success. It was a pleasure. Shutting down..." << std::endl;

	exit(EXIT_SUCCESS);
}

void	exitSDL() {

	if (g_joystick) {
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
	I2c::All_close();
    SDL_Quit();
	exit(EXIT_FAILURE);
}
