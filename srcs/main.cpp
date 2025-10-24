#include "../lib/TEAM1_I2C/include/I2c_PcA9685.hpp"
#include <unistd.h>
#include <iostream>

int main() {
	
	I2c::init(0x60,0x40,"/dev/i2c-1");
	I2c::set_servo_angle(60);
	I2c::motor(0,50,1);
	sleep(3);

	I2c::stop_motors();
	I2c::set_servo_angle(120);
	I2c::motor(0,50,1);
	sleep(3);

	I2c::stop_motors();
	I2c::set_servo_angle(60);
	I2c::motor(0,50,0);
	sleep(3);

	I2c::stop_motors();
	I2c::set_servo_angle(0);
	I2c::motor(0,50,0);
	sleep(3);

	I2c::set_servo_angle(60);
	I2c::motor(0,50,0);

	I2c::stop_motors();
}



/*
//Axis value (range: -32768 to 32767)

#include <SDL2/SDL.h>
#include <iostream>
#include "../lib/sdl.h"


int main() {
	
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
		std::cerr << "ERROR! SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return (1);
	}

	if(SDL_NumJoysticks() <= 0) {
		std::cerr << "ERROR! No Joysticks detected: " << SDL_GetError() << std::endl;
		return (1);
	}

	SDL_Joystick *joystick = SDL_JoystickOpen(index_0_controller);
	if (joystick == nullptr) {
		std::cerr << "ERROR! Failed to open joystick: " << SDL_GetError() << std::endl;
		return (1);
	}

	std::cout << "Joystick detected: " << SDL_JoystickName(joystick) << std::endl;

	Sint16 x_move, y_move;

	while (true) {

		x_move = SDL_JoystickGetAxis(joystick, 0);
		y_move = SDL_JoystickGetAxis(joystick, 1);

		std::cout << x_move << "\n" << y_move << std::endl;
	}

	while (true) {
		//SDL_PollEvent(&e);
		float axis0 = SDL_JoystickGetAxis(joystick, 0) / 32767.0f; // steering
		float axis1 = SDL_JoystickGetAxis(joystick, 1) / 32767.0f; // throttle
		std::cout << "Steering: " << axis0 << " Throttle: " << -axis1 << "\r";
		SDL_Delay(50);
	}
}





if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	if (SDL_NumJoysticks() < 1) {
		std::cout << "No joystick connected!\n";
		return 1;
	}

	SDL_Joystick* joystick = SDL_JoystickOpen(0);
	if (joystick == nullptr) {
		std::cerr << "Failed to open joystick: " << SDL_GetError() << std::endl;
		return 1;
	}

	std::cout << "Joystick detected: " << SDL_JoystickName(joystick) << std::endl;

	SDL_Event e;
	while (true) {
		SDL_PollEvent(&e);
		float axis0 = SDL_JoystickGetAxis(joystick, 0) / 32767.0f; // steering
		float axis1 = SDL_JoystickGetAxis(joystick, 1) / 32767.0f; // throttle
		std::cout << "Steering: " << axis0 << " Throttle: " << -axis1 << "\r";
		SDL_Delay(50);
	}

	SDL_JoystickClose(joystick);
	SDL_Quit();
	return 0;*/