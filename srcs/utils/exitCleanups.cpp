#include "../../include/sdl.h"

void	cleanExit() {

	 if (g_joystick) {
		 SDL_JoystickClose(g_joystick);
		 g_joystick = nullptr;
	 }
	I2c::stop_all();
    SDL_Quit();
    gpioTerminate();
}

void signalHandler(int signum) {

    std::cout << "Interrupt operation (" << signum << ") received.";
	cleanExit();
    exit(signum);
}
