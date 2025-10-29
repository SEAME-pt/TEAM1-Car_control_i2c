#include "../include/sdl.h"

SDL_Joystick* initCar() {

	if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
		throw (GenException::InitException());

	if (SDL_NumJoysticks() <= 0)
			throw (GenException::InitException());

	SDL_Joystick *joystick = SDL_JoystickOpen(CONTROLLER_0);
	if (joystick == nullptr)
		throw (GenException::InitException());
	std::cout << "Joystick detected: " << SDL_JoystickName(joystick) << std::endl;
 
	I2c::init(0x60,0x40,"/dev/i2c-1");
	I2c::set_servo_angle(MID_ANGLE);

	return (joystick);
}
