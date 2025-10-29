#include "../include/sdl.h"
#include "initCar.cpp"

//Axis value (range: -32768 to 32767)

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

	int steering = MID_ANGLE;
	int throttle = 0;
	SDL_Joystick *joystick;

	signal(SIGINT, signalHandler);

	try {
		joystick = initCar();
	} catch (std::exception(&e)) {
		std::cerr << e.what() << std::endl;
	}

	I2c::init(0x60,0x40,"/dev/i2c-1");
	I2c::set_servo_angle(MID_ANGLE);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		std::cout << "The loop began bitches!" << std::endl;

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
		if (SDL_PollEvent(&e))
		{
			switch (e.type) {
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
		std::cout << "Steering PWM: " << steering << " Throttle PWM: " << -throttle << std::endl;
		
		SDL_Delay(50);
	}

	std::cout << "The loop ended bitches!" << std::endl;
	SDL_JoystickClose(joystick);
	SDL_Quit();
	return (0);
}

//c++ srcs/main.cpp libs/TEAM1_I2C/srcs/I2c_PcA9685.cpp -lSDL2

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