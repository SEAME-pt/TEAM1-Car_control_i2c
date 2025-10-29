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

int main() {

	int i = 0;
	int steering = MID_ANGLE;
	int throttleAngle = 0;
	int throttleDir = 0;

	bool l2 = false;
	bool r2 = false;

	SDL_Joystick *joystick = NULL;

	signal(SIGINT, signalHandler);

	try {
		joystick = initCar();
	} catch (std::exception(&e)) {
		std::cerr << e.what() << std::endl;
	}

	SDL_Event e;
	while (true) {

		//float axis0 = SDL_JoystickGetAxis(joystick, 2) / 32767.0f; // steering
		float axis1 = SDL_JoystickGetAxis(joystick, 1) / 32767.0f; // throttle

		// angle is decided directly from axis1
        throttleAngle = static_cast<int>(mapAxisToAngle(axis1, 0, 120, 60));
        I2c::set_servo_angle(throttleAngle);

        // speed magnitude based on axis absolute value (0..100)
        int speed = static_cast<int>(std::min(1.0f, std::fabs(axis1)) * 100);

		while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_JOYBUTTONDOWN:
                    // set button state when pressed
                    if (e.jbutton.button == R2_BUTTON) 
						r2 = true;
                    else if (e.jbutton.button == L2_BUTTON) 
						l2 = true;
                    else if (e.jbutton.button == START_BUTTON) {
                        std::cout << "Button start pressed. Exiting...\n";
                        SDL_JoystickClose(joystick);
                        I2c::stop_all();
                        SDL_Quit();
                        return 0;
                    }
                    break ;

                case SDL_JOYBUTTONUP:
                    // clear button state when released
                    if (e.jbutton.button == R2_BUTTON) 
						r2 = false;
                    else if (e.jbutton.button == L2_BUTTON) 
						l2 = false;
                    break ;

                case SDL_JOYAXISMOTION:
                    // optional: debug axis changes
                    // std::cout << "AXIS " << static_cast<int>(e.jaxis.axis) << " value=" << e.jaxis.value << std::endl;
                    break;

                default:
                    break;
            }
			// decide motion: R2 = forward, L2 = backward, otherwise stop
        	if (r2 && !l2) {
            	I2c::motor(0, speed, 1); // forward
        	} else if (l2 && !r2) {
            	I2c::motor(0, speed, 0); // backward
        	} else {
            	I2c::stop_motors(); // stop if neither or both pressed
        	}
        SDL_Delay(15);
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