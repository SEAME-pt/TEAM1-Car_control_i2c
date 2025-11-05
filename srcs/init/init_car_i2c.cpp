#include "../../include/sdl.h"

static void    initI2c() {

    try {
        I2c::All_init();
        I2c::set_servo_angle(MID_ANGLE);
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string("I2C initialization failed: ") + e.what());
    }
}

SDL_Joystick* initCar() {
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
		throw (GenException::InitException());

	if (SDL_NumJoysticks() <= 0) {
		SDL_Quit();
		throw (GenException::InitException());
	}

	SDL_Joystick *joystick = SDL_JoystickOpen(CONTROLLER_0);
	if (joystick == nullptr) {
		SDL_Quit();
		throw (GenException::InitException());
	}

	std::cout << "Joystick detected: " << SDL_JoystickName(joystick) << std::endl;

	try {
        initI2c();
    } catch (const std::exception &e) {
        SDL_JoystickClose(joystick);
        SDL_Quit();
        throw ;
    }

	return (joystick);
}
