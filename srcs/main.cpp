#include "../lib/TEAM1_I2C/include/I2c_PcA9685.hpp"
#include <unistd.h>
#include <iostream>

/* int main() {
	
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
} */




//Axis value (range: -32768 to 32767)

#include <SDL2/SDL.h>
#include <iostream>
#include "../lib/sdl.h"

void signalHandler(int signum) {
	std::cout << "Interrupt signal (" << signum << ") received.\n";
	I2c::stop_all(); 
	SDL_Quit();
	exit(signum);  
}

double mapAxisToAngle(double axisValue, double angleMin, double angleMax, double angleCenter) {
    if (axisValue < 0) {
        // Map [-1, 0] -> [angleMin, angleCenter]
        return ((angleCenter - angleMin) * (axisValue - (-1)) / (0 - (-1)) + angleMin);
    } else {
        // Map [0, 1] -> [angleCenter, angleMax]
        return ((angleMax - angleCenter) * (axisValue - 0) / (1 - 0) + angleCenter);
    }
}

int main() 
{

    signal(SIGINT, signalHandler);
	int steering = 60;
	int throttle = 0;
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
		std::cerr << "ERROR! SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return (1);
	}

	if(SDL_NumJoysticks() <= 0) {
		std::cerr << "ERROR! No Joysticks detected: " << SDL_GetError() << std::endl;
		return (1);
	}
	I2c::init(0x60,0x40,"/dev/i2c-1");
	I2c::set_servo_angle(steering);
	SDL_Joystick *joystick = SDL_JoystickOpen(index_0_controller);
	if (joystick == nullptr) {
		std::cerr << "ERROR! Failed to open joystick: " << SDL_GetError() << std::endl;
		return (1);
	}

	std::cout << "Joystick detected: " << SDL_JoystickName(joystick) << std::endl;

	SDL_Event e;
	while (true) {

		float axis0 = SDL_JoystickGetAxis(joystick, 2) / 32767.0f; // steering
		float axis1 = SDL_JoystickGetAxis(joystick, 1) / 32767.0f; // throttle

		steering = static_cast<int>(mapAxisToAngle(axis0, 0, 120, 60));
		throttle = static_cast<int>(mapAxisToAngle(axis1, -100, 100, 0));

		I2c::set_servo_angle(steering);
		
		if (throttle > 0) {
			I2c::motor(0, throttle, 1); // Forward
		} else if (throttle < 0) {
			I2c::motor(0, -throttle, 0); // Backward
		} else {
			I2c::stop_motors(); // Stop
		}
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_JOYBUTTONDOWN:
					if (e.jbutton.button == 11) // Assuming button 0 is the 'A' button
					{
						std::cout << "Button A pressed. Exiting...\n";
						SDL_JoystickClose(joystick);
						I2c::stop_all(); 
						SDL_Quit();
						return 0;
					}
					break;
			}
		}
		//std::cout << "Steering PWM: " << steering << " Throttle PWM: " << -throttle << std::endl;
		
		SDL_Delay(10);
	}

	SDL_JoystickClose(joystick);
	SDL_Quit();
	return (0);
}



/*

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